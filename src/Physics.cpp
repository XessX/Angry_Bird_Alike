//
// Created by Jubair on 2024-03-25.
//
#include "../include/Physics.h"
#include <boost/numeric/odeint.hpp>
#include <vector>

using State = std::array<double, 4>; // x, y, vx, vy
using Stepper = boost::numeric::odeint::runge_kutta4<State>;

// Physics system for the projectile
void projectileSystem(const State &state, State &dstate_dt, const double) {
    dstate_dt[0] = state[2]; // dx/dt = vx
    dstate_dt[1] = state[3]; // dy/dt = vy
    dstate_dt[2] = 0;        // dvx/dt = 0
    dstate_dt[3] = GRAVITY;  // dvy/dt = gravity
}
