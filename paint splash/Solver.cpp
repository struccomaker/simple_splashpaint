#include "firework.h"

constexpr float gravity = 500.0f;

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "SpaceBar to clear screen", sf::Style::Close, settings);
    window.setFramerateLimit(120);

    sf::Font font;
    font.loadFromFile("OpenSans-Regular.ttf");

    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(36); 
    fpsText.setFillColor(sf::Color::Red);
    fpsText.setPosition(10, 10);

    sf::Event event;

    sf::Clock fpsClock;
    unsigned int frameCount = 0;
    sf::Clock deltaClock;
    float dt = 1.0f / 60.0f; // 1/60 seconds

    //this is to render texture for the fireworks
    sf::RenderTexture fadeTexture;
    fadeTexture.create(1600, 900);

    sf::CircleShape blastBoom; // this represents the blast
    blastBoom.setRadius(50);
    blastBoom.setPointCount(100);

    std::vector<Firework> Fireworks; // store alr drawn fireworks
    std::vector<Firework> FireWorksAftermath; // store the effects of splash

    bool MousePressed = false;
    bool SpacePressed = false;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(sf::Mouse::getPosition(window)); //mouse pos relative to current active window

                    Fireworks.emplace_back
                    (
                        (float)randomInt(12, 16),
                        sf::Vector2f(mousePos.x, mousePos.y), //  use mouse position without offset
                        sf::Vector2f{},
                        sf::Color((sf::Uint8)randomInt(120, 255), (sf::Uint8)randomInt(120, 255), (sf::Uint8)randomInt(120, 255))
                    );

                    MousePressed = true;
                }
                break; //these needs to go both ways
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Space) {
                    Fireworks.clear();
                    FireWorksAftermath.clear();
                    fadeTexture.clear();
                    SpacePressed = true; 
                }
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Space) {
                    SpacePressed = false; 
                }
                break;
            }
        }

        window.clear();

        if (!SpacePressed) {
            static float fadeElapsedTime = 0.0f;
            fadeElapsedTime += dt;

            if (!Fireworks.empty()) { //if not empty we loop and continusely draw
                for (size_t i = 0; i < Fireworks.size(); i++) {
                    auto& current = Fireworks[i];

                    current.Velocity.y += gravity * dt;

                    if (current.Velocity.y > 0.0f) {
                        createExplosion(FireWorksAftermath, current);

                        Fireworks.erase(Fireworks.begin() + i);
                        i--;
                        continue;
                    }

                    current.Position += current.Velocity * dt;

                    blastBoom.setRadius(current.Radius);
                    blastBoom.setFillColor(current.Color);
                    blastBoom.setOrigin(sf::Vector2f(current.Radius, current.Radius));
                    blastBoom.setPosition(current.Position);

                    fadeTexture.draw(blastBoom);
                }
            }

            for (size_t i = 0; i < FireWorksAftermath.size(); i++) { // this is for the blast effect
                auto& current = FireWorksAftermath[i];

                current.Velocity.y += gravity * dt;

                current.Position += current.Velocity * dt;

                current.Radius -= 10.0f * dt;
                if (current.Radius <= 1.0f) {
                    FireWorksAftermath.erase(FireWorksAftermath.begin() + i);
                    i--;
                    continue;
                }

                blastBoom.setRadius(current.Radius);
                blastBoom.setFillColor(current.Color);
                blastBoom.setOrigin(sf::Vector2f(current.Radius, current.Radius));
                blastBoom.setPosition(current.Position);

                fadeTexture.draw(blastBoom);
            }

            fadeTexture.display();

            window.draw(sf::Sprite(fadeTexture.getTexture()));
        }

        //this is for fps kinda whack imo
        frameCount++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            float fps = static_cast<float>(frameCount) / fpsClock.restart().asSeconds();
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

            frameCount = 0;
        }
        window.draw(fpsText);

        window.display();

        dt = deltaClock.restart().asSeconds();

     
    }
    return 0;
}
