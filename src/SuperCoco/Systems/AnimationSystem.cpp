#include <SuperCoco/Systems/AnimationSystem.hpp>
#include <SuperCoco/Components/GraphicsComponent.hpp>
#include <SuperCoco/SpriteSheet.hpp>
#include <SuperCoco/Components/SpritesheetComponent.hpp>
#include <SuperCoco/Sprite.hpp>

namespace Sce
{
	AnimationSystem::AnimationSystem(entt::registry* registry) :
	m_registry(registry)
	{
	}

	void AnimationSystem::Update(float deltaTime)
	{
		auto view = m_registry->view<SpritesheetComponent>();
		for (auto&& [entity, spritesheet] : view.each())
		{
			spritesheet.Update(deltaTime);
		}
	}
}
