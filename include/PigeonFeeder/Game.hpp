#pragma once

#include <entt/fwd.hpp>
#include <SuperCoco/Transform.hpp>
#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/Sprite.hpp>
#include <SuperCoco/Core.hpp>

namespace PigeonFeeder
{
	enum class EnemyType
	{
		Cyclop,
		Spider,
		EvilWizard,
		Ghost,
		Mimic
	};

	class Game
	{
	public:
		Game();
		~Game();
		Game(Game&) = delete;
		Game(Game&&) noexcept = delete;

		Game& operator=(Game&) = delete;
		Game& operator=(Game&&) noexcept = delete;

		entt::handle CreatePlayer(entt::registry& world, Sce::Renderer& renderer, Sce::Vector2f position, int layer);
		entt::handle CreateTile(entt::registry& world, Sce::Renderer& renderer, SDL_Rect rect, Sce::Vector2f position, Sce::Vector2f origin, int layer);
		entt::handle CreateWeapon(entt::registry& world, Sce::Renderer& renderer, SDL_Rect rect, Sce::Vector2f position, Sce::Vector2f origin, int layer);
		entt::handle CreateEnemy(entt::registry& world, Sce::Renderer& renderer, EnemyType type, Sce::Vector2f position, int layer, Sce::Core& core);

		static Game& Instance();

	private:
		std::shared_ptr<Sce::Sprite> BuildPlayerSprite(Sce::Renderer& renderer, int layer);
		std::shared_ptr<Sce::Sprite> BuildTileSprite(Sce::Renderer& renderer, SDL_Rect rect, Sce::Vector2f origin, int layer);

		static Game* s_instance;
	};
}


