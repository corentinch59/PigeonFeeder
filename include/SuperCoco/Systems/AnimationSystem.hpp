#ifndef SUPERCOCO_ANIMATIONSYSTEM_HPP
#define SUPERCOCO_ANIMATIONSYSTEM_HPP

#pragma once

#include <SuperCoco/Export.hpp>
#include <entt/entt.hpp>

namespace Sce
{
	class SUPER_COCO_API AnimationSystem
	{
	public:
		AnimationSystem(entt::registry* registry);

		void Update(float deltaTime);
	private:
		entt::registry* m_registry;
	};
}

#endif