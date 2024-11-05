#include <SuperCoco/Components/SpritesheetComponent.hpp>
#include <SuperCoco/Components/GraphicsComponent.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <SuperCoco/Sprite.hpp>
#include <nlohmann/json.hpp>
#include <entt/entt.hpp>
#include <imgui.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <SDL.h>

namespace Sce
{
	SpritesheetComponent::SpritesheetComponent(std::shared_ptr<const Spritesheet> spriteSheet, std::shared_ptr<Sprite> targetSprite) :
	m_spriteSheet(std::move(spriteSheet)),
	m_targetSprite(std::move(targetSprite)),
	m_currentAnimation(0),
	m_time(0.f),
	m_currentFrameIndex(0)
	{
	}

	void SpritesheetComponent::PlayAnimation(const std::string& animName)
	{
		auto indexOpt = m_spriteSheet->GetAnimationByName(animName);
		if (!indexOpt.has_value())
		{
			fmt::print(stderr, fg(fmt::color::red), "animation \"{}\" not found \n", animName);
			return;
		}

		PlayAnimation(indexOpt.value());
	}

	void SpritesheetComponent::PlayAnimation(std::size_t animIndex)
	{
		if (m_currentAnimation == animIndex)
			return;

		m_currentAnimation = animIndex;
		m_currentFrameIndex = 0;
		m_time = 0.f;
	}

	void SpritesheetComponent::PopulateInspector(WorldEditor& worldEditor)
	{
		const char* anim = m_spriteSheet->GetAnimation(m_currentAnimation).name.c_str();
		if (ImGui::BeginCombo("Animation", anim))
		{
			for (std::size_t i = 0; i < m_spriteSheet->GetAnimationCount(); ++i)
			{
				bool isSelected = (i == m_currentAnimation);
				if (ImGui::Selectable(m_spriteSheet->GetAnimation(i).name.c_str()))
					PlayAnimation(i);

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::Text("Current frame: %u", m_currentFrameIndex);
	}

	nlohmann::json SpritesheetComponent::Serialize(entt::handle entity) const
	{
		nlohmann::json doc;
		doc["SpritesheetPath"] = m_spriteSheet->GetFilepath();

		if (const GraphicsComponent* gfxEntity = entity.try_get<GraphicsComponent>())
		{
			if (gfxEntity->m_renderable == m_targetSprite)
			{
				// On indique que le renderable est bien notre sprite
				doc["SpriteGfx"] = true;
			}
		}

		return doc;
	}

	void SpritesheetComponent::Unserialize(entt::handle entity, const nlohmann::json& doc)
	{
		GraphicsComponent* gfxEntity = entity.try_get<GraphicsComponent>();
		if (gfxEntity && doc["SpriteGfx"] == true)
		{
			std::shared_ptr<const Spritesheet> spriteSheet = ResourceManager::Instance().GetSpritesheet(doc["SpritesheetPath"]);
			std::shared_ptr<Sprite> targetSprite = std::static_pointer_cast<Sprite>(gfxEntity->m_renderable); // static_cast mais pour les std::shared_ptr

			entity.emplace<SpritesheetComponent>(std::move(spriteSheet), std::move(targetSprite));
		}
	}

	void SpritesheetComponent::Update(float deltaTime)
	{
		if (m_currentAnimation >= m_spriteSheet->GetAnimationCount())
			return;

		const Spritesheet::Animation& anim = m_spriteSheet->GetAnimation(m_currentAnimation);

		m_time += deltaTime;
		while (m_time >= anim.frameDuration)
		{
			m_time -= anim.frameDuration;

			if (++m_currentFrameIndex >= anim.frameCount)
				m_currentFrameIndex = 0;

			SDL_Rect rect{ anim.start.x + anim.size.x * m_currentFrameIndex, anim.start.y, anim.size.x, anim.size.y };
			m_targetSprite->SetRect(rect);
		}
	}
}