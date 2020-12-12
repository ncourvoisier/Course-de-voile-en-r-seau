#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <iostream>

#include "ClientBoat.h"

namespace sail
{

    ClientBoat::ClientBoat(gf::Color4f color)
    : gf::Entity(1)
    , m_size(20.0f)
    , m_color(color)
    {

    }

    ClientBoat::~ClientBoat()
    {

    }

    void ClientBoat::update(gf::Time time)
    {
        // TODO : prediction
    }

    void ClientBoat::render(gf::RenderTarget &target, const gf::RenderStates &states)
    {
        gf::Vector2f position(x, y);
        //std::cout << "POSITION : " << position.x << ", " << position.y << "\n"; NO PROBLEM HERE

        gf::RectangleShape sailShip({20.0f, 35.0f});
        sailShip.setPosition(position);
        sailShip.setColor(m_color);
        sailShip.setAnchor(gf::Anchor::Center);
        sailShip.setRotation(angle);
        target.draw(sailShip, states);

        gf::RenderStates shipStates;
        shipStates.transform = sailShip.getTransform();

        gf::RectangleShape sail({5.0f, 25.0f});
        sail.setPosition({10.0f, 17.5f});
        sail.setColor(gf::Color::Red);
        sail.setAnchor(gf::Anchor::Center);
        sail.setRotation(sail_angle);
        target.draw(sail, shipStates);

        gf::RectangleShape rudder({5.0f,10.0f});
        rudder.setPosition({10.0f,35.0f});
        rudder.setColor(gf::Color::Blue);
        rudder.setAnchor(gf::Anchor::Center);
        rudder.setRotation(rudder_angle);
        target.draw(rudder, shipStates);
    }

    void ClientBoat::fromBoatData(BoatData data)
    {
        x = data.xPos * Scale;
        y = data.yPos * Scale;
        angle = data.angle;
        sail_angle = data.sailAngle;
        rudder_angle = data.rudderAngle;
    }

}