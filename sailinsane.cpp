#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Window.h>
#include <gf/ResourceManager.h>
#include <gf/Shapes.h>
#include <gf/Packet.h>

#include <iostream>

#include "src/Protocol.h"

class Square {
public:
    Square(gf::Vector2f position, float size, gf::Color4f color)
            : m_position(position)
            , m_velocity(0, 0)
            , m_size(size)
            , m_color(color)
    {
    }
    void setVelocity(gf::Vector2f velocity) {
        m_velocity = velocity;
    }
    void update(float dt) {
        m_position += dt * m_velocity;
    }
    void render(gf::RenderTarget& target) {
        gf::RectangleShape shape({ m_size, m_size });
        shape.setPosition(m_position);
        shape.setColor(m_color);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape);
    }
private:
    gf::Vector2f m_position; // center of the square
    gf::Vector2f m_velocity;
    float m_size;
    gf::Color4f m_color;
};

int main() {
    if (__cplusplus == 201703L) std::cout << "C++17\n";
    else if (__cplusplus == 201402L) std::cout << "C++14\n";
    else if (__cplusplus == 201103L) std::cout << "C++11\n";
    else if (__cplusplus == 199711L) std::cout << "C++98\n";
    else std::cout << "pre-standard C++\n";

    // Create the main window and the renderer
    gf::Window window("Example", { 640, 480 });
    gf::RenderWindow renderer(window);
    // Creating a resources manager
    gf::ResourceManager resources;
    resources.addSearchDir("../resources");

    // Load a sprite to display
    gf::Texture& texture = resources.getTexture("world.png");
    gf::Sprite sprite(texture);
    sprite.setPosition({ 300, 200 });
    // Create a graphical text to display
    gf::Font& font = resources.getFont("arial.ttf");
    gf::Text text("Hello gf!", font, 50);
    text.setPosition({ 100, 100 });
    renderer.clear(gf::Color::White);
    // Init the game clock
    gf::Clock clock;
    // Init an entity
    static constexpr gf::Vector2u ScreenSize(500, 500);
    Square entity(ScreenSize / 2, 50.0f, gf::Color::Red);
    // Init speed and velocity
    static constexpr float Speed = 100.0f;
    gf::Vector2f velocity(0, 0);
    // Start the game loop

    bool x_accelerate {false};
    bool y_accelerate {false};

    while (window.isOpen()) {
        // Process events
        gf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case gf::EventType::Closed:
                    window.close();
                    break;
                case gf::EventType::KeyPressed:
                    switch (event.key.keycode) {
                        case gf::Keycode::Up:
                            velocity.y -= Speed;
                            y_accelerate = true;
                            break;
                        case gf::Keycode::Down:
                            velocity.y += Speed;
                            y_accelerate = true;
                            break;
                        case gf::Keycode::Left:
                            velocity.x -= Speed;
                            x_accelerate = true;
                            break;
                        case gf::Keycode::Right:
                            velocity.x += Speed;
                            x_accelerate = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case gf::EventType::KeyReleased:
                    switch (event.key.keycode) {
                        case gf::Keycode::Up:
                        case gf::Keycode::Down:
                            y_accelerate = false;
                            break;
                        case gf::Keycode::Left:
                        case gf::Keycode::Right:
                            x_accelerate = false;
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        // Simulate simple inertia :
        if (!y_accelerate)
            velocity.y *= 0.95f;
        if (!x_accelerate)
            velocity.x *= 0.95f;
        if (abs(velocity.x) < (0.01f * Speed))
            velocity.x = 0;
        if (abs(velocity.y) < (0.01f * Speed))
            velocity.y = 0;
        //
        entity.setVelocity(velocity);
        // Get the dt and update entity
        float dt = clock.restart().asSeconds();
        entity.update(dt);
        // Draw the entity
        renderer.clear();
        entity.render(renderer);
        renderer.display();
    }
    return 0;
}
