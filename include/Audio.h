#pragma once

#include <SDL_mixer.h>
#include <cassert>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

class AudioSource {

public:

	AudioSource(const char* name);
	void SetSoundEffect(const char* name);
	void SetVolume(int volume);

	void SetAudioListener(const glm::vec2* receiver, const float* receiverAngle);
	void SetPosition(const glm::vec2& source);
	
	void Play();
	void Stop();

	bool Playing();

private:

	Mix_Chunk* m_SoundEffect = nullptr;
	int m_CurrentChannel = -1;

	const glm::vec2* m_Receiver = nullptr;
	const float* m_ReceiverAngle = nullptr;

};

class Audio {

public:

	static void Init();
	static void Uninit();

	static void PlaySoundEffect(const char* name, int loops = 0);
	static void PlayMusic(const char* name, int loops = 0);

	static void SetSoundEffectVolume(const char* name, int volume);
	static void SetMusicVolume(int volume);

	static void PlaySoundEffectAtPosition(const char* name, const glm::vec2& receiver, float receiverAngle, const glm::vec2& source);
	static void PlaySoundEffectAtAngleDistance(const char* name, float angle, float distance);

	static void SetMaxDistance(int dist) { MaxDistance = dist; }

private:

	static inline std::unordered_map<std::string, Mix_Chunk*> SoundEffects;
	static inline std::unordered_map<std::string, Mix_Music*> Music;

	static inline const int NumChannels = MIX_CHANNELS;
	static inline int MaxDistance = 100;

};