#include "SoundManager.h"
#include "stdafx.h"

bool SoundEntity::ms_toggleSound = true;
unsigned int SoundEntity::ms_volume = 100;

SoundEntity::SoundEntity(float minimumDistance, float attenuation, bool looping)
:m_minDistance(minimumDistance)
,m_attenuation(attenuation)
,m_looping(looping)
{
    mp_buffer = nullptr;
}

SoundEntity::~SoundEntity()
{
    if(mp_buffer != nullptr)
    {
        delete mp_buffer;
        mp_buffer = nullptr;
    }
}

void SoundEntity::Init(std::string filename)
{
	mp_buffer = new sf::SoundBuffer();
	mp_buffer->loadFromFile(filename);
}
void SoundEntity::ToogleSound()
{
    ms_toggleSound = !ms_toggleSound;
}
bool SoundEntity::IsMuted()
{
    return !ms_toggleSound;
}
sf::Sound* SoundEntity::CreateSound(sf::Vector2f position)
{
    sf::Sound* sound = new sf::Sound();
    
    sound->setBuffer(*mp_buffer);
    sound->setPosition(position.x, 0, position.y);
    sound->setMinDistance(m_minDistance);
    sound->setAttenuation(m_attenuation);
    sound->setVolume(ms_volume);
    sound->setLoop(m_looping);
    return sound;
}
float SoundEntity::GetMinimumDistance()
{
    return m_minDistance;
}
void SoundEntity::SetMinimumDistance(float minimumDistance)
{
    m_minDistance = minimumDistance;
}
float SoundEntity::GetAttenuation()
{
    return m_attenuation;
}
void SoundEntity::SetAttenuation(float attenuation)
{
    m_attenuation = attenuation;
}
unsigned int SoundEntity::GetVolume()
{
    return ms_volume;
}
void SoundEntity::SetVolume(unsigned int volume)
{
    ms_volume = volume;
}

bool MusicEntity::ms_toggleSound = true;
unsigned int MusicEntity::ms_volume = 100;

MusicEntity::MusicEntity(sf::Music* song)
:mp_musicStream(song)
{
    mp_musicStream->setRelativeToListener(true);
    mp_musicStream->setPosition(0, 0, -1);
}
MusicEntity::~MusicEntity()
{
    if (mp_musicStream != nullptr)
    {
        delete mp_musicStream;
        mp_musicStream = nullptr;
    }
}

void MusicEntity::Play()
{
    if(!ms_toggleSound)
    {
        return;
    }
    mp_musicStream->play();
}
void MusicEntity::Stop()
{
    mp_musicStream->stop();
}
void MusicEntity::Pause()
{
    mp_musicStream->pause();
}

sf::Music* MusicEntity::GetMusic()
{
    return mp_musicStream;
}
void MusicEntity::SetVolume(float volume)
{
    ms_volume = volume;
}
float MusicEntity::GetVolume()
{
    return ms_volume;
}
void MusicEntity::ToggleSound()
{
    ms_toggleSound = !ms_toggleSound;
}
bool MusicEntity::IsMuted()
{
    return !ms_toggleSound;
}

SoundManager::SoundManager()
{
    m_listener.setPosition(0, 0, 0);
    Sounds.clear();
    Songs.clear();
}
SoundManager::~SoundManager()
{
    for (auto it = Sounds.begin(); it != Sounds.end(); it++)
    {
        if (it->second != nullptr)
        {
            delete it->second;
            it->second = nullptr;
        }
    }
    for (auto it = Songs.begin(); it != Songs.end(); it++)
    {
        if (it->second != nullptr)
        {
            delete it->second;
            it->second = nullptr;
        }
    }
    Sounds.clear();
    Songs.clear();
}

std::string SoundManager::newSound(std::string filename, bool Looping, float minimumDistance, float attenuation)
{
    if(Sounds.find(filename)!=Sounds.end())
    {
        return filename;
    }
	SoundEntity* newSoundEntity = new SoundEntity(minimumDistance, attenuation, Looping);
    newSoundEntity->Init(filename);
	Sounds.insert(std::make_pair(filename, newSoundEntity));
	return filename;
}

std::string SoundManager::newSong(std::string filename, bool Looping)
{
    if(Songs.find(filename)!=Songs.end())
    {
        return filename;
    }
	sf::Music* song = new sf::Music;
	song->openFromFile(filename);
    song->setLoop(Looping);
    song->setVolume(MusicEntity::GetVolume());
    MusicEntity* songEntity = new MusicEntity(song);
	Songs.insert(std::make_pair(filename, songEntity));
	return filename;
}

void SoundManager::Update(sf::Vector2f playerPosition)
{
    m_listener.setPosition(playerPosition.x, 0, playerPosition.y);
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

MusicEntity* SoundManager::GetSong(std::string filename)
{
    auto song = Songs.find(filename);
    if(song == Songs.end())
    {
        return nullptr;
    }
    return song->second;
}

unsigned long SoundManager::GetSoundCount()
{
    return Sounds.size();
}

unsigned long SoundManager::GetSongCount()
{
    return Songs.size();
}

void SoundManager::ToggleSound()
{
    SoundEntity::ToogleSound();
}

void SoundManager::ToggleMusic()
{
    MusicEntity::ToggleSound();
    for (auto it = Songs.begin(); it != Songs.end(); it++)
    {
        if(!MusicEntity::IsMuted())
        {
            it->second->Play();
        }
        else
        {
            it->second->Pause();
        }
    }
}

void SoundManager::SetSoundVolume(unsigned int volume)
{
    SoundEntity::SetVolume(volume);
    for (auto it = Sounds.begin(); it != Sounds.end(); it++)
    {
        it->second->SetVolume(volume);
    }
}
float SoundManager::GetSoundVolume()
{
    return SoundEntity::GetVolume();
}

void SoundManager::SetSongVolume(unsigned int volume)
{
    MusicEntity::SetVolume(volume);
    for (auto it = Songs.begin(); it != Songs.end(); it++)
    {
        it->second->SetVolume(volume);
    }
}
float SoundManager::GetSongVolume()
{
    return MusicEntity::GetVolume();
}