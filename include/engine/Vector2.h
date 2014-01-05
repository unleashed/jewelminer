/* Vector2.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * A 2D vector. Useful for almost all games.
 */

#ifndef ENGINE_VECTOR2_H__
#define ENGINE_VECTOR2_H__

#include "util/Math.h"

#include <cmath>

namespace Engine {

template <typename T>
class Vector2 {
	public:
		explicit Vector2(T x = 0, T y = 0) : m_x(x), m_y(y) {}

		T X() const { return m_x; }
		T Y() const { return m_y; }
		void X(T x) { m_x = x; }
		void Y(T y) { m_y = y; }

		Vector2 operator+(Vector2 const& rhs) const {
			return Vector2(m_x + rhs.m_x, m_y + rhs.m_y);
		}

		Vector2& operator+=(Vector2 const& rhs) {
			m_x += rhs.m_x;
			m_y += rhs.m_y;
			return *this;
		}

		Vector2 operator-(Vector2 const& rhs) const {
			return Vector2(m_x - rhs.m_x, m_y - rhs.m_y);
		}

		Vector2& operator-=(Vector2 const& rhs) {
			m_x -= rhs.m_x;
			m_y -= rhs.m_y;
			return *this;
		}

		Vector2 operator*(T scalar) const {
			return Vector2(m_x * scalar, m_y * scalar);
		}

		Vector2& operator*=(T scalar) {
			m_x *= scalar;
			m_y *= scalar;
			return *this;
		}

		Vector2 operator/(T scalar) const {
			return Vector2(m_x * scalar, m_y * scalar);
		}

		Vector2& operator/=(T scalar) {
			m_x /= scalar;
			m_y /= scalar;
			return *this;
		}

		bool operator==(Vector2 rhs) {
			return m_x == rhs.m_x && m_y == rhs.m_y;
		}

		Vector2& Set(T x, T y) {
			m_x = x;
			m_y = y;
			return *this;
		}

		// Length
		T Len() const {
			return std::sqrt(m_x * m_x + m_y * m_y);
		}

		// Normalization
		Vector2& Nor() {
			T len = this->Len();
			if (len > 0)
				(*this) /= len;
			return *this;
		}

		// Get the vector's angle relative to origin
		double Angle() const {
			double angle = Util::to_degrees(std::atan2(m_y, m_x));

			if (angle < 0)
				angle += 360;

			return angle;
		}

		// a simplified version of Vector2(scalar, 0).Rotate(angle)
		Vector2& Angle(double angle, T scalar = 1) {
			double rad = Util::to_radians(angle);

			m_x = std::cos(rad) * scalar;
			m_y = std::sin(rad) * scalar;

			return *this;
		}

		T Dist(Vector2 const& other) const {
			return std::sqrt(this->DistSquared(other));
		}

		/* DistSquared is useful when there is no need to perform sqrt()
		 * ie. collision detection code
		 */
		T DistSquared(Vector2 const& other) const {
			T distX = m_x - other.m_x;
			T distY = m_y - other.m_y;

			return distX * distX + distY * distY;
		}

		/* Rotate the vector, storing a translation vector in trans
		 * so that you could add trans to the original vector and
		 * get the rotated one.
		 */
		Vector2& Rotate(double angle, Vector2 *trans = nullptr) {
			double rad = Util::to_radians(angle);
			double cos = std::cos(rad);
			double sin = std::sin(rad);
			T new_x = m_x * cos - m_y * sin;
			T new_y = m_x * sin + m_y * cos;

			if (trans) {
				trans->m_x = new_x - m_x;
				trans->m_y = new_y - m_y;
			}

			m_x = new_x;
			m_y = new_y;

			return *this;
		}

		// Same as Rotate, but with regards to a certain point
		Vector2& RotateFrom(Vector2 const& point, double angle, Vector2 *trans = nullptr) {
			(*this) -= point;
			this->Rotate(angle, trans);
			(*this) += point;

			return *this;
		}

		// clamps dimensions in value ranges
		Vector2& ClampX(T min, T max) {
			if (m_x < min)
				m_x = min;
			else if (m_x > max)
				m_x = max;
		}

		Vector2& ClampY(T min, T max) {
			if (m_y < min)
				m_y = min;
			else if (m_y > max)
				m_y = max;
		}

	private:
		/* These could be made public, but I consider it a
		 * premature optimization. Only do that if really
		 * needed.
		 */
		T m_x;
		T m_y;
};

}	// Engine

#endif
