#include <PigeonFeeder/PigeonFood.hpp>
#include <SuperCoco/Texture.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <SuperCoco/SpriteSheet.hpp>
#include <SuperCoco/CollisionShape.hpp>
#include <SuperCoco/TimerManager.hpp>
#include <SuperCoco/Components/SpritesheetComponent.hpp>
#include <SuperCoco/Components/GraphicsComponent.hpp>
#include <SuperCoco/Components/RigidBodyComponent.hpp>
#include <SuperCoco/Components/TextComponent.hpp>
#include <PigeonFeeder/DeathComponent.hpp>
#include <PigeonFeeder/Game.hpp>
#include <stdexcept>
#include <iostream>

namespace PigeonFeeder
{
	PigeonFood::PigeonFood(entt::registry& world, Sce::Renderer& renderer, Sce::Core& core, Game* game, Sce::Vector2f position, int layer) :
	m_foodState(FoodState::Good)
	{
		entt::entity entity = world.create();

		m_transform = &world.emplace<Sce::Transform>(entity);

		std::shared_ptr<Sce::Sprite> sprite = BuildFoodSprite(renderer, layer);
		auto& gcomponent = world.emplace<Sce::GraphicsComponent>(entity);
		gcomponent.m_renderable = sprite;

		Sce::Spritesheet spritesheet;
		std::shared_ptr<Sce::Spritesheet> spritesheetPtr = std::make_shared<Sce::Spritesheet>(std::move(spritesheet));
		auto& spritesheetComponent = world.emplace<Sce::SpritesheetComponent>(entity, spritesheetPtr, sprite);

		auto col = [](cpArbiter* arb, cpSpace* space, void* data) -> cpBool { return true; };
		auto& rb = world.emplace<Sce::RigidBodyComponent>(entity, 10.f, 0.0f);
		rb.AddShape(std::make_shared<Sce::BoxShape>(48.f, 48.f), Sce::Vector2f{ 0.f, 0.f }, col);
		rb.SetSensor(true);
		rb.SetTag(Sce::Tag::Enemy);
		rb.TeleportTo(position);

		m_transform->SetPosition(position);

		m_handle = entt::handle(world, entity);

		m_spoiltimer = Sce::TimerManager::CreateTimer(7.f, [&]() 
			{
				m_foodState = FoodState::Stale;

				std::shared_ptr<Sce::Sprite> sprite = BuildStaleFoodSprite(renderer, layer);
				gcomponent.m_renderable = sprite;
			});

	}

	FoodState PigeonFood::GetFoodState() const
	{
		return m_foodState;
	}

	const Sce::Transform& PigeonFood::GetTransform() const
	{
		return *m_transform;
	}

	const entt::handle& PigeonFood::GetHandle() const
	{
		return m_handle;
	}

	void PigeonFood::MarkConsumed()
	{
		ClearTimer();
		if(m_handle)
			m_handle.emplace<DeathComponent>();
	}

	void PigeonFood::ClearTimer()
	{
		if (m_spoiltimer.expired())
			return;

		auto t = m_spoiltimer.lock();
		//Stopping the Timer func so the timer is cleared from the TimerSystem
		t.get()->Stop(false);
	}

	void PigeonFood::EatFood()
	{		
		MarkConsumed();
	}

	std::shared_ptr<Sce::Sprite> PigeonFood::BuildFoodSprite(Sce::Renderer& renderer, int layer)
	{
		std::shared_ptr<Sce::Texture> texturetile = Sce::ResourceManager::Instance().GetTexture("assets/tilemap_packed.png");
		Sce::Sprite foodSprite(texturetile, SDL_Rect{ 16 * 8, 16 * 9, 16, 16 }, 0.5f, layer);
		foodSprite.Resize(48, 48);

		std::shared_ptr<Sce::Sprite> ptr = std::make_shared<Sce::Sprite>(std::move(foodSprite));
		return ptr;
	}

	std::shared_ptr<Sce::Sprite> PigeonFood::BuildStaleFoodSprite(Sce::Renderer& renderer, int layer)
	{
		std::shared_ptr<Sce::Texture> texturetile = Sce::ResourceManager::Instance().GetTexture("assets/tilemap_packed.png");
		Sce::Sprite staleSprite(texturetile, SDL_Rect{ 16 * 7, 16 * 9, 16, 16 }, 0.5f, layer);
		staleSprite.Resize(48, 48);

		std::shared_ptr<Sce::Sprite> ptr = std::make_shared<Sce::Sprite>(std::move(staleSprite));
		return ptr;
	}

}
