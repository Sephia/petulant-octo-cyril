// Level.cpp

#include "Level.h"
#include "Settings.h"
#include "stdafx.h"

Level::Level(const std::string& c_path) {
	mp_levelTexture = new sf::Texture;
	mp_levelSprite = new sf::Sprite;

	mp_levelTexture->loadFromFile("../data/" + c_path + "level.png");
	mp_levelSprite->setTexture(*mp_levelTexture);
}

Level::~Level() {
}

void Level::Draw() {
	Settings::ms_window->draw(*mp_levelSprite);
}

Vec2f Level::GetSpriteSize() {
	return Vec2f(mp_levelSprite->getLocalBounds().width, mp_levelSprite->getLocalBounds().height);
}