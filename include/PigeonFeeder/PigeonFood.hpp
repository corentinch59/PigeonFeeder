#ifndef PIGEONFEEDER_FOODSTATE_HPP
#define PIGEONFEEDER_FOODSTATE_HPP

#pragma once

#include <mutex>
#include <entt/entt.hpp>
#include <SuperCoco/Timer.hpp>
#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/Vector2.hpp>
#include <SuperCoco/Transform.hpp>
#include <SuperCoco/Sprite.hpp>
#include <SuperCoco/Core.hpp>

namespace PigeonFeeder
{
	class Game;

	enum class FoodState
	{
		Good,
		Stale
	};

	class PigeonFood
	{
		public:
			PigeonFood(entt::registry& world, Sce::Renderer& renderer, Sce::Core& core, Game* game, Sce::Vector2f position, int layer);

			FoodState GetFoodState() const;
			const Sce::Transform& GetTransform() const;
			const entt::handle& GetHandle() const;

			void MarkConsumed();
			void ClearTimer();
			void EatFood();

		private:
			std::shared_ptr<Sce::Sprite> BuildFoodSprite(Sce::Renderer& renderer, int layer);
			std::shared_ptr<Sce::Sprite> BuildStaleFoodSprite(Sce::Renderer& renderer, int layer);

			entt::handle m_handle;

			Sce::Transform* m_transform;
			FoodState m_foodState;

			std::weak_ptr<Sce::Timer> m_spoiltimer;
	};
}

#endif