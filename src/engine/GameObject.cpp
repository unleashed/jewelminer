/* GameObject.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class implements a static game object optionally tied to a graphical
 * representation. It is a base class for more specialized objects.
 */

#include "engine/Vector2.h"
#include "engine/TextureRegion.h"
#include "engine/Graphics.h"	// for GRect

#include "engine/GameObject.h"

#include <cmath>

namespace Engine {

GameObject::GameObject() : m_Position{}, m_Width{0}, m_Height{0},
		m_Angle{0}, m_Alpha{1.0}, mp_Region{} {}

GameObject::GameObject(double x, double y, int width, int height, double angle) :
		m_Position{x, y}, m_Width{width}, m_Height{height},
		m_Angle{angle}, m_Alpha{1.0}, mp_Region{} {}

GameObject::GameObject(Vector2<double> const& position, int width, int height, double angle) :
		m_Position{position}, m_Width{width}, m_Height{height},
		m_Angle{angle}, m_Alpha{1.0}, mp_Region{} {}

GameObject::~GameObject() {}

void GameObject::Update(double deltatime) {}

Vector2<int> GameObject::RoundPos() const
{
	return Vector2<int>(static_cast<int>(lround(m_Position.X())),
		static_cast<int>(lround(m_Position.Y())));
}

Vector2<int> GameObject::RoundLL() const
{
	return Vector2<int>{this->RoundPos()} - Vector2<int>{m_Width/2, m_Height/2};
}

GRect GameObject::Rect() const
{
	GRect res;
	res.x = lround(m_Position.X() - m_Width/2);
	res.y = lround(m_Position.Y() - m_Height/2);
	res.w = m_Width;
	res.h = m_Height;
	return res;
}

void GameObject::SetPosition(Vector2<double> const& pos)
{
	m_Position = pos;
}

void GameObject::SetPosition(double x, double y)
{
	m_Position.Set(x, y);
}

}	// Engine
