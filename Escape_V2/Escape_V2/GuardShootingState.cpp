// GuardShootingState.cpp

#include "GuardShootingState.h"
#include "AnimatedSprite.h"
#include "CollisionManager.h"
#include "Settings.h"

GuardShootingState::GuardShootingState() {
	m_nextState = "GuardSearchState";
}

GuardShootingState::~GuardShootingState() {

}

void GuardShootingState::Enter() {
	std::cout << "Entering GuardShootingState\n";
	m_done = false;
	m_timer = 0.0f;
}

void GuardShootingState::Exit() {

}

void GuardShootingState::Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite) {
	m_waypoints.clear();

	mp_position = p_position;
	
	m_done = false;
	m_nextState = "";

	mp_sprite = sprite;
}

bool GuardShootingState::Update() {
	
	return m_done;
}

std::string GuardShootingState::Next() {
	return m_nextState;
}

bool GuardShootingState::IsType(const std::string &c_type) {
	return c_type.compare("GuardShootingState") == 0;
}


void GuardShootingState::Cleanup() {

}

void GuardShootingState::UpdateAnimation() {
	mp_sprite->Update();
}

void GuardShootingState::AddWaypointToFront(sf::Vector2f waypoint) {
	
}

bool GuardShootingState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager) {
	sf::Vector2f direction(sf::Vector2f(fabs(mp_position->x - playerPosition.x), fabs(mp_position->y - playerPosition.y)));
	float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);
	while(distance > 20) {
		if(p_collisionManager->Circle_WallCollision(*mp_position, 46)) {
			return false;
		}
		if(direction.x > 11) {
			direction.x -= 5;
		}
		else if(direction.x < -11) {
			direction.x += 5;
		}

		if(direction.y > -11) {
			direction.y -= 5;
		}
		else if(direction.y < -15) {
			direction.y += 5;
		}
		distance = sqrtf(direction.x * direction.x + direction.y * direction.y);
	}
	m_timer += Settings::ms_deltatime;
	if(m_timer > 30) {
		Settings::ms_gameOver = true;
	}
	return false;
}