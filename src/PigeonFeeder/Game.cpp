#include <PigeonFeeder/Game.hpp>
#include <PigeonFeeder/HealthComponent.hpp>
#include <PigeonFeeder/DeathComponent.hpp>
#include <SuperCoco/Texture.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <SuperCoco/SpriteSheet.hpp>
#include <SuperCoco/TimerManager.hpp>
#include <SuperCoco/CollisionShape.hpp>
#include <SuperCoco/Maths.hpp>
#include <SuperCoco/Components/GraphicsComponent.hpp>
#include <SuperCoco/Components/RigidBodyComponent.hpp>
#include <SuperCoco/Components/SpritesheetComponent.hpp>
#include <fmt/core.h>
#include <stdexcept>

namespace PigeonFeeder
{
	Game* Game::s_instance = nullptr;

	Game::Game()
	{
		if (s_instance != nullptr)
			throw std::runtime_error("There is more than 1 game object");

		s_instance = this;
	}

	Game::~Game()
	{
		s_instance = nullptr;
	}

	entt::handle Game::CreatePlayer(entt::registry& world, Sce::Renderer& renderer, Sce::Vector2f position, int layer)
	{
		entt::entity entity = world.create();

		auto& transform = world.emplace<Sce::Transform>(entity);

		std::shared_ptr<Sce::Sprite> sprite = BuildPlayerSprite(renderer, layer);
		auto& gcomponent = world.emplace<Sce::GraphicsComponent>(entity);
		gcomponent.m_renderable = sprite;

		Sce::Spritesheet spritesheet;
		std::shared_ptr<Sce::Spritesheet> spritesheetPtr = std::make_shared<Sce::Spritesheet>(std::move(spritesheet));
		auto& spritesheetComponent = world.emplace<Sce::SpritesheetComponent>(entity, spritesheetPtr, sprite);

		auto& rb = world.emplace<Sce::RigidBodyComponent>(entity, 10.0f, 1.0f);
		rb.AddShape(std::make_shared<Sce::BoxShape>(48.f, 48.f));
		rb.SetCenterOfGravity({ 0.5, 0.5 });
		rb.SetTag(Sce::Tag::Player);
		rb.TeleportTo(position);

		return entt::handle{world, entity};
	}

	entt::handle Game::CreateTile(entt::registry& world, Sce::Renderer& renderer, SDL_Rect rect, Sce::Vector2f position, Sce::Vector2f origin, int layer)
	{
		entt::entity entity = world.create();

		auto& transform = world.emplace<Sce::Transform>(entity);
		transform.SetPosition(position);

		std::shared_ptr<Sce::Sprite> sprite = BuildTileSprite(renderer, rect, origin, layer);
		auto& gcomponent = world.emplace<Sce::GraphicsComponent>(entity);
		gcomponent.m_renderable = sprite;

		return entt::handle{ world, entity };
	}

	entt::handle Game::CreateWeapon(entt::registry& world, Sce::Renderer& renderer, SDL_Rect rect, Sce::Vector2f position, Sce::Vector2f origin, int layer)
	{
		entt::entity entity = world.create();

		auto& transform = world.emplace<Sce::Transform>(entity);

		std::shared_ptr<Sce::Sprite> sprite = BuildTileSprite(renderer, rect, {0.5f, 1.5f}, layer);
		auto& gcomponent = world.emplace<Sce::GraphicsComponent>(entity);
		gcomponent.m_renderable = sprite;

		auto& rb = world.emplace<Sce::RigidBodyComponent>(entity, 1.0f);
		rb.AddShape(std::make_shared<Sce::BoxShape>(32.f, 48.f), {0.f,-48.f});
		rb.SetCenterOfGravity(origin);
		rb.SetTag(Sce::Tag::Weapon);
		rb.SetSensor(true);
		rb.TeleportTo(position);		

		return entt::handle{ world, entity };
	}

