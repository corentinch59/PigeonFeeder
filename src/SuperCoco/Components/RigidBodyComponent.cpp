#include <SuperCoco/Components/RigidBodyComponent.hpp>
#include <SuperCoco/CollisionShape.hpp>
#include <SuperCoco/WorldEditor.hpp>
#include <SuperCoco/Systems/PhysicsSystem.hpp>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <imgui.h>
#include <fmt/core.h>

namespace Sce
{
	//RigidBodyComponent::RigidBodyComponent(float mass, float moment) :
	//m_body(ChipmunkBody::Build(PhysicsSystem::Instance()->GetSpace(), mass, moment)),
	//m_callbackData(nullptr)
	//{
	//}

	RigidBodyComponent::RigidBodyComponent(float mass, float moment, std::function<void()> callback) :
	m_body(ChipmunkBody::Build(PhysicsSystem::Instance()->GetSpace(), mass, moment)),
	m_callbackData(std::make_unique<std::function<void()>>(std::move(callback)))
	{
		cpBodySetUserData(m_body.GetHandle(), m_callbackData.get());
	}

	RigidBodyComponent::RigidBodyComponent(Kinematic) :
	m_body(ChipmunkBody::BuildKinematic(PhysicsSystem::Instance()->GetSpace()))
	{
	}

	RigidBodyComponent::RigidBodyComponent(Static) :
	m_body(ChipmunkBody::BuildStatic(PhysicsSystem::Instance()->GetSpace()))
	{
	}

	void RigidBodyComponent::AddShape(std::shared_ptr<CollisionShape> shape, const Vector2f& offset, std::function<cpBool(cpArbiter* arb, cpSpace* space, void* data)> collisionBeginFunc, bool recomputeMoment)
	{
		ShapeData shapeData{
			shape->Build(m_body, offset),
			offset
		};

		auto [it, inserted] = m_shapes.emplace(std::move(shape), std::move(shapeData));

		if (collisionBeginFunc)
		{
			cpCollisionHandler* handler = cpSpaceAddCollisionHandler(PhysicsSystem::Instance()->GetSpace().GetHandle(), static_cast<cpCollisionType>(Tag::Weapon), static_cast<cpCollisionType>(Tag::Enemy));
			handler->beginFunc = [](cpArbiter* arb, cpSpace* space, void* data) -> cpBool
				{
					auto* func = static_cast<std::function<cpBool(cpArbiter*, cpSpace*, void*)>*>(data);
					cpBody* firstBody;
					cpBody* secondBody;
					cpArbiterGetBodies(arb, &firstBody, &secondBody);

					std::function<void()>* temp1 = static_cast<std::function<void()>*>(cpBodyGetUserData((firstBody)));
					std::function<void()>& callback1 = *temp1;
					if(callback1)
						callback1();

					std::function<void()>* temp2 = static_cast<std::function<void()>*>(cpBodyGetUserData((secondBody)));
					std::function<void()>& callback2 = *temp2;
					if (callback2)
						callback2();

					return (*func)(arb, space, nullptr);
				};
			handler->userData = new std::function<cpBool(cpArbiter*, cpSpace*, void*)>(collisionBeginFunc);
		}
		
		// Si la shape a bien été ajoutée
		if (inserted && recomputeMoment)
			RecomputeMoment();
	}

	float RigidBodyComponent::GetAngularVelocity() const
	{
		return m_body.GetAngularVelocity();
	}

	Vector2f RigidBodyComponent::GetCenterOfGravity() const
	{
		return m_body.GetCenterOfGravity();
	}

	Vector2f RigidBodyComponent::GetLinearVelocity() const
	{
		return m_body.GetLinearVelocity();
	}

	Vector2f RigidBodyComponent::GetPosition() const
	{
		return m_body.GetPosition();
	}

	float RigidBodyComponent::GetRotation() const
	{
		return m_body.GetRotation();
	}

	cpCollisionType RigidBodyComponent::GetTag() const
	{
		if (m_shapes.size() != 0)
		{
			return m_shapes.begin()->second.physicsShape.GetType(m_shapes.begin()->second.physicsShape);
		}
	}

	cpBool RigidBodyComponent::GetSensor() const
	{
		if (m_shapes.size() != 0)
		{
			return m_shapes.begin()->second.physicsShape.GetSensor(m_shapes.begin()->second.physicsShape);
		}
	}

