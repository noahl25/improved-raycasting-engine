#include "Raycaster.h"

Raycaster::Raycaster(const Renderer& renderer)
	: m_Renderer(renderer), m_Width(renderer.GetWidth()), m_Height(renderer.GetHeight()), m_ZBuffer(new float[renderer.GetWidth()]), m_NumThreads(std::thread::hardware_concurrency())
{
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

void Raycaster::Draw(const Camera camera)
{

	RaycastMT(camera);
	DrawSprites(camera);
	
	m_Renderer.BlitRenderingSurface();
}

//static Texture texture("res/textures/cobble.png");

void Raycaster::RaycastMT(const Camera camera)
{
	const glm::vec2 cameraDir = glm::normalize(glm::vec2(glm::cos(glm::radians(camera.Yaw)), glm::sin(glm::radians(camera.Yaw))));
	const glm::vec2 cameraRight = glm::normalize(glm::vec2(-glm::sin(glm::radians(camera.Yaw)), glm::cos(glm::radians(camera.Yaw))));
	
	int division = m_Width / m_NumThreads;
	int begin = -m_Width / 2;
	int end = begin + division;

	for (int i = 0; i < m_NumThreads; i++) {

		m_ThreadPool.Do(
			[this, camera, cameraDir, cameraRight, begin, end] {

				glm::vec2 src = glm::vec2(2.0f, 2.0f);

				Ray ray;
				ray.Start = camera.Position;
				int halfWidth = m_Width / 2;

				for (int i = begin; i < end; i++) {

					if (m_TerminateThreads) return;

					ray.Direction = glm::normalize(cameraDir + cameraRight * ((float)i / (float)m_Width));
					RaycastHit hit = m_World->CastRay(ray);
					hit.HitAngle = AngleBetween(cameraDir, ray.Direction);

					if (hit.Hit) {

						/* --walls-- */

						float normalizedDist = hit.Distance * glm::cos(hit.HitAngle);

						m_ZBuffer[i + halfWidth] = normalizedDist;

						float lineHeight = m_Height / normalizedDist;
						float step = 16.0f / (float)lineHeight;
						float texOffset = 0.0f;


						float lineOffset = (m_Height - lineHeight) / 2;

						lineOffset += camera.Pitch;


						float wallX = 0;
						if (hit.Side == 0)
							wallX = hit.HitPos.y;
						else
							wallX = hit.HitPos.x;
						wallX -= glm::floor(wallX);

						if (-FALLOFF * hit.Distance + 1 > 0) {

							float texY = texOffset * step;

							int texX = (int)(wallX * (float)hit.TextureHit->GetWidth());

							if (hit.Side == 0 && ray.Direction.x > 0)
								texX = hit.TextureHit->GetWidth() - 1 - texX;
							if (hit.Side == 1 && ray.Direction.y < 0)
								texX = hit.TextureHit->GetWidth() - 1 - texX;

							float colorMod = -FALLOFF * hit.Distance + 1;
							if (colorMod < 0) colorMod = 0;

							if (lineOffset >= 0) {

								for (int j = lineOffset; j <= lineOffset + lineHeight; j++) {

									uint32_t color = hit.TextureHit->PixelAt((int)texX, (int)texY);

									//color = Util::Blend(color, Util::AsUInt32({ 1.0f, 0.85f, 0.45f, 1.0f }), 1.0f / glm::distance(hit.HitPos, src));
									

									color = Util::MultiplyRGBA(color, colorMod);

									if (j < m_Height)
										m_Renderer.GetRenderingSurface()[j * m_Width + i + halfWidth] = color;

									texY += step;
									texY = std::min(std::max(texY, 0.0f), (float)hit.TextureHit->GetHeight() - 1);

								}
							}
							else {

								int diff = glm::abs(lineOffset);
								texY += step * diff;

								for (int j = 0; j <= m_Height; j++) {

									uint32_t color = hit.TextureHit->PixelAt((int)texX, (int)texY);
									//	if (hit.Side == 0) color = Util::MultiplyRGBA(color, 1.3f);

									//color = Util::Blend(color, Util::AsUInt32({ 1.0f, 0.85f, 0.45f, 1.0f }), 1.0f / glm::distance(hit.HitPos, src));
									

									color = Util::MultiplyRGBA(color, colorMod);

									
									m_Renderer.GetRenderingSurface()[j * m_Width + i + halfWidth] = color;

									texY += step;
									texY = std::min(std::max(texY, 0.0f), (float)hit.TextureHit->GetHeight() - 1);

								}

							}
						}
						else {
							for (int j = lineOffset; j <= lineOffset + lineHeight; j++) {
								if (j < m_Height && j >= 0)
									m_Renderer.GetRenderingSurface()[j * m_Width + i + halfWidth] = 0x000000ff;
							}
						}
						/* --floor-- */

						//thanks to lodev for help with this!

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

						int start = (offset + 1) * m_Width + i + halfWidth;

						for (int j = offset + 1; j < m_Height; j++) {

							float currentDist = m_Height / (2.0f * (j - camera.Pitch) - m_Height);

							float floorColorMod = -FALLOFF * currentDist + 1;

							if (floorColorMod <= 0) {
								m_Renderer.GetRenderingSurface()[start] = 0x000000ff;
								start += m_Width;
								continue;
							}

							float weight = currentDist / normalizedDist;

							float currentFloorX = weight * floorXWall + (1.0f - weight) * camera.Position.x;
							float currentFloorY = weight * floorYWall + (1.0f - weight) * camera.Position.y;

							const Texture& texture = m_World->GetTextureAtPos(currentFloorX, currentFloorY);

							int floorTexX = (int)(currentFloorX * texture.GetWidth()) & (texture.GetWidth() - 1);
							int floorTexY = (int)(currentFloorY * texture.GetHeight()) & (texture.GetHeight() - 1);


							uint32_t color = texture.PixelAt(floorTexX, floorTexY);

							//color = Util::Blend(color, Util::AsUInt32({ 1.0f, 0.85f, 0.45f, 1.0f }), glm::clamp(1.0f / glm::distance({ currentFloorX, currentFloorY }, src), 0.0f, 1.0f));
							
							m_Renderer.GetRenderingSurface()[start] = Util::MultiplyRGBA(color, floorColorMod);
							start += m_Width;
							
						}

						for (int j = 0; j < lineOffset; j++) {
							if (j < m_Height)
								m_Renderer.GetRenderingSurface()[j * m_Width + i + halfWidth] = 0x000000ff;
						}

					}


				}

			}
		);

		begin += division;
		end += division;


	}
 
	m_ThreadPool.Wait();
}

void Raycaster::DrawSprites(const Camera camera)
{
	Sprite::HoveredSprite = nullptr;

	for (const Sprite& sprite : m_World->GetSpritesSorted(camera)) {

		sprite.Draw(m_Renderer, camera, m_Renderer.GetRenderingSurface(), m_ZBuffer);

	}

}

Raycaster::~Raycaster()
{
	delete[] m_ZBuffer;
}
