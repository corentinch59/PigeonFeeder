#pragma once

#include <SuperCoco/Export.hpp>
#include <SuperCoco/Matrix.hpp>
#include <SuperCoco/Vector2.hpp>
#include <chipmunk/chipmunk.h>
#include <vector>

struct cpShape;

namespace Sce
{
	class ChipmunkBody;

	class SUPER_COCO_API ChipmunkShape
	{
		public:
			ChipmunkShape(const ChipmunkShape&) = delete;
			ChipmunkShape(ChipmunkShape&& body) noexcept;
			virtual ~ChipmunkShape();

			void SetFriction(float friction);
			void SetType(const ChipmunkShape& shape, cpCollisionType type);
			void SetSensor(const ChipmunkShape& shape, bool value);

			cpShape* GetHandle() const;
			cpCollisionType GetType(const ChipmunkShape& shape) const;
			cpBool GetSensor(const ChipmunkShape& shape) const;

			ChipmunkShape& operator=(const ChipmunkShape&) = delete;
			ChipmunkShape& operator=(ChipmunkShape&& body) noexcept;

			static ChipmunkShape BuildBox(ChipmunkBody& body, float width, float height, float radius = 0.f);
			static ChipmunkShape BuildBox(ChipmunkBody& body, float left, float bottom, float right, float top, float radius = 0.f);
			static ChipmunkShape BuildBox(ChipmunkBody& body, const Vector2f& topLeftCorner, const Vector2f& bottomRightCorner, float radius = 0.f);
			static ChipmunkShape BuildCircle(ChipmunkBody& body, float radius, const Vector2f& offset = Vector2f(0.f, 0.f));
			static ChipmunkShape BuildConvexHull(ChipmunkBody& body, const std::vector<Vector2f>& positions, const Matrixf& transform = Matrixf::Identity(3.f), float radius = 0.f);
			static ChipmunkShape BuildSegment(ChipmunkBody& body, const Vector2f& from, const Vector2f& to, float radius = 0.f);

			static float ComputeBoxMoment(float mass, float width, float height);
			static float ComputeBoxMoment(float mass, float left, float bottom, float right, float top);
			static float ComputeBoxMoment(float mass, const Vector2f& topLeftCorner, const Vector2f& bottomRightCorner);
			static float ComputeCircleMoment(float mass, float radius, const Vector2f& offset = Vector2f(0.f, 0.f));
			static float ComputePolyMoment(float mass, const std::vector<Vector2f>& positions, const Matrixf& transform = Matrixf::Identity(3.f), float radius = 0.f);
			static float ComputeSegmentMoment(float mass, const Vector2f& from, const Vector2f& to, float radius);

		private:
			ChipmunkShape(ChipmunkBody& body, cpShape* shape);

			cpShape* m_handle;
	};	
}