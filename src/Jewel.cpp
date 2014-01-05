/* Jewel.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * The Jewel GameObject, representing the jewels that get drawn to screen.
 *
 */

#include "util/Random.h"

#include "miner/Jewel.h"

#include "engine/DynamicGameObject.h"
#include "engine/TextureRegion.h"

#include "Assets.h"

#include "Jewel.h"

#include <list>

Jewel::Jewel(int x, int y, Miner::Jewel::Color color) :
		Engine::DynamicGameObject(x, y, 0, 0),
		m_Color{color}, m_Selected{false},
		m_Exploding{false}, m_ExplodingTime{0},
		m_NilVector{},
		mp_StarList{}
{
	mp_Region = GetJewelRegion();
	if (mp_Region) {
		this->Width(mp_Region->Width());
		this->Height(mp_Region->Height());
	}
}

void Jewel::SetColor(Miner::Jewel::Color color)
{
	m_Color = color;
	mp_Region = GetJewelRegion();
}

void Jewel::Update(double deltatime)
{
	Engine::DynamicGameObject::Update(deltatime);
	if (m_Exploding) {
		m_ExplodingTime -= deltatime;
		if (m_ExplodingTime < 0) {
			m_Exploding = false;
			this->SetColor(Miner::Jewel::Color::None);
			this->AngularAccel(0);
			this->AngularVel(0);
			this->Angle(0);

			// Exploded! Now generate some stars
			if (mp_StarList != nullptr) {
				int nstars = Util::rand_between(5, 10);
				while (nstars--)
					mp_StarList->emplace_back(m_Position.X(), m_Position.Y());
			}
		}
	}
}

void Jewel::Explode(double time, double vel, double accel)
{
	m_Exploding = true;
	m_ExplodingTime = time;
	this->AngularVel(vel);
	this->AngularAccel(accel);
}

bool Jewel::Stopped()
{
	return !m_Exploding && (m_Velocity == m_NilVector);
}

void Jewel::Select()
{
	if (m_Selected == false) {
		m_Selected = true;
		this->Scale(1.2);
	} else {
		m_Selected = false;
		this->Scale(1.0/1.2);
	}
}

Engine::TextureRegion const* Jewel::GetJewelRegion()
{
	Engine::TextureRegion const *region;

	switch (m_Color) {
	case Miner::Jewel::Color::Red:
		region = Assets::Instance().jewelRed;
		break;
	case Miner::Jewel::Color::Yellow:
		region = Assets::Instance().jewelYellow;
		break;
	case Miner::Jewel::Color::Green:
		region = Assets::Instance().jewelGreen;
		break;
	case Miner::Jewel::Color::Blue:
		region = Assets::Instance().jewelBlue;
		break;
	case Miner::Jewel::Color::Purple:
		region = Assets::Instance().jewelPurple;
		break;
	case Miner::Jewel::Color::None:
	default:
		region = nullptr;
	}
	return region;
}
