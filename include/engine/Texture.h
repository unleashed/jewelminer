/* Texture.h - Copyright (c) Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class represents a texture. It is loaded from an image file and
 * stored in VRAM once SetRenderer() is called, so it is generally not
 * modifiable. That means, it is basically used in tandem with
 * TextureRegions.
 */

#ifndef ENGINE_TEXTURE_H__
#define ENGINE_TEXTURE_H__

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <stdexcept>

namespace Engine {

class Texture {
	public:
		Texture(std::string const& filename, SDL_Renderer *renderer = nullptr);

		~Texture();

		// load the texture from the disk and keep a surface (system ram) around
		void Load(std::string const& filename) noexcept(false);

		// this sets the renderer, which will upload the texture to video ram
		void SetRenderer(SDL_Renderer *renderer);

		void Alpha(double alpha);

		double Alpha() const { return m_Alpha; }

		// these two below might have sense in other backends (ie. OpenGL)
		void Bind() {}
		void Reload() {}

		void Dispose();

		int Width() const { return m_Width; }
		int Height() const { return m_Height; }

		SDL_Renderer* Renderer() { return mp_Renderer; }
		SDL_Texture* Tex() { return mp_Texture; }

	private:
		SDL_Renderer *mp_Renderer;
		SDL_Texture *mp_Texture;
		SDL_Surface *mp_Surface;
		int m_Width;
		int m_Height;
		double m_Alpha;
};

}	// Engine

#endif
