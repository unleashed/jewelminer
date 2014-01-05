/*
 * SpriteBatcher.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * Header file containing the SpriteBatcher class declaration.
 *
 * A SpriteBatcher is used to queue drawing operations on a render engine from
 * the contents of a single texture per batch. This way of drawing is optimal for
 * hardware accelerated graphics when you DON'T need to change the texture but
 * only draw some parts of it, which is often the case with sprites and animations.
 *
 * Note: testing suggests SDL 2.0's basic 2D accelerated API already batches drawing
 * operations on textures. However, this class better suits our needs and can be
 * easily ported to other backends.
 */


#ifndef ENGINE_SPRITEBATCHER_H__
#define ENGINE_SPRITEBATCHER_H__

#include "engine/Texture.h"
#include "engine/TextureRegion.h"
#include "engine/Sprite.h"
#include "engine/GameObject.h"

#include <vector>
#include <stdexcept>

namespace Engine {

class SpriteBatcher {
	public:
		SpriteBatcher(Graphics *g) : m_G{g}, mp_Texture{}, mp_Visible{} {}

		void BeginBatch(Texture *texture, GRect *visible = nullptr);

		void EndBatch() noexcept(false);

		void DrawSprite(GameObject const& obj, TextureRegion const& region, GPoint const* center = nullptr) {
			DrawSprite(obj.Rect(), region, obj.Angle(), obj.Alpha(), center);
		}

		void DrawSprite(GameObject const& obj, GPoint const* center = nullptr);

		void DrawSprite(int x, int y, int width, int height, TextureRegion const& region, double angle = 0, double alpha = 1.0, GPoint const* center = nullptr);

		void DrawSprite(GRect const& dest, TextureRegion const& region, double angle = 0, double alpha = 1.0, GPoint const* center = nullptr);

	private:
		Graphics* m_G;
		Texture* mp_Texture;
		GRect* mp_Visible;
		std::vector<Sprite> m_Sprites;
};

}	// Engine

#endif
