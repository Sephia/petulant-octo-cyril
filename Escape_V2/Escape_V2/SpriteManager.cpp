// SpriteManager.cpp

#include "SpriteManager.h"
#include "AnimatedSprite.h"
#include "stdafx.h"

SpriteManager::SpriteManager() {

}


SpriteManager::~SpriteManager() {

}

bool SpriteManager::Init(const std::string& c_directory) {
	m_directory = c_directory;
	return true;
}

void SpriteManager::Cleanup() {
	auto it = m_textures.begin();
	while(it != m_textures.end()) {
		delete it->second;
		it++;
	}
	m_textures.clear();
}

AnimatedSprite* SpriteManager::Load(const std::string& c_filename) {
	std::ifstream stream;
	stream.open(m_directory + c_filename);
	if(!stream.is_open()) {
		return nullptr;
	}
	std::string name;
	std::string row;
	std::getline(stream, row);
	name = row;
	auto it = m_textures.find(row);
	if(it == m_textures.end()) {
		if(!LoadTexture(row)) {
			return nullptr;
		}
		it = m_textures.find(row);
	}
	sf::Texture* texture = it->second;
	texture->setSmooth(true);
	AnimatedSprite* animatedSprite = new AnimatedSprite();

	AnimatedSprite::Frame frame;
	std::vector<AnimatedSprite::Frame> frames;
	while(!stream.eof()) {
		std::getline(stream, row);
		if(row.length() == 0) {
			animatedSprite->AddAnimation(name, frames, new sf::Sprite(*texture));
			std::getline(stream, row);
			name = row;
			frames.clear();
			auto it = m_textures.find(row);
			if(it == m_textures.end()) {
				if(!LoadTexture(row)) {
					return nullptr;
				}
				it = m_textures.find(row);
			}
			texture = it->second;
			continue;
		}

		std::stringstream sstream(row);

		sstream >> frame.duration;
		sstream >> frame.iRect.left;
		sstream >> frame.iRect.top;
		sstream >> frame.iRect.width;
		sstream >> frame.iRect.height;

		frames.push_back(frame);
	}

	animatedSprite->AddAnimation(name, frames, new sf::Sprite(*texture));

	stream.close();

	return animatedSprite;
}

bool SpriteManager::LoadTexture(const std::string& c_filename) {
	std::string path = m_directory + c_filename;
	sf::Texture* texture = new sf::Texture();
	if(!texture->loadFromFile(path)) {
		return false;
	}

	m_textures.insert(std::pair<std::string, sf::Texture*>(c_filename, texture));

	return true;
}