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
#include "SizedThreadPool.h"


class Raycaster {

public:

	Raycaster(int width, int height);
	~Raycaster();
	void Init(SDL_Window* window);
	void SetActiveWorld(World* world);

	void Update(const Camera camera);

private:


	void RaycastMT(const Camera camera, uint32_t* pixels);
	void DrawSprites(const Camera camera, uint32_t* pixels);

	int m_Width, m_Height;
	Renderer m_Renderer;

	World* m_World;

	float* m_ZBuffer = nullptr;
	
	bool m_TerminateThreads = false;

	static inline const float FALLOFF = 0.23f;

	static constexpr int NUM_THREADS = 4;

	SizedThreadPool<NUM_THREADS> m_ThreadPool;


};
