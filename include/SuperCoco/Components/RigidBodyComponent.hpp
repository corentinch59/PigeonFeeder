#pragma once

#include <SuperCoco/Export.hpp>
#include <SuperCoco/ChipmunkBody.hpp>
#include <SuperCoco/ChipmunkShape.hpp>
#include <nlohmann/json_fwd.hpp>
#include <entt/fwd.hpp>
#include <memory>
#include <unordered_map>
#include <chipmunk/chipmunk.h>

namespace Sce
{
	class WorldEditor;
	struct CollisionShape;

	enum class Tag : cpCollisionType
	{
		None,
		Player,
		Enemy,
		Wall,
		Weapon
	};

	class SUPER_COCO_API RigidBodyComponent
	{
		public:
			// Petite astuce, on peut se servir de structures vides comme de "tags", une façon de différencier les constructeurs
			struct Kinematic {};
			struct Static {};

			//RigidBodyComponent(float mass, float moment);
			RigidBodyComponent(float mass, float moment = 0.f, std::function<void()> callback = nullptr);
			RigidBodyComponent(Kinematic);
			RigidBodyComponent(Static);

			void AddShape(std::shared_ptr<CollisionShape> shape, const Vector2f& offset = Vector2f(0.f, 0.f), std::function<cpBool(cpArbiter*, cpSpace*, void*)> collisionBeginFunc = std::function<cpBool(cpArbiter*, cpSpace* , void* )>(), bool recomputeMoment = true);

			float GetAngularVelocity() const;
			Vector2f GetCenterOfGravity() const;
			Vector2f GetLinearVelocity() const;
			Vector2f GetPosition() const;
			float GetRotation() const;
			cpCollisionType GetTag() const;
			cpBool GetSensor() const;
			cpShape* GetShape(size_t index) const;
			cpBody* GetBody() const;

			void RemoveShape(const std::shared_ptr<CollisionShape>& shape, bool recomputeMoment = true);

			void SetAngularVelocity(float angularVelocity);
			void SetCenterOfGravity(const Vector2f& centerOfGravity);
			void SetLinearVelocity(const Vector2f& linearVelocity);
			void SetMass(float mass, bool recomputeMoment = true);
			void SetTag(Tag type);
			void SetSensor(bool value);

			void TeleportTo(const Vector2f& position);
			void TeleportTo(const Vector2f& position, float rotation);

			void PopulateInspector(WorldEditor& worldEditor);

			nlohmann::json Serialize(const entt::handle entity) const;
			static void Unserialize(entt::handle entity, const nlohmann::json& doc);

		private:
			void RecomputeMoment();
			
			struct ShapeData
			{
				ChipmunkShape physicsShape;
				Vector2f offset;
			};

			std::unique_ptr<std::function<void()>> m_callbackData;
			// L'ordre des membres est important : ils sont construits dans l'ordre de déclaration
			// et détruits dans l'ordre inverse
			// ainsi ici, les shapes seront détruits avant que le body ne le soit (l'inverse poserait problème !)
			ChipmunkBody m_body;
			std::unordered_map<std::shared_ptr<CollisionShape>, ShapeData> m_shapes;
	};	
}