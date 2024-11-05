#ifndef SUPERCOCO_SURFACE_HPP
#define SUPERCOCO_SURFACE_HPP

#include <SuperCoco/Export.hpp>
#include <SDL2/SDL_ttf.h>
#include <string>

struct SDL_Surface;
struct SDL_Rect;

namespace Sce
{
	class SUPER_COCO_API Surface
	{
		friend class Texture;

	public:
		Surface(const Surface&) = delete;
		Surface(Surface&& surface) noexcept;
		~Surface();


		void FillRect(const SDL_Rect& rect, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

		Surface& operator=(const Surface&) = delete;
		Surface& operator=(Surface&& surface) noexcept;

		SDL_Surface* GetHandle() { return m_surface; };

		std::uint8_t* GetPixels();
		const std::uint8_t* GetPixels() const;

		static Surface Create(int width, int height);
		static Surface LoadFromFile(const std::string& path);
		static Surface CreateTTFSurface(TTF_Font* font, const std::string& text, const SDL_Color& color);

	private:
		explicit Surface(SDL_Surface* surface);

		inline SDL_Surface* GetHandle() const { return m_surface; };

		SDL_Surface* m_surface;
		
	};
}


#endif