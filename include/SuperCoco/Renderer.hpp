#ifndef SUPERCOCO_RENDERER_H
#define SUPERCOCO_RENDERER_H

#include <SuperCoco/Export.hpp>
#include <cstdint>
#include <cstddef>

struct SDL_Renderer;
struct SDL_Rect;
struct SDL_Vertex;
struct SDL_FPoint;

namespace Sce
{
	class Window;
	class Texture;

	class SUPER_COCO_API Renderer
	{
		friend class Texture;

	public:
		Renderer(Window& window, int renderer = -1, std::uint32_t flags = 0);
		Renderer(const Renderer& renderer) = delete;
		~Renderer();

		Renderer& operator=(const Renderer renderer) = delete;

		inline SDL_Renderer* GetHandle() { return m_renderer; };

		void RenderClear();
		void RenderPresent();
		void RenderDrawColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

		void RenderCopy(const Texture& texture);
		void RenderCopy(const Texture& texture, const SDL_Rect& dstrect);
		void RenderCopy(const Texture& texture, const SDL_Rect& srcrect, const SDL_Rect& dstrect);

		void RenderGeometry(const SDL_Vertex* vertices, int numVertices);
		void RenderGeometry(const Texture& texture, const SDL_Vertex* vertices, int numVertices);
		void RenderGeometry(const SDL_Vertex* vertices, int numVertices, const int* indices, int numIndices);
		void RenderGeometry(const Texture& texture, const SDL_Vertex* vertices, int numVertices, const int* indices, int numIndices);

		void SetDrawColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
		void RenderLines(const SDL_FPoint* points, std::size_t count);

	private:
		SDL_Renderer* GetHandle() const { return m_renderer; };

		SDL_Renderer* m_renderer;
	};
}


#endif