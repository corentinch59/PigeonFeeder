#include <SuperCoco/Sprite.hpp>
#include <SuperCoco/Texture.hpp>
#include <SuperCoco/Renderer.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <SuperCoco/Matrix.hpp>
#include <SuperCoco/JsonSerializer.hpp>
#include <nlohmann/json.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <fstream>

namespace Sce
{
	Sprite::Sprite(std::shared_ptr<Texture> sharedTexture) :
	Sprite(sharedTexture, sharedTexture->GetRect(), 0.5f, 0)
	{
	}

	Sprite::Sprite(std::shared_ptr<Texture> sharedTexture, const SDL_Rect& rect, float origin, int layer) :
	m_texture(std::move(sharedTexture)),
	m_rect(rect),
	m_height(m_rect.h),
	m_width(m_rect.w),
	m_origin(origin),
	m_layer(layer)
	{
		if (m_texture)
			m_texturePath = m_texture->GetFilepath();
	}

	void Sprite::Render(Renderer& renderer, const Matrixf& transformMatrix) const
	{
		Vector2f originShift{ m_width * m_origin.x, m_height * m_origin.y };
		Vector2f screenShift{ 0, 0 };

		Vector2f p1 = (transformMatrix * Matrixf::MakeFromPosition({ -originShift.x, -originShift.y })).GetVector2() + screenShift;
		Vector2f p2 = (transformMatrix * Matrixf::MakeFromPosition({ m_width - originShift.x, -originShift.y })).GetVector2() + screenShift;
		Vector2f p3 = (transformMatrix * Matrixf::MakeFromPosition({ -originShift.x, m_height - originShift.y })).GetVector2() + screenShift;
		Vector2f p4 = (transformMatrix * Matrixf::MakeFromPosition({ m_width - originShift.x, m_height - originShift.y})).GetVector2() + screenShift;

		SDL_Rect textureRect = m_texture->GetRect();
		float invWidth = 1.f / textureRect.w;
		float invHeight = 1.f / textureRect.h;
		
		SDL_Vertex vertices[4];

		vertices[0].position = p1;
		vertices[0].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[0].tex_coord = SDL_FPoint{ static_cast<float>(m_rect.x) * invWidth, static_cast<float>(m_rect.y) * invHeight };

		vertices[1].position = p2;
		vertices[1].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[1].tex_coord = SDL_FPoint{ static_cast<float>(m_rect.x + m_rect.w) * invWidth, static_cast<float>(m_rect.y) * invHeight};

		vertices[2].position = p3;
		vertices[2].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[2].tex_coord = SDL_FPoint{ static_cast<float>(m_rect.x) * invWidth, static_cast<float>(m_rect.y + m_rect.h) * invHeight };

		vertices[3].position = p4;
		vertices[3].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[3].tex_coord = SDL_FPoint{ static_cast<float>(m_rect.x + m_rect.w) * invWidth, static_cast<float>(m_rect.y + m_rect.h) * invHeight };

		int indices[6]{0, 1, 2, 1, 3, 2};

		renderer.RenderGeometry(*m_texture, vertices, 4, indices, 6);
	}

	SDL_FRect Sprite::GetBounds() const
	{
		SDL_FRect bounds = {
			0.0f, 0.0f, float(m_width), float(m_height)
		};

		bounds.x -= m_width * m_origin.x;
		bounds.y -= m_height * m_origin.y;

		return bounds;
	}

	int Sprite::GetLayer() const
	{
		return m_layer;
	}

	void Sprite::PopulateInspector(WorldEditor& worldEditor)
	{
		if (!ImGui::TreeNode("Sprite"))
			return;

		ImGui::InputText("Texture path", &m_texturePath);
		ImGui::SameLine();
		if (ImGui::Button("Update"))
			m_texture = ResourceManager::Instance().GetTexture(m_texturePath);

		float originArray[2] = { m_origin.x, m_origin.y };
		if (ImGui::InputFloat2("Origin", originArray))
			m_origin = Vector2f( originArray[0], originArray[1] );

		int sizeArray[2] = { m_width, m_height };
		if (ImGui::InputInt2("Size", sizeArray))
		{
			m_width = sizeArray[0];
			m_height = sizeArray[1];
		}

		int rectArray[4] = { m_rect.x, m_rect.y, m_rect.w, m_rect.h };
		if (ImGui::InputInt4("Rect", rectArray))
		{
			m_rect.x = rectArray[0];
			m_rect.y = rectArray[1];
			m_rect.w = rectArray[2];
			m_rect.h = rectArray[3];
		}

		ImGui::TreePop();
	}

	nlohmann::json Sprite::Serialize() const
	{
		nlohmann::json renderableDoc = SaveToJSon();
		renderableDoc["Type"] = "Sprite";

		return renderableDoc;
	}

	nlohmann::json Sprite::SaveToJSon() const
	{
		nlohmann::json spriteDoc;
		spriteDoc["Origin"] = m_origin;
		spriteDoc["Rect"] = m_rect;
		spriteDoc["Height"] = m_height;
		spriteDoc["Width"] = m_width;

		if (m_texture)
		{
			const std::string& texturePath = m_texture->GetFilepath();
			if (!texturePath.empty())
				spriteDoc["Texture"] = texturePath;
		}

		return spriteDoc;
	}

	Sprite Sprite::LoadFromFile(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file)
			throw std::runtime_error("failed to open " + filepath);

		nlohmann::json spriteDoc = nlohmann::json::parse(file);
		return LoadFromJson(spriteDoc);
	}

	Sprite Sprite::LoadFromJson(const nlohmann::json& spriteDoc)
	{
		std::shared_ptr<Texture> texture;
		if (std::string texturePath = spriteDoc.value("Texture", ""); !texturePath.empty())
			texture = ResourceManager::Instance().GetTexture(texturePath);

		SDL_Rect rect = spriteDoc["Rect"];
		unsigned int spriteHeight = spriteDoc["Height"];
		unsigned int spriteWidth = spriteDoc["Width"];

		Sprite sprite(std::move(texture), rect);
		sprite.Resize(spriteWidth, spriteHeight);

		return sprite;
	}

	std::shared_ptr<Sprite> Sprite::Unserialize(const nlohmann::json& spriteDoc)
	{
		return std::make_shared<Sprite>(LoadFromJson(spriteDoc));
	}

	void Sprite::Resize(int width, int height)
	{
		m_width = width;
		m_height = height;
	}

	void Sprite::SetRect(const SDL_Rect& rect)
	{
		m_rect = rect;
	}

	void Sprite::SetOrigin(const Vector2f& origin)
	{
		m_origin = origin;
	}

	void Sprite::SetOrigin(float origin)
	{
		m_origin.x = origin;
		m_origin.y = origin;
	}

	void Sprite::SetLayer(int newLayer)
	{
		m_layer = newLayer;
	}

}

