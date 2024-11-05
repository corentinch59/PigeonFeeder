#include <SuperCoco/Components/VelocityComponent.hpp>
#include <SuperCoco/JsonSerializer.hpp>
#include <SuperCoco/WorldEditor.hpp>
#include <SuperCoco/Maths.hpp>
#include <entt/entt.hpp>
#include <imgui.h>
#include <nlohmann/json.hpp>

namespace Sce
{
	void VelocityComponent::PopulateInspector(WorldEditor& worldEditor)
	{
		float velArray[2] = { linearVel.x, linearVel.y };
		if (ImGui::InputFloat2("Linear vel", velArray))
			linearVel = Vector2f( velArray[0], velArray[1] );

		float angular = Deg2Rad * angularVel;
		if (ImGui::SliderAngle("Rotation", &angular))
			angularVel = Rad2Deg * angular;
	}

	nlohmann::json VelocityComponent::Serialize(const entt::handle entity) const
	{
		nlohmann::json doc;
		doc["LinearVel"] = linearVel;
		doc["AngularVel"] = angularVel;

		return doc;
	}

	void VelocityComponent::Unserialize(entt::handle entity, const nlohmann::json& doc)
	{
		auto& vel = entity.emplace<VelocityComponent>();
		vel.angularVel = doc.value("AngularVel", 0.f);
		vel.linearVel = doc.value("LinearVel", Vector2f(0, 0));
	}
}
