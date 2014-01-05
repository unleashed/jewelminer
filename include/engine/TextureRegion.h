/* TextureRegion.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class describes an area of interest within Texture. This area is used as a
 * source for drawing to the screen and is normally predefined (ie. a texture atlas)
 */

#ifndef ENGINE_TEXTUREREGION_H__
#define ENGINE_TEXTUREREGION_H__

#include "engine/Graphics.h"	// for GRect
#include "engine/Vector2.h"
#include "engine/Texture.h"

namespace Engine {

class TextureRegion {
	public:
		TextureRegion();

		TextureRegion(Texture* texture, Vector2<int> const& lowerleft, int width, int height);

		TextureRegion(Texture* texture, int x, int y, int width, int height) :
			TextureRegion(texture, Vector2<int>{x, y}, width, height) {}

		void Init(Texture *texture, Vector2<int> lowerleft, int width, int height);

		void Init(Texture *texture, int x, int y, int width, int height) {
			return Init(texture, Vector2<int>{x, y}, width, height);
		}

		Vector2<int> const& LL() const { return m_LL; }
		Vector2<int>& LL() { return m_LL; }
		int Width() const { return m_Width; }
		int Height() const { return m_Height; }
		int X() const { return m_LL.X(); }
		int Y() const { return m_LL.Y(); }

		GRect Rect() const;

		Texture const* Tex() const { return mp_Texture; }

	private:
		Vector2<int> m_LL;
		int m_Width;
		int m_Height;
		Texture *mp_Texture;
};

}	// Engine

#endif
