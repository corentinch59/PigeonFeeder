#include <SuperCoco/InputManager.hpp>
#include <SDL2/SDL_gamecontroller.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <stdexcept>

namespace Sce
{
	InputManager::InputManager()
	{
		if (s_instance != nullptr)
			throw std::runtime_error("There is more than 1 input manager object");

		s_instance = this;
	}

	InputManager::~InputManager()
	{
		s_instance = nullptr;
	}

	void InputManager::BindKeyPressed(SDL_Keycode key,std::string action)
	{
		if (m_keyInputMap.contains(key))
			m_keyInputMap.erase(m_keyInputMap.find(key));

		m_keyInputMap[key] = std::move(action);
	}

	void InputManager::BindMouseButtonPressed(MouseButton btn, std::string action)
	{
		int mouseButton;
		switch (btn)
		{
			case MouseButton::LEFT_CLICK:   mouseButton = SDL_BUTTON_LEFT;   break;
			case MouseButton::RIGHT_CLICK:  mouseButton = SDL_BUTTON_RIGHT;  break;
			case MouseButton::MIDDLE_CLICK: mouseButton = SDL_BUTTON_MIDDLE; break;
			default:
				return; //< ne devrait pas arriver
		}

		if (m_mouseInputMap.contains(mouseButton))
			m_mouseInputMap.erase(m_mouseInputMap.find(mouseButton));

		m_mouseInputMap[mouseButton] = std::move(action);
	}

	void InputManager::BindControllerBtnPressed(SDL_GameControllerButton btn, std::string action)
	{
		if (m_controllerBtnInputMap.contains(btn))
			m_controllerBtnInputMap.erase(m_controllerBtnInputMap.find(btn));

		m_controllerBtnInputMap[btn] = std::move(action);
	}

	void InputManager::BindControllerAxis(SDL_GameControllerAxis axis, std::string action)
	{
		if (m_controllerAxisInputMap.contains(axis))
			m_controllerAxisInputMap.erase(m_controllerAxisInputMap.find(axis));

		m_controllerAxisInputMap[axis] = std::move(action);
	}

	void InputManager::BindAction(std::string action, std::function<void(bool, int, float)> functor)
	{
		m_actionMap[action].action = std::move(functor);
	}

	void InputManager::UnBindKey(SDL_Keycode key)
	{
		if (!m_keyInputMap.contains(key))
		{
			fmt::print(fg(fmt::color::red), "Trying to unbind an already unbound key\n");
			return;
		}

		m_keyInputMap.erase(m_keyInputMap.find(key));
	}

	void InputManager::UnBindMouse(MouseButton btn)
	{
		if (!m_mouseInputMap.contains(static_cast<int>(btn)))
		{
			fmt::print(fg(fmt::color::red), "Trying to unbind an already unbound button\n");
			return;
		}

		m_mouseInputMap.erase(m_mouseInputMap.find(static_cast<int>(btn)));
	}

	void InputManager::UnBindControllerButton(SDL_GameControllerButton btn)
	{
		if (!m_controllerBtnInputMap.contains(btn))
		{
			fmt::print(fg(fmt::color::red), "Triying to unbind an already unbound button\n");
			return;
		}

		m_controllerBtnInputMap.erase(m_controllerBtnInputMap.find(btn));
	}

	void InputManager::UnBindAction(const std::string& action)
	{
		if (!m_actionMap.contains(action))
		{
			fmt::print(fg(fmt::color::red), "Trying to unbind an action that doesn't exist\n");
			return;
		}

		m_actionMap.erase(m_actionMap.find(action));
	}

	void InputManager::TriggerAction(const std::string& action, int id, float value)
	{
		InputAction& inputAction = m_actionMap[action];
		if (inputAction.nbKeyPressed++ == 0)
		{
			if (inputAction.action)
				inputAction.action(true, id, value);
		}
		else {
			fmt::println("too much inputs, {0}", inputAction.nbKeyPressed);
		}
	}

	void InputManager::ReleaseAction(const std::string& action, int id, float value)
	{
		auto it = m_actionMap.find(action);
		if (it == m_actionMap.end())
			return;

		InputAction& inputAction = it->second;
		if (--inputAction.nbKeyPressed == 0)
		{
			if (inputAction.action)
				inputAction.action(false, id, value);
		}

	}

	void InputManager::HandleEvent(const SDL_Event& event)
	{
		switch (event.type)
		{
			case SDL_CONTROLLERBUTTONDOWN:
			{
				auto it = m_controllerBtnInputMap.find(static_cast<SDL_GameControllerButton>(event.cbutton.button));
				if (it != m_controllerBtnInputMap.end())
					TriggerAction(it->second, event.cbutton.which);

				break;
			}

			case SDL_CONTROLLERBUTTONUP:
			{
				auto it = m_controllerBtnInputMap.find(static_cast<SDL_GameControllerButton>(event.cbutton.button));
				if (it != m_controllerBtnInputMap.end())
					ReleaseAction(it->second, event.cbutton.which);

				break;
			}

			case SDL_CONTROLLERAXISMOTION:
			{
				auto it = m_controllerAxisInputMap.find(static_cast<SDL_GameControllerAxis>(event.caxis.axis));
				if (it != m_controllerAxisInputMap.end())
				{
					float inputVal = event.caxis.value < 0 ? static_cast<float>(event.caxis.value) / 32768.f : static_cast<float>(event.caxis.value) / 32767.f;
					TriggerAction(it->second, event.caxis.which, inputVal);
					ReleaseAction(it->second, event.caxis.which, inputVal);
				}

				break;
			}

			case SDL_KEYDOWN:
			{
				if (event.key.repeat != 0)
					break;

				auto it = m_keyInputMap.find(event.key.keysym.sym);
				if (it != m_keyInputMap.end())
					TriggerAction(it->second);

				break;
			}

			case SDL_KEYUP:
			{
				if (event.key.repeat != 0)
					break;

				auto it = m_keyInputMap.find(event.key.keysym.sym);
				if (it != m_keyInputMap.end())
					ReleaseAction(it->second);

				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				auto it = m_mouseInputMap.find(event.button.button);
				if (it != m_mouseInputMap.end())
					TriggerAction(it->second);

				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				auto it = m_mouseInputMap.find(event.button.button);
				if (it != m_mouseInputMap.end())
					ReleaseAction(it->second);

				break;
			}

			case SDL_CONTROLLERDEVICEADDED:
			{
				SDL_GameController* controller = SDL_GameControllerOpen(event.cdevice.which);
				SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
				SDL_JoystickID joyID = SDL_JoystickInstanceID(joystick);
				if (controller)
				{
					m_gameControllers[joyID] = std::move(controller);
					fmt::println("Controller : {0} connected", SDL_GameControllerName(controller));
					fmt::println("Controller ID : {0}", joyID);
				}
				else 
					fmt::println("Error trying to connect controller : {0}", SDL_GetError());

				break;
			}

			case SDL_CONTROLLERDEVICEREMOVED:			{
				auto it = m_gameControllers.find(event.cdevice.which);
				if (it != m_gameControllers.end())
					SDL_GameControllerClose(it->second);
				
				fmt::println("Controller {0} disconnected", event.cdevice.which);
				break;
			}
		}
	}

	bool InputManager::IsActive(const std::string& action)
	{
		auto it = m_actionMap.find(action);
		if (it == m_actionMap.end())
			return false;

		const InputAction& actionData = it->second;
		return actionData.nbKeyPressed > 0;
	}

	InputManager& InputManager::Instance()
	{
		return *s_instance;
	}

	InputManager* InputManager::s_instance = nullptr;
}

