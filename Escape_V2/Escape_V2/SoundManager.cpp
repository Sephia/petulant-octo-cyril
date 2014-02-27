/*
#include "SoundManager.h"

SoundManager::SoundManager(){
}

sf::SoundBuffer* SoundManager::LoadSound(std::string Filename){
	if (SoundMap.find(Filename) == SoundMap.end()){
		sf::SoundBuffer *Load = new sf::SoundBuffer;
		Load->loadFromFile(Filename);
		SoundMap.insert(std::pair<std::string, sf::SoundBuffer*>(Filename, Load));
		return Load;
	}
	else
		return SoundMap.find(Filename)->second;
}
sf::SoundBuffer* SoundManager::GetSound(std::string Filename)
{
	if (SoundMap.find(Filename) == SoundMap.end())
	{
		return nullptr;
	}
	else
	{
		return SoundMap.find(Filename)->second;
	}
}

void SoundManager::Clear(){
	for (std::map<std::string, sf::SoundBuffer*>::iterator i = SoundMap.begin(); i != SoundMap.end(); i = SoundMap.begin()){
		delete i->second;
		SoundMap.erase(i);
	}

	SoundMap.clear();

}

SoundManager::~SoundManager(){
	for (std::map<std::string, sf::SoundBuffer*>::iterator i = SoundMap.begin(); i != SoundMap.end(); i = SoundMap.begin()){
		delete i->second;
		SoundMap.erase(i);
	}

}
*/

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "SoundManager.h"


void SoundEntity::Loop()
{
	Sound.setPlayingOffset(startingPoint);
}

void SoundEntity::Play(bool stop = true)        // stops the sound, then starts it from the beginning of the loop, then starts it
{
	if (stop)
		Sound.stop();
	Sound.setPlayingOffset(startingPoint);
	Sound.play();
}

void SoundEntity::Init(std::string filename)
{
	//Buffer = new sf::SoundBuffer;
	Buffer.loadFromFile(filename);
	//Sound = new sf::Sound;
	Sound.setBuffer(Buffer);
	startingPoint = sf::microseconds(0);
	endPoint = Buffer.getDuration();
}

SoundEntity::SoundEntity()
{
	startingPoint = sf::seconds(0);
	endPoint = sf::seconds(0);
}

void SoundEntity::Update()
{
	if (Sound.getPlayingOffset() >= endPoint && Sound.getLoop())
	{
		Loop();
	}
	else if (Sound.getPlayingOffset() >= endPoint)
	{
		Sound.stop();
	}
}

int SoundManager::newSound(std::string filename, bool Looping)
{
	SoundEntity newSoundEntity;
	Sounds.push_back(newSoundEntity);
	Sounds[Sounds.size() - 1].Init(filename);
	Sounds[Sounds.size() - 1].Sound.setLoop(Looping);
	return Sounds.size() - 1;
}

void SoundManager::Update()
{
	for (int i = 0; i < Sounds.size(); i++)
	{
		Sounds[i].Update();
	}

}
