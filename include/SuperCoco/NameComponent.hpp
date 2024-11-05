#pragma once

#include <SuperCoco/Export.hpp>
#include <entt/fwd.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>

namespace Sce
{
	class WorldEditor;

	// Petit composant pour nommer une entité (pour l'inspecteur)
	struct SUPER_COCO_API NameComponent
	{
		explicit NameComponent(std::string Name);

		void PopulateInspector(WorldEditor& worldEditor);
		nlohmann::json Serialize(const entt::handle entity) const;
		static void Unserialize(entt::handle entity, const nlohmann::json& doc);

		std::string name;
	};
}