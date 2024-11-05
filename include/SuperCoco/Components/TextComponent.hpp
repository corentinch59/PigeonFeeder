#pragma once
#include <SuperCoco/Export.hpp>
#include <SuperCoco/Font.hpp>
#include <SuperCoco/Texture.hpp>
#include <SuperCoco/Renderable.hpp>
#include <SuperCoco/Vector2.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/handle.hpp>
#include <nlohmann/json_fwd.hpp>
#include <memory>
#include <string>

namespace Sce
{
	struct SUPER_COCO_API TextComponent : public IRenderable
	{
		TextComponent();
		TextComponent(std::shared_ptr<Font> font, const std::string& txt = "", const SDL_Color& color = SDL_Color(255, 255, 255));
		TextComponent(const TextComponent&) = delete;
		TextComponent(TextComponent&& textComponent);
		~TextComponent() = default;

		TextComponent& operator=(const TextComponent&) = delete;
		TextComponent& operator=(TextComponent&&) = delete;

		void SetText(const std::string& txt);
		void SetFont(const std::shared_ptr<Font> font);
		void SetOrigine(const Vector2f& origine);
		void SetColor(SDL_Color color);

		void PopulateInspector(WorldEditor& worldEditor) override;

		void Render(Renderer& renderer, const Matrixf& transformMatrix) const override;
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

		bool m_isDirty;

		std::string m_fontEditor;
		float m_colorEditor[4];
		int m_sizeEditor;
		bool m_loadFail;
	};
}

