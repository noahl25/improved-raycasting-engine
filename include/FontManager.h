#pragma once

#include <SDL_ttf.h>
#include <unordered_map>
#include <string>

class FontManager {
public:

	TTF_Font* Get(const char* path, int size);
	~FontManager();

private:

	struct Font {
		const char* Name;
		int Size;

		bool operator==(const Font& other) const {
			return strcmp(Name, other.Name) == 0 && Size == other.Size;
		}
	};

	struct FontHash {
		std::size_t operator()(const Font& font) const {
			return std::hash<std::string>()(font.Name) ^ std::hash<int>()(font.Size);
		}
	};

	std::unordered_map<Font, TTF_Font*, FontHash> m_Fonts;

};