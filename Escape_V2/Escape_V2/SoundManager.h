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
#include <SFML\Audio.hpp>
#include <SFML\System.hpp>

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
};

class SoundManager
{
public:
	std::vector<SoundEntity> Sounds;
	int newSound(std::string filename, bool Looping);
	void Update();
};
