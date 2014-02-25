// OptionsState.cpp

#include "OptionsState.h"

OptionsState::OptionsState() {
	m_nextState = "";
}

OptionsState::~OptionsState() {

}

void OptionsState::Enter() {
	m_nextState = "";
}

void OptionsState::Exit() {

}

bool OptionsState::Update() {
	return false;
}

void OptionsState::Draw() {

}

std::string OptionsState::Next() {
	return m_nextState;
}

bool OptionsState::IsType(const std::string &c_type) {
	return c_type.compare("OptionsState") == 0;
}

void OptionsState::Reset() {
	m_nextState = "";
}