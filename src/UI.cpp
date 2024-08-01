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

void UI::MoveToTop(const UIComponent* component)
{
	int i = 0;
	for (; i < m_UIComponents.size(); i++)
		if (component == m_UIComponents[i])
			break;
	auto it = m_UIComponents.begin() + i;
	std::rotate(it, it + 1, m_UIComponents.end());
}

UI::~UI()
{
	for (auto& component : m_UIComponents) {
		delete component;
	}
}
