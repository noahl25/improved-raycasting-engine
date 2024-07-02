#include "Raycaster.h"

Raycaster::Raycaster(int width, int height)
	: m_Width(width), m_Height(height), m_Renderer(width, height)
{
	m_ZBuffer = new float[m_Width];
	m_Hits.resize(m_Width);

}

void Raycaster::Init(SDL_Window* window)
{
	m_Renderer.Init(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));
 
}

void Raycaster::SetActiveWorld(World* world)
{
	m_World = world;
}

static float AngleBetween(const glm::vec2& first, const glm::vec2& second) {

	//normal equality causing issues
	if (glm::epsilonEqual(first.x, second.x, 0.001f) && glm::epsilonEqual(first.y, second.y, 0.001f))
		return 0;

	return glm::acos(glm::dot(first, second) / (glm::length(first) * glm::length(second)));
}

static Texture texture("res/textures/cobble.png");
 
void Raycaster::Update(const Camera& camera)
{
 
	m_Renderer.Clear();

	Ray ray;
	ray.Start = camera.Position;

	glm::vec2 cameraDir = glm::normalize(glm::vec2(glm::cos(glm::radians(camera.Yaw)), glm::sin(glm::radians(camera.Yaw))));
	glm::vec2 cameraRight = glm::normalize(glm::vec2(-glm::sin(glm::radians(camera.Yaw)), glm::cos(glm::radians(camera.Yaw))));

	static SDL_Surface* surface = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	static uint32_t* pixels = static_cast<uint32_t*>(surface->pixels);

	for (int i = -m_Width / 2; i < m_Width / 2; i++) {


		ray.Direction = glm::normalize(cameraDir + cameraRight * ((float)i / (float)m_Width));
		RaycastHit hit = m_World->CastRay(ray);
		hit.HitAngle = AngleBetween(cameraDir, ray.Direction);

		if (hit.Hit) {

			/* --walls-- */

			float normalizedDist = hit.Distance * glm::cos(hit.HitAngle);

			m_ZBuffer[i + m_Width / 2] = normalizedDist;

			float lineHeight = m_Height / normalizedDist;

			float step = 16.0f / (float)lineHeight;
			float texOffset = 0.0f;

			if (lineHeight > m_Height) {
				texOffset = (float)(lineHeight - m_Height) / 2.0f;
				lineHeight = m_Height;
			}
			float lineOffset = (m_Height - lineHeight) / 2;
			
			float texY = texOffset * step;

			float wallX = 0;
			if (hit.Side == 0)
				wallX = hit.HitPos.y;
			else
				wallX = hit.HitPos.x;
			wallX -= glm::floor(wallX);

			int texX = (int)(wallX * (float)TEX_SIZE);

			if (hit.Side == 0 && ray.Direction.x > 0)
				texX = TEX_SIZE - 1 - texX;
			if (hit.Side == 1 && ray.Direction.y < 0)
				texX = TEX_SIZE - 1 - texX;

			float colorMod = -FALLOFF * hit.Distance + 1;
			if (colorMod < 0) colorMod = 0;

			for (int j = lineOffset; j <= lineOffset + lineHeight; j++) {

				uint32_t color = texture.PixelAtOptimized((int)texX, (int)texY);
			//	if (hit.Side == 0) color = Util::MultiplyRGBA(color, 1.3f);
				color = Util::MultiplyRGBA(color, colorMod);

				pixels[j * m_Width + i + (m_Width / 2)] = color;

				texY += step;
				texY = glm::clamp(texY, 0.0f, (float)TEX_SIZE - 1);

			}

			/* --floor-- */

			float floorXWall, floorYWall;

			if (hit.Side == 0 && ray.Direction.x > 0) {
				floorXWall = hit.MapX;
				floorYWall = hit.MapY + wallX;
			}
			else if (hit.Side == 0 && ray.Direction.x < 0) {
				floorXWall = hit.MapX + 1.0f;
				floorYWall = hit.MapY + wallX;
			}
			else if (hit.Side == 1 && ray.Direction.y > 0) {
				floorXWall = hit.MapX + wallX;
				floorYWall = hit.MapY;
			}
			else {
				floorXWall = hit.MapX + wallX;
				floorYWall = hit.MapY + 1.0f;
			}

			int offset = lineOffset + lineHeight;
			if (offset < 0) offset = m_Height;

			float distWall = normalizedDist;

			for (int j = offset + 1; j < m_Height; j++) {
				float currentDist = m_Height / (2.0f * j - m_Height);
				float weight = currentDist / distWall;

				float currentFloorX = weight * floorXWall + (1.0f - weight) * camera.Position.x;
				float currentFloorY = weight * floorYWall + (1.0f - weight) * camera.Position.y;

				int floorTexX = (int)(currentFloorX * TEX_SIZE) % TEX_SIZE;
				int floorTexY = (int)(currentFloorY * TEX_SIZE) % TEX_SIZE;

				colorMod = -FALLOFF * currentDist + 1;
				
				uint32_t color = texture.PixelAtOptimized(floorTexX, floorTexY);
				color = Util::MultiplyRGBA(color, colorMod);
				pixels[j * m_Width + i + (m_Width / 2)] = color;
			}

			for (int j = 0; j < lineOffset; j++) {
				pixels[j * m_Width + i + (m_Width / 2)] = 0x000000ff;
			}

		}


	}

	
	for (const Sprite& sprite : m_World->GetSprites()) {
		
		sprite.Draw(m_Renderer, camera, pixels, m_ZBuffer);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer.GetRenderer(), surface);
	m_Renderer.Blit(texture);

	SDL_DestroyTexture(texture);

	/* --sprites-- */

	m_Renderer.Present();

}

Raycaster::~Raycaster()
{
	delete[] m_ZBuffer;
}