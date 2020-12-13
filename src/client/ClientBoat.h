#ifndef SAILINSANE_CLIENTBOAT_H
#define SAILINSANE_CLIENTBOAT_H

#include <gf/Entity.h>
#include <gf/Vector.h>
#include <gf/Texture.h>
#include "../Protocol.h"
#include "../physics/Boat.h"

namespace sail
{

    class ClientBoat : public Boat, public gf::Entity
    {
    public:
        ClientBoat(gf::Color4f color);

        ~ClientBoat() override;

        void update(gf::Time time) override;

        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

        void fromBoatData(BoatData data);

    private:
        static constexpr int Scale = 100000;

        const gf::Texture &m_texture;

        gf::RectF m_boat;
        gf::RectF m_sail;
        gf::RectF m_rudder;
    };

}

#endif //SAILINSANE_CLIENTBOAT_H