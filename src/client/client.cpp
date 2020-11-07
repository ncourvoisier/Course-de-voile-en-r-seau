#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>
#include <gf/TcpSocket.h>
#include <iostream>
#include "ClientNetworkHandler.h"
#include "../Protocol.h"
#include "BoatEntity.h"
#include "ClientPlayer.h"
#include <gf/Queue.h>

int main()
{

    //// CONNECTING
    sail::ClientNetworkHandler clientHandler;
    clientHandler.connect("127.0.0.1", "24680");
    ////

    static constexpr gf::Vector2u ScreenSize(512, 512);
    static constexpr gf::Vector2f ViewSize(512.0f, 512.0f); // dummy values
    static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f); // dummy values
    // initialization
    gf::Window window("Game", ScreenSize);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    gf::RenderWindow renderer(window);
    // views
    gf::ViewContainer views;
    gf::ExtendView mainView(ViewCenter, ViewSize);
    views.addView(mainView);
    gf::ScreenView hudView;
    views.addView(hudView);
    views.setInitialFramebufferSize(ScreenSize);
    // actions
    gf::ActionContainer actions;
    gf::Action closeWindowAction("Close window");
    closeWindowAction.addCloseControl();
    closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
    actions.addAction(closeWindowAction);
    gf::Action fullscreenAction("Fullscreen");
    fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
    actions.addAction(fullscreenAction);
    gf::Action leftAction("Left");
    leftAction.addScancodeKeyControl(gf::Scancode::A);
    leftAction.addScancodeKeyControl(gf::Scancode::Left);
    leftAction.setContinuous();
    actions.addAction(leftAction);
    gf::Action rightAction("Right");
    rightAction.addScancodeKeyControl(gf::Scancode::D);
    rightAction.addScancodeKeyControl(gf::Scancode::Right);
    rightAction.setContinuous();
    actions.addAction(rightAction);
    gf::Action upAction("Up");
    upAction.addScancodeKeyControl(gf::Scancode::W);
    upAction.addScancodeKeyControl(gf::Scancode::Up);
    upAction.setContinuous();
    actions.addAction(upAction);
    gf::Action downAction("Down");
    downAction.addScancodeKeyControl(gf::Scancode::S);
    downAction.addScancodeKeyControl(gf::Scancode::Down);
    downAction.setContinuous();
    actions.addAction(downAction);
    // entities
    gf::EntityContainer mainEntities;
    // add entities to mainEntities
    gf::EntityContainer hudEntities;
    // add entities to hudEntities
    // game loop
    renderer.clear(gf::Color::White);
    gf::Clock clock;

    // THE FOLLOWING IS QUICK AND DIRTY CODE, WILL BE MOVED IN OTHER PARTS

    // Establishing connection
    std::string userName = "username";
    sail::ClientGreeting greeting = { userName }; // TODO : TEMPORARY
    clientHandler.send(greeting);
    clientHandler.setBlocking();
    gf::Packet serverGreetingP;
    clientHandler.receive(serverGreetingP);
    auto serverGreeting {serverGreetingP.as<sail::ServerGreeting>()};

    // Defining players Datas

    std::cout << "Server sent ID : " << serverGreeting.playerId << "\n";
    std::map<gf::Id, sail::ClientPlayer> players;

    players.insert(std::pair<gf::Id, sail::ClientPlayer>(serverGreeting.playerId, sail::ClientPlayer(serverGreeting.playerId, userName)));
    sail::ClientPlayer& localPlayer = players.at(serverGreeting.playerId);
    sail::BoatEntity& localBoat =  localPlayer.getBoat();

    for (auto& p : serverGreeting.players)
    {
        std::cout << "Player already here : " << p.name << "\n";
        players.insert(std::pair<gf::Id, sail::ClientPlayer>(p.id,
                sail::ClientPlayer(p.id, p.name))); // TODO : create a player manager
    }

    for (;;)
    {
        gf::Packet waitingP;
        if (clientHandler.receive(waitingP) == gf::SocketStatus::Data)
        {
            if (waitingP.getType() == sail::PlayerJoins::type)
            {
                sail::PlayerJoins waiting {waitingP.as<sail::PlayerJoins>()};
                std::cout << "Opponent connected : " << waiting.player.name << "\n";
                players.insert(std::pair<gf::Id, sail::ClientPlayer>(waiting.player.id,
                        sail::ClientPlayer(waiting.player.id, waiting.player.name)));
            }
            else if (waitingP.getType() == sail::GameReady::type)
            {
                break;
            }
        }
    }

    std::cout << "Game is ready\n";

    // Adding references to entities
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        mainEntities.addEntity((it->second).getBoat());
    }


    // Launching the thread
    clientHandler.run();

    static constexpr gf::Time UpdateDelayMs = gf::milliseconds(10);
    static constexpr gf::Time SendKeyDelayMs = gf::milliseconds(200);
    gf::Time lag = gf::milliseconds(0);
    gf::Time keyDelay = gf::milliseconds(0);

    auto lastAction = sail::PlayerAction::Type::None;

    while (window.isOpen())
    {
        gf::Time time = clock.restart();
        lag += time;
        keyDelay += time;
        
        // 1. input
        gf::Event event;
        while (window.pollEvent(event))
        {
            actions.processEvent(event);
            views.processEvent(event);
        }
        if (closeWindowAction.isActive())
        {
            window.close();
        }
        if (fullscreenAction.isActive())
        {
            window.toggleFullscreen();
        }

        if (rightAction.isActive()) {
            //std::cout << "right\n";
            lastAction = sail::PlayerAction::Type::Right;
            rightAction.reset();
        }
        else if (leftAction.isActive())
        {
           // std::cout << "left\n";
            lastAction = sail::PlayerAction::Type::Left;
            leftAction.reset();
        }
        else if (upAction.isActive())
        {
            //std::cout << "up\n";
            lastAction = sail::PlayerAction::Type::Up;
            upAction.reset();
        }
        else if (downAction.isActive())
        {
            //std::cout << "down\n";
            lastAction = sail::PlayerAction::Type::Down;
            downAction.reset();
        } else
        {

        }

        if (keyDelay > SendKeyDelayMs && lastAction != sail::PlayerAction::Type::None)
        {
            std::cout << "action\n";
            sail::PlayerAction action { lastAction };
            clientHandler.send(action);
            keyDelay = gf::milliseconds(0);
            lastAction = sail::PlayerAction::Type::None;
        }

        while (lag > UpdateDelayMs)
        {
            /////////////////////////
            /// Receiving packets ///
            /////////////////////////

            gf::Packet packet;
            if (clientHandler.getQueue().poll(packet))
            {
                clientHandler.getQueue(); // Fix a bit but ... there's too much packets
                //std::cout << "Data received...\n";
                switch (packet.getType())
                {
                    case sail::GameState::type:
                    {
                        // Proceed to send our new position first (just an idea... probably a bad one :D)
                        sail::ClientBoatData boatData = localBoat.getClientBoatData();
                        clientHandler.send(boatData); // Concurrency with incoming packet in the other thread ? maybe ..?

                        sail::GameState state {packet.as<sail::GameState>()};
                        for (auto& boat : state.boats)
                        {
                            sail::BoatEntity& entity = players.at(boat.playerId).getBoat();
                            std::cout << "position : " << boat.position.x << ", " << boat.position.y << "\n";
                            /*entity.setVelocity(boat.velocity);*/
                            entity.setPosition(boat.position);
                        }
                        break;
                    }
                }
            }

            /////////////////////////

            lag -= UpdateDelayMs;
        }

        //std::cout << "x : " << localBoat.getVelocity().x << ", y : " << localBoat.getVelocity().y << "\n"; >>>>>> NO PROBLEM HERE

        // 2. update
        //localBoat.update(time);
        //mainEntities.update(time);
        //hudEntities.update(time);
        // 3. draw
        renderer.clear();
        renderer.setView(mainView);
        mainEntities.render(renderer);
        renderer.setView(hudView);
        hudEntities.render(renderer);
        renderer.display();
        actions.reset();
    }
    return 0;

}