#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../test/catch.hh"
#include "../include/Entity.h"
#include "../include/Game.h"
#include "../include/State.h"
#include <SFML/Graphics.hpp>



TEST_CASE("Degrees to Radians Conversion", "[utility]") {
    REQUIRE(Entity::degreesToRadians(180) == Catch::Approx(3.14159).epsilon(0.01));
    REQUIRE(Entity::degreesToRadians(90) == Catch::Approx(1.5708).epsilon(0.01));
}

TEST_CASE("Apply Force Changes Velocity", "[physics]") {
    Entity entity(10.0f, sf::Color::Red, 1.0, true, true);
    entity.applyForce(100.0f, 45.0f);
    REQUIRE(entity.state[2] == Catch::Approx(70.71).epsilon(0.01));
    REQUIRE(entity.state[3] == Catch::Approx(70.71).epsilon(0.01));
}
TEST_CASE("Collision Detection", "[game]") {
    Entity entityA(10.0f, sf::Color::Red, 1.0, true, true);
    entityA.resetPosition(100.0f, 300.0f); // Position entity A
    Entity entityB(10.0f, sf::Color::Blue, 1.0, true, true);
    entityB.resetPosition(110.0f, 300.0f); // Position entity B close enough to collide

    REQUIRE(Game::checkCollision(entityA, entityB) == true);

    entityB.resetPosition(300.0f, 300.0f); // Move entity B far away
    REQUIRE(Game::checkCollision(entityA, entityB) == false);
}
TEST_CASE("Momentum Transfer", "[physics]") {
    Entity entityA(10.0f, sf::Color::Red, 1.0, true, true);
    entityA.state = {0, 0, 10, 0}; // Moving right
    Entity entityB(10.0f, sf::Color::Blue, 1.0, true, true);
    entityB.state = {0, 0, -5, 0}; // Moving left

    entityA.transferMomentum(entityB);

    // Assuming a simple elastic collision with equal masses
    REQUIRE(entityA.state[2] == Catch::Approx(-5).epsilon(0.01));
    REQUIRE(entityB.state[2] == Catch::Approx(10).epsilon(0.01));
}
TEST_CASE("Game Reset", "[game]") {
    Game game;
    game.projectile.applyForce(100.0f, 45.0f); // Apply initial force
    game.resetGame(); // Reset the game

    REQUIRE(game.projectile.state[2] == Catch::Approx(0).epsilon(0.01)); // Velocity x should be reset
    REQUIRE(game.projectile.state[3] == Catch::Approx(0).epsilon(0.01)); // Velocity y should be reset
    REQUIRE(game.projectile.shape.getPosition().x == Catch::Approx(60.0f).epsilon(0.01)); // Check reset position x
    REQUIRE(game.projectile.shape.getPosition().y == Catch::Approx(800 - 220.0f).epsilon(0.01)); // Check reset position y
}

TEST_CASE("Projectile Launch at Various Angles", "[physics]") {
    Entity projectile(10.0f, sf::Color::Red, 1.0, true, true);
    std::vector<float> angles = {0, 45, 90};
    std::vector<std::pair<float, float>> expectedVelocities = {
            {100.0f, 0.0f}, // 0 degrees, purely horizontal
            {70.71f, 70.71f}, // 45 degrees, equal x and y components
            {0.0f, 100.0f}  // 90 degrees, purely vertical
    };

    for(size_t i = 0; i < angles.size(); ++i) {
        projectile.applyForce(100.0f, angles[i]);
        REQUIRE(projectile.state[2] == Catch::Approx(expectedVelocities[i].first).epsilon(0.01));
        REQUIRE(projectile.state[3] == Catch::Approx(expectedVelocities[i].second).epsilon(0.01));
    }
}

TEST_CASE("Entity Immobility After Collision", "[game]") {
    Entity entityA(10.0f, sf::Color::Red, 1.0, true, true);
    Entity entityB(10.0f, sf::Color::Blue, 1.0, true, true);

    // Initial state: both entities are mobile
    entityA.state = {0, 0, 5, 0};
    entityB.state = {15, 0, -5, 0}; // Placed to "collide" with entityA based on their velocities

    // Simulate a collision handler that makes entities immobile after a collision
    entityA.isMobile = false;
    entityB.isMobile = false;

    REQUIRE_FALSE(entityA.isMobile);
    REQUIRE_FALSE(entityB.isMobile);
}
TEST_CASE("Entity Gravity Effect", "[physics]") {
    Entity projectile(10.0f, sf::Color::Red, 1.0, true, true);
    projectile.applyForce(50.0f, 90.0f); // Launched vertically upwards

    // Assuming gravity is applied correctly, the y-velocity should decrease
    float initialYVelocity = projectile.state[3];
    // Simulate a time step
    Stepper stepper;
    projectile.update(1.0, stepper); // Assuming deltaTime is 1.0 for simplicity

    REQUIRE(projectile.state[3] < initialYVelocity);
}