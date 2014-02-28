// Guard.cpp

#include "Guard.h"
#include "GuardStateManager.h"
#include "GuardInvestigateState.h"
#include "GuardPatrolState.h"
#include "GuardSearchState.h"
#include "GuardShootingState.h"
#include "Settings.h"
#include "AnimatedSprite.h"
#include "stdafx.h"
#include "Grid2D.h"

Guard::Guard(int number, AnimatedSprite* p_sprite, Grid2D* p_grid) {
	m_number = number;

	mp_sprite = p_sprite;
	mp_sprite->Init("male_Guard_Walking.png");
	mp_sprite->getSprite()->setOrigin(mp_sprite->getSprite()->getLocalBounds().width / 2, mp_sprite->getSprite()->getLocalBounds().height / 2);

	m_position = Settings::ms_guards.at(number);
	
	mp_guardStateManager = new GuardStateManager();
	mp_guardStateManager->Attach(new GuardPatrolState());
	mp_guardStateManager->Attach(new GuardSearchState());
	mp_guardStateManager->Attach(new GuardShootingState());
	mp_guardStateManager->Attach(new GuardInvestigateState());
	mp_guardStateManager->SetState("GuardPatrolState");
	mp_guardStateManager->Init(m_number, &m_position, mp_sprite, p_grid);

	m_speed = 10;
}


Guard::~Guard() {
	if(mp_guardStateManager != nullptr) {
		delete mp_guardStateManager;
		mp_guardStateManager = nullptr;
	}
}

void Guard::Update(sf::Vector2f playerPosition, CollisionManager* p_collisionManager) {

	mp_guardStateManager->Update();
	/*sf::Vector2f distancev = m_position - playerPosition;
	float distance = sqrtf(distancev.x * distancev.x + distancev.y * distancev.y);
	std::string type = "GuardShootingState";
	if(distance < 250) {
		if(mp_guardStateManager->Detected(playerPosition, p_collisionManager) && !mp_guardStateManager->IsCurrent(type)) {
			mp_guardStateManager->SetState("GuardShootingState");
		}
	}
	else if(mp_guardStateManager->IsCurrent(type)) {
		mp_guardStateManager->SetState("GuardPatrolState");
	}*/

	UpdateAnimation();
}

void Guard::UpdateAnimation() {
	mp_sprite->getSprite()->setPosition(m_position);
	mp_guardStateManager->UpdateAnimation();
}

void Guard::Draw() {
	Settings::ms_window->draw(*mp_sprite->getSprite());
}

sf::Vector2f Guard::GetPosition() {
	return m_position;
}

void Guard::AddWaypointToFront(sf::Vector2f waypoint) {
	mp_guardStateManager->AddWaypointToFront(waypoint);
}

AnimatedSprite* Guard::GetSprite() {
	return mp_sprite;
}