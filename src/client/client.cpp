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
#include "ClientBoat.h"
#include "ClientPlayer.h"
#include "Singletons.h"
#include "Weathercock.h"
#include <gf/Queue.h>
#include <gf/ResourceManager.h>
#include <gf/Sprite.h>
#include "config.h"
#include "Terrain.h"

int main()
{

    //// CONNECTING
    sail::ClientNetworkHandler clientHandler;
    clientHandler.connect("127.0.0.1", "24680");
    ////

    static constexpr gf::Vector2u ScreenSize(525, 525);
    static constexpr gf::Vector2f ViewSize(500.0f, 500.0f); // dummy values
    static constexpr gf::Vector2f ViewCenter(250.0f, 250.0f); // dummy values
    // initialization
    gf::Window window("Game", ScreenSize);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    gf::RenderWindow renderer(window);

    gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(sail::gResourceManager);
    sail::gResourceManager().addSearchDir(SAILINSANE_DATA_DIR);

    gf::SingletonStorage<gf::TextureAtlas> storageForTextureAtlas(sail::gTextureAtlas, "sheet.xml", sail::gResourceManager());

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
    gf::Action sailLeftAction("Sail left");
    sailLeftAction.addScancodeKeyControl(gf::Scancode::A);
    sailLeftAction.addScancodeKeyControl(gf::Scancode::Down);
    sailLeftAction.setContinuous();
    actions.addAction(sailLeftAction);
    gf::Action sailRightAction("Sail right");
    sailRightAction.addScancodeKeyControl(gf::Scancode::D);
    sailRightAction.addScancodeKeyControl(gf::Scancode::Up);
    sailRightAction.setContinuous();
    actions.addAction(sailRightAction);
    gf::Action rubberLeftAction("Rubber left");
    rubberLeftAction.addScancodeKeyControl(gf::Scancode::K);
    rubberLeftAction.addScancodeKeyControl(gf::Scancode::Left);
    rubberLeftAction.setContinuous();
    actions.addAction(rubberLeftAction);
    gf::Action rubberRightAction("Rubber right");
    rubberRightAction.addScancodeKeyControl(gf::Scancode::M);
    rubberRightAction.addScancodeKeyControl(gf::Scancode::Right);
    rubberRightAction.setContinuous();
    actions.addAction(rubberRightAction);
    gf::Action rudderCenterAction("Rudder center");
    rudderCenterAction.addScancodeKeyControl(gf::Scancode::Space);
    actions.addAction(rudderCenterAction);
    gf::Action fullRenderAction("Full render");
    fullRenderAction.addScancodeKeyControl(gf::Scancode::Tab);
    fullRenderAction.setContinuous();
    actions.addAction(fullRenderAction);

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
    gf::Packet serverGreetingP;
    clientHandler.receive(serverGreetingP);
    auto serverGreeting {serverGreetingP.as<sail::ServerGreeting>()};

    // Defining players Datas

    std::cout << "Server sent ID : " << serverGreeting.playerId << "\n";
    std::map<gf::Id, sail::ClientPlayer> players;

    players.insert(std::pair<gf::Id, sail::ClientPlayer>(serverGreeting.playerId, sail::ClientPlayer(serverGreeting.playerId, userName)));
    sail::ClientPlayer& localPlayer = players.at(serverGreeting.playerId);
    sail::ClientBoat& localBoat =  localPlayer.getBoat();

    // Defining the terrain entity
    sail::Terrain terrain(localBoat);

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
                auto waiting (waitingP.as<sail::PlayerJoins>());
                std::cout << "Opponent connected : " << waiting.player.name << "\n";
                players.insert(std::pair<gf::Id, sail::ClientPlayer>(waiting.player.id,
                        sail::ClientPlayer(waiting.player.id, waiting.player.name)));
            }
            else if (waitingP.getType() == sail::GameReady::type)
            {
                auto ready (waitingP.as<sail::GameReady>());
                std::cout << "Terrain : " << ready.terrain.getSize().x << ", " << ready.terrain.getSize().y << "\n";
                std::cout << " -> " << ready.terrain({0, 0}) << "\n";
                terrain.load(ready.terrain);
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
    sail::Weathercock cock;
    mainEntities.addEntity(cock);

    mainEntities.addEntity(terrain);

    // Launching the thread
    clientHandler.run();

    static constexpr gf::Time UpdateDelayMs = gf::milliseconds(50);
    static constexpr gf::Time SendKeyDelayMs = gf::milliseconds(100);
    gf::Time lag = gf::milliseconds(0);
    gf::Time keyDelay = gf::milliseconds(0);

    auto lastActionSail = sail::PlayerAction::Type::None;
    auto lastActionRubber = sail::PlayerAction::Type::None;
    int actionNb = 0;

    bool rudderActive = false;

    while (window.isOpen())
    {
        gf::Time time = clock.restart();
        lag += time;
        keyDelay += time;
        
        // 1. input
        struct gf::Event event;
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

        if (sailRightAction.isActive()) {
            //std::cout << "right\n";
            lastActionSail = sail::PlayerAction::Type::Right;
        }
        else if (sailLeftAction.isActive())
        {
           // std::cout << "left\n";
            lastActionSail = sail::PlayerAction::Type::Left;
        }

        if (rubberLeftAction.isActive())
        {
            //std::cout << "up\n";
            rudderActive = true;
            lastActionRubber = sail::PlayerAction::Type::Left;
        }
        else if (rubberRightAction.isActive())
        {
            //std::cout << "down\n";
            rudderActive = true;
            lastActionRubber = sail::PlayerAction::Type::Right;
        }
        else if (rudderActive)
        {
            rudderActive = false;
            lastActionRubber = sail::PlayerAction::Type::Center;
        }
        else if (rudderCenterAction.isActive())
        {
            lastActionRubber = sail::PlayerAction::Type::Center;
        }


        if (fullRenderAction.isActive())
        {
            terrain.setFullRender(true);
        }

        if (keyDelay > SendKeyDelayMs
            && (lastActionSail != sail::PlayerAction::Type::None
                || lastActionRubber != sail::PlayerAction::Type::None))
        {
            //std::cout << "action : " << ++actionNb << "\n";
            sail::PlayerAction action { lastActionSail, lastActionRubber };
            clientHandler.send(action);
            keyDelay = gf::milliseconds(0);
            lastActionSail = sail::PlayerAction::Type::None;
            lastActionRubber = sail::PlayerAction::Type::None;
        }

        while (lag > UpdateDelayMs)
        {
            /////////////////////////
            /// Receiving packets ///
            /////////////////////////

            gf::Packet packet;
            if (clientHandler.getQueue().poll(packet))
            {
                switch (packet.getType())
                {
                    case sail::GameState::type:
                    {
                        sail::GameState state {packet.as<sail::GameState>()};
                        for (auto& boat : state.boats)
                        {
                            sail::ClientBoat& entity = players.at(boat.playerId).getBoat();
                            if (boat.playerId == localPlayer.getId())
                                std::cout << "Boat: " << boat.xPos << ", " << boat.yPos << " | angle : " << boat.angle
                                << ", sail : " << boat.sailAngle << ", rudder : " << boat.rudderAngle << "\n";
                            entity.fromBoatData(boat);
                        }
                        cock.fromWindData(state.wind);
                        break;
                    }
                }
            }
            
            /////////////////////////

            lag -= UpdateDelayMs;
        }

        // Centering the view
        mainView.setCenter({ static_cast<float>(localBoat.getLongitude()), static_cast<float>(localBoat.getLatitude()) });

        // 2. update
        //localBoat.update(time);
        mainEntities.update(time);
        //hudEntities.update(time);
        // 3. draw
        renderer.clear();
        renderer.setView(mainView);
        mainEntities.render(renderer);
        //std::cout << "rendering entities \n";
        renderer.setView(hudView);
        hudEntities.render(renderer);
        renderer.display();
        actions.reset();
    }
    return 0;

}