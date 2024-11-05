#pragma once
#include <SuperCoco/Export.hpp>
#include <string>
#include <memory>
#include <SDL2/SDL_ttf.h>

namespace Sce
{
	class Texture;
	class Renderer;

	class SUPER_COCO_API Font
	{
	public:
		Font() = delete;
		Font(const Font&) = delete;
		Font(Font&& font) noexcept;
		~Font();

		Font& operator=(const Font&) = delete;
		Font& operator=(Font&& font) noexcept;

		int GetSize() const;
		const std::string& GetAssetName() const;

		Texture GenerateText(std::string text, SDL_Color color) const;

		static Font OpenFont(Renderer& renderer, const std::string& path, int size);

	private:
		explicit Font(Renderer* renderer, TTF_Font* font, int size, const std::string& assetName);

		TTF_Font* m_font;
		int m_size;

		Renderer* m_renderer;
		std::string m_assetName;
	};
}

