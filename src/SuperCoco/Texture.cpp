#include <SuperCoco/Texture.hpp>
#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/Surface.hpp>
#include <stdexcept>
#include <fmt/core.h>

namespace Sce
{
	
	Texture::Texture(SDL_Texture* texture) :
	m_texture(texture)
	{

	}

	Texture::~Texture()
	{
		if (m_texture)
			SDL_DestroyTexture(m_texture);
	}

	Texture::Texture(Texture&& texture) noexcept :
	m_texture(texture.m_texture)
	{
		texture.m_texture = nullptr;
	}

	Texture Texture::CreateFromSurface(const Renderer& renderer, const Surface& surface)
	{
		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer.GetHandle(), surface.GetHandle());
		if (!tex)
			throw std::runtime_error("failed to create texture");

		return Texture(tex);
	}

	Texture& Texture::operator=(Texture&& texture) noexcept
	{
		std::swap(m_texture, texture.m_texture);
		return *this;
	}

	Texture Texture::LoadFromFile(const Renderer& renderer, const std::string& path)
	{
		return CreateFromSurface(renderer, Surface::LoadFromFile(path));
	}

	std::string Texture::GetPath() const
	{
		return m_path;
	}

	SDL_Rect Texture::GetRect() const
	{
		SDL_Rect rect{ 0, 0 };
		SDL_QueryTexture(m_texture, nullptr, nullptr, &rect.w, &rect.h);

		return rect;
	}
}

