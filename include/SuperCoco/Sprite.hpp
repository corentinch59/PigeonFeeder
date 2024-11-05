#ifndef SUPERCOCO_SPRITE_HPP
#define SUPERCOCO_SPRITE_HPP

#include <SuperCoco/Export.hpp>
#include <SuperCoco/Vector2.hpp>
#include <SuperCoco/Renderable.hpp>
#include <nlohmann/json_fwd.hpp>
#include <SDL2/SDL.h>
#include <memory>

namespace Sce
{
	class Texture;
	class Renderer;
	template<typename T> class Matrix;
	using Matrixf = Matrix<float>;

	class SUPER_COCO_API Sprite : public IRenderable
	{
	public:
		Sprite(std::shared_ptr<Texture> sharedTexture);
		Sprite(std::shared_ptr<Texture> sharedTexture, const SDL_Rect& rect, float origin = 0.5f, int layer = 0);

		virtual void Render(Renderer& renderer, const Matrixf& transformMatrix) const override;

		SDL_FRect GetBounds() const override;
		int GetLayer() const override;

		void PopulateInspector(WorldEditor& worldEditor) override;

		void Resize(int width, int height);
		void SetRect(const SDL_Rect& rect);
		void SetOrigin(const Vector2f& origin);
		void SetOrigin(float origin);
		void SetLayer(int newLayer);

		nlohmann::json Serialize() const override;
		nlohmann::json SaveToJSon() const;

		static Sprite LoadFromFile(const std::string& filepath);
		static Sprite LoadFromJson(const nlohmann::json& spriteDoc);
		static std::shared_ptr<Sprite> Unserialize(const nlohmann::json& spriteDoc);

	private:
		int m_width;
		int m_height;
		int m_layer;

		Vector2f m_origin;

		SDL_Rect m_rect;

		std::shared_ptr<Texture> m_texture;
		std::string m_texturePath;
	};
}


#endif