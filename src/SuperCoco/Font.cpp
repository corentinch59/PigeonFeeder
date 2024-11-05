#include <SuperCoco/Font.hpp>
#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/Texture.hpp>
#include <SuperCoco/Surface.hpp>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>

namespace Sce
{
	Font::Font(Renderer* renderer, TTF_Font* font, int size, const std::string& assetName) :
		m_renderer(renderer),
		m_font(font),
		m_size(size),
		m_assetName(assetName)
	{
		m_assetName;
	}

	Font::~Font()
	{
		TTF_CloseFont(m_font);
	}

	Font::Font(Font&& font) noexcept
	{
		m_renderer = std::move(font.m_renderer);
		m_font = font.m_font;
		m_size = font.m_size;
		m_assetName = font.m_assetName;
		font.m_font = nullptr;
	}

	Font& Font::operator=(Font&& font) noexcept
	{
		m_size = font.m_size;
		m_assetName = font.m_assetName;
		std::swap(m_font, font.m_font);

		return *this;
	}

	int Font::GetSize() const
	{
		return m_size;
	}

	const std::string& Font::GetAssetName() const
	{
		return m_assetName;
	}

	Texture Font::GenerateText(std::string text, SDL_Color color) const
	{
		return Texture::CreateFromSurface(*m_renderer, Surface::CreateTTFSurface(m_font, text, color));
	}

	Font Font::OpenFont(Renderer& renderer, const std::string& path, int size)
	{
		TTF_Font* font = TTF_OpenFont(path.c_str(), size);
	
		if (!font)
			throw std::runtime_error("Failed to open font");

		return Font(&renderer, font, size, path);
	}
}
