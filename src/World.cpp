#include "World.h"
#include <iostream>

void World::LoadRoom(const char* path, const char* config)
{
	Texture map(path);

	m_MapWidth = map.GetWidth();
	m_MapHeight = map.GetHeight();

	m_Room.resize(m_MapWidth * m_MapHeight);

	for (int i = 0; i < m_MapWidth * m_MapHeight; i++)
		m_Room[i] = 0;

	std::ifstream stream(config);
	std::string line;

	std::vector<std::string> configs;
	std::stringstream sstream;

	while (std::getline(stream, line)) {
		if ((line[0] == '/' && line[1] == '/') || line.empty())
			continue;
		if ((line == "WALL" || line == "FLOOR" || line == "STARTING_POS") && !sstream.str().empty()) {
			configs.push_back(sstream.str());
			sstream.str("");
		}
		sstream << line << "\n";
	}

	if (!sstream.str().empty())
		configs.push_back(sstream.str());

	enum Status {
		PARSING_WALL,
		PARSING_FLOOR,
		PARSING_STARTINGPOS
	};

	int wallTextureID = 1;
	int floorTextureID = 16;
	std::unordered_map<uint32_t, int> m_IDAtlas;

	uint32_t startingPosColor;
	int underPlayerTexture;
	
	for (int i = 0; i < configs.size(); i++) {
		std::stringstream config(configs[i]);

		std::string feature;
		Status status;

		std::getline(config, feature, '\n');
		if (feature == "WALL")
			status = PARSING_WALL;
		else if (feature == "FLOOR")
			status = PARSING_FLOOR;
		else if (feature == "STARTING_POS")
			status = PARSING_STARTINGPOS;
		else
			__debugbreak();

		while (std::getline(config, feature, '\n')) {

			if (status == PARSING_WALL || status == PARSING_FLOOR) {
				if (feature.find(':') != std::string::npos) {
					std::string key = feature.substr(0, feature.find(':'));
					std::string value = feature.substr(feature.find(':') + 1);
					if (key == "color") {
						std::stringstream ss;
						uint32_t color;
						ss << std::hex << value;
						ss >> color;
						m_IDAtlas[color] = status == PARSING_WALL ? wallTextureID : floorTextureID;
					}
					else if (key == "texture") {
						m_RoomTextureAtlas[status == PARSING_WALL ? wallTextureID : floorTextureID] = Texture(value.c_str());
					}
				}
			}

			if (status == PARSING_STARTINGPOS) {
				std::string key = feature.substr(0, feature.find(':'));
				std::string value = feature.substr(feature.find(':') + 1);
				if (key == "color") {
					std::stringstream ss;
					ss << std::hex << value;
					ss >> startingPosColor;
				}
				else if (key == "texture") {
					m_RoomTextureAtlas[floorTextureID] = Texture(value.c_str());
					underPlayerTexture = floorTextureID;
				}
			}

		}

		if (status == PARSING_WALL)
			wallTextureID++;
		else if (status == PARSING_FLOOR || status == PARSING_STARTINGPOS)
			floorTextureID++;

	}

	for (int i = 0; i < m_MapHeight; i++) {
		for (int j = 0; j < m_MapWidth; j++) {
			if (m_IDAtlas.find(map.PixelAt(j, i)) != m_IDAtlas.end()) {
				m_Room[i * m_MapWidth + j] = m_IDAtlas[map.PixelAt(j, i)];
			}
			if (map.PixelAt(j, i) == startingPosColor) {
				m_PlayerStartingPos = glm::vec2(j + 0.5f, i + 0.5f);
			}
		}
	}

	m_Room[(int)m_PlayerStartingPos.y * m_MapWidth + (int)m_PlayerStartingPos.x] = underPlayerTexture;

}
 
