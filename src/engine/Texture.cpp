/* Texture.cpp - Copyright (c) Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class represents a texture. It is loaded from an image file and
 * stored in VRAM once SetRenderer() is called, so it is generally not
 * modifiable. That means, it is basically used in tandem with
 * TextureRegions.
 */

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <stdexcept>

#include "engine/Texture.h"

namespace Engine {

Texture::Texture(std::string const& filename, SDL_Renderer *renderer) :
		mp_Renderer{nullptr},
		mp_Texture{nullptr},
		mp_Surface{nullptr},
		m_Width{0}, m_Height{0}, m_Alpha{1.0}
{
	this->Load(filename);
	if (renderer != nullptr)
		this->SetRenderer(renderer);
}

Texture::~Texture()
{
	if (mp_Surface != nullptr)
		SDL_FreeSurface(mp_Surface);

	if (mp_Texture != nullptr)
		SDL_DestroyTexture(mp_Texture);
}

// load the texture from the disk and keep a surface (system ram) around
void Texture::Load(std::string const& filename) noexcept(false)
{
	SDL_Surface *tmpsurface = IMG_Load(filename.c_str());

	if (tmpsurface == nullptr)
		throw std::runtime_error(IMG_GetError());

	mp_Surface = SDL_ConvertSurfaceFormat(tmpsurface, SDL_PIXELFORMAT_RGBA8888, 0);
	SDL_FreeSurface(tmpsurface);

	if (mp_Surface == nullptr)
		throw std::runtime_error(IMG_GetError());

	m_Width = tmpsurface->w;
	m_Height = tmpsurface->h;
}

// this sets the renderer, which will upload the texture to video ram
void Texture::SetRenderer(SDL_Renderer *renderer)
{
	mp_Renderer = renderer;

	if (mp_Texture != nullptr)
		SDL_DestroyTexture(mp_Texture);

	mp_Texture = SDL_CreateTextureFromSurface(mp_Renderer, mp_Surface);
	// free unused surface
	SDL_FreeSurface(mp_Surface);
	mp_Surface = nullptr;

	if (mp_Texture == nullptr)
		throw std::runtime_error(SDL_GetError());

	SDL_SetTextureBlendMode(mp_Texture, SDL_BLENDMODE_BLEND);
}

void Texture::Alpha(double alpha)
{
	m_Alpha = alpha;
	SDL_SetTextureAlphaMod(mp_Texture, m_Alpha * 255);
}

void Texture::Dispose()
{
	if (mp_Texture != nullptr)
		SDL_DestroyTexture(mp_Texture);
}

}	// Engine
