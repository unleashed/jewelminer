/* GameObject.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class implements a static game object optionally tied to a graphical
 * representation. It is a base class for more specialized objects.
 */

#ifndef ENGINE_GAMEOBJECT_H__
#define ENGINE_GAMEOBJECT_H__

#include "engine/Vector2.h"
#include "engine/TextureRegion.h"
#include "engine/Graphics.h"	// for GRect

#include <cmath>

namespace Engine {

class GameObject {
	public:
		GameObject();
		GameObject(double x, double y, int width, int height, double angle = 0);
		GameObject(Vector2<double> const& position, int width, int height, double angle = 0);

		virtual ~GameObject();

		virtual void Update(double deltatime);

		virtual TextureRegion const* Region() const { return mp_Region; }

		Vector2<int> RoundPos() const;

		Vector2<int> RoundLL() const;

		GRect Rect() const;

		Vector2<double> const& Position() const { return m_Position; }
		Vector2<double>& Position() { return m_Position; }

		void SetPosition(Vector2<double> const& pos);

		void SetPosition(double x, double y);

		int Width() const { return m_Width; }
		int Height()  const { return m_Height; }

		void Width(int width) {
			m_Width = width;
		}

		void Height(int height) {
			m_Height = height;
		}

		void Size(int width, int height) {
			m_Width = width;
			m_Height = height;
		}

		void Scale(double wscale, double hscale) {
			m_Width *= wscale;
			m_Height *= hscale;
		}

		void Scale(double scalar) { this->Scale(scalar, scalar); }

		double Angle() const { return m_Angle; }
		double Alpha() const { return m_Alpha; }

		void Angle(double angle) { m_Angle = angle; }
		void Alpha(double alpha) { m_Alpha = alpha; }

	protected:
		Vector2<double> m_Position;
		int m_Width;
		int m_Height;
		double m_Angle;
		double m_Alpha;
		TextureRegion const* mp_Region;
};

}	// Engine

#endif
