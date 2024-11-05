#ifndef SUPERCOCO_GRAPHICSCOMPONENT_HPP
#define SUPERCOCO_GRAPHICSCOMPONENT_HPP

#pragma once

#include <SuperCoco/Export.hpp>
#include <nlohmann/json_fwd.hpp>
#include <entt/fwd.hpp>
#include <memory>
#include <string>

namespace Sce
{
	class IRenderable;
	class WorldEditor;

	struct SUPER_COCO_API GraphicsComponent
	{
		std::shared_ptr<IRenderable> m_renderable;
		std::string filepath;

		void PopulateInspector(WorldEditor& worldEditor);
		nlohmann::json Serialize(const entt::handle entity) const;

		static void Unserialize(entt::handle entity, const nlohmann::json& doc);
	};
}


#endif