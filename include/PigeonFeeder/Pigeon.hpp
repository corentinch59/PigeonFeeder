#ifndef PIGEONFEEDER_PIGEON_HPP
#define PIGEONFEEDER_PIGEON_HPP

#pragma once

#include <PigeonFeeder/PigeonState.hpp>
#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/Vector2.hpp>
#include <SuperCoco/Transform.hpp>
#include <SuperCoco/Sprite.hpp>
#include <SuperCoco/Core.hpp>
#include <entt/entt.hpp>
#include <string>

namespace PigeonFeeder
{
	class Game;

	class Pigeon
	{
		public:
			Pigeon(entt::registry& world, Sce::Renderer& renderer, Sce::Core& core, std::string name, Sce::Vector2f position, int layer);

			void AnimatePigeon(float delta, float current);
			void BeginBehavior(Game& game, bool& alive);
			void SetState(std::unique_ptr<PigeonState>&& state);

			const Sce::Transform& GetTransform() const;
			const entt::handle& GetPigeonHandle() const;

		private:
			std::shared_ptr<Sce::Sprite> BuildPigeonSprite(Sce::Renderer& renderer, int layer);

			entt::handle m_pigeonHandle;
			entt::handle m_nameHandle;
			
			std::unique_ptr<PigeonState> m_state;
			Sce::Transform* m_transform;

	};
}

#endif