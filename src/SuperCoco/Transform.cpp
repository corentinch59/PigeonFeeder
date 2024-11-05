#include <SuperCoco/Transform.hpp>
#include <SuperCoco/WorldEditor.hpp>
#include <SuperCoco/Maths.hpp>
#include <SuperCoco/Matrix.hpp>
#include <SuperCoco/JsonSerializer.hpp>
#include <SuperCoco/NameComponent.hpp>
#include <fmt/core.h>
#include <fmt/color.h>
#include <imgui.h>
#include <cassert>

namespace Sce
{
	void to_json(nlohmann::json& j, const Transform& transform)
	{
		j["Position"] = transform.GetPosition();
		j["Rotation"] = transform.GetRotation();
		j["Scale"] = transform.GetScale();
	}

	void from_json(const nlohmann::json& j, Transform& transform)
	{
		transform.SetPosition(j.at("Position"));
		transform.SetRotation(j.at("Rotation"));
		transform.SetScale(j.at("Scale"));
	}

	Transform::Transform() :
	m_parent(nullptr),
	m_position(0.f, 0.f),
	m_rotation(0.f),
	m_scale(1.f, 1.f)
	{
	}

	Transform::Transform(const Transform& transform) :
	m_parent(nullptr),
	m_position(transform.m_position),
	m_rotation(transform.m_rotation),
	m_scale(transform.m_scale)
	{
		SetParent(transform.m_parent);
	}

	Transform::Transform(Transform&& transform) noexcept :
	m_children(std::move(transform.m_children)),
	m_parent(nullptr),
	m_position(transform.m_position),
	m_rotation(transform.m_rotation),
	m_scale(transform.m_scale)
	{
		SetParent(transform.m_parent);
		for (Transform* child : m_children)
		{
			child->m_parent = this;
		}
	}

	Transform::~Transform()
	{
		if (m_parent)
			m_parent->DetachChild(this);

		for (Transform* child : m_children)
			child->m_parent = nullptr;
	}

	Vector2f Transform::GetGlobalPosition() const
	{
		if (!m_parent)
			return m_position;

		return m_parent->LocalToWorldPoint(m_position);
	}

	float Transform::GetGlobalRotation() const
	{
		if (!m_parent)
			return m_rotation;

		return m_parent->GetGlobalRotation() + m_rotation;
	}

	Vector2f Transform::GetGlobalScale() const
	{
		if (!m_parent)
			return m_scale;

		return m_parent->GetGlobalScale() * m_scale;
	}

	const std::vector<Transform*>& Transform::GetChildren() const
	{
		return m_children;
	}

	Transform* Transform::GetParent() const
	{
		return m_parent;
	}

	const Vector2f& Transform::GetPosition() const
	{
		return m_position;
	}

	float Transform::GetRotation() const
	{
		return m_rotation;
	}

	const Vector2f& Transform::GetScale() const
	{
		return m_scale;
	}

	void Transform::PopulateInspector(WorldEditor& worldEditor)
	{
		float posArray[2] = { m_position.x, m_position.y };
		if (ImGui::InputFloat2("Position", posArray))
			SetPosition({ posArray[0], posArray[1] });

		float rot = Deg2Rad * m_rotation;
		if (ImGui::SliderAngle("Rotation", &rot))
			SetRotation(Rad2Deg * rot);

		float scaleArray[2] = { m_scale.x, m_scale.y };
		if (ImGui::InputFloat2("Scale", scaleArray))
			SetScale({ scaleArray[0], scaleArray[1] });

		if (ImGui::Button("Reparent..."))
			ImGui::OpenPopup("Parent");

		if (ImGui::BeginPopup("Parent"))
		{
			bool closePopup = false;
			if (ImGui::BeginCombo("New parent", "Choose..."))
			{
				if (m_parent)
				{
					if (ImGui::Selectable("Unparent"))
					{
						SetParent(nullptr);
						closePopup = true;
					}
				}

				entt::registry& registry = worldEditor.GetRegistry();
				for (auto&& [entity, transform] : registry.view<Transform>().each())
				{
					if (&transform == this)
						continue;

					std::string entityName = fmt::format("Entity #{}", static_cast<std::uint32_t>(entity)); //< On se rappelle qu'un entt::entity est un entier
					if (const Sce::NameComponent* nameComponent = registry.try_get<Sce::NameComponent>(entity))
					{
						if (!nameComponent->name.empty())
							entityName = fmt::format("{} - {}", entityName, nameComponent->name);
					}

					if (ImGui::Selectable(entityName.c_str()))
					{
						SetParent(&transform);
						closePopup = true;
					}
				}

				ImGui::EndCombo();
			}

			if (closePopup)
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}

		if (ImGui::TreeNode("Transform matrix"))
		{
			Matrixf transformMatrix = GetTransformMatrix3x3();
			ImGui::Text("%f %f %f", transformMatrix[Vector2i(0, 0)], transformMatrix[Vector2i(0, 1)], transformMatrix[Vector2i(0, 2)]);
			ImGui::Text("%f %f %f", transformMatrix[Vector2i(1, 0)], transformMatrix[Vector2i(1, 1)], transformMatrix[Vector2i(1, 2)]);
			ImGui::Text("%f %f %f", transformMatrix[Vector2i(2, 0)], transformMatrix[Vector2i(2, 1)], transformMatrix[Vector2i(2, 2)]);
			ImGui::TreePop();
		}
	}

