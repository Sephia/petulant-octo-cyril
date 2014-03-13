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

#include "SoundManager.h"
#include "stdafx.h"

bool SoundEntity::toggleSound = true;


void SoundEntity::Loop()
{
	Sound.setPlayingOffset(startingPoint);
}

void SoundEntity::Play(bool stop)        // stops the sound, then starts it from the beginning of the loop, then starts it
{
	if (toggleSound){
		if (stop)
			Sound.stop();
		Sound.setPlayingOffset(startingPoint);
		Sound.play();
	}
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
	std::cout << "Hi\n";
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



std::string SoundManager::newSound(std::string filename, bool Looping)
{
    if(Sounds.find(filename)!=Sounds.end())
    {
        return filename;
    }
	SoundEntity* newSoundEntity = new SoundEntity;
    newSoundEntity->Init(filename);
    newSoundEntity->Sound.setLoop(Looping);
	Sounds.insert(std::make_pair(filename, newSoundEntity));
	return filename;
}

std::string SoundManager::newSong(std::string filename, bool Looping)
{
    if(Songs.find(filename)!=Songs.end())
    {
        return filename;
    }
	sf::Music* newSoundEntity = new sf::Music;
	newSoundEntity->openFromFile(filename);
    newSoundEntity->setLoop(Looping);
	Songs.insert(std::make_pair(filename, newSoundEntity));
	return filename;
}

void SoundManager::Update()
{
	for (auto i = Sounds.begin(); i != Sounds.end(); i++)
	{
		i->second->Update();
	}

}

SoundEntity* SoundManager::GetSound(std::string filename)
{
    auto sound = Sounds.find(filename);
    if(sound == Sounds.end())
    {
        return nullptr;
    }
    return sound->second;
}

sf::Music* SoundManager::GetSong(std::string filename)
{
    auto song = Songs.find(filename);
    if(song == Songs.end())
    {
        return nullptr;
    }
    return song->second;
}

int SoundManager::GetSoundCount()
{
    return Sounds.size();
}

int SoundManager::GetSongCount()
{
    return Songs.size();
}

void SoundManager::ToggleSound(bool active)
{
    SoundEntity::toggleSound = active;
}

void SoundManager::ToggleMusic(bool active)
{
    for (auto it = Songs.begin(); it != Songs.end(); it++)
    {
        if(active)
        {
            it->second->play();
        }
        else
        {
            it->second->pause();
        }
    }
}

void SoundManager::SetSoundVolume(float volume)
{
    for (auto it = Sounds.begin(); it != Sounds.end(); it++)
    {
        
    }
}
float SoundManager::GetSoundVolume()
{
    if(Sounds.size()!=0)
    {
        return Sounds.begin()->second->Sound.getVolume();
    }
    else
    {
        return 1;
    }
}

void SoundManager::SetSongVolume(float volume)
{
    for (auto it = Sounds.begin(); it != Sounds.end(); it++)
    {
        
    }
}
float SoundManager::GetSongVolume()
{
    if(Songs.size()!=0)
    {
        return Songs.begin()->second->getVolume();
    }
    else
    {
        return 1;
    }
}