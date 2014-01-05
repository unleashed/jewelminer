/* FPSCounter.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This is an extremely simple FPS counter, useful for profiling and debugging.
 */

#ifndef ENGINE_FPSCOUNTER_H__
#define ENGINE_FPSCOUNTER_H__

#include <SDL2/SDL_timer.h>

#include <functional>

namespace Engine {

class FPSCounter {
	public:
		FPSCounter(std::function<void(unsigned int)> func) : m_Func{func},
			m_Frames{0}, m_StartTime{SDL_GetTicks()} {}

		unsigned int Frames() const { return m_Frames; }
		void LogFrame() {
			++m_Frames;
			if (SDL_GetTicks() - m_StartTime >= 1000U) {
				m_Func(m_Frames);
				m_Frames = 0;
				m_StartTime = SDL_GetTicks();
			}
		}

	private:
		std::function<void(unsigned int)> m_Func;
		unsigned int m_Frames;
		Uint32 m_StartTime;
};

}	// Engine

#endif
