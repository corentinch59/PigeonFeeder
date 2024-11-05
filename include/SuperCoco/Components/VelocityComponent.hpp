#ifndef SUPERCOCO_VELOCITYCOMPONENT_HPP
#define SUPERCOCO_VELOCITYCOMPONENT_HPP

#include <SuperCoco/Export.hpp>
#include <SuperCoco/Vector2.hpp>
#include <nlohmann/json_fwd.hpp>
#include <entt/fwd.hpp>

namespace Sce
{
	class WorldEditor;

	struct SUPER_COCO_API VelocityComponent
	{

	public :
		Vector2f linearVel = Vector2f(0.f, 0.f);
		float angularVel = 0.f;

		void PopulateInspector(WorldEditor& worldEditor);
		nlohmann::json Serialize(const entt::handle entity) const;
		static void Unserialize(entt::handle entity, const nlohmann::json& doc);
	};


}

#endif