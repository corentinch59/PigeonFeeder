#include <SuperCoco/Surface.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <cassert>

namespace Sce
{
	Surface::Surface(SDL_Surface* surface) :
		m_surface(surface)
	{

	}

	Surface::Surface(Surface&& surface) noexcept
	{
		m_surface = surface.m_surface;
		surface.m_surface = nullptr;
	}

	Surface::~Surface()
	{
		if (m_surface)
			SDL_FreeSurface(m_surface);
	}

	void Surface::FillRect(const SDL_Rect& rect, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	{
		assert(m_surface);
		SDL_FillRect(m_surface, &rect, SDL_MapRGBA(m_surface->format, r, g, b, a));
	}

	Surface& Surface::operator=(Surface&& surface) noexcept
	{
		std::swap(m_surface, surface.m_surface);

		return *this;
	}

	std::uint8_t* Surface::GetPixels()
	{
		assert(m_surface);
		return static_cast<std::uint8_t*>(m_surface->pixels);
	}

	const std::uint8_t* Surface::GetPixels() const
	{
		assert(m_surface);
		return static_cast<const std::uint8_t*>(m_surface->pixels);
	}

	Surface Surface::Create(int width, int height)
	{
		SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
		if (!surface)
			throw std::runtime_error(SDL_GetError());

		return Surface(surface);
	}

	Surface Surface::LoadFromFile(const std::string& path)
	{
		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface)
			throw std::runtime_error(SDL_GetError());

		return Surface(surface);
	}

	Surface Surface::CreateTTFSurface(TTF_Font* font, const std::string& text, const SDL_Color& color)
	{
		SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
		if (!surface)
			throw std::runtime_error(SDL_GetError());

		return Surface(surface);
	}
}

