#include <SuperCoco/Systems/GravitySystem.hpp>
#include <SuperCoco/Components/VelocityComponent.hpp>

namespace Sce
{
	GravitySystem::GravitySystem(entt::registry* registry) :
	m_registry(registry)
	{
	}

	void GravitySystem::ApplyGravity(float deltaTime)
	{
		auto view = m_registry->view<VelocityComponent>();
		for (auto&& [entity, velocity] : view.each())
			velocity.linearVel.y += 1000.f * deltaTime;
	}
}
