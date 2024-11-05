#include <SuperCoco/Window.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <Windows.h>
#include <stdexcept>

namespace Sce
{
	Window::Window(const std::string& title, int width, int height, std::uint32_t flags) :
		Window(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags)
	{

	}

	Window::Window(const std::string& title, int x, int y, int width, int height, std::uint32_t flags)
	{
		m_window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
		if (!m_window)
			throw std::runtime_error("failed to create window");
	}

	Window::~Window()
	{
		SDL_DestroyWindow(m_window);
	}

	bool Window::MakeWindowTransparent(std::uint8_t r, std::uint8_t g, std::uint8_t b)
	{
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(m_window, &wmInfo);
		HWND hWnd = wmInfo.info.win.window;


		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		return SetLayeredWindowAttributes(hWnd, RGB(r, g, b), 0, LWA_COLORKEY);
	}

	Vector2i Window::GetSize() const
	{
		Vector2i size;
		SDL_GetWindowSize(m_window, &size.x, &size.y);

		return size;
	}

	std::string Window::GetTitle() const
	{
		return SDL_GetWindowTitle(m_window);
	}

	void Window::GetDesktopResolution(int& horizontal, int& vertical)
	{
		RECT desktop;
		// Get a handle to the desktop window
		const HWND hDesktop = GetDesktopWindow();
		// Get the size of screen to the variable desktop
		GetWindowRect(hDesktop, &desktop);
		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		horizontal = desktop.right;
		vertical = desktop.bottom;
	}
	
}

