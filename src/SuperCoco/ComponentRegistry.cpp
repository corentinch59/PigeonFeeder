#include <SuperCoco/ComponentRegistry.hpp>
#include <SuperCoco/Components/CameraComponent.hpp>
#include <SuperCoco/Components/GraphicsComponent.hpp>
#include <SuperCoco/NameComponent.hpp>
#include <SuperCoco/Components/SpritesheetComponent.hpp>
#include <SuperCoco/Transform.hpp>
#include <SuperCoco/Components/VelocityComponent.hpp>
#include <SuperCoco/Components/RigidBodyComponent.hpp>
#include <SuperCoco/Components/TextComponent.hpp>

namespace Sce
{
	ComponentRegistry::ComponentRegistry()
	{
		RegisterEngineComponents();
	}

	void ComponentRegistry::ForEachComponent(const std::function<void(const Entry&)>& callback) const
	{
		for (const auto& entry : m_componentTypes)
			callback(entry);
	}

	void ComponentRegistry::Register(Entry&& data)
	{
		m_componentTypes.push_back(std::move(data));
	}

	void ComponentRegistry::RegisterEngineComponents()
	{
		Register({
			.id = "name",
			.label = "Name",
			.addComponent = [](entt::handle entity)
			{
				entity.emplace<NameComponent>("Unnamed");
			},
			.hasComponent = BuildHasComponent<NameComponent>(),
			.removeComponent = BuildRemoveComponent<NameComponent>(),
			.inspect = BuildInspect<NameComponent>(),
			.serialize = BuildSerialize<NameComponent>(),
			.unserialize = BuildUnserialize<NameComponent>()
		});
		
		Register({
			.id = "camera",
			.label = "Camera",
			.addComponent = BuildAddComponent<CameraComponent>(),
			.hasComponent = BuildHasComponent<CameraComponent>(),
			.removeComponent = BuildRemoveComponent<CameraComponent>(),
			.serialize = BuildSerialize<CameraComponent>(),
			.unserialize = BuildUnserialize<CameraComponent>()
		});
		
		Register({
			.id = "transform",
			.label = "Transform",
			.addComponent = BuildAddComponent<Transform>(),
			.hasComponent = BuildHasComponent<Transform>(),
			.removeComponent = BuildRemoveComponent<Transform>(),
			.inspect = BuildInspect<Transform>(),
			.serialize = BuildSerialize<Transform>(),
			.unserialize = BuildUnserialize<Transform>()
		});

		Register({
			.id = "velocity",
			.label = "VelocityComponent",
			.addComponent = BuildAddComponent<VelocityComponent>(),
			.hasComponent = BuildHasComponent<VelocityComponent>(),
			.removeComponent = BuildRemoveComponent<VelocityComponent>(),
			.inspect = BuildInspect<VelocityComponent>(),
			.serialize = BuildSerialize<VelocityComponent>(),
			.unserialize = BuildUnserialize<VelocityComponent>()
		});

		Register({
			.id = "graphics",
			.label = "GraphicsComponent",
			.addComponent = BuildAddComponent<GraphicsComponent>(),
			.hasComponent = BuildHasComponent<GraphicsComponent>(),
			.removeComponent = BuildRemoveComponent<GraphicsComponent>(),
			.inspect = BuildInspect<GraphicsComponent>(),
			.serialize = BuildSerialize<GraphicsComponent>(),
			.unserialize = BuildUnserialize<GraphicsComponent>()
		});

		Register({
			.id = "spritesheet",
			.label = "SpritesheetComponent",
			.hasComponent = BuildHasComponent<SpritesheetComponent>(),
			.removeComponent = BuildRemoveComponent<SpritesheetComponent>(),
			.inspect = BuildInspect<SpritesheetComponent>(),
			.serialize = BuildSerialize<SpritesheetComponent>(),
			.unserialize = BuildUnserialize<SpritesheetComponent>()
		});

		Register({
			.id = "rigidbody",
			.label = "RigidBodyComponent",
			.addComponent = [](entt::handle entity)
			{
				entity.emplace<RigidBodyComponent>(0.01f, 10.f);
			},
			.hasComponent = BuildHasComponent<RigidBodyComponent>(),
			.removeComponent = BuildRemoveComponent<RigidBodyComponent>(),
			.inspect = BuildInspect<RigidBodyComponent>(),
			.serialize = BuildSerialize<RigidBodyComponent>(),
			.unserialize = BuildUnserialize<RigidBodyComponent>()
		});

		Register({
		.id = "text",
		.label = "TextComponent",
		.addComponent = [](entt::handle entity) {
			entity.emplace<TextComponent>();
		},
		.hasComponent = BuildHasComponent<TextComponent>(),
		.removeComponent = BuildRemoveComponent<TextComponent>(),
		.inspect = BuildInspect<TextComponent>(),
		.serialize = BuildSerialize<TextComponent>(),
		.unserialize = BuildUnserialize<TextComponent>()
		});
	}
}