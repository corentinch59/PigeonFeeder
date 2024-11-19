#include <PigeonFeeder/Game.hpp>
#include <PigeonFeeder/DeathComponent.hpp>
#include <PigeonFeeder/PigeonFood.hpp>
#include <PigeonFeeder/Pigeon.hpp>
#include <SuperCoco/Texture.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <SuperCoco/SpriteSheet.hpp>
#include <SuperCoco/TimerManager.hpp>
#include <SuperCoco/CollisionShape.hpp>
#include <SuperCoco/Maths.hpp>
#include <SuperCoco/Components/GraphicsComponent.hpp>
#include <SuperCoco/Components/RigidBodyComponent.hpp>
#include <SuperCoco/Components/SpritesheetComponent.hpp>
#include <fmt/core.h>
#include <stdexcept>
#include <limits>

namespace PigeonFeeder
{
	Game* Game::s_instance = nullptr;

	Game::Game()
	{
		if (s_instance != nullptr)
			throw std::runtime_error("There is more than 1 game object");

		s_instance = this;
	}

	Game::~Game()
	{
		s_instance = nullptr;
	}

	void Game::SpawnFood(entt::registry& world, Sce::Renderer& renderer, Sce::Core& core, Sce::Vector2f position, int layer)
	{
		auto food = std::make_shared<PigeonFood>(world, renderer, core, this, position, layer);
		m_pigeonFood.push_back(food);
		Sce::TimerManager::CreateTimer(10.f, [food, this]()
			{
				if (food)
				{
					food->MarkConsumed();
					RemoveFood(food);
				}
			});
	}

	void Game::RemoveFood(const std::shared_ptr<PigeonFood>& food)
	{
		std::lock_guard<std::mutex> lock(m_foodListMutex);

		auto it = std::find(m_pigeonFood.begin(), m_pigeonFood.end(), food);
		if (it != m_pigeonFood.end())
			m_pigeonFood.erase(it);
	}

	const std::vector<std::shared_ptr<PigeonFood>>& Game::GetFoodList() const
	{
		return m_pigeonFood;
	}

	std::shared_ptr<PigeonFood> Game::FindClosestFood(const Pigeon& pigeon)
	{
		const Sce::Vector2f& pigeonPos = pigeon.GetTransform().GetPosition();
		std::shared_ptr<PigeonFood> closestFood = nullptr;
		float minDistance = std::numeric_limits<float>::max();

		for (const auto& food : GetFoodList()) {
			if (!food) continue; // Ignore les objets nulls

			float distance = Sce::Vector2f::SquareDistance(pigeonPos, food->GetTransform().GetPosition());
			if (distance < minDistance && food->GetFoodState() != FoodState::Stale) {
				minDistance = distance;
				closestFood = food;
			}
		}

		return closestFood;
	}

	Game& Game::Instance()
	{
		return *s_instance;
	}
}

