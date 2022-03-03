//============================================================================
// Name        : Particle.cpp
// Author      : Polona Mihalič
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <SDL.h>
#include <math.h>
#include "Screen.h"
#include <iomanip>
#include <time.h>
#include "Swarm.h"

using namespace std;
using namespace ParticleExplosion;

int main() {

	// CREATE THE SCREEN
	Screen screen;

	if(screen.init() == false) {
		cout << "Error initialising SDL." << endl;
	}

	// CREATE THE SWARM OF PARTICLES
	Swarm swarm;
	// seed the random number generator that we use in Particle
	srand(time(NULL));

	// THE MAIN LOOP THAT CHANGES THE POSITION AND COLOUR OF THE PARTICLES
	while(true) {

		int elapsed = SDL_GetTicks(); //returns number of milliseconds since the program starts

		// UPDATE THE POSITION OF THE PARTICLES
		swarm.update(elapsed);

		// UPDATE THE COLOUR OF THE PARTICLES
		unsigned char green = (unsigned char)((1 + sin(elapsed * 0.0001)) * 128); // * 0.001 to get smoother change
		unsigned char red = (unsigned char)((1 + sin(elapsed * 0.0002)) * 128);
		unsigned char blue = (unsigned char)((1 + sin(elapsed * 0.0003)) * 128);

		// Get pointers to the particles in the swarm
		Particle * pParticles = swarm.getParticles();

		// write the colour to each of the particle in a swarm
		for(int i = 0; i < Swarm::NPARTICLES; i++) {
			Particle particle = pParticles[i];
			int x = (1 + particle.m_x) * Screen::SCREEN_WIDTH/2; // we write it like this, so the (0,0) coordinates are in centre of the window
			int y = particle.m_y * Screen::SCREEN_WIDTH/2 + Screen::SCREEN_HEIGHT/2;
			// but we also need to consider that particle move in the same direction at the same speed
			// so that we get circle and not ellipse as we get if we have y written in the same way as x
			screen.setPixel(x, y, red, green, blue);
		}

		// blur the screen
		screen.boxBlur();

		// DRAW THE PARTICLES ON SCREEN
		screen.update();

		// CHECK FOR QUIT
		if(screen.processEvents() == false) {
			break;
		}
	}

	// DELETE ALL SCREEN BUFFERS, PARTICLES ARE DELETED IN SWARM DESTRUCTOR
	screen.close();

	return 0;
}
