#pragma once

#include <PigeonFeeder/PigeonState.hpp>

namespace PigeonFeeder
{
	class IdleState : public PigeonState
	{
		public:
			virtual void StateAction(Game& game) override;
	};
}


