/* Spark.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class describes the animation of a spark following a very concrete
 * path defined by the contents of the background.
 */

#include "engine/DynamicGameObject.h"
#include "engine/Vector2.h"

#include "Assets.h"

#include "Spark.h"

#include <vector>

Spark::Spark(double time) : mp_Anim{Assets::Instance().spark},
		acctime{}, m_Done{false}, i{}, coords {
			Engine::Vector2<double>(262, 555),
			Engine::Vector2<double>(252, 542),
			Engine::Vector2<double>(254, 514),
			Engine::Vector2<double>(213, 507),
			Engine::Vector2<double>(207, 373),
			Engine::Vector2<double>(178, 372),
		}
{
	m_Position.Set(262, 555);
	m_Velocity = Spark::distance / time;
}

void Spark::Update(double deltatime)
{
	acctime += deltatime;
	if (m_Position == coords[i]) {
		if (i < coords.size() - 1)
			this->MoveTo(coords[++i], m_Velocity);
		else {
			m_Done = true;
		}
	}
	DynamicGameObject::Update(deltatime);

	mp_Region = mp_Anim->KeyFrame(acctime);
	m_Width = mp_Region->Width();
	m_Height = mp_Region->Height();
}

// useful to reuse this instance
void Spark::Reset() {
	m_Position.Set(262, 555);
	m_Done = false;
	i = 0;
	acctime = 0;
}
