/*
 * SpriteBatcher.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
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


#include "engine/Texture.h"
#include "engine/TextureRegion.h"
#include "engine/Sprite.h"
#include "engine/GameObject.h"

#include "engine/SpriteBatcher.h"

#include <SDL2/SDL.h>

#include <stdexcept>

namespace Engine {

void SpriteBatcher::BeginBatch(Texture *texture, GRect *visible)
{
	mp_Texture = texture;
	mp_Visible = visible;
	m_Sprites.clear();
}

void SpriteBatcher::EndBatch() noexcept(false) {
	double texAlpha, lastAlpha;

	texAlpha = lastAlpha = mp_Texture->Alpha();

	for (auto& sprite : m_Sprites) {
		double alpha = sprite.Alpha();
		if (alpha != lastAlpha) {
			lastAlpha = alpha;
			mp_Texture->Alpha(alpha);
		}
		m_G->RenderCopyEx(mp_Texture, sprite.SrcRect(), sprite.DstRect(), sprite.Angle(), sprite.Center());
	}

	// reset the alpha on the texture to the original value
	if (lastAlpha != texAlpha)
		mp_Texture->Alpha(texAlpha);
}

void SpriteBatcher::DrawSprite(GameObject const& obj, GPoint const* center) {
	TextureRegion const* region = obj.Region();
	// objects that don't have a region assigned are ignored
	if (region != nullptr)
		DrawSprite(obj.Rect(), *region, obj.Angle(), obj.Alpha(), center);
}

void SpriteBatcher::DrawSprite(int x, int y, int width, int height,
		TextureRegion const& region, double angle, double alpha, GPoint const* center)
{
	GRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = width;
	dest.h = height;

	DrawSprite(dest, region, angle, alpha, center);
}

void SpriteBatcher::DrawSprite(GRect const& dest,
		TextureRegion const& region, double angle, double alpha, GPoint const* center)
{
	/* if there's a visibility rectangle set, we have to adjust
	 * the relation between dest and the source region, ie:
	 * _______      _______
	 * |   __|  =>  |_|   |
	 * |___|_|  =>  |_____|
	 */
	if (mp_Visible != nullptr) {
		/* Rectangle */
		GRect visible;

		if (SDL_IntersectRect(&dest, mp_Visible, &visible) == SDL_TRUE) {
			/* We got an intersection, with "visible" holding the
			 * destination rect. We must now adjust the source rect
			 * because we don't want the full object strecthed into
			 * dest, but just a part of it, since we are just drawing
			 * a partially hidden object.
			 */
			GPoint cnt;
			GRect src = region.Rect();
			double scalex = static_cast<double>(src.w) / static_cast<double>(dest.w);
			double scaley = static_cast<double>(src.h) / static_cast<double>(dest.h);
			src.x += (visible.x - dest.x) * scalex;
			src.y += (visible.y - dest.y) * scaley;
			src.w += (visible.w - dest.w) * scalex;
			src.h += (visible.h - dest.h) * scaley;

			/* If we didn't specify a rotation center, let's set
			 * the destination's center as the rotation center in order
			 * to produce a seamless rotation.
			 *
			 * Note: SDL 2.0 with textures is problematic with an intersect
			 * that actually gets a narrowed final destination rectangle.
			 * It performs the rotation AFTER everything else , which is
			 * why we will get a 'cut' object rotating instead of the
			 * whole object rotating with only a part of it visible.
			 *
			 * This, however, might be subject to change on other libraries
			 * and/or if we used system memory surfaces. The realistic
			 * solution, however, is to eventually switch to OpenGL,
			 * which grants full control on these details.
			 */
			if (center == nullptr) {
				cnt.x = dest.w / 2;
				cnt.y = dest.h / 2;
				center = &cnt;
			}

			m_Sprites.emplace_back(visible, src, angle, alpha, center);
		}

		// No intersection means no visibility of this object, so skip it

	} else {
		m_Sprites.emplace_back(dest, region.Rect(), angle, alpha, center);
	}
}

}	// Engine
