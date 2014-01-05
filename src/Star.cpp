/* Star.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * A rotating, popping and fading star that gets ejected when a jewel "explodes"
 */

#include "engine/DynamicGameObject.h"

#include "util/Random.h"

#include "Assets.h"

#include "Star.h"

Star::Star(int x, int y) : DynamicGameObject(x, y, 0, 0),
			m_Rotation{Util::rand_between(0, 1) ? 1 : -1}
{
	m_Width = m_Height = Util::rand_between(4, maxWidth);
	m_Accel.Set(0 * m_Rotation, 9.81 * 100);
	m_Velocity.Set(Util::rand_between(20, maxIniVelX) * m_Rotation, -Util::rand_between(100, maxIniVelY));
	m_AngularVel = 360 * m_Rotation;
	mp_Region = Assets::Instance().star;
}

void Star::Update(double deltatime)
{
	DynamicGameObject::Update(deltatime);

	m_Alpha -= (deltatime / fadetime);
	if (m_Alpha < 0.0)
		m_Alpha = 0.0;
}
