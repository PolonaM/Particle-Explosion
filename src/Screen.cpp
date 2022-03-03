/*
 * Screen.cpp
 *
 *  Created on: 3 Mar 2022
 *      Author: polona
 */

#include "Screen.h"

namespace ParticleExplosion {

// we initialise each of the variable to NULL, we need to include SDL.h in Screen.h to have NULL defined
Screen::Screen():
		m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer(NULL), m_buffer_blur(NULL) {
}

bool Screen::init() {
	// false if it fails, true if succeeds
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
			return false;
		}

		// We need an object of type SDL_Window pointer called window
		// 2. and 3. arguments enable to position the window, we are going to use special constant SDL_WINDOWPOS_UNDEFINED
		m_window = SDL_CreateWindow("Particle Colourful Explosion",
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(m_window == NULL) {
			SDL_Quit();
			return false;
		}

		// We need a renderer to draw stuff on the window
		// Renderer will do stuff to the texture and then display it to the window
		// We will use a buffer where we will have information about the pixels which we will pass to the texture and then the renderer will display it on the window
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
		// -1 means that we want to initialise first render driver available
		// renderer is synchronised with the vertical refresh of the screen; if we don't use this flag we get screen tearing
		if(m_renderer == NULL) {
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}

		m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
		// RGBA8888 - uses one 4 bytes for each pixel - one byte for each colour (R, G, B) and one for alpha (transparency), but alpha will not be used since there are some issues with it
		// SDL_TEXTUREACCESS_STATIC - the way we want to write to the texture - load a memory(buffer) an write to texture
		if(m_texture == NULL) {
			SDL_DestroyWindow(m_window);
			SDL_DestroyRenderer(m_renderer);
			SDL_Quit();
			return false;
		}

		// We need to allocate the memory
		// This is a type declared by SDL; for every pixel we have 4 bytes -> 32 bits
		m_buffer = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
		m_buffer_blur = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
		// In case if we can't allocate the memory, 'new' will return NULL, ideally we should check this in a program

		// Write some pixel information in the buffer
		// memset is very fast and useful function
		// We will write 255 in every byte in all of the pixels
		// another way of representing 255 is hexadecimal 255 = 0xFF; the advantage is that we always use two digits for one byte
		memset(m_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT*sizeof(Uint32));
		memset(m_buffer_blur, 0, SCREEN_WIDTH * SCREEN_HEIGHT*sizeof(Uint32));

		return true;
}

void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) { // Uint8 is SDL type
	// If pixel is off the screen don't do anything
	if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
		return;
	}

	Uint32 color = 0;

	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<= 8;
	color += 0xFF; // this is alpha, we will not use it

	//m_buffer is an array so we need to translate x and y coordinates into array
	//pixels in an window go from top left to bottom right
	//x is the row, y is the column
	m_buffer[(y * SCREEN_WIDTH) + x] = color;
}

void Screen::update() {
	SDL_UpdateTexture(m_texture, NULL, m_buffer, SCREEN_WIDTH*sizeof(Uint32));
	// SCREEN_WIDTH*sizeof(Uint32) = PITCH = number of bytes in a row
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	SDL_RenderPresent(m_renderer); // presenting render to the window
}

void Screen::boxBlur() {
	// Swap the buffers, so pixel is in M_buffer_blur and we are drawing to m_buffer
	Uint32 *temp = m_buffer; // we store the location m_buffer is pointing at
	m_buffer = m_buffer_blur; // m_buffer is now pointer to M-buffer_blur
	m_buffer_blur = temp;

	// Now we want to blur the pixels each loop and apply the blurred image to the m_buffer
	// m_buffer is not cleared anymore so each loop there is more blurred pixels on the screen
	// 1. step is to iterate trough the pixels
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for(int x = 0; x < SCREEN_WIDTH; x++) {
			/* boxBlur takes surrounding pixels (1-8) and pixel in question,
			 * calculates average and returns the average colour of the pixel
			 * p1 p2 p3
			 * p4 p p5
			 * p6 p7 p6
			 */

			int redTotal = 0;
			int greenTotal = 0;
			int blueTotal = 0;

			// 2. step is to get the surrounding pixels, thus to get the box of 9 pixels
			for(int row = -1; row <=1; row++) {
				for(int col = -1; col <= 1; col++) {
					int currentX = x + col;
					int currentY = y + row;

					// 3. step is to check if the box of pixels is on the screen:
					if(currentX >= 0 && currentX < SCREEN_WIDTH && currentY >= 0 && currentY < SCREEN_HEIGHT) {
						// 4. step is to get the color from the box of pixels
						Uint32 color = m_buffer_blur[currentY*SCREEN_WIDTH + currentX];

						Uint8 red = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >>8;

						// 5. step is to calculate the sum of the pixels
						redTotal += red;
						greenTotal += green;
						blueTotal += blue;
					}
				}
			}

			// 6. step is to calculate the average and apply it to the screen
			Uint8 red = redTotal/9;
			Uint8 green = greenTotal/9;
			Uint8 blue = blueTotal/9;
			setPixel(x, y, red, green, blue);
		}
	}

}

bool Screen::processEvents() {
	// SDL provides its way to check for multiple events
	// this loop will run until SDL_PollEvent returns true
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		// in case if we close the window:
		if(event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

void Screen::close() {
	// Free the resources
	delete [] m_buffer;
	delete [] m_buffer_blur;
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyTexture(m_texture);
	SDL_DestroyWindow(m_window);
	SDL_Quit(); // To shut down the SDL system
}

} /* namespace ParticleExplosion */
