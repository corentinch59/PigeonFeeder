#include <fmt/core.h>
#include <fmt/color.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <entt/entt.hpp>
#include <memory>
#include <vector>
#include <thread>

#pragma region ENGINE_INCLUDES
#include <SuperCoco/Core.hpp>
#include <SuperCoco/Window.hpp>
#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/Texture.hpp>
#include <SuperCoco/Sprite.hpp>
#include <SuperCoco/Surface.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <SuperCoco/InputManager.hpp>
#include <SuperCoco/Stopwatch.hpp>
#include <SuperCoco/Transform.hpp>
#include <SuperCoco/SpriteSheet.hpp>
#include <SuperCoco/Model.hpp>
#include <SuperCoco/ComponentRegistry.hpp>
#include <SuperCoco/TimerManager.hpp>
#include <SuperCoco/Maths.hpp>
#include <SuperCoco/Systems/RenderSystem.hpp>
#include <SuperCoco/Systems/VelocitySystem.hpp>
#include <SuperCoco/Systems/GravitySystem.hpp>
#include <SuperCoco/Systems/AnimationSystem.hpp>
#include <SuperCoco/Systems/PhysicsSystem.hpp>
#include <SuperCoco/Components/VelocityComponent.hpp>
#include <SuperCoco/Components/GraphicsComponent.hpp>
#include <SuperCoco/Components/CameraComponent.hpp>
#include <SuperCoco/Components/SpritesheetComponent.hpp>
#include <SuperCoco/Components/RigidBodyComponent.hpp>
#include <SuperCoco/Components/TextComponent.hpp>
#include <SuperCoco/CollisionShape.hpp>
#pragma endregion
#pragma region GAME
#include <PigeonFeeder/Game.hpp>
#include <PigeonFeeder/DeathSystem.hpp>
#include <PigeonFeeder/Pigeon.hpp>
#include <PigeonFeeder/PigeonFood.hpp>
#include <PigeonFeeder/IdleState.hpp>

#pragma endregion

#ifdef WITH_SCE_EDITOR
#include <SuperCoco/WorldEditor.hpp>
#include <SuperCoco/ImGuiRenderer.hpp>
#include <imgui.h>
#endif

#include <SuperCoco/WelcomeMsg.inl>

int main(int argc, char* argv[])
{
	#pragma region SuperCocoEngine
	WelcomeMessage();
	
	srand(time(NULL));

	//Initialising Engine
	Sce::Core core(SDL_INIT_GAMECONTROLLER);
	Sce::Window window("Pigeon Feeder", 1080, 769);
	Sce::Renderer renderer(window, 1);
	Sce::ResourceManager rcmgr(&renderer);
	Sce::InputManager inputmgr;
	Sce::TimerManager timermgr;

#ifdef WITH_SCE_EDITOR
	Sce::ImGuiRenderer imgui(window, renderer);
	ImGui::SetCurrentContext(imgui.GetContext());
#endif

	//Initialising Systems
	entt::registry world;
	Sce::RenderSystem renderSystem(&world, &renderer);
	Sce::VelocitySystem velocitySystem(&world);
	Sce::GravitySystem gravitySystem(&world);
	Sce::AnimationSystem animationSystem(&world);
	Sce::PhysicsSystem physicSystem(world);

	Sce::ComponentRegistry componentRegistry;

#ifdef WITH_SCE_EDITOR
	std::optional<Sce::WorldEditor> worldEditor;
#endif
#pragma endregion

	PigeonFeeder::Game game;
	PigeonFeeder::DeathSystem deathSystem(&world);

	inputmgr.BindKeyPressed(SDLK_ESCAPE, "Close");
	inputmgr.BindMouseButtonPressed(Sce::MouseButton::LEFT_CLICK, "SpawnFood");

	#pragma region ENTITIES
	//Creating camera
	entt::handle camera = core.CreateCamera(world, { 0.f, 0.f }, {1.f, 1.f});

	//Creating Pigeons
	PigeonFeeder::Pigeon pigeon1(world, renderer, core, "Pigeon1", { (1080.f / 2.0f), (769.f / 2.f) }, 1);
	PigeonFeeder::Pigeon pigeon2(world, renderer, core, "Pigeon2", { (1080.f / 2.0f) - 200.f, (769.f / 2.f) }, 1);

	#ifdef WITH_SCE_EDITOR
	inputmgr.BindKeyPressed(SDLK_F1, "OpenEditor");
	inputmgr.BindAction("OpenEditor", [&](bool active, int, float) 
	{
		if (!active)
			return;

		if (worldEditor)
		{
			worldEditor.reset();
		}
		else
		{
			worldEditor.emplace(window, world, componentRegistry);
		}
	});
	#endif
	#pragma endregion

	Sce::Stopwatch stopwatch;
	 
	bool isOpen = true;

	//Launching threads
	std::thread tp1(&PigeonFeeder::Pigeon::BeginBehavior, &pigeon1, std::ref(game), std::ref(isOpen));
	std::thread tp2(&PigeonFeeder::Pigeon::BeginBehavior, &pigeon2, std::ref(game), std::ref(isOpen));

	tp1.detach();
	tp2.detach();

	inputmgr.BindAction("Close", [&isOpen](bool, int, float) { isOpen = false; });
	inputmgr.BindAction("SpawnFood", [&](bool active, int, float)
		{
			if (!active)
				return;
			Sce::Vector2i pos = core.GetMousePosition();
			game.SpawnFood(world, renderer, core, { (float)pos.x, (float)pos.y }, 0);
		});

	while (isOpen)
	{
		float deltaTime = stopwatch.Restart();

		SDL_Event event;
		while (Sce::Core::PollEvent(event) > 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				isOpen = false;
				break;
			}

			inputmgr.HandleEvent(event);

#ifdef WITH_SCE_EDITOR
			imgui.ProcessEvent(event);
#endif
		}

		core.Update();
		renderer.RenderDrawColor(100, 0, 0, 0);
		renderer.RenderClear();

#ifdef WITH_SCE_EDITOR
		imgui.NewFrame();
#endif
		//Engine systems
		timermgr.UpdateTimers(deltaTime);
		animationSystem.Update(deltaTime);
		gravitySystem.ApplyGravity(deltaTime);
		velocitySystem.ApplyVelocity(deltaTime);
		physicSystem.Update(deltaTime);

		//Game systems
		deathSystem.DeathNote();

		renderSystem.Render(deltaTime);

#ifdef WITH_SCE_EDITOR
		physicSystem.DebugDraw(renderer, core.GetCameraTransform(world).WorldToLocalMatrix());

		if (worldEditor)
			worldEditor->Render();

		imgui.Render(renderer);
#endif

		renderer.RenderPresent();
	}
	

	return 0;
}
