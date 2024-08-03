#include "Audio.h"

static void ResetPositionalAudio(int channel) {
	Mix_SetPosition(channel, 0, 0);
}


void Audio::Init()
{
	assert(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0);

	for (int i = 0; i < NumChannels; i++)
		Mix_HaltChannel(i);

	Mix_ChannelFinished(&ResetPositionalAudio);
}

void Audio::Uninit()
{
	for (auto& [key, value] : Music)
		Mix_FreeMusic(value);

	for (auto& [key, value] : SoundEffects)
		Mix_FreeChunk(value);

	Music.clear();
	SoundEffects.clear();
}

void Audio::PlaySoundEffect(const char* name, int loops)
{
	if (SoundEffects.find(name) == SoundEffects.end())
		SoundEffects[name] = Mix_LoadWAV(name);

	Mix_PlayChannel(-1, SoundEffects[name], loops);

}

void Audio::PlayMusic(const char* name, int loops)
{
	if (Music.find(name) == Music.end())
		Music[name] = Mix_LoadMUS(name);

	if (Mix_PlayingMusic())
		Mix_HaltMusic();

	Mix_PlayMusic(Music[name], loops);

}

void Audio::SetSoundEffectVolume(const char* name, int volume)
{
	if (SoundEffects.find(name) == SoundEffects.end())
		SoundEffects[name] = Mix_LoadWAV(name);

	Mix_VolumeChunk(SoundEffects[name], volume);
}

void Audio::SetMusicVolume(int volume)
{
	Mix_VolumeMusic(volume);
}

static float AngleBetween(const glm::vec2& first, const glm::vec2& second) {

	//normal equality causing issues
	if (glm::epsilonEqual(first.x, second.x, 0.001f) && glm::epsilonEqual(first.y, second.y, 0.001f))
		return 0;

	if (glm::dot(first, second) == 0.0f)
		return 90.0f;

	return glm::acos(glm::dot(first, second) / (glm::length(first) * glm::length(second)));
}


void Audio::PlaySoundEffectAtPosition(const char* name, const glm::vec2& receiver, float receiverAngle, const glm::vec2& source)
{
	float distance = glm::distance(receiver, source);
	if (distance > MaxDistance)
		distance = MaxDistance;

	glm::vec2 srcDir = source - receiver;
	glm::vec2 receiverDir = { glm::sin(glm::radians(receiverAngle)), glm::cos(glm::radians(receiverAngle)) };
	
	float angle = AngleBetween(srcDir, receiverDir);

	PlaySoundEffectAtAngleDistance(name, angle, distance);
}

void Audio::PlaySoundEffectAtAngleDistance(const char* name, float angle, float distance)
{

	if (SoundEffects.find(name) == SoundEffects.end())
		SoundEffects[name] = Mix_LoadWAV(name);

	uint8_t scaled = (uint8_t)((distance / 100.0f) * 255.0f);
	
	int channel = Mix_PlayChannel(-1, SoundEffects[name], 0);
	Mix_SetPosition(channel, angle, scaled);

}

AudioSource::AudioSource(const char* name)
	: m_SoundEffect(Mix_LoadWAV(name))
{

}

void AudioSource::SetSoundEffect(const char* name)
{
	Mix_FreeChunk(m_SoundEffect);
	m_SoundEffect = Mix_LoadWAV(name);
}

void AudioSource::SetVolume(int volume)
{
	Mix_VolumeChunk(m_SoundEffect, volume);
}


void AudioSource::Play()
{
	m_CurrentChannel = Mix_PlayChannel(-1, m_SoundEffect, 0);
}

void AudioSource::Stop()
{
	Mix_HaltChannel(m_CurrentChannel);
}

bool AudioSource::Playing()
{
	if (m_CurrentChannel == -1)
		return false;

	return Mix_Playing(m_CurrentChannel);
}

void AudioSource::SetAudioListener(const glm::vec2* receiver, const float* receiverAngle)
{
	m_Receiver = receiver;
	m_ReceiverAngle = receiverAngle;
}

void AudioSource::SetPosition(const glm::vec2& source)
{
	if (m_Receiver == nullptr || m_ReceiverAngle == nullptr)
		return;

	float distance = glm::distance(*m_Receiver, source);
	uint8_t scaled = (uint8_t)((distance / 100.0f) * 255.0f);

	glm::vec2 srcDir = source - *m_Receiver;
	glm::vec2 receiverDir = { glm::cos(glm::radians(*m_ReceiverAngle)), glm::sin(glm::radians(*m_ReceiverAngle)) };

	float angle = AngleBetween(srcDir, receiverDir);

	Mix_SetPosition(m_CurrentChannel, angle, scaled);

}

