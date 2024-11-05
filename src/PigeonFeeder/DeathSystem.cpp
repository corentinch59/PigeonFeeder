#include <PigeonFeeder/DeathSystem.hpp>
#include <PigeonFeeder/DeathComponent.hpp>
#include <entt/entt.hpp>

namespace PigeonFeeder
{
	DeathSystem::DeathSystem(entt::registry* registry ) :
	m_registry(registry)
	{
	}

	void DeathSystem::DeathNote()
	{
		auto view = m_registry->view<DeathComponent>();
		for (auto&& [entity] : view.each())
		{
			if(m_registry->valid(entity))
				m_registry->destroy(entity);
		}
	}

}

