// Guard.cpp

#include "Guard.h"
#include "GuardStateManager.h"
#include "GuardInvestigateState.h"
#include "GuardPatrolState.h"
#include "GuardSearchState.h"
#include "GuardShootingState.h"
#include "GuardChaseState.h"
#include "Settings.h"
#include "AnimatedSprite.h"
#include "stdafx.h"
#include "Grid2D.h"

Guard::Guard(int number, AnimatedSprite* p_sprite, Grid2D* p_grid) {
	m_number = number;

	mp_sprite = p_sprite;
	mp_sprite->Init("Guard1Walking.png");
	mp_sprite->getSprite()->setOrigin(mp_sprite->getSprite()->getLocalBounds().width / 2, mp_sprite->getSprite()->getLocalBounds().height / 2);

	m_position = Settings::ms_guards.at(number);
	m_prevPosition = m_position;
	
	mp_guardStateManager = new GuardStateManager();
	mp_guardStateManager->Attach(new GuardPatrolState());
	mp_guardStateManager->Attach(new GuardSearchState());
	mp_guardStateManager->Attach(new GuardShootingState());
	mp_guardStateManager->Attach(new GuardInvestigateState());
	mp_guardStateManager->Attach(new GuardChaseState());
	mp_guardStateManager->SetState("GuardPatrolState");
	mp_guardStateManager->Init(m_number, &m_position, &m_rotation, mp_sprite, p_grid);

	m_speed = 15;

	m_foot = 0;
}

Guard::~Guard() {
	if(mp_guardStateManager != nullptr) {
		delete mp_guardStateManager;
		mp_guardStateManager = nullptr;
	}
}

void Guard::Update(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) {
	m_prevPosition = m_position;

	mp_guardStateManager->Update(playerPosition, p_collisionManager, p_furnitureManager);

	UpdateAnimation(playerPosition);
}

void Guard::UpdateAnimation(sf::Vector2f playerPosition) {
	mp_sprite->getSprite()->setPosition(m_position);
	mp_guardStateManager->UpdateAnimation(playerPosition);
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

bool Guard::IsWalking() {
	if(m_position == m_prevPosition) {
		return false;
	}

	return true;
}

int Guard::GetFoot() {
	m_foot++;
	m_foot %= 2;
	return m_foot;
}