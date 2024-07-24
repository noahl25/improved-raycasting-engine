#include "UI.h"

UI::UI(const Renderer& renderer)
	: m_Renderer(renderer)
{
}

void UI::Update()
{

	m_UIComponents.erase(std::remove_if(m_UIComponents.begin(), m_UIComponents.end(), [](const auto& component) {
		return !component->IsActive();
	}), m_UIComponents.end());

	for (auto& component : m_UIComponents) {
		component->Update();
	}

}

void UI::Draw() const
{

	for (const auto& component : m_UIComponents) {
		if (!component->Hidden())
			component->Draw(m_Renderer);
	}

}

UI::~UI()
{
	for (auto& component : m_UIComponents) {
		delete component;
	}
}
