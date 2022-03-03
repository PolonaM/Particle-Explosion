/*
 * Particle.h
 *
 *  Created on: 3 Mar 2022
 *      Author: polona
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

namespace ParticleExplosion {

// in struct all the members are public, in class by default all members are private (if we don't specify anything)
// So we could also use struct instead of class since everything in the Particle should be public
// m_x and m_y have to be public because we use it in our main program
class Particle {
public:
	double m_x; // double is because we want to move gradually between pixels and round it to nearest pixel
	double m_y;
	// Using polar coordinate system
	double m_speed;
	double m_direction;

public:
	Particle();
	virtual ~Particle();
	void update(int interval);
private:
	void init();
};

} /* namespace ParticleExplosion */

#endif /* PARTICLE_H_ */