	void Transform::Rotate(float rotation)
	{
		m_rotation += rotation;
	}

	void Transform::Scale(float scale)
	{
		m_scale *= scale;
	}

	void Transform::Scale(const Vector2f& scale)
	{
		m_scale *= scale;
	}

	void Transform::SetParent(Transform* parent)
	{
		if (m_parent == parent)
			return;

		if (m_parent)
			m_parent->DetachChild(this);

		m_parent = parent;
		if (m_parent)
			m_parent->AttachChild(this);
	}

	void Transform::SetPosition(const Vector2f& position)
	{
		m_position = position;
	}

	void Transform::SetRotation(const float& rotation)
	{
		m_rotation = rotation;
	}

	void Transform::SetScale(const Vector2f& scale)
	{
		m_scale = scale;
	}

	void Transform::Translate(const Vector2f& translation)
	{
		m_position += translation;
	}

	Matrixf Transform::LocalToWorldMatrix() const
	{
		return m_parent ? m_parent->LocalToWorldMatrix() * GetTransformMatrix3x3() : GetTransformMatrix3x3();
	}

	nlohmann::json Transform::Serialize(const entt::handle entity) const
	{
		// On sauvegarde les propriétés du transform
		nlohmann::json doc;
		doc["Position"] = m_position;
		doc["Rotation"] = m_rotation;
		doc["Scale"] = m_scale;

		return doc;
	}

	Matrixf Transform::GetTransformMatrix3x3() const
	{
		return Matrixf::MakeTransform3x3(m_position, m_rotation, m_scale);
	}

	Matrixf Transform::LocalToWorld(const Matrixf& other) const
	{
		return LocalToWorldMatrix() * other;
	}

	Vector2f Transform::LocalToWorldPoint(const Vector2f& point) const
	{
		return LocalToWorld(Matrixf::MakeFromPosition(point)).GetVector2();
	}

	Matrixf Transform::WorldToLocal(const Matrixf& other) const
	{
		return WorldToLocalMatrix() * other;
	}

	Matrixf Transform::WorldToLocalMatrix() const
	{
		return m_parent ? m_parent->WorldToLocalMatrix() * GetTransformMatrix3x3().InvertByRowReduction().Split(2) : GetTransformMatrix3x3().InvertByRowReduction().Split(2);
	}

	Vector2f Transform::WorldToLocalPoint(const Vector2f& point) const
	{
		return WorldToLocal(Matrixf::MakeFromPosition(point)).GetVector2();
	}

	Transform& Transform::operator=(const Transform& transform)
	{
		m_position = transform.m_position;
		m_rotation = transform.m_rotation;
		m_scale = transform.m_scale;
		SetParent(transform.m_parent);

		return *this;
	}

	Transform& Transform::operator=(Transform&& transform) noexcept
	{
		for (Transform* child : m_children)
			child->m_parent = nullptr;

		m_children = std::move(transform.m_children);
		m_position = transform.m_position;
		m_rotation = transform.m_rotation;
		m_scale = transform.m_scale;
		SetParent(transform.m_parent);

		for (Transform* child : m_children)
			child->m_parent = this;

		return *this;
	}

	std::string Transform::ToString() const
	{
		return "Position :\n" + m_position.ToString() + "\nRotation :\n" + std::to_string(m_rotation) + "\nScale :\n" + m_scale.ToString() + "\n";
	}

	void Transform::AttachChild(Transform* child)
	{
		m_children.push_back(child);
	}

	void Transform::DetachChild(Transform* child)
	{
		auto it = std::find(m_children.begin(), m_children.end(), child);
		assert(it != m_children.end());

		m_children.erase(it);
	}

	void Transform::Unserialize(entt::handle entity, const nlohmann::json& doc)
	{
		auto& node = entity.emplace<Transform>();
		node.SetPosition(doc.value("Position", Vector2f(0, 0)));
		node.SetRotation(doc.value("Rotation", 0.f));
		node.SetScale(doc.value("Scale", Vector2f(1.f, 1.f)));
	}
	
}