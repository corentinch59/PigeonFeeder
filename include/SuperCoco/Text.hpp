#ifndef SUPERCOCO_TEXT_HPP
#define SUPERCOCO_TEXT_HPP

#pragma once

#include <SuperCoco/Export.hpp>
#include <SuperCoco/Renderable.hpp>
#include <SuperCoco/Vector2.hpp>
#include <entt/fwd.hpp>
#include <SDL2/SDL.h>
#include <memory>
#include <string>

namespace Sce
{
	class Texture;
	class Font;

	class SUPER_COCO_API Text : public IRenderable
	{
	public :
		Text();
		Text(std::shared_ptr<Font> font, std::string txt, const SDL_Color& color, Vector2f origin);
		Text(const Text&) = delete;
		Text(Text&& text) noexcept;
		~Text() = default;

		Text& operator=(const Text&) = delete;
		Text& operator=(Text&&) noexcept = delete;

		void SetText(const std::string& txt);
		void SetFont(const std::shared_ptr<Font> font);
		void SetOrigin(const Vector2f& origin);
		void SetColor(SDL_Color color);

		void PopulateInspector(WorldEditor& worldEditor) override;

		virtual void Render(Renderer& renderer, const Matrixf& transformMatrix) const override;
		SDL_FRect GetBounds() const override;
		int GetLayer() const override;

		nlohmann::json Serialize() const override;
		nlohmann::json Serialize(entt::handle entity) const;

		static void Unserialize(entt::handle entity, const nlohmann::json& doc);

	private:
		std::unique_ptr<Texture> m_texture;
		std::shared_ptr<Font> m_font;
		SDL_Color m_color;
		std::string m_text;
		Vector2f m_origin;

		//Editor
		std::string m_fontEditor;
		float m_colorEditor[4];
		int m_sizeEditor;
		bool m_loadFail;
	};
}

#endif