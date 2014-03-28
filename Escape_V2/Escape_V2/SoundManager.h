/*
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class SoundManager{
private:
	std::map<std::string, sf::SoundBuffer*> SoundMap;
public:
	SoundManager();
	sf::SoundBuffer* LoadSound(std::string Filename);
	sf::SoundBuffer* GetSound(std::string Filename);
	void Clear();
	~SoundManager();
};

*/

#pragma once
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "state.h"
#include <map>


class SoundEntity
{
public:
    SoundEntity(float minimumDistance, float attenuation, bool lopping);
    ~SoundEntity();
    
	void Init(std::string filename);
    static void ToogleSound();
    static bool IsMuted();
    sf::Sound* CreateSound(sf::Vector2f position);
    float GetMinimumDistance();
    void SetMinimumDistance(float minimumDistance);
    float GetAttenuation();
    void SetAttenuation(float attenuation);
    static unsigned int GetVolume();
    static void SetVolume(unsigned int volume);
    
private:
    sf::SoundBuffer* mp_buffer;
    static bool ms_toggleSound;
    static unsigned int ms_volume;
    float m_minDistance, m_attenuation;
    bool m_looping;
	
};

class MusicEntity
{
public:
    MusicEntity(sf::Music* song);
    ~MusicEntity();
    
    void Play();
    void Stop();
    void Pause();
    
    sf::Music* GetMusic();
    static void SetVolume(float volume);
    static float GetVolume();
    static void ToggleSound();
    static bool IsMuted();
    
private:
    sf::Music* mp_musicStream;
    static bool ms_toggleSound;
    static unsigned int ms_volume;
    
};

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();
    
    std::string newSound(std::string filename, bool Looping, float minimumDistance, float attenuation);
    std::string newSong(std::string filename, bool Looping);
    SoundEntity* GetSound(std::string filename);
    MusicEntity* GetSong(std::string filename);
    unsigned long GetSoundCount();
    unsigned long GetSongCount();
    
	void Update(sf::Vector2f playerPosition);
    void ToggleSound();
    void ToggleMusic();
    
    void SetSoundVolume(unsigned int volume);
    float GetSoundVolume();
    void SetSongVolume(unsigned volume);
    float GetSongVolume();
    
private:
    std::map <std::string, SoundEntity*> Sounds;
	std::map <std::string, MusicEntity*> Songs;
    
};
