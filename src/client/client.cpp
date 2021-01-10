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
#include "../Constants.h"
#include "ClientBoat.h"
#include "ClientPlayer.h"
#include "Singletons.h"
#include "NavigationArrow.h"
#include <gf/Queue.h>
#include <gf/ResourceManager.h>
#include <gf/Sprite.h>
#include "config.h"
#include "Terrain.h"
#include "Banner.h"
#include "PredictionEngine.h"

void printUsage(char* execName)
{
    std::cout << "Usage: " << execName << " [address] [port] [username]\n";
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        printUsage(argv[0]);
        exit(1);
    }

    std::string usr = argv[3];

    //// CONNECTING
    sail::ClientNetworkHandler clientHandler;
    clientHandler.connect(argv[1], argv[2]);
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
    std::string userName = usr;
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

    gf::Vector2d endingPos;

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
            else if (waitingP.getType() == sail::WorldData::type)
            {
                auto ready (waitingP.as<sail::WorldData>());
                std::cout << " start : " << ready.startingPosition.x << ", " << ready.startingPosition.y
                    << ", end : " << ready.endingPosition.x << ", " << ready.endingPosition.y << "\n";
                endingPos = ready.endingPosition * sail::WorldScale;
                terrain.load(ready.terrain, ready.windDirection, ready.windSpeed, ready.endingPosition);
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

    mainEntities.addEntity(terrain);

    sail::NavigationArrow endArrow (endingPos, localBoat, hudView);
    hudEntities.addEntity(endArrow);

    sail::Banner banner({5000, 5000}, gf::Color::White, hudView);
    hudEntities.addEntity(banner);

    banner.displayText("START");

    // Launching the thread
    clientHandler.run();

    static constexpr gf::Time UpdateDelayMs = gf::milliseconds(50);
    static constexpr gf::Time SendKeyDelayMs = gf::milliseconds(50);
    gf::Time lag = gf::milliseconds(0);
    gf::Time keyDelay = gf::milliseconds(0);

    auto lastActionSail = sail::PlayerAction::Type::None;
    auto lastActionRubber = sail::PlayerAction::Type::None;
    int actionNb = 0;

    bool rudderActive = false;

    sail::PredictionEngine engine(localBoat, terrain);

    while (window.isOpen())
    {
       // std::cout << "Pos : " << localBoat.getLongitude() << ", " << localBoat.getLatitude() << "\n";

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

        if (keyDelay > SendKeyDelayMs)
        {
            sail::PlayerAction action { lastActionSail, lastActionRubber };
            engine.pushAction(action);
            if (lastActionSail != sail::PlayerAction::Type::None
                 || lastActionRubber != sail::PlayerAction::Type::None)
            {
                clientHandler.send(action);
                keyDelay = gf::milliseconds(0);
                lastActionSail = sail::PlayerAction::Type::None;
                lastActionRubber = sail::PlayerAction::Type::None;
            }
        }

        while (lag > UpdateDelayMs)
        {
            /////////////////////////
            /// Receiving packets ///
            /////////////////////////

            gf::Packet packet;
            while (clientHandler.getQueue().poll(packet))
            {
                switch (packet.getType())
                {
                    case sail::GameState::type:
                    {
                        sail::GameState state {packet.as<sail::GameState>()};
                        for (auto& boat : state.boats)
                        {
                            sail::ClientBoat& entity = players.at(boat.playerId).getBoat();
                            entity.fromBoatData(boat);
                        }
                        engine.reconciliate(state.lastAckActionId);
                        break;
                    }
                    case sail::PlayerEvent::type:
                    {
                        auto eventPacket {packet.as<sail::PlayerEvent>()};
                        switch (eventPacket.event)
                        {
                            case sail::PlayerEvent::EventType::Death:
                                std::cout << "A player is dead\n";
                                break;
                            case sail::PlayerEvent::EventType::Finish:
                                std::cout << "A player finished\n";
                                break;
                            case sail::PlayerEvent::EventType::Checkpoint:
                                std::cout << "Player found a checkpoint\n";
                                break;
                            default:
                                std::cout << "Received unknown PlayerEvent type\n";
                        }
                    }
                    break;
                }
            }
            
            /////////////////////////

            lag -= UpdateDelayMs;
        }

        engine.update(gf::milliseconds(50)); // TODO : i know it's the wrong place

        // Centering the view
        mainView.setCenter({ static_cast<float>(localBoat.getLongitude()), static_cast<float>(localBoat.getLatitude()) });

        // 2. update
        //localBoat.update(time);
        mainEntities.update(time);
        hudEntities.update(time);
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