#ifndef SAILINSANE_CLIENTBOAT_H
#define SAILINSANE_CLIENTBOAT_H

#include <gf/Entity.h>
#include <gf/Vector.h>
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
        static constexpr int Scale = 150000;

        float m_size;

        gf::Color4f m_color;
    };

}

#endif //SAILINSANE_CLIENTBOAT_H

// serveur authoritaire
// intégration physique
//