/* Animation.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class describes a simple lineal animation with looping mode or one-time
 * modes.
 */

#ifndef ENGINE_ANIMATION_H__
#define ENGINE_ANIMATION_H__

#include "engine/TextureRegion.h"

#include <vector>

namespace Engine {

class Animation {
	public:
		enum class Mode {
			Looping,
			NonLooping
		};

		Animation(double frameduration, Mode mode, std::vector<TextureRegion*> keyframes);

		void SetMode(Mode mode);

		// get the keyframe matching statetime on a lineal function
		TextureRegion const* KeyFrame(double statetime) const;

	private:
		double const m_FrameDuration;
		Mode m_Mode;
		std::vector<TextureRegion*> const m_KeyFrames;
};

}	// Engine

#endif
