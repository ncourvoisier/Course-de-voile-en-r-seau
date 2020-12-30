#ifndef SAILINSANE_TERRAIN_H
#define SAILINSANE_TERRAIN_H

#include <gf/Array2D.h>
#include <gf/Entity.h>
#include <gf/VertexArray.h>

#include "../Constants.h"

#include "ClientBoat.h"
#include "WindArrow.h"

namespace sail
{

    class Terrain: public gf::Entity
    {
    public:
        static constexpr unsigned DisplayHalfRange = 100;
        static constexpr float TileSize = TileDegree * WorldScale; // 5.0f

        Terrain(ClientBoat& playerBoat);

        void load(gf::Array2D<float> elevations, gf::Array2D<float> windD, gf::Array2D<float> windS);

        void setFullRender(bool fullRender);

        void update(gf::Time time) override;

        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
        gf::Array2D<gf::Color4f> m_terrain;
        gf::Array2D<float> m_windDirection;
        gf::Array2D<float> m_windSpeed;

        gf::VertexArray m_vertices;

        std::vector<WindArrow> m_arrows;

        gf::Vector2u m_oldPosition;

        ClientBoat& m_playerBoat;

        bool m_fullRender;
    };

}

#endif //SAILINSANE_TERRAIN_H
