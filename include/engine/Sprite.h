/* Sprite.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class just stores information for the SpriteBatcher to store. It is
 * not meant as a general sprite class. If you want that, go look at GameObject.
 */

#ifndef ENGINE_SPRITE_H__
#define ENGINE_SPRITE_H__

#include "engine/Graphics.h"

namespace Engine {

class Sprite {
	public:
		Sprite(GRect const& dst, GRect const& src, double angle = 0, double alpha = 1.0, GPoint const* center = nullptr) :
			m_DstRect(dst), m_SrcRect(src), m_Angle{angle}, m_Alpha{alpha} {
			if (center != nullptr) {
				m_Center = *center;
			} else {
				m_Center.x = dst.w / 2;
				m_Center.y = dst.h / 2;
			}
		}

		double Angle() const { return m_Angle; }
		double Alpha() const { return m_Alpha; }
		void Alpha(double alpha) { m_Alpha = alpha; }
		GRect* SrcRect() { return &m_SrcRect; }
		GRect* DstRect() { return &m_DstRect; }
		GPoint* Center() { return &m_Center; }

	private:
		GRect m_DstRect;
		GRect m_SrcRect;
		GPoint m_Center;
		double m_Angle;
		double m_Alpha;
};

}	// Engine

#endif
