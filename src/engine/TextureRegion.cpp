/* TextureRegion.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class describes an area of interest within Texture. This area is used as a
 * source for drawing to the screen and is normally predefined (ie. a texture atlas)
 */

#include "engine/Graphics.h"	// for GRect
#include "engine/Vector2.h"
#include "engine/Texture.h"

#include "engine/TextureRegion.h"

namespace Engine {

TextureRegion::TextureRegion() :
		m_LL{}, m_Width{0}, m_Height{0}, mp_Texture{nullptr} {}

TextureRegion::TextureRegion(Texture* texture,
			Vector2<int> const& lowerleft, int width, int height) :
		m_LL{lowerleft},
		m_Width{width},
		m_Height{height},
		mp_Texture{texture} {}

void TextureRegion::Init(Texture *texture, Vector2<int> lowerleft, int width, int height)
{
	m_LL = lowerleft;
	m_Width = width;
	m_Height = height;
	mp_Texture = texture;
}

GRect TextureRegion::Rect() const {
	GRect rect;
	rect.x = m_LL.X();
	rect.y = m_LL.Y();
	rect.w = m_Width;
	rect.h = m_Height;
	return rect;
}

}	// Engine
