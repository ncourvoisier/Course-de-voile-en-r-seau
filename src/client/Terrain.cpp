#include "Terrain.h"

#include <gf/ColorRamp.h>
#include <gf/Unused.h>
#include <gf/RenderTarget.h>
#include <iostream>
#include <gf/Shape.h>
#include <gf/Polygon.h>
#include <gf/Shapes.h>

namespace sail
{

    Terrain::Terrain(ClientBoat& playerBoat)
    : m_playerBoat(playerBoat)
    , m_terrain({MapSize, MapSize})
    , m_oldPosition({0, 0})
    , m_fullRender(false)
    , m_vertices(gf::PrimitiveType::Triangles)
    , m_arrows()
    {
        m_endingSpot = gf::CircleShape(50);
        m_endingSpot.setAnchor(gf::Anchor::Center);
        m_endingSpot.setOutlineThickness(3);
        m_endingSpot.setColor(gf::Color::fromRgba32(0, 180, 30, 160));
        m_endingSpot.setOutlineColor(gf::Color::fromRgba32(0, 140, 30, 230));
    }

    void Terrain::load(gf::Array2D<float> elevations, gf::Array2D<float> windD, gf::Array2D<float> windS, gf::Vector2d end)
    {
        gf::ColorRamp rampTerrain;
        rampTerrain.addColorStop(0.250f, gf::Color::fromRgba32(  9,  62,  92)); // Deep Water
        rampTerrain.addColorStop(0.499f, gf::Color::fromRgba32(17, 82, 118)); // Water
        rampTerrain.addColorStop(0.500f, gf::Color::fromRgba32(255, 251, 121)); // Sand
        rampTerrain.addColorStop(0.550f, gf::Color::fromRgba32(255, 251, 121)); // Sand
        rampTerrain.addColorStop(1.000f, gf::Color::fromRgba32(38, 143, 14)); // Ground

        for (auto row : elevations.getRowRange())
        {
            for (auto col : elevations.getColRange())
            {
                float elevation = elevations({ row, col });
                m_terrain({ row, col }) = rampTerrain.computeColor(elevation);
            }
        }

        m_elevations = elevations;
        m_windDirection = windD;
        m_windSpeed = windS;

        m_endingPos = end;
        m_endingSpot.setPosition(m_endingPos * WorldScale);
    }

    void Terrain::setFullRender(bool fullRender)
    {
        m_fullRender = fullRender;
    }

    void Terrain::update(gf::Time time)
    {
        gf::unused(time);

        auto newCol = static_cast<unsigned>(m_playerBoat.getLongitude() / TileDegree);
        auto newRow = static_cast<unsigned>(m_playerBoat.getLatitude() / TileDegree);

        if (m_oldPosition.x == newCol && m_oldPosition.y == newRow)
            return;

        m_oldPosition = { newCol, newRow };

        m_vertices.clear();

        if (m_fullRender)
        {
            for (unsigned row = 0; row < MapSize; ++row)
            {
                for (unsigned col = 0; col < MapSize; ++col)
                {
                    gf::Vertex vertices[4];

                    vertices[0].position = { col * 1.0f,row * 1.0f };
                    vertices[1].position = {(col + 1) * 1.0f,row * 1.0f };
                    vertices[2].position = {col * 1.0f,(row + 1) * 1.0f };
                    vertices[3].position = {(col + 1) * 1.0f,(row + 1) * 1.0f };

                    vertices[0].color = m_terrain({ row, col });
                    vertices[1].color = m_terrain({ row,col + 1 });
                    vertices[2].color = m_terrain({row + 1, col });
                    vertices[3].color = m_terrain({row + 1,col + 1 });

                    // first triangle
                    m_vertices.append(vertices[0]);
                    m_vertices.append(vertices[1]);
                    m_vertices.append(vertices[2]);

                    // second triangle
                    m_vertices.append(vertices[2]);
                    m_vertices.append(vertices[1]);
                    m_vertices.append(vertices[3]);
                }
            }

            return;
        }

        unsigned rowMin = (newRow > DisplayHalfRange) ? (newRow - DisplayHalfRange) : 0;
        unsigned rowMax = (newRow + DisplayHalfRange < MapSize) ? (newRow + DisplayHalfRange) : MapSize - 1;
        unsigned colMin = (newCol > DisplayHalfRange) ? (newCol - DisplayHalfRange) : 0;
        unsigned colMax = (newCol + DisplayHalfRange < MapSize) ? (newCol + DisplayHalfRange) : MapSize - 1;

        //std::cout << "rowMin : " << rowMin << ", rowMax : " << rowMax << ", colMin : " << colMin << ", colMax : " << colMax << "\n";

        m_arrows.clear();

        for (unsigned row = rowMin; row < rowMax; ++row)
        {
            for (unsigned col = colMin; col < colMax; ++col)
            {
                gf::Vertex vertices[4];

                vertices[0].position = {col * TileSize,row * TileSize };
                vertices[1].position = {(col + 1) * TileSize,row * TileSize };
                vertices[2].position = {col * TileSize,(row + 1) * TileSize };
                vertices[3].position = {(col + 1) * TileSize,(row + 1) * TileSize };

                vertices[0].color = m_terrain({ row, col });
                vertices[1].color = m_terrain({ row,col + 1 });
                vertices[2].color = m_terrain({row + 1, col });
                vertices[3].color = m_terrain({row + 1,col + 1 });

                // first triangle
                m_vertices.append(vertices[0]);
                m_vertices.append(vertices[1]);
                m_vertices.append(vertices[2]);

                // second triangle
                m_vertices.append(vertices[2]);
                m_vertices.append(vertices[1]);
                m_vertices.append(vertices[3]);

                if (row % 10 == 0 && col % 10 == 0 && m_elevations({ row, col }) < 0.5f) // TODO : temporary, need to pick the radius more wisely, otherwise most triangle will be invisible
                {
                    WindArrow arrow(m_windSpeed({ row, col }),
                            m_windDirection({ row, col }),
                            sqrt(pow(abs(newRow - row), 2) + pow(abs(newCol - col), 2)));
                    arrow.setPosition({ col * TileSize, row * TileSize });
                    m_arrows.push_back(arrow);
                }
            }
        }

        m_showEnding = gf::euclideanDistance({static_cast<float>(m_playerBoat.getLongitude()),
                                            static_cast<float>(m_playerBoat.getLatitude())}, m_endingPos) < 0.02;
    }

    void Terrain::render(gf::RenderTarget &target, const gf::RenderStates& states)
    {
        target.draw(m_vertices, states);
        for (auto& arrow : m_arrows)
        {
            target.draw(arrow);
        }
        if (m_showEnding)
            target.draw(m_endingSpot);
    }

    gf::Array2D<float>& Terrain::getWindDirectionArray()
    {
        return m_windDirection;
    }

    gf::Array2D<float>& Terrain::getWindSpeedArray()
    {
        return m_windSpeed;
    }

}