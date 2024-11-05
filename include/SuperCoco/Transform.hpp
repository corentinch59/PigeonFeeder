#pragma once

#ifndef SUPER_COCO_TRANSFORM_HPP
#define SUPER_COCO_TRANSFORM_HPP

#include <SuperCoco/Export.hpp>
#include <SuperCoco/Vector2.hpp>
#include <entt/entt.hpp>
#include <nlohmann/json_fwd.hpp>
#include <vector>
#include <string>

namespace Sce
{
	template<typename T> class Matrix;
	using Matrixf = Matrix<float>;

	class WorldEditor;

	class SUPER_COCO_API Transform
	{
		public:
			Transform();
			Transform(const Transform& transform);
			Transform(Transform&& transform) noexcept;
			~Transform();

			Vector2f GetGlobalPosition() const;
			float GetGlobalRotation() const;
			Vector2f GetGlobalScale() const;

			const std::vector<Transform*>& GetChildren() const;
			Transform* GetParent() const;
			const Vector2f& GetPosition() const;
			float GetRotation() const;
			const Vector2f& GetScale() const;

			void PopulateInspector(WorldEditor& worldEditor);

			void Rotate(float rotation);
			void Scale(float scale);
			void Scale(const Vector2f& scale);

			nlohmann::json Serialize(const entt::handle entity) const;

			void SetParent(Transform* parent);
			void SetPosition(const Vector2f& position);
			void SetRotation(const float& rotation);
			void SetScale(const Vector2f& scale);

			void Translate(const Vector2f& translation);

			Matrixf LocalToWorldMatrix() const;
			Matrixf GetTransformMatrix3x3() const;
			Matrixf LocalToWorld(const Matrixf& other) const;
			Vector2f LocalToWorldPoint(const Vector2f& point) const;
			Matrixf WorldToLocal(const Matrixf& other) const;
			Matrixf WorldToLocalMatrix() const;
			Vector2f WorldToLocalPoint(const Vector2f& point) const;

			Transform& operator=(const Transform& transform);
			Transform& operator=(Transform&& transform) noexcept;

			std::string ToString() const;

			static void Unserialize(entt::handle entity, const nlohmann::json& doc);

		private:
			void AttachChild(Transform* child);
			void DetachChild(Transform* child);

			std::vector<Transform*> m_children;
			Transform* m_parent;
			Vector2f m_position;
			Vector2f m_scale;
			float m_rotation;
	};
}


#endif
