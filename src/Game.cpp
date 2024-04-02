//
// Created by Jubair on 2024-03-25.
//
#include "/Users/jubair/CLionProjects/untitled/include/Game.h"
#include "/Users/jubair/CLionProjects/untitled/include/Entity.h"
#include "/Users/jubair/CLionProjects/untitled/include/Physics.h"
#include <iostream>
#include <cmath>
#include <boost/numeric/odeint.hpp>
#include <vector>
#include <thread>

using State = std::array<double, 4>; // x, y, vx, vy
using Stepper = boost::numeric::odeint::runge_kutta4<State>;

const int WINDOW_WIDTH = static_cast<int>(800 * 1.30);
const int WINDOW_HEIGHT = static_cast<int>(600 * 1.30);
// Static variables initialization
float Game::launchAngleDegrees = 45.0f;
float Game::force = 1000.0f;
float trajectoryPoints=60;
float launchLength=1000.0f;
Game::Game(): window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Projectile Simulation"),
        // Assign mass of 1.0 to the bird (projectile)
         projectile(20.0f, sf::Color::Red, 1.0, true, true) {
    trajectoryLine.setPrimitiveType(sf::LinesStrip);
    launchArrow.setSize(sf::Vector2f(50.0f, 5.0f));
    launchArrow.setFillColor(sf::Color::White);
    launchArrow.setOrigin(0, 2.5f); // Origin at the base, center for rotation
    resetGame();
}


void Game::resetGame() {
    projectile.resetPosition(60.0f, WINDOW_HEIGHT - 200.0f);
    projectile.isMobile = false;
    birdResetCounter = 0;
    // Initialize targets with positions and add them to the vector
    targets.emplace_back(20.0f, sf::Color::Green, 2.0, false, false);
    targets.back().resetPosition(WINDOW_WIDTH - 200.0f, WINDOW_HEIGHT - 200.0f);

    targets.emplace_back(20.0f, sf::Color::Green, 2.0, false, false);
    targets.back().resetPosition(WINDOW_WIDTH - 150.0f, WINDOW_HEIGHT - 200.0f);

    targets.emplace_back(20.0f, sf::Color::Green, 2.0, false, false);
    targets.back().resetPosition(WINDOW_WIDTH - 250.0f, WINDOW_HEIGHT - 200.0f);
    force = 1000.0f; // Reset force/speed to default
    launchAngleDegrees = 45.0f; // Reset angle to default
    simulateTrajectory(launchAngleDegrees, force);
    updateLaunchArrow(launchAngleDegrees, force);
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
                handleKeyPress(event.key.code);
        }
        update();
        render();
    }
}

// In the Game class, adjust the simulation and rendering to reflect changes in force and angle
void Game::handleKeyPress(sf::Keyboard::Key key) {
    float forceAdjustment = 100.0f; // Adjust this value as needed for the game's scale
    float angleAdjustment = 5.0f;
    float angleAdjustmentSub=0.5f;

    if (key == sf::Keyboard::Up && launchAngleDegrees < 90) {
        launchAngleDegrees += angleAdjustment;
    } else if (key == sf::Keyboard::Down && launchAngleDegrees > 0) {
        launchAngleDegrees -= angleAdjustment;
    } else if (key == sf::Keyboard::D && launchAngleDegrees < 90) {
        launchAngleDegrees += angleAdjustmentSub;
    } else if (key == sf::Keyboard::C && launchAngleDegrees > 0) {
        launchAngleDegrees -= angleAdjustmentSub;
    } else if (key == sf::Keyboard::Right) {
        force += forceAdjustment;
    } else if (key == sf::Keyboard::Left && force > forceAdjustment) {
        force -= forceAdjustment;
    } else if (key == sf::Keyboard::Space && !projectile.isMobile) {
        projectile.applyForce(force, launchAngleDegrees);
    } else if (key == sf::Keyboard::R) {
        resetGame();
    } else if (key == sf::Keyboard::Q) { // Quit the game
        window.close();
    }
    // Recalculate trajectory with the updated force and angle
    simulateTrajectory(launchAngleDegrees, force);
    updateLaunchArrow(launchAngleDegrees, force);
}

