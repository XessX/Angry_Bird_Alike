/**
 * @file Physics.h
 * @brief Physics system for projectiles, including constants and basic physics operations.
 *
 * Defines the physics system responsible for updating the state of entities in the simulation.
 * It includes fundamental physics constants and the function prototype for the projectile system.
 */

//
// Created by Jubair on 2024-03-25.
//

#ifndef UNTITLED_PHYSICS_H
#define UNTITLED_PHYSICS_H

#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "/Users/jubair/CLionProjects/untitled/include/State.h"

const float PI = 3.14159265f; ///< The value of pi.
const float GRAVITY = -981.0f; ///< Acceleration due to gravity (cm/s^2).

class Entity;

/**
 * @brief Updates the derivative of the state for projectile motion.
 * @param state The current state of the entity.
 * @param dstate_dt The derivative of the state to be updated.
 * @param time The current time step for the simulation.
 */
// Physics system for the projectile
void projectileSystem(const State &state, State &dstate_dt, double time);
#endif // PHYSICS_HPP

#endif //UNTITLED_PHYSICS_H
