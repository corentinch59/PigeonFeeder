#include <SuperCoco/Core.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <SuperCoco/Texture.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <SuperCoco/Sprite.hpp>
#include <SuperCoco/Vector2.hpp>
#include <SuperCoco/Transform.hpp>
#include <SuperCoco/SpriteSheet.hpp>
#include <SuperCoco/Model.hpp>
#include <SuperCoco/Components/GraphicsComponent.hpp>
#include <SuperCoco/Components/SpritesheetComponent.hpp>
#include <SuperCoco/Components/VelocityComponent.hpp>
#include <SuperCoco/Components/CameraComponent.hpp>
#include <SuperCoco/Components/RigidBodyComponent.hpp>
#include <SuperCoco/Components/TextComponent.hpp>
#include <SuperCoco/CollisionShape.hpp>
#include <fmt/core.h>

namespace Sce
{
	Core::Core(std::uint32_t flags) :
	millisecPreviousFrame(0)
	{
		if (SDL_Init(flags) != 0)
		{
			throw std::runtime_error("Failed to initialize SDL");
		}
		if (TTF_Init() != 0)
		{
			throw std::runtime_error("Failed to initialize SDL_ttf");
		}
	}

	Core::~Core()
	{
		TTF_Quit();
		SDL_Quit();
	}

	bool Core::PollEvent(SDL_Event& event)
	{
		return SDL_PollEvent(&event) > 0;
	}

	void Core::Update()
	{
		int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks64() - millisecPreviousFrame);
		if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
			SDL_Delay(timeToWait);

		millisecPreviousFrame = SDL_GetTicks64();
	}

	std::shared_ptr<Sprite> Core::BuildRunnerSprite(Renderer& renderer, int layer)
	{
		std::shared_ptr<Texture> runnerTexture = ResourceManager::Instance().GetTexture("assets/runner.png");
		Sprite runnerSprite(runnerTexture, SDL_Rect{ 0, 0, 32, 32 }, 0.5f, layer);
		runnerSprite.Resize(256, 256);

		std::shared_ptr<Sprite> ptr = std::make_shared<Sprite>(runnerSprite);

		return ptr;
	}

	entt::handle Core::CreateCamera(entt::registry& registry, const Sce::Vector2f& position, const Sce::Vector2f& scale)
	{
		entt::entity entity = registry.create();
		auto& transform = registry.emplace<Transform>(entity);
		transform.SetPosition(position);
		transform.SetScale(scale);

		registry.emplace<CameraComponent>(entity);

		return entt::handle{ registry, entity };
	}

	entt::handle Core::CreateRunner(entt::registry& registry, Renderer& renderer, const Vector2f& position, int layer)
	{
		entt::entity entity = registry.create();

		auto& transform = registry.emplace<Transform>(entity);
		transform.SetPosition({0.f,0.f});
		transform.SetRotation(0.f);
		transform.SetScale({ 1.f, 1.f });

		std::shared_ptr<Sprite> sprite = BuildRunnerSprite(renderer, layer);
		auto& gcomponent = registry.emplace<GraphicsComponent>(entity);
		gcomponent.m_renderable = sprite;

		Spritesheet runnerSpritesheet;
		runnerSpritesheet.AddAnimation("Idle", 5, .1f, { 0, 0 }, { 32, 32 });
		runnerSpritesheet.AddAnimation("Walk", 8, .1f, { 0, 32 }, { 32, 32 });
		runnerSpritesheet.AddAnimation("Jump", 4, .1f, { 0, 64 }, { 32, 32 });
		std::shared_ptr<Spritesheet> spritesheetPtr = std::make_shared<Spritesheet>(std::move(runnerSpritesheet));

		auto& spritesheetComponent = registry.emplace<SpritesheetComponent>(entity, spritesheetPtr, sprite);
		spritesheetComponent.PlayAnimation("Idle");

		auto& rb = registry.emplace<Sce::RigidBodyComponent>(entity, 10.f);
		rb.AddShape(std::make_shared<Sce::BoxShape>(200.f, 200.f));
		rb.SetCenterOfGravity({ 0.5, 0.5 });
		rb.SetTag(Tag::Player);
		rb.TeleportTo(position);

		return entt::handle{ registry, entity };
	}

	entt::handle Core::CreateModel(entt::registry& registry, Sce::Renderer& renderer, const Sce::Vector2f& position, const std::string& filepath)
	{
		entt::entity entity = registry.create();

		auto& transform = registry.emplace<Transform>(entity);
		transform.SetPosition(position);
		transform.SetRotation(0.f);
		transform.SetScale({ 1.f, 1.f });

		Model model = Model::LoadFromFile(filepath);
		std::shared_ptr<Model> modelptr = std::make_shared<Model>(std::move(model));
		registry.emplace<GraphicsComponent>(entity, modelptr);

		return entt::handle{ registry, entity };
	}

	entt::handle Core::CreateText(entt::registry& registry, std::string text, const Sce::Vector2f& position)
	{
		entt::entity entity = registry.create();

		auto& transform = registry.emplace<Transform>(entity);
		transform.SetPosition(position);
		transform.SetRotation(0.f);
		transform.SetScale({ 1.f, 1.f });

		std::shared_ptr<Font> font = ResourceManager::Instance().GetFont("assets/Designer.otf", 18);
		auto& textcomp = registry.emplace<TextComponent>(entity, font, std::move(text), SDL_Color(0, 0, 0, 255));
		std::shared_ptr<TextComponent> textptr = std::make_shared<TextComponent>(std::move(textcomp));
		auto& gfx = registry.emplace<GraphicsComponent>(entity);
		gfx.m_renderable = textptr;
		
		return entt::handle{ registry, entity};
	}

	Transform Core::GetCameraTransform(entt::registry& registry)
	{
		auto view = registry.view<CameraComponent, Transform>();

		for (entt::entity entity : view)
			return view.get<Transform>(entity);
	}

	entt::handle Core::GetHoveredEntity(entt::registry& registry, entt::handle camera)
	{
		Vector2i mousePos = GetMousePosition();
		Vector2f relativePos(mousePos.x, mousePos.y);

		auto& cameraTransform = camera.get<Transform>();

		auto view = registry.view<Transform, GraphicsComponent>();
		for (auto&& [entity, transform, gfx] : view.each())
		{
			Vector2f worldPos = cameraTransform.LocalToWorldPoint(relativePos);
			Vector2f localPos = transform.WorldToLocalPoint(worldPos);

			SDL_FRect bounds = gfx.m_renderable->GetBounds();
			if (localPos.x >= bounds.x && localPos.y >= bounds.y &&
				localPos.x < bounds.x + bounds.w && localPos.y < bounds.y + bounds.h)
			{
				return entt::handle(registry, entity);
			}
		}

		return entt::handle{};
	}

	Vector2i Core::GetMousePosition()
	{
		Vector2i mousePos;
		SDL_GetMouseState(&mousePos.x, &mousePos.y);

		return mousePos;
	}
}

