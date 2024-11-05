#include <SuperCoco/Components/GraphicsComponent.hpp>
#include <SuperCoco/Model.hpp>
#include <SuperCoco/Sprite.hpp>
#include <SuperCoco/Renderable.hpp>
#include <SuperCoco/ResourceManager.hpp>
#include <entt/entt.hpp>
#include <fmt/color.h>
#include <fmt/format.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <nlohmann/json.hpp>

namespace Sce
{
	void GraphicsComponent::PopulateInspector(WorldEditor& worldEditor)
	{
		if (m_renderable)
		{
			if (ImGui::Button("Clear"))
				m_renderable = nullptr;
			else
				m_renderable->PopulateInspector(worldEditor);
		}
		else
		{
			ImGui::InputText("Path", &filepath);

			if (ImGui::Button("Create model"))
				m_renderable = ResourceManager::Instance().GetModel(filepath);
			else if (ImGui::Button("Create sprite"))
				m_renderable = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture(filepath));
		}
	}

	nlohmann::json GraphicsComponent::Serialize(const entt::handle entity) const
	{
		nlohmann::json doc;
		if (m_renderable)
			doc["Renderable"] = m_renderable->Serialize();

		return doc;
	}

	void GraphicsComponent::Unserialize(entt::handle entity, const nlohmann::json& doc)
	{
		auto& gfxComponent = entity.emplace<GraphicsComponent>();

		if (auto it = doc.find("Renderable"); it != doc.end())
		{
			const nlohmann::json& renderableDoc = it.value();

			// On identifie le type de renderable via la valeur "Type" que Model et Sprite écrivent
			// Ce n'est pas très scalable, on devrait faire une factory (une map entre un nom et une fonction instanciant le renderable)
			std::string renderableType = renderableDoc["Type"];
			if (renderableType == "Model")
				gfxComponent.m_renderable = Model::Unserialize(renderableDoc);
			else if (renderableType == "Sprite")
				gfxComponent.m_renderable = Sprite::Unserialize(renderableDoc);
			else
				fmt::print(fg(fmt::color::red), "unknown renderable \"{}\"\n", renderableType);
		}
	}
}