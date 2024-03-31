/*
 * lorenz_gsl.hpp
 *
 *  Created on: May 12, 2011
 *      Author: mario
 */

#ifndef LORENZ_GSL_HPP_
#define LORENZ_GSL_HPP_

#include <gsl/gsl_matrix.h>

int lorenz_gsl( const double t , const double y[] , double f[] , void *params)
{
    const double sigma = 10.0;
    const double R = 28.0;
    const double b = 8.0 / 3.0;

    f[0] = sigma * ( y[1] - y[0] );
    f[1] = R * y[0] - y[1] - y[0] * y[2];
    f[2] = y[0]*y[1] - b * y[2];
    return GSL_SUCCESS;
}

#endif