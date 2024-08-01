#include "FontManager.h"

TTF_Font* FontManager::Get(const char* path, int size)
{
	if (m_Fonts.find(Font{ path, size }) == m_Fonts.end()) {
		return TTF_OpenFont(path, size);
	}
	else {
		return m_Fonts[Font{ path, size }];
	}
}

FontManager::~FontManager()
{
	for (auto& [key, val] : m_Fonts) {
		TTF_CloseFont(val);
	}
}