// Update the `update` method to use the modified checkCollision method correctly
void Game::update() {
    float deltaTime = clock.restart().asSeconds();
    projectile.update(deltaTime, stepper);

    // Reset collision flags before collision detection
    projectile.collisionFlag = false;
    for (auto &target : targets) {
        target.collisionFlag = false;
    }

    // Check for collision between projectile and each target
    for (auto &target : targets) {
        if (checkCollision(projectile, target)) {
            printVelocities("Before", projectile, target);
            projectile.transferMomentum(target);
            printVelocities("After", projectile, target);

            target.isMobile = true;
        }
        target.update(deltaTime, stepper);
    }

    // Check for collisions among targets
    for (size_t i = 0; i < targets.size(); ++i) {
        for (size_t j = i + 1; j < targets.size(); ++j) {
            if (checkCollision(targets[i], targets[j])) {
                printVelocities("Before", targets[i], targets[j]);
                targets[i].transferMomentum(targets[j]);
                printVelocities("After", targets[i], targets[j]);

                targets[i].isMobile = true;
                targets[j].isMobile = true;
            }
        }
    }

    // Reset bird if it goes out of bounds, with limited resets
    if (isOutOfWindow(projectile) && birdResetCounter < maxBirdResets) {
        resetBirdPosition();
        birdResetCounter++;
    }
}

bool Game::isOutOfWindow(const Entity& entity) {
    // Check if entity is outside the window bounds
    auto pos = entity.shape.getPosition();
    return pos.x < 0 || pos.x > WINDOW_WIDTH || pos.y < 0 || pos.y > WINDOW_HEIGHT;
}

void Game::resetBirdPosition() {
    // Reset the bird to its initial position and make it immobile
    projectile.resetPosition(60.0f, WINDOW_HEIGHT - 200.0f);
    projectile.isMobile = false;
}


void Game::simulateTrajectory(float angleDegrees, float speed) {
    trajectoryLine.clear();
    State tempState = projectile.state;
    float angleRadians = Entity::degreesToRadians(angleDegrees);
    tempState[2] = std::cos(angleRadians) * speed; // Set initial horizontal velocity
    tempState[3] = std::sin(angleRadians) * speed; // Set initial vertical velocity

    // Simulate the trajectory for a set number of steps or until a break condition is met
    for (size_t i = 0; i < trajectoryPoints; ++i) { // Increase iteration count for longer trajectory
        stepper.do_step(projectileSystem, tempState, 0, 0.02); // Decrease time step for finer trajectory line

        // Continue appending points even if they go out of the window bounds
        trajectoryLine.append(sf::Vertex(sf::Vector2f(static_cast<float>(tempState[0]), WINDOW_HEIGHT - static_cast<float>(tempState[1])), sf::Color::Yellow));

        // Optional: Break if the projectile goes too far out of bounds (to avoid infinite trajectory)
        if (tempState[0] > WINDOW_WIDTH * 2 || tempState[1] > WINDOW_HEIGHT * 2 ||
            tempState[0] < -WINDOW_WIDTH || tempState[1] < -WINDOW_HEIGHT) {
            break;
        }
    }
}

// Modify the checkCollision method to accept two Entity objects and return a bool
bool Game::checkCollision(const Entity& a, const Entity& b) {
    float dx = a.shape.getPosition().x - b.shape.getPosition().x;
    float dy = a.shape.getPosition().y - b.shape.getPosition().y;
    float distance = sqrt(dx * dx + dy * dy);

    return distance < (a.shape.getRadius() + b.shape.getRadius());
}

// Adjust the launch arrow orientation to match the launch direction
void Game::updateLaunchArrow(float angleDegrees, float force) {
    launchArrow.setPosition(projectile.shape.getPosition());
    // Angle adjustment might be needed to align with your coordinate system
    launchArrow.setRotation( -angleDegrees);
    float lengthScale = force / launchLength;
    launchArrow.setSize(sf::Vector2f(50.0f * lengthScale, launchArrow.getSize().y));
}

void Game::render() {
    window.clear();
    // Draw the projectile and launch arrow if the projectile is not mobile
    if (!projectile.isMobile) {
        window.draw(trajectoryLine);
        window.draw(launchArrow);
    }
    window.draw(projectile.shape);
    // Draw each target by iterating through the targets vector
    for (auto& target : targets) {
        window.draw(target.shape);
    }
    window.display();
}

// Utility function to print velocities of two entities
void Game::printVelocities(const std::string& phase, const Entity& a, const Entity& b) {
    std::cout << phase << " collision: "
              << "Entity A vx = " << a.state[2] << ", vy = " << a.state[3]
              << " | Entity B vx = " << b.state[2] << ", vy = " << b.state[3] << std::endl;
}
