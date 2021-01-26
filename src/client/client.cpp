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
#include <gf/ResourceManager.h>
#include <gf/Log.h>
#include "config.h"
#include "ClientWorld.h"
#include "Banner.h"
#include "PredictionEngine.h"
#include "ClientStringConstants.h"

void printUsage(char* execName)
{
    std::cout << "Usage: " << execName << " <address> <port> <username> [-predictionOff|-pOff]\n";
}

int main(int argc, char *argv[])
{
    bool enablePrediction = true;

    if (argc != 4 && argc != 5) {
        printUsage(argv[0]);
        exit(1);
    }

    if (argc == 5)
    {
        if ((strcmp(argv[4], "-predictionOff") == 0 || strcmp(argv[4], "-pOff") == 0))
            enablePrediction = false;
        else
        {
            printUsage(argv[0]);
            exit(1);
        }
    }

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
    renderer.clear(gf::Color::Black);

    // Establishing connection
    sail::ClientGreeting greeting = { argv[3] };
    clientHandler.send(greeting);
    gf::Packet serverResponseP;
    clientHandler.receive(serverResponseP);
    if (serverResponseP.getType() == sail::GameRunning::type)
    {
        gf::Log::error(sail::ClientStringConstants::GameAlreadyRunning);
        exit(3);
    }
    else if (serverResponseP.getType() != sail::ServerGreeting::type)
    {
        gf::Log::error(sail::ClientStringConstants::InvalidServerGreeting);
        exit(4);
    }

    auto serverGreeting {serverResponseP.as<sail::ServerGreeting>()};

    // Defining players Datas

    gf::Log::info(sail::ClientStringConstants::GreetingId, serverGreeting.playerId);
    std::map<gf::Id, sail::ClientPlayer> players;

    players.insert(std::pair<gf::Id, sail::ClientPlayer>(serverGreeting.playerId, sail::ClientPlayer(serverGreeting.playerId, argv[3])));
    sail::ClientPlayer& localPlayer = players.at(serverGreeting.playerId);
    sail::ClientBoat& localBoat =  localPlayer.getBoat();

    // Defining the terrain entity
    sail::ClientWorld terrain(localBoat);

    for (auto& p : serverGreeting.players)
    {
        gf::Log::info(sail::ClientStringConstants::PlayerAlreadyHere, p.name.c_str());
        players.insert(std::pair<gf::Id, sail::ClientPlayer>(p.id,
                sail::ClientPlayer(p.id, p.name)));
    }

    gf::Vector2d endingPos;

    bool start = false;
    while (! start)
    {
        gf::Packet waitingP;
        if (clientHandler.receive(waitingP) == gf::SocketStatus::Data)
        {
            switch (waitingP.getType())
            {
                case sail::PlayerJoins::type:
                {
                    auto waiting(waitingP.as<sail::PlayerJoins>());
                    gf::Log::info(sail::ClientStringConstants::PlayerConnected, waiting.player.name.c_str());
                    players.insert(std::pair<gf::Id, sail::ClientPlayer>(waiting.player.id,
                                                                         sail::ClientPlayer(waiting.player.id,
                                                                                            waiting.player.name)));
                    break;
                }
                case sail::WorldData::type:
                {
                    auto ready(waitingP.as<sail::WorldData>());
                    endingPos = ready.endingPosition;
                    localBoat.setLongitude(ready.startingPosition.x);
                    localBoat.setLatitude(ready.startingPosition.y);
                    terrain.load(ready.terrain, ready.windDirection, ready.windSpeed, ready.startingPosition, ready.endingPosition);
                    break;
                }
                case sail::GameStart::type:
                {
                    start = true;
                    break;
                }
                default:
                    gf::Log::error(sail::ClientStringConstants::UnexpectedPacketType);
            }
        }
    }

    gf::Log::info(sail::ClientStringConstants::GameReady);


    // Adding references to entities
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        mainEntities.addEntity((it->second).getBoat());
    }

    mainEntities.addEntity(terrain);

    sail::NavigationArrow endArrow (endingPos, localBoat, hudView);
    hudEntities.addEntity(endArrow);

    sail::Banner banner(hudView);
    hudEntities.addEntity(banner);

    // Launching the thread
    clientHandler.run();

    static constexpr gf::Time UpdateDelayMs = gf::milliseconds(50);

    auto lastActionSail = sail::PlayerAction::Type::None;
    auto lastActionRubber = sail::PlayerAction::Type::None;

    bool rudderActive = false;

    sail::PredictionEngine engine(localBoat, terrain);

    if (! enablePrediction)
        engine.disable();

    gf::Clock clock;
    gf::Time nextFrameTime = gf::Time::Zero;

    while (window.isOpen())
    {
        nextFrameTime += sail::FrameTime;

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
            lastActionSail = sail::PlayerAction::Type::Right;
        }
        else if (sailLeftAction.isActive())
        {
            lastActionSail = sail::PlayerAction::Type::Left;
        }

        if (rubberLeftAction.isActive())
        {
            rudderActive = true;
            lastActionRubber = sail::PlayerAction::Type::Left;
        }
        else if (rubberRightAction.isActive())
        {
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


        sail::PlayerAction action { lastActionSail, lastActionRubber };

        engine.pushAction(action);

        // 20 packets per second are sent, otherwise the prediction doesn't work efficiently

        if (clientHandler.send(action) != gf::SocketStatus::Data) {
            exit(2);
        }
        lastActionSail = sail::PlayerAction::Type::None;
        lastActionRubber = sail::PlayerAction::Type::None;

        do
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
                        auto& name = players.at(eventPacket.id).getBoat().getName();
                        switch (eventPacket.event)
                        {
                            case sail::PlayerEvent::EventType::Death:
                                banner.displayText(name + " is dead!");
                                break;
                            case sail::PlayerEvent::EventType::Finish:
                                banner.displayText(name + " won!");
                                break;
                            case sail::PlayerEvent::EventType::Checkpoint:
                                banner.displayText(name + " found a checkpoint!");
                                break;
                            default:
                                gf::Log::error(sail::ClientStringConstants::UnknownPlayerEvent);
                        }
                        break;
                    }
                    case sail::PlayerLeaves::type:
                    {
                        auto leavePacket {packet.as<sail::PlayerLeaves>()};
                        auto& boat = players.at(leavePacket.id).getBoat();
                        mainEntities.removeEntity(&boat);
                        banner.displayText(boat.getName() + " left the game");
                        break;
                    }
                }
            }
            
            /////////////////////////

        } while (clock.getElapsedTime() < nextFrameTime);

        // Centering the view
        mainView.setCenter({ static_cast<float>(localBoat.getScaledX()), static_cast<float>(localBoat.getScaledY()) });

        // 2. update
        mainEntities.update(UpdateDelayMs);
        hudEntities.update(UpdateDelayMs);
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