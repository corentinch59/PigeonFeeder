#include <SuperCoco/Components/TextComponent.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/Matrix.hpp>
#include <nlohmann/json.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Sce
{
	TextComponent::TextComponent() : TextComponent(nullptr)
	{
	}

	TextComponent::TextComponent(std::shared_ptr<Font> font, const std::string& txt, const SDL_Color& color) : m_font(font), m_text(txt), m_color(color)
		, m_colorEditor(), m_sizeEditor(1), m_loadFail(false)
	{
		if (m_font && !m_text.empty())
			m_texture = std::make_unique<Texture>(m_font->GenerateText(m_text, m_color));
	}

	TextComponent::TextComponent(TextComponent&& textComponent)
	{
		m_texture = std::move(textComponent.m_texture);
		m_font = std::move(textComponent.m_font);
		m_color = textComponent.m_color;
		m_text = textComponent.m_text;
		m_origin = textComponent.m_origin;
	}

	void TextComponent::Render(Renderer& renderer, const Matrixf& transformMatrix) const
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

	SDL_FRect TextComponent::GetBounds() const
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

	int TextComponent::GetLayer() const
	{
		return 10;
	}

	nlohmann::json TextComponent::Serialize() const
	{
		/*document.SetObject();
		rapidjson::Document::AllocatorType allocator = document.GetAllocator();

		const std::string& asset = m_font->GetAssetName();
		document.AddMember("Font", rapidjson::Value(asset.c_str(), asset.size()), allocator);
		document.AddMember("Size", m_font->GetSize(), allocator);
		document.AddMember("Text", rapidjson::Value(m_text.c_str(), m_text.size()), allocator);

		rapidjson::Value dataArray(rapidjson::kArrayType);
		dataArray.PushBack(m_origin.x, allocator);
		dataArray.PushBack(m_origin.y, allocator);
		document.AddMember("Origin", dataArray, allocator);

		dataArray.SetArray();
		dataArray.PushBack(m_color.r, allocator);
		dataArray.PushBack(m_color.g, allocator);
		dataArray.PushBack(m_color.b, allocator);
		dataArray.PushBack(m_color.a, allocator);
		document.AddMember("Color", dataArray, allocator);*/
		return nlohmann::json();
	}

	nlohmann::json TextComponent::Serialize(entt::handle entity) const
	{
		return nlohmann::json();
	}

	void TextComponent::Unserialize(entt::handle entity, const nlohmann::json& doc)
	{
		/*std::shared_ptr<Font> fontPtr = ResourceManager::GetInstance().GetFont(document["Font"].GetString(), document["Size"].GetInt());
		TextComponent& txtcp = entity.emplace<TextComponent>(fontPtr, document["Text"].GetString());
		txtcp.SetOrigine({ document["Origin"][0].GetFloat(), document["Origin"][1].GetFloat() });

		SDL_Color color(document["Color"][0].GetUint(), document["Color"][1].GetUint(), document["Color"][2].GetUint(), document["Color"][3].GetUint());
		txtcp.SetColor(color);*/
	}

	void TextComponent::SetText(const std::string& txt)
	{
		m_text = txt;
		m_texture = std::make_unique<Texture>(m_font->GenerateText(m_text, m_color));
	}

	void TextComponent::SetFont(const std::shared_ptr<Font> font)
	{
		m_font = font;
		m_texture = std::make_unique<Texture>(m_font->GenerateText(m_text, m_color));
	}

	void TextComponent::SetOrigine(const Vector2f& origine)
	{
		m_origin = origine;
	}

	void TextComponent::SetColor(SDL_Color color)
	{
		m_color = color;
		m_texture = std::make_unique<Texture>(m_font->GenerateText(m_text, m_color));
	}

	void TextComponent::PopulateInspector(WorldEditor& worldEditor)
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
				m_isDirty = true;
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
			m_isDirty = true;
		}

		if (ImGui::InputTextMultiline("Text", &m_text, ImVec2(0, 100)))
			m_isDirty = true;

		float originArray[2] = { m_origin.x, m_origin.y };
		if (ImGui::InputFloat2("Origin", originArray))
			m_origin = Vector2f{ originArray[0], originArray[1] };
	}

}

