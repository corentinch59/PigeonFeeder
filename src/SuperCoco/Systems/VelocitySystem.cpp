#include <SuperCoco/Systems/VelocitySystem.hpp>
#include <SuperCoco/Components/VelocityComponent.hpp>
#include <SuperCoco/Transform.hpp>

namespace Sce
{
	VelocitySystem* VelocitySystem::s_instance = nullptr;

	VelocitySystem::VelocitySystem(entt::registry* registry) :
	m_registry(registry)
	{
		if (!s_instance)
			s_instance = this;
		else
			throw std::runtime_error("There is more than 1 Velocity system object");
	}

	VelocitySystem::~VelocitySystem()
	{
		s_instance = nullptr;
	}

	void VelocitySystem::ApplyVelocity(float deltaTime)
	{
		auto view = m_registry->view<Transform, VelocityComponent>();
		for (auto&& [entity, transform, velocity] : view.each())
		{
			transform.Translate(velocity.linearVel * deltaTime);

			Vector2f pos = transform.GetPosition();
			if (pos.y > 720 - 128)
			{
				pos.y = 720 - 128;
				transform.SetPosition(pos);
				velocity.linearVel.y = 0.f;
			}
		}
	}

	VelocitySystem& VelocitySystem::Instance()
	{
		return *s_instance;
	}
}