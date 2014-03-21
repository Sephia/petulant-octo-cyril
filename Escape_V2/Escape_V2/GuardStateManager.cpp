// GuardStateManager.cpp

#include "GuardStateManager.h"
#include "GuardState.h"
#include "GuardInvestigateState.h"
#include "GuardPatrolState.h"
#include "GuardSearchState.h"
#include "GuardShootingState.h"
#include "Grid2D.h"
#include "stdafx.h"


GuardStateManager::GuardStateManager() {
	mp_currentState = nullptr;
}

GuardStateManager::~GuardStateManager() {
	auto it = m_states.begin();
	while(it != m_states.end()) {
		delete(*it);
		//sätt dem som nullptr också, kan vara användbart vid senare tillfällen (inte just denna)
		++it;
	}
	//behövs inte, m_states försvinner ur scopen och rensas efter destruktorn är klar
	m_states.clear();
	mp_currentState = nullptr;
}

void GuardStateManager::Attach(GuardState *p_state) {
	m_states.push_back(p_state);
}

bool GuardStateManager::Update(sf::Vector2f player_position, CollisionManager* p_collisionManager) {
	if(mp_currentState == nullptr) {
		return true;
	}

	if(mp_currentState->Update(player_position, p_collisionManager)) {
		ChangeState();
		return false;
	}
	return true;
}

void GuardStateManager::SetState(const std::string &c_type) {
	for(unsigned int i = 0; i < m_states.size(); i++) {
		if(m_states[i]->IsType(c_type)) {
			mp_currentState = m_states[i];
			mp_currentState->Enter();
			return;
		}
	}
}

void GuardStateManager::ChangeState() {
	std::string next = mp_currentState->Next();

	if(mp_currentState != nullptr) {
		mp_currentState->Exit();
		mp_currentState = nullptr;
	}

	for(unsigned int i = 0; i < m_states.size(); i++) {
		if(m_states[i]->IsType(next)) {
			mp_currentState = m_states[i];
			mp_currentState->Enter();
			return;
		}
	}
}

void GuardStateManager::Init(int number, sf::Vector2f* p_position, float* p_rotation, AnimatedSprite* sprite, Grid2D* p_grid) {
	for(unsigned int i = 0; i < m_states.size(); i++) {
        m_states.at(i)->Init(number, p_position, p_rotation, sprite, p_grid);
	}
}

void GuardStateManager::Cleanup() {
	for(unsigned int i = 0; i < m_states.size(); i++) {
        m_states.at(i)->Cleanup();
	}
}

void GuardStateManager::UpdateAnimation(sf::Vector2f playerPosition) {
	mp_currentState->UpdateAnimation(playerPosition);
}

void GuardStateManager::AddWaypointToFront(sf::Vector2f waypoint) {
	mp_currentState->AddWaypointToFront(waypoint);
}

bool GuardStateManager::Detected(sf::Vector2f player_position, CollisionManager* p_collisionManager) {
	return mp_currentState->Detected(player_position, p_collisionManager);
}

bool GuardStateManager::IsCurrent(std::string& type) {
	return mp_currentState->IsType(type);
}