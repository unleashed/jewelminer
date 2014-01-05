/* Animation.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class describes a simple lineal animation with looping mode or one-time
 * modes.
 */

#include "engine/TextureRegion.h"

#include "engine/Animation.h"

#include <vector>

namespace Engine {

Animation::Animation(double frameduration, Mode mode, std::vector<TextureRegion*> keyframes) :
		m_FrameDuration{frameduration}, m_Mode{mode},
		m_KeyFrames{keyframes} {}

void Animation::SetMode(Mode mode)
{
	m_Mode = mode;
}

// get the keyframe matching statetime on a lineal function
TextureRegion const* Animation::KeyFrame(double statetime) const
{
	std::vector<TextureRegion>::size_type framenumber =
		static_cast<std::vector<TextureRegion>::size_type>(statetime / m_FrameDuration);

	if (m_Mode == Mode::NonLooping) {
		framenumber = std::min(m_KeyFrames.size() - 1, framenumber);
	} else {
		framenumber = framenumber % m_KeyFrames.size();
	}

	return m_KeyFrames[framenumber];
}

}	// Engine
