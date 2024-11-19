#ifndef PIGEONFEEDER_DEATHSYSTEM_HPP
#define PIGEONFEEDER_DEATHSYSTEM_HPP

#pragma once

#include <entt/fwd.hpp>

namespace PigeonFeeder
{
	class DeathSystem
	{
		public :
			DeathSystem(entt::registry* registry);

			void DeathNote();

		private:
			entt::registry* m_registry;
	};
}

#endif