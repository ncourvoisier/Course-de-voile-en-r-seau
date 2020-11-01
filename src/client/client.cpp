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

int main()
{
    //// CONNECTING
    sail::ClientNetworkHandler clientHandler;
    clientHandler.connect("127.0.0.1", "24680");
    ////

    static constexpr gf::Vector2u ScreenSize(1024, 576);
    static constexpr gf::Vector2f ViewSize(100.0f, 100.0f); // dummy values
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
    sail::ClientGreeting greeting = {"username"}; // TODO : TEMPORARY
    clientHandler.send(greeting);
    clientHandler.setBlocking();
    gf::Packet serverGreetingP;
    clientHandler.receive(serverGreetingP);
    auto serverGreeting {serverGreetingP.as<sail::ServerGreeting>()};
    // Defining player Datas
    const gf::Id playerId {serverGreeting.playerId};
    sail::BoatEntity playerBoat(gf::Color::Green);
    std::cout << "Server sent ID : " << playerId << "\n";

    for (auto& p : serverGreeting.players)
    {
        std::cout << "Player already here : " << p.name << "\n";
    }

    clientHandler.setNonBlocking();

    bool gameReady = false;

    while (! gameReady)
    {
        gf::Packet waitingP;
        if (clientHandler.receive(waitingP) == gf::SocketStatus::Data) {
            if (waitingP.getType() == sail::PlayerJoins::type)
            {
                std::cout << "Opponent connected : " << waitingP.as<sail::PlayerJoins>().player.name << "\n";
            }
            else if (waitingP.getType() == sail::GameReady::type)
                gameReady = true;
        }
    }

    std::cout << "Game is ready\n";

    // Adding references to entities
    //mainEntities.addEntity(playerBoat);
    //mainEntities.addEntity(opponentBoat);

    while (window.isOpen())
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// GF Socket's "Non Blocking" mode doesn't work (???), need to run the following part in an other Thread ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /*std::cout << "window opened...\n";
        gf::Packet packet;

        gf::SocketStatus status = clientHandler.receive(packet);

        std::cout << "Packet received\n";

        switch (status)
        {
            case gf::SocketStatus::Data:
            {
                std::cout << "Data received...\n";
                switch (packet.getType())
                {
                    case sail::GameState::type:
                    {
                        *//*sail::GameState status {packet.as<sail::GameState>()};
                        for (auto& boat : status.boats)
                        {
                            sail::BoatEntity* boatE;
                            if (playerId == boat.playerId)
                                boatE = &playerBoat;
                            else
                                //boatE = &opponentBoat;
                            boatE->setPosition(boat.position);
                            boatE->setVelocity(boat.velocity);
                        }*//*
                        break;
                    }
                }
                break;
            }
            case gf::SocketStatus::Error:
            {
                std::cout << "Error...\n";
                break;
            }
            case gf::SocketStatus::Close:
            {
                std::cout << "Closed...\n";
                break;
            }
            case gf::SocketStatus::Block:
            {
                std::cout << "Blocking...\n";
                break;
            }
            default:
                std::cout << "Something else ???\n";
        }

        std::cout << "after switch\n";*/

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

        sail::PlayerAction::Type playerActionT;

        if (rightAction.isActive()) {
            std::cout << "right\n";
            playerActionT = sail::PlayerAction::Type::Right;
            rightAction.reset();
        }
        else if (leftAction.isActive())
        {
            std::cout << "left\n";
            playerActionT = sail::PlayerAction::Type::Left;
            leftAction.reset();
        }
        else if (upAction.isActive())
        {
            std::cout << "up\n";
            playerActionT = sail::PlayerAction::Type::Up;
            upAction.reset();
        }
        else if (downAction.isActive())
        {
            std::cout << "down\n";
            playerActionT = sail::PlayerAction::Type::Down;
            downAction.reset();
        } else
        {
            // do something
        }

        if (playerActionT)
        { // TODO : impossible to send packet every loop, need to send them every tick ... (yay, going to be fun)
            sail::PlayerAction playerAction {playerActionT};
            clientHandler.send(playerAction);
        }

        // 2. update
        gf::Time time = clock.restart();
        mainEntities.update(time);
        hudEntities.update(time);
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