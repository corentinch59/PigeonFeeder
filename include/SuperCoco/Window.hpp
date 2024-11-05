#ifndef SUPERCOCO_WINDOW_H
#define SUPERCOCOWINDOW_H

#include <SuperCoco/Export.hpp>
#include <SuperCoco/Vector2.hpp>
#include <cstdint>
#include <string>

struct SDL_Window;

namespace Sce
{

	class SUPER_COCO_API Window
	{
	public:
		Window(const std::string& title, int width, int height, std::uint32_t flags = 0);
		Window(const std::string& title, int x, int y, int width, int height, std::uint32_t flags = 0);
		Window(const Window& window) = delete;
		~Window();

		inline SDL_Window* GetHandle() const { return m_window; };
		Vector2i GetSize() const;
		std::string GetTitle() const;

		Window& operator=(const Window& window) = delete;

		bool MakeWindowTransparent(std::uint8_t r, std::uint8_t g, std::uint8_t b);
		void GetDesktopResolution(int& horizontal, int& vertical);


	private:
		SDL_Window* m_window;
	};
}


#endif