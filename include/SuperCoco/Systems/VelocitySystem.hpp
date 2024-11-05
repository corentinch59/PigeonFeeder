#ifndef SUPERCOCO_VELOCITYSYSTEM_HPP
#define SUPERCOCO_VELOCITYSYSTEM_HPP

#include <SuperCoco/Export.hpp>
#include <entt/entt.hpp>

namespace Sce
{
	class VelocityComponent;
	class Transform;

	class SUPER_COCO_API VelocitySystem
	{
	public:
		VelocitySystem(entt::registry* registry);
		~VelocitySystem();

		void ApplyVelocity(float deltaTime);
		
		static VelocitySystem& Instance();

	private:
		entt::registry* m_registry;

		static VelocitySystem* s_instance;
	};
}


#endif