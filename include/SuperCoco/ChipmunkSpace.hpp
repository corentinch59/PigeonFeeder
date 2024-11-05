#pragma once

#include <SuperCoco/Export.hpp>
#include <SuperCoco/Matrix.hpp>
#include <SuperCoco/Vector2.hpp>

struct cpSpace;

namespace Sce
{
	class Renderer;

	class SUPER_COCO_API ChipmunkSpace
	{
		public:
			ChipmunkSpace();
			ChipmunkSpace(const ChipmunkSpace&) = delete;
			ChipmunkSpace(ChipmunkSpace&& space) noexcept;
			~ChipmunkSpace();

			void DebugDraw(Renderer& renderer, const Matrixf& cameraInverseTransform);

			cpSpace* GetHandle() const;

			void SetDamping(float damping);
			void SetGravity(const Vector2f& gravity);

			void Step(float deltaTime);

			ChipmunkSpace& operator=(const ChipmunkSpace&) = delete;
			ChipmunkSpace& operator=(ChipmunkSpace&& space) noexcept;

		private:
			cpSpace* m_handle;
	};	
}