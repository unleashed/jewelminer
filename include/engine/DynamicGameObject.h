/* DynamicGameObject.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class add specialization to game objects. In particular, this one deals with
 * movement and rotation. The MoveTo() method enables target mode, which is specially
 * useful to define waypoints or paths when chained.
 */

#ifndef ENGINE_DYNAMICGAMEOBJECT_H__
#define ENGINE_DYNAMICGAMEOBJECT_H__

#include "engine/Vector2.h"
#include "engine/GameObject.h"

namespace Engine {

class DynamicGameObject : public GameObject {
	public:
		DynamicGameObject();

		DynamicGameObject(double x, double y, int width,
			int height, double angle = 0, double angularvel = 0);

		DynamicGameObject(Vector2<double> const& position, int width,
			int height, double angle = 0, double angularvel = 0);

		virtual ~DynamicGameObject();

		virtual void Update(double deltatime);

		virtual void UpdatePosition(double deltatime);

		virtual void UpdateAngle(double deltatime);

		Vector2<double> const& Velocity() const { return m_Velocity; }
		Vector2<double> const& Accel() const { return m_Accel; }

		void Velocity(Vector2<double> const& velocity);

		double AngularVel() const { return m_AngularVel; }
		double AngularAccel() const { return m_AngularAccel; }

		void AngularVel(double angularvel) { m_AngularVel = angularvel; }
		void AngularAccel(double angularaccel) { m_AngularAccel = angularaccel; }

		void Accel(Vector2<double> const& accel);

		void MoveTo(Vector2<double> const& targetpos, double vel, double accel = 0);

	protected:
		Vector2<double> m_Velocity;
		Vector2<double> m_Accel;
		Vector2<double> m_TargetPos;
		bool m_TargetMode;
		double m_AngularVel;
		double m_AngularAccel;
};

}	// Engine

#endif
