/*
 * Screen.h
 *
 *  Created on: 3 Mar 2022
 *      Author: polona
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL.h>

namespace ParticleExplosion {

class Screen {
private:
	SDL_Window *m_window; // prefix m_ means that they are instance (member) variables and not local variables
	SDL_Renderer *m_renderer;
	SDL_Texture *m_texture;
	Uint32 *m_buffer;
	Uint32 *m_buffer_blur;

public:
	// Public because we will have to refer to screen width and height
	// It doesn't really matter how big screen is
	// but if we want full screen from this window we should have some standard measurements like 800x600
	const static int SCREEN_WIDTH = 800;
	const static int SCREEN_HEIGHT = 600;
	Screen(); // Auto-generated constructor
	bool init(); // false if it fails, true if succeeds
	void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue); // Uint8 is SDL type
	void update();
	void clear();
	void boxBlur();
	bool processEvents();
	void close();
};

} /* namespace ParticleExplosion */

#endif /* SCREEN_H_ */