	cpShape* RigidBodyComponent::GetShape(size_t index) const
	{
		if (m_shapes.size() <= 0 || index >= m_shapes.size())
			return nullptr;

		auto shape = m_shapes.begin();

		std::advance(shape, index);

		return shape->second.physicsShape.GetHandle();
	}

	cpBody* RigidBodyComponent::GetBody() const
	{
		return m_body.GetHandle();
	}

	void RigidBodyComponent::RemoveShape(const std::shared_ptr<CollisionShape>& shape, bool recomputeMoment)
	{
		std::size_t count = m_shapes.erase(shape);
		if (count > 0 && recomputeMoment) // si on a bien enlevé un shape
			RecomputeMoment();
	}

	void RigidBodyComponent::SetAngularVelocity(float angularVelocity)
	{
		m_body.SetAngularVelocity(angularVelocity);
	}

	void RigidBodyComponent::SetCenterOfGravity(const Vector2f& centerOfGravity)
	{
		m_body.SetCenterOfGravity(centerOfGravity);
	}

	void RigidBodyComponent::SetLinearVelocity(const Vector2f& linearVelocity)
	{
		m_body.SetLinearVelocity(linearVelocity);
	}

	void RigidBodyComponent::SetMass(float mass, bool recomputeMoment)
	{
		m_body.SetMass(mass);
		if (recomputeMoment)
			RecomputeMoment();
	}

	void RigidBodyComponent::SetTag(Tag type)
	{
		if (m_shapes.size() != 0)
		{
			m_shapes.begin()->second.physicsShape.SetType(m_shapes.begin()->second.physicsShape, static_cast<cpCollisionType>(type));
		}
	}

	void RigidBodyComponent::SetSensor(bool value)
	{
		if (m_shapes.size() != 0)
		{
			m_shapes.begin()->second.physicsShape.SetSensor(m_shapes.begin()->second.physicsShape, value);
		}
	}

	void RigidBodyComponent::TeleportTo(const Vector2f& position)
	{
		m_body.SetPosition(position);

		// Les objets statiques doivent être réindexés manuellement en cas de déplacement
		if (m_body.IsStatic())
			m_body.ReindexShapes();
	}

	void RigidBodyComponent::TeleportTo(const Vector2f& position, float rotation)
	{
		m_body.SetPosition(position);
		m_body.SetRotation(rotation);
	}

	void RigidBodyComponent::PopulateInspector(WorldEditor& worldEditor)
	{
		float mass = m_body.GetMass();
		if (ImGui::InputFloat("Mass", &mass))
		{
			if (mass == 0)
			{
				mass = 0.001f;
			}
			SetMass(mass, false);
		}

		Vector2f pos = m_body.GetPosition();
		float posArray[2]{ pos.x, pos.y };
		if (ImGui::InputFloat2("Position :", posArray))
			TeleportTo({posArray[0], posArray[1]});

		if (m_shapes.size() != 0)
		{
			std::string idTag = "";
			switch (GetTag())
			{
				case static_cast<cpCollisionType>(Tag::None):
					idTag = "None";
					break;
				case static_cast<cpCollisionType>(Tag::Player):
					idTag = "Player";
					break;
				case static_cast<cpCollisionType>(Tag::Enemy):
					idTag = "Enemy";
					break;
				case static_cast<cpCollisionType>(Tag::Wall):
					idTag = "Wall";
					break;

			}

			if (ImGui::BeginCombo("Tag", idTag.c_str()))
			{
				if (ImGui::Selectable("None"))
					SetTag(Tag::None);

				if (ImGui::Selectable("Player"))
					SetTag(Tag::Player);

				if (ImGui::Selectable("Enemy"))
					SetTag(Tag::Enemy);
				
				ImGui::EndCombo();
			}

		}

		
	}

	nlohmann::json RigidBodyComponent::Serialize(const entt::handle entity) const
	{
		return nlohmann::json();
	}

	void RigidBodyComponent::Unserialize(entt::handle entity, const nlohmann::json& doc)
	{
	}

	void RigidBodyComponent::RecomputeMoment()
	{
		if (!m_body.IsDynamic())
			return; // on ne touche pas aux objets kinématiques ou statiques

		float mass = m_body.GetMass();

		// Le moment angulaire final est la somme de tous les moments angulaires
		float moment = 0.f;
		for (auto&& [shape, shapeData] : m_shapes)
			moment += shape->ComputeMoment(mass, shapeData.offset);
		
		m_body.SetMoment(moment);
	}
}
