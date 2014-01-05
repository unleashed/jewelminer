/* Assets.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * Singleton asset manager for the game.
 *
 * Its data is tied to the texture atlas used.
 */

#ifndef ASSETS_H__
#define ASSETS_H__

#include "engine/Graphics.h"
#include "engine/Texture.h"
#include "engine/TextureRegion.h"
#include "engine/Animation.h"
#include "engine/Font.h"

class Assets {
	private:
		static Assets* instance;

		Engine::TextureRegion *spark1, *spark2, *spark3, *spark4;

		Assets();

		Assets(Assets const& rhs);

		Assets& operator=(Assets const& rhs);

		~Assets();

	public:
		Engine::Texture *background;
		Engine::TextureRegion *backgroundRegion;
		Engine::Texture *atlas;
		Engine::TextureRegion *jewelYellow;
		Engine::TextureRegion *jewelRed;
		Engine::TextureRegion *jewelPurple;
		Engine::TextureRegion *jewelGreen;
		Engine::TextureRegion *jewelBlue;
		Engine::TextureRegion *star;
		Engine::Animation *spark;
		Engine::Font *font;

		static Assets& Instance();

		// helper needed to set renderer on textures
		void SetGraphics(Engine::Graphics const& g);
};

#endif
