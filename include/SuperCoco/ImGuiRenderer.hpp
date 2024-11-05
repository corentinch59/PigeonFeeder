#pragma once

#include <SuperCoco/Export.hpp>

union SDL_Event;
struct ImGuiContext;

namespace Sce
{
	class Renderer;
	class Window;
		
	class SUPER_COCO_API ImGuiRenderer
	{
		public:
			ImGuiRenderer(Window& window, Renderer& renderer);
			ImGuiRenderer(const ImGuiRenderer&) = delete;
			ImGuiRenderer(ImGuiRenderer&&) = delete;
			~ImGuiRenderer();

			ImGuiContext* GetContext();

			void Render(Renderer& renderer);

			void NewFrame();

			void ProcessEvent(SDL_Event& event);

			ImGuiRenderer& operator=(const ImGuiRenderer&) = delete;
			ImGuiRenderer& operator=(ImGuiRenderer&&) = delete;

		private:
			ImGuiContext* m_context;
	};
}
