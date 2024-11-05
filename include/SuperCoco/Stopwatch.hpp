#ifndef SUPERCOCO_STOPWATCH_HPP
#define SUPERCOCO_STOPWATCH_HPP

#include <SuperCoco/Export.hpp>
#include <cstdint>

namespace Sce
{
	class SUPER_COCO_API Stopwatch
	{
	public:
		Stopwatch();

		float GetElapsedTime() const;

		float Restart();

	private:
		std::uint64_t m_lastTime;
	};

}

#endif