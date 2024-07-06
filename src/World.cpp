#include "World.h"
 

void World::LoadWorld(const char* path)
{
	Texture temp(path);
	bool startingPosFound = false;

	m_MapWidth = temp.GetWidth();
	m_MapHeight = temp.GetHeight();

	m_Map.resize(m_MapWidth * m_MapHeight);

	for (int i = 0; i < m_MapHeight; i++) {
		for (int j = 0; j < m_MapWidth; j++) {

			if (temp.PixelAtVec(j, i) == Color::Black) {
				m_Map[i * m_MapWidth + j] = 1;
			}

			if (temp.PixelAtVec(j, i) == Color::Red) {
				m_PlayerStartingPos = glm::vec2(i + 0.5f, j + 0.5f);
				startingPosFound = true;
			}

		}
	}

	if (!startingPosFound)
		__debugbreak();

	Sprite sprite(glm::vec3(3.5f, 4.0f, 1.0f));
	sprite.SetTexture(Renderer::GetActiveRenderer(), "res/sprites/barrel.png");
	sprite.SetScale(2.0f);
	m_Sprites.push_back(sprite);
	sprite.SetPosition(glm::vec3(3.5f, 2.0f, 1.0f));
	m_Sprites.push_back(sprite);

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
			if (m_Map[mapCheck.y * m_MapWidth + mapCheck.x] == 1)
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

	return hit;

}

World::~World()
{
 
}