#include <PigeonFeeder/IdleState.hpp>
#include <PigeonFeeder/WalkingState.hpp>
#include <PigeonFeeder/Pigeon.hpp>
#include <PigeonFeeder/Game.hpp>
#include <SuperCoco/TimerManager.hpp>
#include <SuperCoco/Timer.hpp>
#include <entt/entt.hpp>
#include <random>
#include <iostream>

namespace PigeonFeeder
{
	void IdleState::StateAction(Game& game)
	{
		bool ready = false;
		bool shouldfear = false;
		std::weak_ptr<Sce::Timer> timer;
		Sce::Vector2f targetPos = {};
		timer = Sce::TimerManager::CreateTimer(2.f, [&targetPos, &ready, &shouldfear]()
			{
				std::random_device rd;
				std::mt19937 generator(rd());
				std::uniform_int_distribution<int> distribution(-100, 400);
				targetPos = { (float)distribution(generator), (float)distribution(generator) };
				ready = true;
				shouldfear = true;
			});

		while (!ready)
		{
			if (game.GetFoodList().size() > 0)
				ready = true;

		}

		if (!timer.expired() && !shouldfear)
		{
			timer.lock().get()->Stop(false);

			std::shared_ptr<PigeonFood> targetfood = game.FindClosestFood(*m_pigeon);
			if (!targetfood)
			{
				auto newState = std::make_unique<IdleState>();
				m_pigeon->SetState(std::move(newState));
			}
			else {
				auto newState = std::make_unique<WalkingState>(targetfood);
				m_pigeon->SetState(std::move(newState));
			}
		}
		else
		{
			auto newState = std::make_unique<WalkingState>(nullptr, m_pigeon->GetTransform().GetPosition() + targetPos);
			m_pigeon->SetState(std::move(newState));
		}
	}
}

