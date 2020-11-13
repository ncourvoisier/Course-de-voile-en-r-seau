#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <iostream>
#include <gf/Sprite.h>
#include <gf/ResourceManager.h>

#include "BoatEntity.h"

namespace sail
{

    BoatEntity::BoatEntity(gf::Color4f color)
    : m_position({0, 0})
    , m_velocity({0, 0})
    , m_size(20.0f)
    , m_color(color)
    {

    }

    BoatEntity::~BoatEntity()
    {

    }

    void BoatEntity::update(gf::Time time)
    {
        //std::cout << "dt : " << time.asSeconds() << "\n";
        m_position += time.asSeconds() * m_velocity;
        //std::cout << "Velocity : x = " << m_velocity.x << ", y = " << m_velocity.y << "\n";
        //std::cout << "Pos : x = " << m_position.x << ", y = " << m_position.y << "\n";
    }

    void BoatEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
    {
        gf::RectangleShape sailShip({20.0f, 35.0f});
        sailShip.setPosition(m_position);
        sailShip.setColor(m_color);
        sailShip.setAnchor(gf::Anchor::Center);
        sailShip.setRotation(0.0); //TODO boat.getAngle
        target.draw(sailShip, states);

        gf::RectangleShape sail({5.0f, 25.0f});
        sail.setPosition({m_position[0],m_position[1]-10.0f});
        sail.setColor(gf::Color::Red);
        sail.setAnchor(gf::Anchor::Center);
        sail.setRotation(0.0); //TODO boat.getSailAngle
        target.draw(sail, states);

        gf::RectangleShape rudder({5.0f,10.0f});
        rudder.setPosition({m_position[0],m_position[1]+15.0f});
        rudder.setColor(gf::Color::Blue);
        rudder.setAnchor(gf::Anchor::Center);
        rudder.setRotation(0.0); //TODO boat.getRudderAngle
        target.draw(rudder, states);
    }

    gf::Vector2f BoatEntity::getPosition()
    {
        return m_position;
    }

    void BoatEntity::setPosition(const gf::Vector2f &mPosition)
    {
        m_position = mPosition;
    }

    gf::Vector2f BoatEntity::getVelocity()
    {
        return m_velocity;
    }

    void BoatEntity::setVelocity(const gf::Vector2f &mVelocity)
    {
        m_velocity = mVelocity;
    }

    ClientBoatData BoatEntity::getClientBoatData()
    {
        return { m_position, m_velocity };
    }

}