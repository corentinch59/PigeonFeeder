#include <PigeonFeeder/Pigeon.hpp>
#include <SuperCoco/SpriteSheet.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <SuperCoco/CollisionShape.hpp>
#include <SuperCoco/TimerManager.hpp>
#include <SuperCoco/Components/SpritesheetComponent.hpp>
#include <SuperCoco/Components/GraphicsComponent.hpp>
#include <SuperCoco/Components/RigidBodyComponent.hpp>
#include <PigeonFeeder/Game.hpp>
#include <PigeonFeeder/IdleState.hpp>
#include <iostream>
#include <thread>

namespace PigeonFeeder
{
	Pigeon::Pigeon(entt::registry& world, Sce::Renderer& renderer, Sce::Core& core, std::string name, Sce::Vector2f position, int layer)
	{
		m_state = std::make_unique<IdleState>();
		m_state->SetPigeon(this);

		entt::entity entity = world.create();

		m_transform = &world.emplace<Sce::Transform>(entity);

		std::shared_ptr<Sce::Sprite> sprite = BuildPigeonSprite(renderer, layer);
		auto& gcomponent = world.emplace<Sce::GraphicsComponent>(entity);
		gcomponent.m_renderable = sprite;

		Sce::Spritesheet spritesheet;
		std::shared_ptr<Sce::Spritesheet> spritesheetPtr = std::make_shared<Sce::Spritesheet>(std::move(spritesheet));
		auto& spritesheetComponent = world.emplace<Sce::SpritesheetComponent>(entity, spritesheetPtr, sprite);

		m_nameHandle = core.CreateText(world, name, Sce::Vector2f{ -42.f, -42.f }, { 255, 255, 255, 255 });
		m_nameHandle.get<Sce::Transform>().SetParent(m_transform);

		auto& rb = world.emplace<Sce::RigidBodyComponent>(entity, 10.f, 1.f);
		rb.AddShape(std::make_shared<Sce::BoxShape>(48.f, 48.f));
		rb.SetCenterOfGravity({ 0.5f, 0.5f });
		rb.SetTag(Sce::Tag::Player);
		rb.TeleportTo(position);

		m_pigeonHandle = entt::handle(world, entity);

		Sce::TimerManager::CreateContinuousTimer(2.0f, [&](float delta, float current) {AnimatePigeon(delta, current); }, std::function<void()>(), 0.f, true);

	}

	void Pigeon::AnimatePigeon(float delta, float current)
	{
		float ratio = 1.f / 4.f * sin(PI / 2.f * current) + 3.f / 4.f;
		m_transform->SetScale(Sce::Vector2f{ 1.f, 0.f } + Sce::Vector2f{ 0.f , 1.f } * ratio);
		Sce::Transform* ntransform = &m_nameHandle.get<Sce::Transform>();

		ntransform->SetScale(Sce::Vector2f{ 1.f, 0.f } + Sce::Vector2f{ 0.f , 1.f } / ratio);
		ntransform->SetPosition(Sce::Vector2f{ -42.f, 0.f } + Sce::Vector2f{ 0.f, -42.f } / ratio);
	}

	void Pigeon::BeginBehavior(Game& game, bool& alive)
	{
		while (alive)
		{
			m_state->StateAction(game);
			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
	}

	void Pigeon::SetState(std::unique_ptr<PigeonState>&& state)
	{
		m_state = std::move(state);
		m_state->SetPigeon(this);
	}

	const Sce::Transform& Pigeon::GetTransform() const
	{
		return *m_transform;
	}

	const entt::handle& Pigeon::GetPigeonHandle() const
	{
		return m_pigeonHandle;
	}

	std::shared_ptr<Sce::Sprite> Pigeon::BuildPigeonSprite(Sce::Renderer& renderer, int layer)
	{
		std::shared_ptr<Sce::Texture> textureTile = Sce::ResourceManager::Instance().GetTexture("assets/tilemap_packed.png");
		Sce::Sprite pigeonSprite(textureTile, SDL_Rect{ 16 * 0, 16 * 10, 16, 16 }, 0.5f, layer);
		pigeonSprite.Resize(48, 48);

		std::shared_ptr<Sce::Sprite> ptr = std::make_shared<Sce::Sprite>(std::move(pigeonSprite));
		return ptr;
	}
}