/*
 * Swarm.h
 *
 *  Created on: 3 Mar 2022
 *      Author: polona
 */

#ifndef SWARM_H_
#define SWARM_H_

#include "Particle.h"

namespace ParticleExplosion {

class Swarm {
private:
	Particle * m_pParticles; // this will be an array of particle pointers
	int lastTime;
public:
	//Particle * m_pParticles; // this is a pointer to Particle object/instance
	const static int NPARTICLES = 10000;
	Swarm();
	virtual ~Swarm();

	// We want to be able to get particles from the swarm
	// This is a method that returns particle pointer (which is private)
	// We make const pointer and const Particle so that the pointer that is returned
	// can't point to anything but Particle and that the Particle can't be changed by this pointer
	Particle * const getParticles() {return m_pParticles;};
	void update(int elapsed);
};

} /* namespace ParticleExplosion */

#endif /* SWARM_H_ */
