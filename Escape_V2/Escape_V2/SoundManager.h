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
	sf::Time startingPoint;
	sf::Time endPoint;
	sf::Sound Sound;
	sf::SoundBuffer Buffer;
	void Loop();
	SoundEntity();
	void Play(bool stop);
	void Init(std::string filename);
	void Update();
	static bool toggleSound;
	
};

class SoundManager
{
public:
    std::string newSound(std::string filename, bool Looping);
    std::string newSong(std::string filename, bool Looping);
    SoundEntity* GetSound(std::string filename);
    sf::Music* GetSong(std::string filename);
    int GetSoundCount();
    int GetSongCount();
    
	void Update();
    void ToggleSound(bool active);
    void ToggleMusic(bool active);
    
    void SetSoundVolume(float volume);
    float GetSoundVolume();
    void SetSongVolume(float volume);
    float GetSongVolume();
private:
    std::map <std::string, SoundEntity*> Sounds;
	std::map <std::string, sf::Music*> Songs;
    
};
