/* Spark.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class describes the animation of a spark following a very concrete
 * path defined by the contents of the background.
 */

#ifndef SPARK_H__
#define SPARK_H__

#include "engine/DynamicGameObject.h"
#include "engine/Animation.h"
#include "engine/Vector2.h"

#include <vector>

class Spark : public Engine::DynamicGameObject {
	Engine::Animation *mp_Anim;
	double acctime;
	double m_Velocity;
	bool m_Done;

	std::vector<Engine::Vector2<double>>::size_type i;
	std::vector<Engine::Vector2<double>> const coords;

	/* Given the specified coords, moving from initial position
	 * following the path to the final position is this many pixels
	 * (could be computed dynamically, but I'm not bothering for this
	 * concrete use case). I am adding a little to allow the spark
	 * to arrive a bit earlier to the dynamite and account for
	 * the way that MoveTo() works, since a frame's extra velocity
	 * is discarded on waypoints, and that adds to the time it
	 * takes to arrive to the final destination.
	 */
	static constexpr double distance = 249.4674 + 4;

	// let's limit copies
	Spark(Spark const& rhs) = delete;
	Spark& operator=(Spark const& rhs) = delete;

	public:

	Spark(double time);

	void Update(double deltatime);

	// useful to reuse this instance
	void Reset();

	// true when at the last "waypoint"
	bool Done() const {
		return m_Done;
	}
};

#endif
