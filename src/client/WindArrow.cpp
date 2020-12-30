#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Color.h>

#include "WindArrow.h"

namespace sail
{

    WindArrow::WindArrow(double speed, double angle, double distanceToBoat)
    : m_arrow(gf::PrimitiveType::Triangles, 6)
    , m_direction(angle)
    , m_speed(speed)
    {
        gf::Vertex arrowVertices[4];
        arrowVertices[0].position = { 3, 0 };
        arrowVertices[1].position = { 6, 7 };
        arrowVertices[2].position = { 3, 6 };
        arrowVertices[3].position = { 0, 7 };

        float red, green, blue;
        int gradientPosition = m_speed / 7.5f;
        float normalize = fmod(m_speed, 7.5f); // put m_speed between 0 and 7.5
        switch (gradientPosition)
        {
            case 0:
                red = 0.0f;
                green = (normalize * 255.0f) / 7.5f;
                blue = 255.0f;
                break;
            case 1:
                red = 0.0f;
                green = 255.0f;
                blue = 255.0f - (normalize * 255.0f) / 7.5f;
                break;
            case 2:
                red = (normalize * 255.0f) / 7.5f;
                green = 255.0f;
                blue = 0.0f;
                break;
            case 3:
                red = 255.0f;
                green = 255.0f - (normalize * 255.0f) / 7.5f;
                blue = 0.0f;
                break;
            default:
                assert(false);
        }

        for (int i = 0; i < 4; i++)
        {
            double transparency;
            if (distanceToBoat >= MaxDistance)
                transparency = 0;
            else
                transparency = (1.0 - distanceToBoat / MaxDistance) * 230.0;
            arrowVertices[i].color = gf::Color::fromRgba32(red, green, blue, transparency);
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
        setRotation(m_direction);

        gf::RenderStates newState;
        newState.transform = states.transform * getTransform();
        target.draw(m_arrow, newState);
    }

}