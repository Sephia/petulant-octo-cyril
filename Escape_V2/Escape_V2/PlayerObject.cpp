// PlayerObject.cpp

#include "PlayerObject.h"
#include "AnimatedSprite.h"
#include "Settings.h"
#include "stdafx.h"

PlayerObject::PlayerObject(AnimatedSprite* sprite) {
	mp_sprite = sprite;
	mp_sprite->Init("PlayerIdle.png");
	mp_sprite->getSprite()->setOrigin(mp_sprite->getSprite()->getLocalBounds().width / 2, mp_sprite->getSprite()->getLocalBounds().height / 2);
	m_position = Settings::ms_enter;
	m_sneak = true;
	m_speed = 100;
	m_direction.x = 0.0f;
	m_direction.y = 0.0f;
}

PlayerObject::~PlayerObject() {

}

void PlayerObject::SetPosition(sf::Vector2f position) {
	m_position = m_position;
}

sf::Vector2f PlayerObject::GetPosition() {
	return m_position;
}

void PlayerObject::Update() {

	sf::Vector2f vec(0.0f, 0.0f);

	/*if(Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::W && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::A))) {
		SetDirection(sf::Vector2f(-1, -1));
	}
	else if(Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::W && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::D))) {
		SetDirection(sf::Vector2f(1, -1));	
	}
	else if(Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::S && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::D))) {
		SetDirection(sf::Vector2f(1, 1));	
	}
	else if(Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::S && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::A))) {
		SetDirection(sf::Vector2f(-1, 1));	
	}*/
	
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
	Movement();
	UpdateSprite();
	
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

void PlayerObject::Movement() {
	if(m_direction.x > 0.001 || m_direction.y > 0.001 || m_direction.x < -0.001 || m_direction.y < -0.001) {
		if(m_sneak) {
			m_position = m_position + m_direction * m_speed * Settings::ms_deltatime;
		}
		else {
			m_position = m_position + m_direction * m_speed * 3.0f * Settings::ms_deltatime;
		}
		mp_sprite->getSprite()->setRotation(atan2(m_direction.y, m_direction.x) * 180 / 3.141592f + 90);
	}
}

void PlayerObject::NormalizeDirection() {
	float distance = sqrtf(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
	if(distance < 0) {
		distance *= -1;
	}
	else if(distance == 0) {
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

bool PlayerObject::CollisionDetected(int tries) {
	if(tries == 0) {
		m_direction.x *= -1;
		Movement();
		m_direction.x *= -1;
		return false;
	}
	else if(tries == 1) {
		m_direction.y *= -1;
		Movement();
		m_direction.y *= -1;
		return false;
	}
	else if(tries == 2) {
		m_direction.x *= -1;
		m_direction.y *= -1;
		Movement();
		m_direction.x *= -1;
		m_direction.y *= -1;
		return false;
	}
	else {
		return true;
	}
}

bool PlayerObject::ChangeAnimation(const std::string& name) {
	return mp_sprite->ChangeAnimation(name);
}

sf::Sprite* PlayerObject::GetSprite() {
	return mp_sprite->getSprite();
}