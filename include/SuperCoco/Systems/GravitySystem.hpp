#ifndef SUPERCOCO_GRAVITYSYSTEM_HPP
#define SUPERCOCO_GRAVITYSYSTEM_HPP

#pragma once

#include <SuperCoco/Export.hpp>
#include <entt/entt.hpp>

namespace Sce
{
	class SUPER_COCO_API GravitySystem
	{
	public:
		GravitySystem(entt::registry* registry);

		void ApplyGravity(float deltaTime);

	private:
		entt::registry* m_registry;
	};
}

#endif