	entt::handle Game::CreateEnemy(entt::registry& world, Sce::Renderer& renderer, EnemyType type, Sce::Vector2f position, int layer, Sce::Core& core)
	{
		entt::entity entity = world.create();

		auto& transform = world.emplace<Sce::Transform>(entity);

		SDL_Rect enemyRect = { 16 * 2, 16 * 10, 16, 16 };
		switch (type)
		{
			case EnemyType::Cyclop:
				enemyRect = { 16, 16 * 9, 16, 16 };
				break;
			case EnemyType::Spider:
				enemyRect = { 16 * 2, 16 * 10, 16, 16 };
				break;
			case EnemyType::EvilWizard:
				enemyRect = { 16 * 3, 16 * 9, 16, 16 };
				break;
			case EnemyType::Ghost:
				enemyRect = { 16, 16 * 10, 16, 16 };
				break;
			case EnemyType::Mimic:
				enemyRect = { 16 * 8, 16 * 7, 16, 16 };
				break;
		}

		std::shared_ptr<Sce::Sprite> sprite = BuildTileSprite(renderer, enemyRect, { 0.5f, 0.5f }, layer);
		auto& gcomponent = world.emplace<Sce::GraphicsComponent>(entity);
		gcomponent.m_renderable = sprite;

		auto ondeath = [entity, &world]
			{
				world.emplace<DeathComponent>(entity);
			};
		auto& hcomp = world.emplace<HealthComponent>(entity, 100.f);
		hcomp.SetOnDeathCallback(ondeath);

		std::function<void()> collisionLambda = [entity, &core, &world]()
			{
				Sce::Transform& eTransform = world.get<Sce::Transform>(entity);
				entt::handle text = core.CreateText(world, "Ouch", eTransform.GetPosition() + Sce::Vector2f{0.f, -48.f});

				HealthComponent& ehp = world.get<HealthComponent>(entity);
				ehp.ModifyHealth(-50.f);

				Sce::TimerManager::CreateContinuousTimer(0.75f, [entity, &world](float delta, float current)
				{
					if (!world.valid(entity))
						return;
					Sce::Transform& transform = world.get<Sce::Transform>(entity);
					float progress = Sce::Maths::EaseOutBack(current / 0.75f);
					transform.SetScale({ progress, progress });
				});

				Sce::TimerManager::CreateTimer(.25f,
				[text, &world]
				{
					if(world.valid(text))
						world.emplace<DeathComponent>(text);
				}, 0.75f);
			};
		auto col = [](cpArbiter* arb, cpSpace* space, void* data) -> cpBool { return true; };
		auto& rb = world.emplace<Sce::RigidBodyComponent>(entity, 10.f, 0.0f, collisionLambda);
		rb.AddShape(std::make_shared<Sce::BoxShape>(48.f, 48.f), Sce::Vector2f{0.f,0.f }, col);
		rb.SetSensor(true);
		rb.SetTag(Sce::Tag::Enemy);
		rb.TeleportTo(position);



		return entt::handle{ world, entity };
	}

	std::shared_ptr<Sce::Sprite> Game::BuildPlayerSprite(Sce::Renderer& renderer, int layer)
	{
		std::shared_ptr<Sce::Texture> textureTile = Sce::ResourceManager::Instance().GetTexture("assets/tilemap_packed.png");
		Sce::Sprite playerSprite(textureTile, SDL_Rect{ 16 * 2, 16 * 8, 16, 16 }, 0.5f, layer);
		playerSprite.Resize(48, 48);

		std::shared_ptr<Sce::Sprite> ptr = std::make_shared<Sce::Sprite>(std::move(playerSprite));
		return ptr;
	}

	std::shared_ptr<Sce::Sprite> Game::BuildTileSprite(Sce::Renderer& renderer, SDL_Rect rect, Sce::Vector2f origin, int layer)
	{
		std::shared_ptr<Sce::Texture> textureTile = Sce::ResourceManager::Instance().GetTexture("assets/tilemap_packed.png");
		Sce::Sprite sprite(textureTile, rect, origin.x, layer);
		sprite.SetOrigin(origin);
		sprite.Resize(48, 48);

		std::shared_ptr<Sce::Sprite> ptr = std::make_shared<Sce::Sprite>(std::move(sprite));
		return ptr;
	}

	Game& Game::Instance()
	{
		return *s_instance;
	}
}

