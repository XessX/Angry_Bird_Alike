//
// Created by Jubair on 2024-03-25.
//

#include "/Users/jubair/CLionProjects/untitled/include/Game.h"
#include "/Users/jubair/CLionProjects/untitled/include/Entity.h"
#include "/Users/jubair/CLionProjects/untitled/include/Physics.h"
#include <boost/numeric/odeint.hpp>
#include <cmath>
#include <boost/numeric/odeint.hpp>
#include <vector>

using State = std::array<double, 4>; // x, y, vx, vy
using Stepper = boost::numeric::odeint::runge_kutta4<State>;

const int WINDOW_WIDTH = static_cast<int>(800 * 1.30);
const int WINDOW_HEIGHT = static_cast<int>(600 * 1.30);
/**
 * Constructs an Entity with specified properties.
 * @param radius The radius of the entity.
 * @param color The color of the entity.
 * @param m The mass of the entity.
 * @param mobile Indicates if the entity is mobile.
 * @param gravity Indicates if the entity is affected by gravity.
 */
Entity::Entity(float radius, sf::Color color, double m, bool mobile, bool gravity)
        : mass(m), isMobile(mobile), affectedByGravity(gravity), state({0, 0, 0, 0}) {
    shape.setRadius(radius);
    shape.setFillColor(color);
    shape.setOrigin(radius, radius);
}
/**
 * Applies a force to the entity, modifying its velocity based on the force magnitude and direction.
 * The force is directly converted to initial velocity components using trigonometric calculations.
 */
void Entity::applyForce(float forceMagnitude, float angleDegrees) {
    // Directly convert force to initial speed
    float initialSpeed = forceMagnitude;
    float angleRadians = degreesToRadians(angleDegrees);

    // Calculate velocity components based on the angle
    state[2] = std::cos(angleRadians) * initialSpeed; // vx
    state[3] = std::sin(angleRadians) * initialSpeed; // vy

    isMobile = true;

    // Print the calculated initial velocity
    std::cout << "Launched with velocity: vx = " << state[2] << ", vy = " << state[3] << std::endl;
}
/**
 * Updates the entity's position based on its velocity.
 * Uses the numerical integration stepper to compute the new state of the entity over time.
 */
void Entity::update(float deltaTime, Stepper& stepper) {
    if (isMobile) {
        // Update position based on velocity
        stepper.do_step(projectileSystem, state, 0, deltaTime);
        // Convert from simulation coordinates to SFML coordinates if necessary
        shape.setPosition(static_cast<float>(state[0]), WINDOW_HEIGHT - static_cast<float>(state[1]));
    }
}
/**
 * Resets the entity's position to specified coordinates, making it immobile and unaffected by gravity.
 */
void Entity::resetPosition(float x, float y) {
    shape.setPosition(x, y);
    state = {x, WINDOW_HEIGHT - y, 0.0, 0.0};
    isMobile = false;
    affectedByGravity = false;
}
/**
 * Transfers momentum between this entity and another during a collision, following the laws of conservation of momentum.
 * Assumes a perfectly elastic collision for simplicity.
 */
void Entity::transferMomentum(Entity &other) {
    // Elastic collision equations for final velocities
    double m1 = this->mass, m2 = other.mass;
    double v1x_initial = this->state[2], v2x_initial = other.state[2];
    double v1y_initial = this->state[3], v2y_initial = other.state[3];

    double v1x_final = ((m1 - m2) * v1x_initial + 2 * m2 * v2x_initial) / (m1 + m2);
    double v2x_final = ((m2 - m1) * v2x_initial + 2 * m1 * v1x_initial) / (m1 + m2);

    double v1y_final = ((m1 - m2) * v1y_initial + 2 * m2 * v2y_initial) / (m1 + m2);
    double v2y_final = ((m2 - m1) * v2y_initial + 2 * m1 * v1y_initial) / (m1 + m2);

    // Update the velocities of both entities
    this->state[2] = v1x_final;
    this->state[3] = v1y_final;
    other.state[2] = v2x_final;
    other.state[3] = v2y_final;

    // Ensure velocities are not set to 0 due to rounding or precision issues
    if (std::abs(this->state[2]) < 0.001) this->state[2] = 0;
    if (std::abs(this->state[3]) < 0.001) this->state[3] = 0;
    if (std::abs(other.state[2]) < 0.001) other.state[2] = 0;
    if (std::abs(other.state[3]) < 0.001) other.state[3] = 0;
}