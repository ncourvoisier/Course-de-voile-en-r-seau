#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <iostream>

#include "ClientBoat.h"

namespace sail
{

    ClientBoat::ClientBoat(gf::Color4f color)
    : m_size(20.0f)
    , m_color(color)
    {

    }

    ClientBoat::~ClientBoat()
    {

    }

    void ClientBoat::update(gf::Time time)
    {
        //std::cout << "dt : " << time.asSeconds() << "\n";
        // m_position += time.asSeconds() * m_velocity;
        //std::cout << "Velocity : x = " << m_velocity.x << ", y = " << m_velocity.y << "\n";
        //std::cout << "Pos : x = " << m_position.x << ", y = " << m_position.y << "\n";
    }

    void ClientBoat::render(gf::RenderTarget &target, const gf::RenderStates &states)
    {
        gf::Vector2f position(x, y);

        gf::RectangleShape sailShip({20.0f, 35.0f});
        sailShip.setPosition(position);
        sailShip.setColor(m_color);
        sailShip.setAnchor(gf::Anchor::Center);
        sailShip.setRotation(angle); //TODO boat.getAngle
        target.draw(sailShip, states);

        gf::RectangleShape sail({5.0f, 25.0f});
        sail.setPosition({position[0],position[1]-10.0f});
        sail.setColor(gf::Color::Red);
        sail.setAnchor(gf::Anchor::Center);
        sail.setRotation(sail_angle); //TODO boat.getSailAngle
        target.draw(sail, states);

        gf::RectangleShape rudder({5.0f,10.0f});
        rudder.setPosition({position[0],position[1]+15.0f});
        rudder.setColor(gf::Color::Blue);
        rudder.setAnchor(gf::Anchor::Center);
        rudder.setRotation(rudder_angle); //TODO boat.getRudderAngle
        target.draw(rudder, states);
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