// SpriteManager.h

#pragma once

#include "stdafx.h"

class AnimatedSprite;

class SpriteManager
{
public:
	

public:
	SpriteManager();
	~SpriteManager();

	bool Init(const std::string& c_directory);
	void Cleanup();

	AnimatedSprite* Load(const std::string &c_filename);
	bool LoadTexture(const std::string &c_filename);

private:
	std::string m_directory;
	std::map<std::string, sf::Texture*> m_textures;
};

