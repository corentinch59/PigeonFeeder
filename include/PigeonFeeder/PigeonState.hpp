#ifndef PIGEONFEEDER_PIGEONSTATE_HPP
#define PIGEONFEEDER_PIGEONSTATE_HPP

#pragma once

#include <memory>

namespace PigeonFeeder
{
	class Pigeon;
	class Game;

	//Base classe for the State machine pattern
	class PigeonState
	{

		public:
			virtual ~PigeonState() = default;
			virtual void StateAction(Game& game) = 0;

			void SetPigeon(Pigeon* pigeon) { this->m_pigeon = pigeon; };

		protected:
			Pigeon* m_pigeon = nullptr;
	};
}

#endif