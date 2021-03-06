// GuardStateManager.cpp

#include "GuardStateManager.h"
#include "GuardState.h"
#include "GuardInvestigateState.h"
#include "GuardPatrolState.h"
#include "GuardSearchState.h"
#include "GuardShootingState.h"


GuardStateManager::GuardStateManager() {
	mp_currentState = nullptr;
}

GuardStateManager::~GuardStateManager() {
	auto it = m_states.begin();
	while(it != m_states.end()) {
		delete(*it);
		//s�tt dem som nullptr ocks�, kan vara anv�ndbart vid senare tillf�llen (inte just denna)
		++it;
	}
	//beh�vs inte, m_states f�rsvinner ur scopen och rensas efter destruktorn �r klar
	m_states.clear();
	mp_currentState = nullptr;
}

void GuardStateManager::Attach(GuardState *p_state) {
	m_states.push_back(p_state);
}

bool GuardStateManager::Update() {
	if(mp_currentState == nullptr) {
		return true;
	}

	if(mp_currentState->Update()) {
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

void GuardStateManager::Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite) {
	for(unsigned int i = 0; i < m_states.size(); i++) {
        m_states.at(i)->Init(number, p_position, sprite);
	}
}

void GuardStateManager::Cleanup() {
	for(unsigned int i = 0; i < m_states.size(); i++) {
        m_states.at(i)->Cleanup();
	}
}

void GuardStateManager::UpdateAnimation() {
	mp_currentState->UpdateAnimation();
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