void World::LoadSprites(const char* path, const char* config)
{
	assert(m_Room.size() != 0);

	std::unordered_map<uint32_t, Sprite> spriteAtlas;
	std::ifstream stream(config);
	std::string line;

	Sprite sprite;
	uint32_t color;
	bool parsingSprites = false;
	
	while (std::getline(stream, line)) {
		if (line[0] == '/' && line[1] == '/')
			continue;

		if (line == "SPRITE") {
			if (parsingSprites) {
				spriteAtlas[color] = sprite;
			}
			parsingSprites = true;
			sprite = Sprite();
		}
		else if (!line.empty()) {
			std::string key = line.substr(0, line.find(':'));
			std::string value = line.substr(line.find(':') + 1);
			std::stringstream ss;
			if (key == "color") {
				ss << std::hex << value;
				ss >> color;
			}
			else if (key == "texture") {
				sprite.SetTexture(Renderer::GetActiveRenderer(), {value.begin(), value.end()});
			}
			else if (key == "offset") {
				std::string str = { value.begin(), value.end() };
				sprite.SetHeightOffset(std::stoi(str));
			}
			else if (key == "scale") {
				float scale;
				ss << value;
				ss >> scale;
				sprite.SetScale(scale);
			}
			//else if (key == "identifier") {
			//	sprite.m_Identifier = { value.begin(), value.end() };
			//}

		}
	}

	spriteAtlas[color] = sprite;
	
	stream.close();

	Texture map(path);

	for (int i = 0; i < map.GetHeight(); i++) {
		for (int j = 0; j < map.GetWidth(); j++) {

			if (spriteAtlas.find(map.PixelAt(j, i)) != spriteAtlas.end()) {
				spriteAtlas[map.PixelAt(j, i)].SetPosition(glm::vec3((float)j / (float)map.GetWidth() * m_MapWidth, (float)i / (float)map.GetHeight() * m_MapHeight, 0));
				m_Sprites.push_back(spriteAtlas[map.PixelAt(j, i)]);
			}

		}
	}

}

RaycastHit World::CastRay(const Ray& ray)
{

	RaycastHit hit;

	glm::vec2 rayStart = ray.Start;
	glm::vec2 rayDir = ray.Direction;

	glm::vec2 rayUnitStepSize = { sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x)), sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y)) };
	glm::ivec2 mapCheck = rayStart;
	glm::vec2 rayLength1D;
	glm::ivec2 step;

	if (rayDir.x < 0)
	{
		step.x = -1;
		rayLength1D.x = (rayStart.x - float(mapCheck.x)) * rayUnitStepSize.x;
	}
	else
	{
		step.x = 1;
		rayLength1D.x = (float(mapCheck.x + 1) - rayStart.x) * rayUnitStepSize.x;
	}

	if (rayDir.y < 0)
	{
		step.y = -1;
		rayLength1D.y = (rayStart.y - float(mapCheck.y)) * rayUnitStepSize.y;
	}
	else
	{
		step.y = 1;
		rayLength1D.y = (float(mapCheck.y + 1) - rayStart.y) * rayUnitStepSize.y;
	}

	bool tileFound = false;
	float maxDist = 100.0f;
	float dist = 0.0f;
	while (!tileFound && dist < maxDist)
	{

		if (rayLength1D.x < rayLength1D.y)
		{
			mapCheck.x += step.x;
			dist = rayLength1D.x;
			rayLength1D.x += rayUnitStepSize.x;
			hit.Side = 0;
		}
		else
		{
			mapCheck.y += step.y;
			dist = rayLength1D.y;
			rayLength1D.y += rayUnitStepSize.y;
			hit.Side = 1;
		}


		if (mapCheck.x >= 0 && mapCheck.x < m_MapWidth && mapCheck.y >= 0 && mapCheck.y < m_MapHeight)
		{
			if (m_Room[mapCheck.y * m_MapWidth + mapCheck.x] <= 15)
			{
				tileFound = true;
			}
		}
	}


	if (!tileFound) {
		return hit;
	}

	hit.HitPos = rayStart + rayDir * dist;
	hit.Distance = dist;
	hit.Hit = true;
	hit.MapX = mapCheck.x;
	hit.MapY = mapCheck.y;
	hit.TextureHit = &m_RoomTextureAtlas[m_Room[hit.MapY * m_MapWidth + hit.MapX]];

	return hit;

}

World::~World()
{
 
}