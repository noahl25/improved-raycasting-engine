#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <SDL.h>

#include "State.h"
#include "Util.h"
#include "Renderer.h"
#include "Animation.h"
#include "Audio.h"

class StateMachine {

public:

	StateMachine(const char* name, int width, int height);

	template <typename T, typename... TArgs> requires std::is_base_of_v<State, T> && Util::is_different_v<State, T>
	void Push(TArgs&&... args) {
		m_StateCreators.emplace_back([this, ... args = std::forward<TArgs>(args)]() {
			return new T(m_Renderer, args...);
		});
	}

	void Run();

	~StateMachine();

private:

	std::vector<std::function<State*()>> m_StateCreators;
	State* m_CurrentState = nullptr;
	int m_CurrentStateIndex = 0;

	float m_DeltaTime = 0.016f;
	bool m_Running = true;

	SDL_Window* m_Window;
	Renderer m_Renderer;

};