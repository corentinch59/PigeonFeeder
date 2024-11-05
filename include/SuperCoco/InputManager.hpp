#ifndef SUPERCOCO_INPUTMANAGER_HPP
#define SUPERCOCO_INPUTMANAGER_HPP

#include <SuperCoco/Export.hpp>
#include <SuperCoco/Vector2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#include <string>
#include <functional>
#include <unordered_map>

namespace Sce
{
	enum class MouseButton
	{
		LEFT_CLICK,
		MIDDLE_CLICK,
		RIGHT_CLICK
	};

	struct InputAction
	{
		unsigned char nbKeyPressed = 0;
		std::function<void(bool, int, float)> action;
	};

	class SUPER_COCO_API InputManager
	{
		public:
			InputManager();
			~InputManager();

			void BindKeyPressed(SDL_Keycode key, std::string action);
			void BindMouseButtonPressed(MouseButton btn, std::string action);
			void BindControllerBtnPressed(SDL_GameControllerButton btn, std::string action);
			void BindControllerAxis(SDL_GameControllerAxis axis, std::string action);
			void BindAction(std::string action, std::function<void(bool, int, float)> functor);

			void UnBindKey(SDL_Keycode key);
			void UnBindMouse(MouseButton btn);
			void UnBindControllerButton(SDL_GameControllerButton btn);
			void UnBindAction(const std::string& action);

			void TriggerAction(const std::string& action, int id = 0, float value = 0.f);
			void ReleaseAction(const std::string& action, int id = 0, float value = 0.f);

			void HandleEvent(const SDL_Event& event);

			bool IsActive(const std::string& action);

			static InputManager& Instance();

		private:
			static InputManager* s_instance;

			std::unordered_map<SDL_Keycode, std::string> m_keyInputMap;
			std::unordered_map<int, std::string> m_mouseInputMap;
			std::unordered_map<std::string, InputAction> m_actionMap;

			std::unordered_map<SDL_GameControllerAxis, std::string> m_controllerAxisInputMap;
			std::unordered_map<SDL_GameControllerButton, std::string> m_controllerBtnInputMap;
			std::unordered_map<SDL_JoystickID, SDL_GameController*> m_gameControllers;
	};
}


#endif