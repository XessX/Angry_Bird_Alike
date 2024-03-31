/**
 * @file Entity.h
 * @brief Represents a physical entity in the simulation.
 *
 * This class encapsulates all the properties of a physical entity,
 * including its mass, position, velocity, and provides methods to manipulate these properties.
 */

//
// Created by Jubair on 2024-03-25.
//

#ifndef UNTITLED_ENTITY_H
#define UNTITLED_ENTITY_H

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>
#include <boost/numeric/odeint.hpp>
#include "/Users/jubair/CLionProjects/untitled/include/Physics.h"
#include <vector>

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
 * @class Entity
 * @brief Represents an entity in the game, such as a projectile or target.
 */
class Entity {
public:
    /**
 * Constructs an Entity with specified properties.
 *
 * @param radius The radius of the entity.
 * @param color The color of the entity.
 * @param m The mass of the entity.
 * @param mobile Indicates if the entity is mobile.
 * @param gravity Indicates if the entity is affected by gravity.
 */

    Entity(float radius, sf::Color color, double m, bool mobile, bool gravity);
    /**
  * Applies a force to the entity, modifying its velocity.
  *
  * @param forceMagnitude The magnitude of the force.
  * @param angleDegrees The direction of the force in degrees.
  */
    void applyForce(float forceMagnitude, float angleDegrees);
    /**
 * Updates the entity's position based on its velocity.
 *
 * @param deltaTime The time elapsed since the last update.
 * @param stepper The numerical stepper used for integration.
 */
    void update(float deltaTime, Stepper& stepper);
    /**
 * Resets the entity's position to specified coordinates.
 *
 * @param x The x-coordinate of the new position.
 * @param y The y-coordinate of the new position.
 */
    void resetPosition(float x, float y);
    /**
 * Transfers momentum between this entity and another during a collision.
 *
 * @param other The other entity involved in the collision.
 */
    bool collisionFlag = false; // New flag to indicate a recent collision
    void transferMomentum(Entity &other);

    // Member variables are typically documented where they are declared.
    sf::CircleShape shape; ///< The shape representing the entity in the simulation.
    State state; ///< The current state of the entity (x, y, vx, vy).
    bool isMobile; ///< Flag indicating if the entity can move.
    bool affectedByGravity; ///< Flag indicating if the entity is affected by gravity.
    double mass; ///< The mass of the entity.
    /**
 * @brief Converts an angle from degrees to radians.
 * @param degrees The angle in degrees to convert.
 * @return The angle converted to radians.
 */
    static float degreesToRadians(float degrees) {
        return degrees * PI / 180.0f;
        }

};

#endif // ENTITY_HPP

#endif //UNTITLED_ENTITY_H
