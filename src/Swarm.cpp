/*
 * Swarm.cpp
 *
 *  Created on: 3 Mar 2022
 *      Author: polona
 */

#include "Swarm.h"
#include "Particle.h"

namespace ParticleExplosion {

Swarm::Swarm(): lastTime(0) {
	m_pParticles = new Particle[NPARTICLES]; // m_pParticles are pointers to new array of particle objects
}

Swarm::~Swarm() {
	delete [] m_pParticles;
}

void Swarm::update(int elapsed) {

	int interval = elapsed -lastTime;

	for(int i = 0; i < Swarm::NPARTICLES; i++) {
		m_pParticles[i].update(interval);
	}

	lastTime = elapsed;
}

} /* namespace ParticleExplosion */
