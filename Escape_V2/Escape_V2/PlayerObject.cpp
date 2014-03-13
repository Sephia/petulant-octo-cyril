// PlayerObject.cpp

#include "PlayerObject.h"
#include "AnimatedSprite.h"
#include "Settings.h"
#include "stdafx.h"
#include "CollisionManager.h"
#include "FurnitureManager.h"
#include "Furniture.h"

PlayerObject::PlayerObject(AnimatedSprite* sprite) {
	mp_sprite = sprite;
	mp_sprite->Init("PlayerIdle.png");
	mp_sprite->getSprite()->setOrigin(mp_sprite->getSprite()->getLocalBounds().width / 2, mp_sprite->getSprite()->getLocalBounds().height / 2);
	m_position = Settings::ms_enter;
	m_sneak = true;
	m_speed = 100;
	m_direction.x = 0.0f;
	m_direction.y = 0.0f;
	m_rightFoot = true;
}

PlayerObject::~PlayerObject() {

}

void PlayerObject::SetPosition(sf::Vector2f position) {
	m_position = m_position;
}

sf::Vector2f PlayerObject::GetPosition() {
	return m_position;
}

int PlayerObject::Update(CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) {

	sf::Vector2f vec(0.0f, 0.0f);

	m_sneak = !Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LShift);

	if(Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::W)) {
		vec.y -= 1;	
	}

	if(Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::S)) {
		vec.y += 1;	
	}

	if(Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::A)) {
		vec.x -= 1;
	}

	if(Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::D)) {
		vec.x += 1;
	}

	SetDirection(vec);
	int noice = Movement(p_collisionManager, p_furnitureManager);
	UpdateSprite();
	return noice;
}

void PlayerObject::SetSneak(bool sneak) {
	m_sneak = sneak;
}

void PlayerObject::Draw() {
	Settings::ms_window->draw(*mp_sprite->getSprite());
}

void PlayerObject::SetDirection(sf::Vector2f direction) {
	m_direction = direction;
	NormalizeDirection();
}

int PlayerObject::Movement(CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) {
	int collisionFurnitureX = -1;
	int collisionFurnitureY = -1;
	if(m_direction.x > 0.001 || m_direction.y > 0.001 || m_direction.x < -0.001 || m_direction.y < -0.001) {
		sf::Sprite temp_sprite = *mp_sprite->getSprite();
		if(m_sneak) {
			sf::Vector2f tryPosition = m_position;
			tryPosition.x += m_direction.x * m_speed * Settings::ms_deltatime;
			temp_sprite.setPosition(tryPosition);
			if(!p_collisionManager->Circle_WallCollision(tryPosition, 46)) {
				for(int i = 0; i < p_furnitureManager->GetCount(); i++) {
					if(p_collisionManager->Circle_FurnitureCollision(temp_sprite, *p_furnitureManager->GetFurniture(i))) {
						collisionFurnitureX = 1;
						break;
					}
				}
				if(collisionFurnitureX == -1) {
					m_position.x += m_direction.x * m_speed * Settings::ms_deltatime;
				}			
			}
			tryPosition = m_position;
			tryPosition.y += m_direction.y * m_speed * Settings::ms_deltatime;
			temp_sprite.setPosition(tryPosition);
			if(!p_collisionManager->Circle_WallCollision(tryPosition, 46)) {
				for(int i = 0; i < p_furnitureManager->GetCount(); i++) {
					if(p_collisionManager->Circle_FurnitureCollision(temp_sprite, *p_furnitureManager->GetFurniture(i))) {
						collisionFurnitureX = 1;
						break;
					}
				}
				if(collisionFurnitureX == -1) {
					m_position.y += m_direction.y * m_speed * Settings::ms_deltatime;
				}
			}
		}
		else {
			sf::Vector2f tryPosition = m_position;
			tryPosition.x += m_direction.x * m_speed * 3.0f * Settings::ms_deltatime;
			temp_sprite.setPosition(tryPosition);
			if(!p_collisionManager->Circle_WallCollision(tryPosition, 46)) {
				for(int i = 0; i < p_furnitureManager->GetCount(); i++) {
					if(p_collisionManager->Circle_FurnitureCollision(temp_sprite, *p_furnitureManager->GetFurniture(i))) {
						collisionFurnitureY = 1;
						break;
					}
				}
				if(collisionFurnitureY == -1) {
					m_position.x += m_direction.x * m_speed * 3.0f* Settings::ms_deltatime;
				}
			}
			tryPosition = m_position;
			tryPosition.y += m_direction.y * m_speed * 3.0f * Settings::ms_deltatime;
			temp_sprite.setPosition(tryPosition);
			if(!p_collisionManager->Circle_WallCollision(tryPosition, 46)) {
				for(int i = 0; i < p_furnitureManager->GetCount(); i++) {
					if(p_collisionManager->Circle_FurnitureCollision(temp_sprite, *p_furnitureManager->GetFurniture(i))) {
						collisionFurnitureY = 1;
						break;					
					}
				}
				if(collisionFurnitureY == -1) {
					m_position.y += m_direction.y * m_speed * 3.0f * Settings::ms_deltatime;
				}
			}
		}
		mp_sprite->getSprite()->setRotation(atan2(m_direction.y, m_direction.x) * 180 / 3.141592f + 90);
	}
	if(collisionFurnitureX > collisionFurnitureY)
		return collisionFurnitureX;
	return collisionFurnitureY;
}

void PlayerObject::NormalizeDirection() {
	float distance = sqrtf(m_direction.x * m_direction.x + m_direction.y * m_direction.y);

	if(distance == 0) {
		distance = 0.0001f;
	}

	m_direction /= distance;

}

void PlayerObject::UpdateSprite() {

	if(Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LShift) && (Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::W) || Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::A) || Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::S) || Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::D))) {
		mp_sprite->ChangeAnimation("PlayerRunning.png");
	}
	else if(Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::W) || Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::A) || Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::S) || Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::D)) {
		mp_sprite->ChangeAnimation("PlayerSneaking.png");
	}
	else {
		mp_sprite->ChangeAnimation("PlayerIdle.png");
	}

	mp_sprite->Update();
	mp_sprite->getSprite()->setPosition(m_position);
}

bool PlayerObject::ChangeAnimation(const std::string& name) {
	return mp_sprite->ChangeAnimation(name);
}

sf::Sprite* PlayerObject::GetSprite() {
	return mp_sprite->getSprite();
}

bool PlayerObject::IsRunning() {
	if(!m_sneak && ( m_direction.x != 0 || m_direction.y != 0 )) {
		return true;
	}
	return false;
}

bool PlayerObject::IsRightFoot() {
	m_rightFoot = !m_rightFoot;
	return m_rightFoot;
}