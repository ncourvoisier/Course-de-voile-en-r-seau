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

    while (true)
    {
        gf::Packet opponentP;
        if (clientHandler.receive(opponentP) == gf::SocketStatus::Data) {
            std::cout << "Opponent connected : " << opponentP.as<sail::PlayerJoins>().player.name << "\n";
        }
        sail::PlayerJoins opponent {opponentP.as<sail::PlayerJoins>()};
    }

    assert(false); // Don't go further this line, work in progress

    // Adding references to entities
    mainEntities.addEntity(playerBoat);
    //mainEntities.addEntity(opponentBoat);

    clientHandler.setNonBlocking();
    while (window.isOpen())
    {
        gf::Packet packet;
        switch (clientHandler.receive(packet))
        {
            case gf::SocketStatus::Data:
                switch (packet.getType()) {
                    case sail::GameStatus::type:
                    {
                        sail::GameStatus status {packet.as<sail::GameStatus>()};
                        for (auto& boat : status.boats)
                        {
                            sail::BoatEntity* boatE;
                            if (playerId == boat.playerId)
                                boatE = &playerBoat;
                            else
                                //boatE = &opponentBoat;
                            boatE->setPosition(boat.position);
                            boatE->setVelocity(boat.velocity);
                        }
                        break;
                    }
                }
                break;
            case gf::SocketStatus::Error:

                break;
            case gf::SocketStatus::Close:

                break;
        }

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
            // do something
        }
        else if (leftAction.isActive())
        {
            // do something
        }
        else if (upAction.isActive())
        {
            // do something
        }
        else if (downAction.isActive())
        {
            // do something
        } else
        {
            // do something
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