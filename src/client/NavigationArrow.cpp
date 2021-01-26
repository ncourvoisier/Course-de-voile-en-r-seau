#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Unused.h>

#include "NavigationArrow.h"
#include "Singletons.h"

namespace sail
{

    NavigationArrow::NavigationArrow(gf::Vector2f target, ClientBoat& boat, gf::View& view)
    : gf::Entity(2)
    , m_target(target)
    , m_view(view)
    , m_boat(boat)
    , m_arrow(3)
    {
        m_arrow.setPoint(0, { 1, 0 });
        m_arrow.setPoint(1, { 0, 3 });
        m_arrow.setPoint(2, { 2, 3 });

        m_arrow.setColor(gf::Color::fromRgba32(180, 180, 180, 255));
    }

    void NavigationArrow::update(gf::Time time)
    {
        gf::unused(time);

        double theta = atan2(m_target.y - m_boat.getLatitude(), m_target.x - m_boat.getLongitude());
        gf::Vector2f pos = {static_cast<float>(m_view.getSize().x / 2 + (m_view.getSize().x / 2.2) * cos(theta)),
                            static_cast<float>(m_view.getSize().y / 2 +  (m_view.getSize().y / 2.2) * sin(theta))};

        m_arrow.setPosition(pos);

        m_arrow.setScale(10.0f);
        m_arrow.setRotation(theta + M_PI_2);
    }

    void NavigationArrow::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        target.draw(m_arrow, states);
    }

}