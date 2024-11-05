#include <SuperCoco/Stopwatch.hpp>
#include <SDL2/SDL.h>

namespace Sce
{
	Stopwatch::Stopwatch() :
		m_lastTime(SDL_GetPerformanceCounter())
	{

	}

	float Stopwatch::GetElapsedTime() const
	{
		std::uint64_t now = SDL_GetPerformanceCounter();
		return static_cast<float>(now - m_lastTime) / SDL_GetPerformanceFrequency();
	}

	float Stopwatch::Restart()
	{
		std::uint64_t now = SDL_GetPerformanceCounter();
		float elapsedTime = static_cast<float>(now - m_lastTime) / SDL_GetPerformanceFrequency();
		m_lastTime = now;
		return elapsedTime;
	}
}
