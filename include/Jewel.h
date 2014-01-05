/* Jewel.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * The Jewel GameObject, representing the jewels that get drawn to screen.
 *
 * BIG FAT NOTE: During design phase, I was considering making this implement
 * the Miner::Jewel interface required by Miner::Game. I decided not to
 * in order to keep the development of the core clearly differentiated from
 * the graphical front-end. But at this stage, this certainly could be
 * researched further.
 *
 * The main problem to such end would be controlling and/or reducing copies
 * of objects. Miner::Jewel is very lightweight, but a GameObject is not
 * so much. Since the Matrix class mainly provides a read/copy/update
 * mechanism, it would most likely be a bad fit without further changes to
 * avoid copies and modify elements in place.
 */

#ifndef JEWEL_H__
#define JEWEL_H__

#include "util/Random.h"

#include "miner/Jewel.h"

#include "engine/DynamicGameObject.h"
#include "engine/TextureRegion.h"
#include "engine/Vector2.h"

#include "Star.h"

#include "Assets.h"

#include <list>

class Jewel : public Engine::DynamicGameObject {
	public:

	Jewel(int x = 0, int y = 0, Miner::Jewel::Color color = Miner::Jewel::Color::None);

	void SetColor(Miner::Jewel::Color color);

	void Update(double deltatime);

	void Explode(double time, double vel, double accel);

	bool Stopped();

	// just an alias to Select()
	void Deselect() { Select(); }

	void Select();

	void SetStarList(std::list<Star>* starlist) { mp_StarList = starlist; }

	private:

	Miner::Jewel::Color m_Color;
	bool m_Selected;
	bool m_Exploding;
	double m_ExplodingTime;
	Engine::Vector2<double> const m_NilVector;
	std::list<Star> *mp_StarList;

	Engine::TextureRegion const* GetJewelRegion();
};

#endif
