#include <PigeonFeeder/WalkingState.hpp>
#include <PigeonFeeder/IdleState.hpp>
#include <PigeonFeeder/Game.hpp>
#include <PigeonFeeder/PigeonFood.hpp>
#include <PigeonFeeder/Pigeon.hpp>
#include <SuperCoco/Components/RigidBodyComponent.hpp>
#include <iostream>

namespace PigeonFeeder
{
	WalkingState::WalkingState(std::shared_ptr<PigeonFood> food, Sce::Vector2f target) :
	m_food(food),
	m_target(target),
	m_towardfood(false)
	{

	}

	WalkingState::WalkingState(std::shared_ptr<PigeonFood> food) :
	m_food(food),
	m_towardfood(true)
	{
		auto& transform = food->GetHandle().get<Sce::Transform>();
		m_target = transform.GetPosition();
	}

	void WalkingState::StateAction(Game& game)
	{
		if (m_towardfood && !m_food)
		{
			auto newState = std::make_unique<IdleState>();
			m_pigeon->SetState(std::move(newState));
		}

		if (Sce::Vector2f::Distance(m_pigeon->GetTransform().GetPosition(), m_target) <= 100.f)
		{
			if (m_food)
			{
				if (m_food->GetFoodState() == FoodState::Good)
				{
					game.RemoveFood(m_food);
					m_food->EatFood();
				}
			}

			auto newState = std::make_unique<IdleState>();
			m_pigeon->SetState(std::move(newState));
			
			auto& rb = m_pigeon->GetPigeonHandle().get<Sce::RigidBodyComponent>();
			Sce::Vector2f vel = (m_target - m_pigeon->GetTransform().GetPosition()).Normalize();
			rb.SetLinearVelocity({0.f,0.f});
		}
		else
		{
			auto& rb = m_pigeon->GetPigeonHandle().get<Sce::RigidBodyComponent>();
			Sce::Vector2f vel = (m_target - m_pigeon->GetTransform().GetPosition()).Normalize();
			rb.SetLinearVelocity(vel * 50);
		}
	}
}

