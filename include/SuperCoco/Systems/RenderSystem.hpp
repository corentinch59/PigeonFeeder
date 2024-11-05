#ifndef SUPERCOCO_RENDERSYSTEM_HPP
#define SUPERCOCO_RENDERSYSTEM_HPP

#pragma once

#include <SuperCoco/Export.hpp>
#include <entt/entt.hpp>

namespace Sce
{
	class Transform;
	class Renderer;

	class SUPER_COCO_API RenderSystem
	{
	public:
		RenderSystem(entt::registry* registry, Renderer* renderer);

		void Render(float);

	private:
		entt::registry* m_registry;
		Renderer* m_renderer;
	};
}

#endif