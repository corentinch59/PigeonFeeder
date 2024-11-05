#include <SuperCoco/Spritesheet.hpp>
#include <SuperCoco/JsonSerializer.hpp>
#include <fstream>

namespace Sce
{
	void to_json(nlohmann::json& j, const Spritesheet::Animation& anim)
	{
		j["Name"] = anim.name;
		j["Size"] = anim.size;
		j["Start"] = anim.start;
		j["FrameCount"] = anim.frameCount;
		j["FrameDuration"] = anim.frameDuration;
	}

	void from_json(const nlohmann::json& j, Spritesheet::Animation& anim)
	{
		anim.name = j.at("Name");
		anim.size = j.at("Size");
		anim.start = j.at("Start");
		anim.frameCount = j.at("FrameCount");
		anim.frameDuration = j.at("FrameDuration");

	}

	void Spritesheet::AddAnimation(std::string name, unsigned int frameCount, float frameDuration, Vector2i start, Vector2i size)
	{
		Animation animation;
		animation.name = std::move(name);
		animation.frameCount = frameCount;
		animation.frameDuration = frameDuration;
		animation.size = size;
		animation.start = start;

		AddAnimation(std::move(animation));
	}

	void Spritesheet::AddAnimation(Animation animation)
	{
		assert(m_animationByName.find(animation.name) == m_animationByName.end());
		m_animationByName.emplace(animation.name, m_animations.size());

		m_animations.push_back(std::move(animation));
	}

	void Spritesheet::SaveToFile(const std::string& filepath) const
	{
		nlohmann::json doc;
		doc["Animations"] = m_animations;

		std::ofstream file(filepath);
		file << doc.dump(1, '\t');
	}

	Spritesheet Spritesheet::LoadFromFile(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file)
			throw std::runtime_error("couldn't open file " + filepath);

		nlohmann::json doc = nlohmann::json::parse(file);

		Spritesheet spritesheet;

		for (Animation animation : doc.at("Animations"))
			spritesheet.AddAnimation(std::move(animation));

		return spritesheet;
	}

	const Spritesheet::Animation& Spritesheet::GetAnimation(std::size_t animIndex) const
	{
		return m_animations[animIndex];
	}

	std::optional<std::size_t> Spritesheet::GetAnimationByName(const std::string& animName) const
	{
		auto it = m_animationByName.find(animName);
		if (it == m_animationByName.end())
			return {}; //< on retourne une valeur vide

		return it->second; //< on retourne l'index de l'animation
	}

	std::size_t Spritesheet::GetAnimationCount() const
	{
		return m_animations.size();
	}
}