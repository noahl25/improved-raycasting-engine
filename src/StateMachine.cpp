#include "StateMachine.h"

StateMachine::StateMachine(const char* name, int width, int height)
	: m_Renderer(width, height)
{
	m_Window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, NULL);
	m_Renderer.Init(m_Window);
	m_Renderer.CreateRenderingSurface();

	Audio::Init();
}

void StateMachine::Run()
{
	assert(m_StateCreators.size() != 0);
	m_CurrentState = m_StateCreators[m_CurrentStateIndex]();

	const int FPSTarget = 60;
	const int deltaTimeTarget = 1000 / FPSTarget;

	unsigned frameStart;
	unsigned frameTime;

	unsigned prevTime = SDL_GetTicks();

	while (true) {

		Animation::UpdateCounter();

		frameStart = SDL_GetTicks();

		m_CurrentState->HandleEvents(m_DeltaTime);
		m_CurrentState->Update(m_DeltaTime);
		m_CurrentState->Render(m_DeltaTime);

		frameTime = SDL_GetTicks() - frameStart;
		m_DeltaTime = frameTime / 1000.0f;

		if (m_CurrentState->IsComplete()) {
			delete m_CurrentState;
			m_CurrentStateIndex++;
			if (m_CurrentStateIndex > m_StateCreators.size() - 1)
				break;
			m_CurrentState = m_StateCreators[m_CurrentStateIndex]();
		}

	}
}

StateMachine::~StateMachine()
{
	SDL_DestroyWindow(m_Window);
}
