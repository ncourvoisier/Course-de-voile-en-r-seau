#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <iostream>

#include "../physics/Conversions.h"

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
        gf::RectangleShape shape({m_size, m_size});
        shape.setPosition(toFloatVectorPosition(x, y));
        shape.setColor(m_color);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape, states);
        //std::cout << "Render with : " << m_position.x << ", " << m_position.y << "\n";
    }

    void ClientBoat::fromBoatData(BoatData data)
    {
        x = data.xPos * Scale;
        y = data.yPos * Scale;
        angle = data.angle;
    }

}