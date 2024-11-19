#include <SuperCoco/Text.hpp>
#include <SuperCoco/Font.hpp>
#include <SuperCoco/Texture.hpp>
#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/Matrix.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <nlohmann/json.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <entt/entt.hpp>

namespace Sce
{
	Text::Text() :
	Text(nullptr, "None", {0,0,0,255}, {0.f,0.f})
	{
	}

	Text::Text(std::shared_ptr<Font> font, std::string txt, const SDL_Color& color, Vector2f origin) :
	m_font(font),
	m_text(std::move(txt)),
	m_color(color),
	m_origin(origin)
	{
		if (m_font && !m_text.empty())
			m_texture = std::make_unique<Texture>(m_font->GenerateText(m_text, m_color));
	}

	Text::Text(Text&& text) noexcept
	{
		m_texture = std::move(text.m_texture);
		m_font = std::move(text.m_font);
		m_color = text.m_color;
		m_text = text.m_text;
		m_origin = text.m_origin;
	}

	void Text::Render(Renderer& renderer, const Matrixf& transformMatrix) const
	{
		SDL_Rect texRect{ 0, 0, 1, 1 };
		if (m_texture)
			texRect = m_texture->GetRect();

		Vector2f originShift{ texRect.w * m_origin.x, texRect.h * m_origin.y };

		Vector2f topLeft = transformMatrix * Vector2f(-originShift.x, -originShift.y);
		Vector2f topRight = transformMatrix * Vector2f(texRect.w - originShift.x, -originShift.y);
		Vector2f bottomLeft = transformMatrix * Vector2f(-originShift.x, texRect.h - originShift.y);
		Vector2f bottomRight = transformMatrix * Vector2f(texRect.w - originShift.x, texRect.h - originShift.y);

		float invWidth = 1.f / texRect.w;
		float invHeight = 1.f / texRect.h;

		SDL_Vertex vertices[4];
		vertices[0].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[0].position = SDL_FPoint{ topLeft.x, topLeft.y };
		vertices[0].tex_coord = SDL_FPoint{ 0.f, 0.f };

		vertices[1].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[1].position = SDL_FPoint{ topRight.x, topRight.y };
		vertices[1].tex_coord = SDL_FPoint{ texRect.w * invWidth, 0.f };

		vertices[2].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[2].position = SDL_FPoint{ bottomLeft.x, bottomLeft.y };
		vertices[2].tex_coord = SDL_FPoint{ 0.f, texRect.h * invHeight };

		vertices[3].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[3].position = SDL_FPoint{ bottomRight.x, bottomRight.y };
		vertices[3].tex_coord = SDL_FPoint{ texRect.w * invWidth, texRect.h * invHeight };

		int indices[6] = { 0, 1, 2, 2, 1, 3 };

		if (m_texture)
			renderer.RenderGeometry(*m_texture.get(), vertices, 4, indices, 6);
		else
			renderer.RenderGeometry(vertices, 4, indices, 6);
	}

	SDL_FRect Text::GetBounds() const
	{
		if (!m_texture)
			return SDL_FRect(0, 0, 20, 20);

		SDL_Rect rect = m_texture->GetRect();
		SDL_FRect bounds = {
			0.f, 0.f, float(rect.w), float(rect.h)
		};

		bounds.x -= rect.w * m_origin.x;
		bounds.y -= rect.h * m_origin.y;
		return bounds;
	}

	int Text::GetLayer() const
	{
		return 10;
	}

	nlohmann::json Text::Serialize() const
	{
		return nlohmann::json();
	}

	nlohmann::json Text::Serialize(entt::handle entity) const
	{
		return nlohmann::json();
	}

	void Text::SetText(const std::string& txt)
	{
		m_text = txt;
		m_texture = std::make_unique<Texture>(m_font->GenerateText(m_text, m_color));
	}

	void Text::SetFont(const std::shared_ptr<Font> font)
	{
		m_font = font;
		m_texture = std::make_unique<Texture>(m_font->GenerateText(m_text, m_color));
	}

	void Text::SetOrigin(const Vector2f& origin)
	{
		m_origin = origin;
	}

	void Text::SetColor(SDL_Color color)
	{
		m_color = color;
		m_texture = std::make_unique<Texture>(m_font->GenerateText(m_text, m_color));
	}

	void Text::PopulateInspector(WorldEditor& worldEditor)
	{
		ImGui::InputText("Font", &m_fontEditor);
		if (ImGui::InputInt("Size", &m_sizeEditor) && m_sizeEditor < 1)
			m_sizeEditor = 1;

		ImGui::Spacing();
		ImGui::SameLine(0.f, 1.f + (ImGui::GetWindowWidth() * 0.65f));
		if (ImGui::Button("Load font"))
		{
			try
			{
				m_font = ResourceManager::Instance().GetFont(m_fontEditor, m_sizeEditor);
				m_loadFail = false;
			}
			catch (const std::exception&)
			{
				m_loadFail = true;
			}
		}

		if (m_loadFail)
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "No font found");

		if (ImGui::ColorEdit4("Color", m_colorEditor))
		{
			m_color = { Uint8(255 * m_colorEditor[0]), Uint8(255 * m_colorEditor[1]), Uint8(255 * m_colorEditor[2]), Uint8(255 * m_colorEditor[3]) };
		}

		float originArray[2] = { m_origin.x, m_origin.y };
		if (ImGui::InputFloat2("Origin", originArray))
			m_origin = Vector2f{ originArray[0], originArray[1] };
	}

}
