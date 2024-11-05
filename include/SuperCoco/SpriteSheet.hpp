#ifndef SUPERCOCO_SPRITESHEET_HPP
#define SUPERCOCO_SPRITESHEET_HPP

#pragma once

#include <SuperCoco/Export.hpp>
#include <SuperCoco/Asset.hpp>
#include <SuperCoco/Vector2.hpp>
#include <optional>
#include <unordered_map>
#include <vector>
#include <filesystem>

namespace Sce
{
	class SUPER_COCO_API Spritesheet : public Asset
	{
	public:
		struct Animation
		{
			std::string name;
			Vector2i size;
			Vector2i start;
			unsigned int frameCount;
			float frameDuration;
		};

		using Asset::Asset;

		void AddAnimation(std::string name, unsigned int frameCount, float frameDuration, Vector2i start, Vector2i size);
		void AddAnimation(Animation animation);

		void SaveToFile(const std::string& filepath) const;

		static Spritesheet LoadFromFile(const std::string& filepath);

		const Animation& GetAnimation(std::size_t animIndex) const;
		std::optional<std::size_t> GetAnimationByName(const std::string& animName) const;
		std::size_t GetAnimationCount() const;

	private:
		std::unordered_map<std::string /*animName*/, std::size_t /*animIndex*/> m_animationByName;
		std::vector<Animation> m_animations;
	};
}

#endif