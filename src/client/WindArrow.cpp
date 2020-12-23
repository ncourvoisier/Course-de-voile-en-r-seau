#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Color.h>

#include "WindArrow.h"

namespace sail
{

    WindArrow::WindArrow(double speed, double angle, double distanceToBoat)
    : m_arrow(gf::PrimitiveType::Triangles, 6)
    {
        gf::Vertex arrowVertices[4];
        arrowVertices[0].position = { 3, 0 };
        arrowVertices[1].position = { 6, 7 };
        arrowVertices[2].position = { 3, 6 };
        arrowVertices[3].position = { 0, 7 };
        for (int i = 0; i < 4; i++)
        {
            double transparency;
            if (distanceToBoat >= MaxDistance)
                transparency = 0;
            else
                transparency = (1.0 - distanceToBoat / MaxDistance) * 230.0;
            arrowVertices[i].color = gf::Color::fromRgba32(0, 0, 150, transparency); // TODO : colors will change depending on speed
        }
        // Left Half
        m_arrow[0] = arrowVertices[0];
        m_arrow[1] = arrowVertices[2];
        m_arrow[2] = arrowVertices[3];
        // Right half
        m_arrow[3] = arrowVertices[0];
        m_arrow[4] = arrowVertices[1];
        m_arrow[5] = arrowVertices[2];
    }

    void WindArrow::draw(gf::RenderTarget &target, const gf::RenderStates &states)
    {
        gf::RenderStates newState;
        newState.transform = states.transform * getTransform();
        target.draw(m_arrow, newState);
        /*gf::CircleShape shape(10);
        shape.setColor(gf::Color::Green);
        target.draw(shape, newState);*/
    }

}