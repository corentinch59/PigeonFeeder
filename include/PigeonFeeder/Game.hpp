#pragma once

#include <entt/fwd.hpp>
#include <SuperCoco/Transform.hpp>
#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/Sprite.hpp>
#include <SuperCoco/Core.hpp>
#include <mutex>

namespace PigeonFeeder
{
	class PigeonFood;
	class Pigeon;

	class Game
	{
		public:
			Game();
			~Game();
			Game(Game&) = delete;
			Game(Game&&) noexcept = delete;

			Game& operator=(Game&) = delete;
			Game& operator=(Game&&) noexcept = delete;

			void SpawnFood(entt::registry& world, Sce::Renderer& renderer, Sce::Core& core, Sce::Vector2f position, int layer);
			void RemoveFood(const std::shared_ptr<PigeonFood>& food);

			const std::vector<std::shared_ptr<PigeonFood>>& GetFoodList() const;
			std::shared_ptr<PigeonFood> FindClosestFood(const Pigeon& pigeon);

			static Game& Instance();

		private:
			std::vector<std::shared_ptr<PigeonFood>> m_pigeonFood;
			std::vector<std::shared_ptr<PigeonFood>> m_staleFood;
			std::mutex m_foodListMutex;

			static Game* s_instance;
	};
}


