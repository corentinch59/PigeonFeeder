#ifndef SUPERCOCO_CORE_H
#define SUPERCOCO_CORE_H

#pragma once

#include <SuperCoco/Export.hpp>
#include <cstdint>
#include <memory>
#include <entt/entt.hpp>

union SDL_Event;

const int FPS = 30;
const int MILLISECS_PER_FRAME = 1000 / FPS;

namespace Sce 
{
	class Sprite;
	class Renderer;
	class Transform;

	template<typename T> struct Vector2;
	using Vector2f = Vector2<float>;
	using Vector2i = Vector2<int>;

	class SUPER_COCO_API Core
	{
	public:
		Core(std::uint32_t flags = 0);
		Core(const Core& core) = delete;
		~Core();

		Core& operator=(const Core core) = delete;

		static bool PollEvent(SDL_Event& event);
		void Update();

		std::shared_ptr<Sce::Sprite> BuildRunnerSprite(Sce::Renderer& renderer, int layer);

		entt::handle CreateCamera(entt::registry& registry, const Sce::Vector2f& position, const Sce::Vector2f& scale);
		entt::handle CreateRunner(entt::registry& registry, Sce::Renderer& renderer, const Sce::Vector2f& position, int layer);
		entt::handle CreateModel(entt::registry& registry, Sce::Renderer& renderer, const Sce::Vector2f& position, const std::string& filepath);
		entt::handle CreateText(entt::registry& registry, std::string text, const Sce::Vector2f& position);

		Transform GetCameraTransform(entt::registry& registry);
		entt::handle GetHoveredEntity(entt::registry& registry, entt::handle camera);

		static Vector2i GetMousePosition();

		int millisecPreviousFrame;
	};
}


#endif