/* Star.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * A rotating, popping and fading star that gets ejected when a jewel "explodes"
 */

#ifndef STAR_H__
#define STAR_H__

#include "engine/DynamicGameObject.h"

#include "util/Random.h"

#include "Assets.h"

class Star : public Engine::DynamicGameObject {
	int m_Rotation;

	static constexpr int maxWidth = 20;
	static constexpr double angularVel = 360;
	static constexpr int maxIniVelX = 800;
	static constexpr int maxIniVelY = 800;
	static constexpr double fadetime = 1.6;

	public:

	explicit Star(int x, int y);

	void Update(double deltatime);

	bool Done() const {
		return m_Alpha <= 0.0;
	}
};

#endif
