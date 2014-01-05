/* Graphics.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * A wrapper class on top of SDL 2D API. Currently does not completely hide SDL
 * for other engine classes, but that should be the goal longer term.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

#include "engine/Texture.h"

#include "engine/Graphics.h"

#include <stdexcept>

namespace Engine {

Graphics::Graphics() : mp_Window{nullptr}, mp_Renderer{nullptr}
{
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		throw std::runtime_error(IMG_GetError());

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		throw std::runtime_error(SDL_GetError());
}

// create the game's window
void Graphics::Window(std::string const& title, int width, int height, bool fullscreen)
{
	if (mp_Window != nullptr) {
		SDL_DestroyWindow(mp_Window);
		mp_Window = nullptr;
	}
	mp_Window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN |
		(fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));

	if (mp_Window == NULL)
		throw std::runtime_error(SDL_GetError());

	if (mp_Renderer != nullptr) {
		SDL_DestroyRenderer(mp_Renderer);
		mp_Renderer = nullptr;
	}
	mp_Renderer = SDL_CreateRenderer(mp_Window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (mp_Renderer == NULL)
		throw std::runtime_error(SDL_GetError());

	SDL_SetRenderDrawColor(mp_Renderer, 0, 0, 0, 255);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(mp_Renderer, width, height);

	m_Viewport.x = m_Viewport.y = 0;
	m_Viewport.w = width;
	m_Viewport.h = height;
}

void Graphics::SetLogicalSize(int width, int height)
{
	SDL_RenderSetLogicalSize(mp_Renderer, width, height);
}

void Graphics::SetViewport(GRect const* viewport)
{
	SDL_RenderSetViewport(mp_Renderer, viewport);
}

void Graphics::SetViewport(int x, int y, int w, int h)
{
	GRect vp;
	vp.x = x;
	vp.y = y;
	vp.w = w;
	vp.h = h;
	SetViewport(&vp);
}

void Graphics::Clear()
{
	SDL_RenderClear(mp_Renderer);
}

void Graphics::Present()
{
	SDL_RenderPresent(mp_Renderer);
}

void Graphics::RenderCopy(Texture *texture, GRect const* srcrect,
			GRect const* dstrect) noexcept(false) {
	if (SDL_RenderCopy(mp_Renderer, texture->Tex(), srcrect, dstrect) < 0)
		throw std::runtime_error(SDL_GetError());
}

void Graphics::RenderCopyEx(Texture *texture, GRect const* srcrect, GRect const* dstrect,
			const double angle, GPoint const* center) noexcept(false) {
	if (SDL_RenderCopyEx(mp_Renderer, texture->Tex(), srcrect, dstrect, angle, center, SDL_FLIP_NONE) < 0)
		throw std::runtime_error(SDL_GetError());
}

Graphics::~Graphics() {
	if (mp_Window != nullptr) {
		SDL_DestroyWindow(mp_Window);
		mp_Window = nullptr;
	}
	if (mp_Renderer != nullptr) {
		SDL_DestroyRenderer(mp_Renderer);
		mp_Renderer = nullptr;
	}
	SDL_Quit();
	IMG_Quit();
}

void Graphics::GetWindowSize(int& w, int& h) {
	w = m_Viewport.w;
	h = m_Viewport.h;
}

}	// Engine

