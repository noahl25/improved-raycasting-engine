#pragma once

#include <vector>
#include <algorithm>
#include <memory>

#include "UIComponent.h"
#include "Renderer.h"
#include "Util.h"

class UI {

public:

	UI(const Renderer& renderer);
	void Update();
	void Draw() const;

	template <typename Item, typename...TArgs> requires std::is_base_of_v<UIComponent, Item> && Util::is_different_v<UIComponent, Item>
	Item* RegisterUIComponent(TArgs&&... args) {
		m_UIComponents.push_back(new Item(std::forward<TArgs>(args)...));
		return dynamic_cast<Item*>(m_UIComponents[m_UIComponents.size() - 1]);
	}

	UIComponent& GetComponent(int id) {
		return *m_UIComponents[id];
	}

	~UI();

private:

	const Renderer& m_Renderer;
	std::vector<UIComponent*> m_UIComponents;

};