#ifndef SUPERCOCO_SPRITESHEETCOMPONENT_HPP
#define SUPERCOCO_SPRITESHEETCOMPONENT_HPP

#include <SuperCoco/Export.hpp>
#include <SuperCoco/SpriteSheet.hpp>
#include <entt/fwd.hpp>
#include <nlohmann/json_fwd.hpp>
#include <memory>

namespace Sce
{
	class AnimationSystem;
	class Sprite;
	class WorldEditor;

	class SUPER_COCO_API SpritesheetComponent
	{
		friend AnimationSystem;
	public:
		SpritesheetComponent(std::shared_ptr<const Spritesheet> spriteSheet, std::shared_ptr<Sprite> targetSprite);

		void PlayAnimation(const std::string& animName);
		void PlayAnimation(std::size_t animIndex);

		void PopulateInspector(WorldEditor& worldEditor);

		nlohmann::json Serialize(entt::handle entity) const;
		static void Unserialize(entt::handle entity, const nlohmann::json& doc);

		void Update(float deltaTime);

		std::size_t m_currentAnimation;
		float m_time;
		std::shared_ptr<const Spritesheet> m_spriteSheet;
		std::shared_ptr<Sprite> m_targetSprite;
		unsigned int m_currentFrameIndex;
	};

}

#endif