#ifndef SUPERCOCO_TEXTURE_H
#define SUPERCOCO_TEXTURE_H

#include <SuperCoco/Asset.hpp>
#include <SDL2/SDL.h>
#include <string>

struct SDL_Texture;

namespace Sce
{
	class Renderer;
	class Surface;

	class SUPER_COCO_API Texture : public Asset
	{
		friend class Renderer;

	public:
		Texture(const Texture&) = delete;
		Texture(Texture&& texture) noexcept;
		~Texture();

		Texture& operator=(Texture&) = delete;
		Texture& operator=(Texture&& texture) noexcept;

		inline SDL_Texture* GetTextureHandle() { return m_texture; };

		SDL_Rect GetRect() const;
		std::string GetPath() const;

		static Texture CreateFromSurface(const Renderer& renderer, const Surface& surface);
		static Texture LoadFromFile(const Renderer& renderer, const std::string& filepath);

		std::string m_path;

	private:
		explicit Texture(SDL_Texture* texture);

		SDL_Texture* GetTextureHandle() const { return m_texture; };

		SDL_Texture* m_texture;
	};
}


#endif