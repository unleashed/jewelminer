/* Graphics.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * A wrapper class on top of SDL 2D API. Currently does not completely hide SDL
 * for other engine classes, but that should be the goal longer term.
 */

#ifndef ENGINE_GRAPHICS_H__
#define ENGINE_GRAPHICS_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

#include "engine/Texture.h"

#include <stdexcept>

namespace Engine {

typedef SDL_Point GPoint;
typedef SDL_Rect GRect;
typedef SDL_Window GWindow;
typedef SDL_Renderer GRenderer;

class Graphics {

	public:

	Graphics();

	// create the game's window
	void Window(std::string const& title, int width, int height, bool fullscreen = true);

	void SetLogicalSize(int width, int height);

	void SetViewport(GRect const* viewport);

	void SetViewport(int x, int y, int w, int h);

	void ResetViewport() {
		SetViewport(&m_Viewport);
	}

	void Clear();

	void Present();

	void RenderCopy(Texture *texture, GRect const* srcrect,
			GRect const* dstrect) noexcept(false);

	void RenderCopyEx(Texture *texture, GRect const* srcrect, GRect const* dstrect,
			const double angle, GPoint const* center = nullptr) noexcept(false);

	~Graphics();

	void GetWindowSize(int& w, int& h);

	GWindow* GetWindow() const { return mp_Window; }
	GRenderer* GetRenderer() const { return mp_Renderer; }

	private:

	GWindow* mp_Window;
	GRenderer* mp_Renderer;
	GRect m_Viewport;
};

}	// Engine

#endif
