#include "Background.h"
#include "AnimatedSprite.h"
#include "Settings.h"

Background::Background(AnimatedSprite* sprite)
{
	mp_sprite = sprite;
	mp_sprite->Init("Mockup---Credits_2.png");
}


Background::~Background()
{
}

void Background::Draw() {
	Settings::ms_window->draw(*mp_sprite->getSprite());
}