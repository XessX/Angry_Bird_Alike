/**
 * @file State.h
 * @brief Defines the State type used throughout the simulation.
 *
 * This header file introduces a State type, representing the fundamental state (position and velocity)
 * of entities within the simulation. It is utilized across various components of the project for
 * tracking and updating entity states.
 */

//
// Created by Jubair on 2024-03-25.
//

#ifndef UNTITLED_STATE_H
#define UNTITLED_STATE_H
#include <array>
#include <thread>
/**
 * Represents the state of an entity in terms of its position and velocity.
 * The state is stored as an array of doubles, where indices correspond to:
 * 0: x position, 1: y position, 2: x velocity, 3: y velocity.
 */
using State = std::array<double, 4>; // x, y, vx, vy
#endif //UNTITLED_STATE_H
