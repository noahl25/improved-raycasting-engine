#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <algorithm>
#include <thread>

#include "Renderer.h"
#include "World.h"
#include "Camera.h"
#include "Ray.h"
#include "Color.h"
#include "Texture.h"
#include "ThreadPool.h"

class Raycaster {

public:

	Raycaster(const Renderer& renderer);
	~Raycaster();
	void SetActiveWorld(World* world);

	void Draw(const Camera camera);

private:


	void RaycastMT(const Camera camera);
	void DrawSprites(const Camera camera);

	const Renderer& m_Renderer;
	int m_Width, m_Height;

	World* m_World = nullptr;

	float* m_ZBuffer = nullptr;

	const int m_NumThreads;
	ThreadPool m_ThreadPool;
	bool m_TerminateThreads = false;

	static inline const float FALLOFF = 0.23f;

};
