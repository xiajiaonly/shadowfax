/*******************************************************************************
 * This file is part of Shadowfax
 * Copyright (C) 2016 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 *
 * Shadowfax is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Shadowfax is distributed in the hope that it will be useful,
 * but WITOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Shadowfax. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/**
 * @file StarParticle.cpp
 *
 * @brief Star particle: implementation
 *
 * @author Bert Vandenbroucke (bert.vandenbroucke@ugent.be)
 */
#include "StarParticle.hpp"
#include "MPIMethods.hpp"
#include "RestartFile.hpp"
using namespace std;

/**
  * @brief Construct a default StarParticle with zero mass
  */
StarParticle::StarParticle() : Particle() {
    _mass = 0.;
    _age = 0.;
}

/**
  * @brief Construct a StarParticle with given position and zero mass
  *
  * @param pos Position of the particle
  */
StarParticle::StarParticle(Vec pos) : Particle(pos) {
    _mass = 0.;
    _age = 0.;
}

/**
  * @brief Set the mass of the star particle
  *
  * @param mass The mass of the star particle
  */
void StarParticle::set_mass(double mass) {
    _mass = mass;
}

/**
  * @brief Construct a StarParticle from an MPI buffer (for parallellization
  * without boost)
  *
  * @param buffer Char buffer containing MPI packed data
  * @param bufsize Total size of the buffer
  * @param position Pointer to the current position inside the buffer (is
  * updated internally)
  */
StarParticle::StarParticle(void* buffer, int bufsize, int* position)
        : Particle(buffer, bufsize, position) {
    MyMPI_Unpack(buffer, bufsize, position, &_mass, 1, MPI_DOUBLE);
    MyMPI_Unpack(buffer, bufsize, position, &_age, 1, MPI_DOUBLE);
}

/**
  * @brief Pack particle data into an MPI buffer (for parallellization without
  * boost)
  *
  * @param buffer Char buffer to write MPI packed data to
  * @param bufsize Total size of the buffer
  * @param position Pointer to the current position inside the buffer (is
  * updated internally)
  */
void StarParticle::pack_data(void* buffer, int bufsize, int* position) {
    Particle::pack_data(buffer, bufsize, position);
    MyMPI_Pack(&_mass, 1, MPI_DOUBLE, buffer, bufsize, position);
    MyMPI_Pack(&_age, 1, MPI_DOUBLE, buffer, bufsize, position);
}

/**
 * @brief Dump the star particle to the given RestartFile
 *
 * @param rfile RestartFile to write to
 */
void StarParticle::dump(RestartFile& rfile) {
    Particle::dump(rfile);

    rfile.write(_mass);
    rfile.write(_age);
}

/**
 * @brief Restart constructor. Initialize the dark matter particle based on the
 * given RestartFile
 *
 * @param rfile RestartFile to read from
 */
StarParticle::StarParticle(RestartFile& rfile) : Particle(rfile) {
    rfile.read(_mass);
    rfile.read(_age);
}

/**
 * @brief Dump particle information in ASCII format to the given stream
 *
 * @param stream std::ostream to write to
 */
void StarParticle::dump_ascii(ostream& stream) {
#if ndim_ == 3
    stream << "_x:\n";
    stream << _x[0] << "\t" << _x[1] << "\t" << _x[2] << "\n";
    stream << "_v:\n";
    stream << _v[0] << "\t" << _v[1] << "\t" << _v[2] << "\n";
    stream << "_a_grav_new:\n";
    stream << _a_grav_new[0] << "\t" << _a_grav_new[1] << "\t" << _a_grav_new[2]
           << "\n";
#else
    stream << "_x:\n";
    stream << _x[0] << "\t" << _x[1] << "\n";
    stream << "_v:\n";
    stream << _v[0] << "\t" << _v[1] << "\n";
    stream << "_a_grav_new:\n";
    stream << _a_grav_new[0] << "\t" << _a_grav_new[1] << "\n";
#endif

    stream << "_old_a:\n";
    stream << _old_a << "\n";

    stream << "_id:\n";
    stream << _id << "\n";

    stream << "_starttime:\n";
    stream << _starttime << "\n";

    stream << "_endtime:\n";
    stream << _endtime << "\n";

    stream << "_comp_cost:\n";
    stream << _comp_cost << "\n";

    stream << "_epot:\n";
    stream << _epot << "\n";

    stream << "_hsoft:\n";
    stream << _hsoft << "\n";

    stream << "_mass:\n";
    stream << _mass << "\n";

    stream << "_age:\n";
    stream << _age << "\n";
}
