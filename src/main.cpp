/* main.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * Licensed under the GNU General Public License, version 3 or (at your
 * option), any later version. See COPYING for details.
 *
 * This is the main game loop. Nothing really exciting here, just some
 * engine setup code and game configuration, and a simple game loop.
 *
 */

#include <SDL2/SDL.h>

#include "engine/Graphics.h"
#include "engine/Texture.h"
#include "engine/TextureRegion.h"
#include "engine/SpriteBatcher.h"

#include "Assets.h"

#include "World.h"

#include <string>

int main(int argc, char *argv[])
{
	// allow specifying dimensions of the matrix board in command-line
	int cols = 8, rows = 8;

	if (argc > 1) {
		cols = std::stoi(argv[1]);
		if (argc > 2)
			rows = std::stoi(argv[2]);
		else
			rows = cols;
	}

	if (cols < 2)
		cols = 2;
	if (rows < 2)
		rows = 2;

	Engine::Graphics g;
	// the following is defined by the background we use
	Engine::GRect MineDimensions {
		330,	// x
		100,	// y
		336,	// w
		336	// h
	};

	Engine::Texture *t = Assets::Instance().background;

	// create a window the size of our background
	// could use full screen as well with SDL2's ability
	// to keep the logical window size
	g.Window("Jewel Miner", t->Width(), t->Height(), false);
	Assets::Instance().SetGraphics(g);

	// create a game with 60s timer the size we want
	World w(&g, MineDimensions, 60, cols, rows);

	double delta = 0;
	Uint32 starttime = SDL_GetTicks();
	bool quit = false;

	// below is a classic video game loop
	while (!quit) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
				continue;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				w.Click(x, y, e.type == SDL_MOUSEBUTTONUP);
				continue;
			}
		}

		Uint32 currenttime = SDL_GetTicks();
		delta = (currenttime - starttime) / 1000.0;
		starttime = currenttime;
		w.Update(delta);
		w.Render(delta);
	}

	return 0;
}
