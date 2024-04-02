/**
 * @file Game.h
 * @brief Main game interface for handling game logic and rendering.
 *
 * This file defines the Game class, which is responsible for managing the game loop,
 * handling user input, simulating physics, and rendering entities and other game elements.
 */

//
// Created by Jubair on 2024-03-25.
//

#ifndef UNTITLED_GAME_H
#define UNTITLED_GAME_H

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "/Users/jubair/CLionProjects/untitled/include/Entity.h"
#include <boost/numeric/odeint.hpp>
#include <vector>
#include <thread>

/**
 * @brief Defines the state of an entity within the game.
 *
 * This type is used to represent the position and velocity of entities.
 */
using State = std::array<double, 4>; // Replace with the actual line 29 content.

/**
 * @brief Represents the numerical stepper used for physics integration within the game.
 *
 * This stepper is utilized to advance the state of entities based on the physics simulation.
 */
using Stepper = boost::numeric::odeint::runge_kutta4<State>; // Replace with the actual line 35 content.

/**
 * @class Game
 * @brief Manages the game logic, user input, and rendering.
 *
 * This class is responsible for initializing the game, handling user input,
 * updating game state, and rendering graphics to the screen.
 */
class Game {
public:

/**
 * @brief Constructs a new Game instance.
 */
    Game();
/**
 * @brief Constructs a new Game instance with specified settings.
 * @param maxBirdResets The maximum number of times the bird can be reset in the game.
 */
    Game(const int maxBirdResets);
    /**
 * Runs the main game loop.
 */
    void run();
    /**
 * Handles key press events within the game.
 *
 * @param key The key that was pressed.
 */
    void handleKeyPress(sf::Keyboard::Key key);

    /**
     * Updates the game logic.
     */
    void update();
    /**
  * Checks for collision between two entities.
  *
  * @param a Reference to the first entity.
  * @param b Reference to the second entity.
  * @return True if a collision is detected, false otherwise.
  */
    static bool checkCollision(const Entity& a, const Entity& b);
    /**
 * Simulates the trajectory of the projectile based on the current force and angle.
 *
 * @param degrees The launch angle in degrees.
 * @param speed The launch speed.
 */
    void simulateTrajectory(float degrees, float speed);
    /**
 * Updates the orientation and size of the launch arrow based on the launch angle and force.
 *
 * @param angleDegrees The launch angle in degrees.
 * @param force The force magnitude.
 */
    void updateLaunchArrow(float angleDegrees, float force);
    /**
 * Renders all game elements to the window.
 */
    void render();
    /**
 * Resets the game to its initial state.
 */
    void resetGame();
    /**
   * Resets the position of the bird to its initial state.
   */
    void resetBirdPosition();
    /**
 * Checks if an entity is out of the window bounds.
 *
 * @param entity The entity to check.
 * @return True if the entity is out of bounds, false otherwise.
 */
    static bool isOutOfWindow(const Entity& entity);
    /**
 * Prints the velocities of two entities, typically before and after a collision.
 *
 * @param phase Describes the phase of collision (e.g., "Before", "After").
 * @param a The first entity.
 * @param b The second entity.
 */
    static void printVelocities(const std::string& phase, const Entity& a, const Entity& b);

    static float launchAngleDegrees;
    static float force;

    sf::RenderWindow window; ///< The main window where the game is rendered.
    Entity projectile; ///< The projectile entity (e.g., bird in a slingshot game).
    std::vector<Entity> targets; ///< Targets that the projectile can hit.
    Stepper stepper; ///< Numerical stepper for physics integration.
    sf::VertexArray trajectoryLine; ///< Visual representation of the projectile's trajectory.
    sf::RectangleShape launchArrow; ///< Visual representation of the launch direction and force.
    sf::Clock clock; ///< Clock for measuring time between frames.
    int birdResetCounter = 0; ///< Counts how many times the bird has been reset.
    const int maxBirdResets=2; ///< Maximum number of times the bird can be reset.

};

#endif // GAME_HPP


#endif //UNTITLED_GAME_H
