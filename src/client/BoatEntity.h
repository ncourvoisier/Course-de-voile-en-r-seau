#ifndef SAILINSANE_BOATENTITY_H
#define SAILINSANE_BOATENTITY_H

#include <gf/Entity.h>
#include <gf/Vector.h>
#include "../Protocol.h"

namespace sail
{

    class BoatEntity : public gf::Entity
    {
    public:
        BoatEntity(gf::Color4f color);

        ~BoatEntity() override;

        void update(gf::Time time) override;

        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

        gf::Vector2f getPosition();

        void setPosition(const gf::Vector2f &mPosition);

        gf::Vector2f getVelocity();

        void setVelocity(const gf::Vector2f &mVelocity);

        ClientBoatData getClientBoatData();

    private:
        gf::Vector2f m_position;

        gf::Vector2f m_velocity;

        float m_size;

        gf::Color4f m_color;
    };

}

#endif //SAILINSANE_BOATENTITY_H
