/*
 * Particle.cpp
 *
 *  Created on: 3 Mar 2022
 *      Author: polona
 */

#include "Particle.h"
#include <stdlib.h> //to get rand()
#include <math.h>

namespace ParticleExplosion {

Particle::Particle(): m_x(0), m_y(0) {
	init();
}

void Particle::init() {
	// Create particle with initial position (0,0) and random speed and direction
	m_x = 0;
	m_y = 0;
	m_direction = (2.0 * M_PI * rand())/RAND_MAX;
	m_speed = 0.05 * rand()/RAND_MAX;
	// we want explosion not be so uniform
	m_speed *= m_speed;
}

Particle::~Particle() {

}

void Particle::update(int interval) {
	// Using polar coordinate system
	m_direction += interval * 0.0002;
	double xspeed = m_speed * cos(m_direction);
	double yspeed = m_speed * sin(m_direction);

	// Each loop update the position of the particle
	// The amount of time that has passed is proportional the amount that we move the particle
	// thus ensuring that it will move at the same rate on slower or faster computers
	m_x += xspeed * interval; //
	m_y += yspeed * interval;

	// Initialize particles that go off the screen - put them to (0,0)
/*	if(m_x < -1 || m_x > 1 || m_y < -1 || m_y > 1) {
		init();
	}*/

	// randomly start new explosion by putting particles back to the centre
	if(rand() < RAND_MAX/100) {
		init();
	}

}

} /* namespace ParticleExplosion */
