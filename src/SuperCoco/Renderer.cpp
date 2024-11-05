#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/Window.hpp>
#include <SuperCoco/Texture.hpp>
#include <SDL2/SDL.h>
#include <stdexcept>

namespace Sce
{
	Renderer::Renderer(Window& window, int renderer, std::uint32_t flags)
	{
		m_renderer = SDL_CreateRenderer(window.GetHandle(), renderer, flags);
		if (!m_renderer)
			throw std::runtime_error("failed to create renderer");
	}

	Renderer::~Renderer()
	{
		SDL_DestroyRenderer(m_renderer);
	}

	void Renderer::RenderClear()
	{
		SDL_RenderClear(m_renderer);
	}

	void Renderer::RenderPresent()
	{
		SDL_RenderPresent(m_renderer);
	}

	void Renderer::RenderDrawColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	{
		SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	}

	void Renderer::RenderCopy(const Texture& texture)
	{
		SDL_RenderCopy(m_renderer, texture.GetTextureHandle(), nullptr, nullptr);
	}

	void Renderer::RenderCopy(const Texture& texture, const SDL_Rect& dstrect)
	{
		SDL_RenderCopy(m_renderer, texture.GetTextureHandle(), nullptr, &dstrect);
	}

	void Renderer::RenderCopy(const Texture& texture, const SDL_Rect& srcrect, const SDL_Rect& dstrect)
	{
		SDL_RenderCopy(m_renderer, texture.GetTextureHandle(), &srcrect, &dstrect);
	}

	void Renderer::RenderGeometry(const SDL_Vertex* vertices, int numVertices)
	{
		SDL_RenderGeometry(m_renderer, nullptr, vertices, numVertices, nullptr, 0);
	}

	void Renderer::RenderGeometry(const Texture& texture, const SDL_Vertex* vertices, int numVertices)
	{
		SDL_RenderGeometry(m_renderer, texture.GetTextureHandle(), vertices, numVertices, nullptr, 0);
	}

	void Renderer::RenderGeometry(const SDL_Vertex* vertices, int numVertices, const int* indices, int numIndices)
	{
		SDL_RenderGeometry(m_renderer, nullptr, vertices, numVertices, indices, numIndices);
	}

	void Renderer::RenderGeometry(const Texture& texture, const SDL_Vertex* vertices, int numVertices, const int* indices, int numIndices)
	{
		SDL_RenderGeometry(m_renderer, texture.GetTextureHandle(), vertices, numVertices, indices, numIndices);
	}

	void Renderer::SetDrawColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	{
		SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	}

	void Renderer::RenderLines(const SDL_FPoint* points, std::size_t count)
	{
		SDL_RenderDrawLinesF(m_renderer, points, static_cast<int>(count));
	}
}
