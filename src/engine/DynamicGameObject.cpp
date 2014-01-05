/* DynamicGameObject.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class add specialization to game objects. In particular, this one deals with
 * movement and rotation. The MoveTo() method enables target mode, which is specially
 * useful to define waypoints or paths when chained.
 */

#include "engine/Vector2.h"
#include "engine/GameObject.h"

#include "engine/DynamicGameObject.h"

namespace Engine {

DynamicGameObject::DynamicGameObject() :
	m_Velocity{}, m_Accel{}, m_TargetPos{},
	m_TargetMode{false}, m_AngularVel{0}, m_AngularAccel{0} {}

DynamicGameObject::DynamicGameObject(double x, double y,
			int width, int height, double angle, double angularvel) :
		GameObject(x, y, width, height, angle),
		m_Velocity{}, m_Accel{}, m_TargetPos{},
		m_TargetMode{false}, m_AngularVel{angularvel}, m_AngularAccel{0} {}

DynamicGameObject::DynamicGameObject(Vector2<double> const& position,
			int width, int height, double angle, double angularvel) :
		GameObject(position, width, height, angle),
		m_Velocity{}, m_Accel{}, m_TargetPos{},
		m_TargetMode{false}, m_AngularVel{angularvel}, m_AngularAccel{0} {}

DynamicGameObject::~DynamicGameObject() {}

void DynamicGameObject::Update(double deltatime)
{
	GameObject::Update(deltatime);
	this->UpdatePosition(deltatime);
	this->UpdateAngle(deltatime);
}

void DynamicGameObject::UpdatePosition(double deltatime) {
	m_Velocity += m_Accel * deltatime;
	m_Position += m_Velocity * deltatime;
	if (m_TargetMode) {
		// this check does not compare to zero to make for potential precision loss
		if (std::abs(m_Velocity.Angle() - (m_TargetPos - m_Position).Angle()) > 0.001) {
			m_Position = m_TargetPos;
			m_Velocity.Set(0, 0);
			m_Accel.Set(0, 0);
			m_TargetMode = false;
		}
	}
}

void DynamicGameObject::UpdateAngle(double deltatime) {
	if (m_AngularAccel != 0) {
		m_AngularVel += m_AngularAccel * deltatime;
	}
	if (m_AngularVel != 0) {
		m_Angle += remainder(m_AngularVel * deltatime, 360.0);
	}
}

void DynamicGameObject::Velocity(Vector2<double> const& velocity)
{
	m_Velocity = velocity;
	m_TargetMode = false;
}

void DynamicGameObject::Accel(Vector2<double> const& accel)
{
	m_Accel = accel;
	m_TargetMode = false;
}

void DynamicGameObject::MoveTo(Vector2<double> const& targetpos, double vel, double accel)
{
	m_TargetMode = true;
	m_TargetPos = targetpos;
	m_Velocity = (m_TargetPos - m_Position).Nor();
	m_Accel = m_Velocity * accel;
	m_Velocity *= vel;
}

}	// Engine
