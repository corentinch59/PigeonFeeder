#pragma once

#include <PigeonFeeder/PigeonState.hpp>
#include <SuperCoco/Vector2.hpp>

namespace PigeonFeeder
{
	class PigeonFood;

	class WalkingState : public PigeonState
	{
		public:
			WalkingState(std::shared_ptr<PigeonFood> food, Sce::Vector2f target);
			WalkingState(std::shared_ptr<PigeonFood> food);

			virtual void StateAction(Game& game) override;

		private:
			std::shared_ptr<PigeonFood> m_food;
			Sce::Vector2f m_target;

			bool m_towardfood;

	};
}


