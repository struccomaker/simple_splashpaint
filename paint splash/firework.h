#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <SFML/Graphics.hpp>

class Firework
{
public:
    Firework() = default;

    Firework(float radius, sf::Vector2f position, sf::Vector2f velocity, sf::Color color)
        : Radius(radius)
        , Position(position)
        , Velocity(velocity)
        , Color(color)
    {}

    float Radius = 0.0f;
    sf::Vector2f Position;
    sf::Vector2f Velocity;
    sf::Color    Color;
};

//global variables
std::random_device rgen;

//functions
int randomInt(int min, int max) {
    return std::uniform_int_distribution<int>(min, max)(rgen);
}

float randomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rgen);
}


sf::Vector2f polartoCartesian(float angleDegrees, float magnitude) {
    float angleRadians = angleDegrees * (3.14159f / 180.0f);
    return sf::Vector2f(std::cos(angleRadians) * magnitude, std::sin(angleRadians) * magnitude);
}

void createExplosion(std::vector<Firework>& blast, const Firework& parent) {
    constexpr uint32_t count = 20;

    float msMin = 200.0f;
    float msMax = 450.0f;

    for (uint32_t i = 0; i < count; i++) {
        float angle = randomFloat(0.0f, 360.0f);
        float velocity = randomFloat(msMin, msMax);

        sf::Vector2f v = polartoCartesian(angle, velocity);

        Firework f;

        f.Color = parent.Color;
        f.Radius = randomFloat(6.0f, 12.0f);
        f.Position = parent.Position;
        f.Velocity = v;

        blast.push_back(f);
    }
}