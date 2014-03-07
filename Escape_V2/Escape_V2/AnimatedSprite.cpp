// AnimatedSprite.h

#include "AnimatedSprite.h"
#include "Settings.h"
#include "stdafx.h"

AnimatedSprite::AnimatedSprite() {
	m_time = 0.0f;
	m_currentFrame = 0;
	m_currentAnimationName = "";
}


AnimatedSprite::~AnimatedSprite() {
	if(mp_currentSprite != nullptr) {
		delete mp_currentSprite;
		mp_currentSprite = nullptr;
	}
	
	if(mp_frame != nullptr) {
		mp_frame = nullptr;
	}
	m_currentAnimation.clear();
}

void AnimatedSprite::AddAnimation(const std::string& name, std::vector<Frame> frame, sf::Sprite* p_sprites) {
	m_allFrames.insert(std::make_pair(name, frame));
	m_allSprites.insert(std::make_pair(name, p_sprites));
}

void AnimatedSprite::Update(sf::Vector2f position) {
	m_time += Settings::ms_deltatime;
	if(m_time >= m_currentAnimation[m_currentFrame].duration) {
		m_time = 0.0f;
		m_currentFrame = ++m_currentFrame % m_currentAnimation.size();
		mp_frame = &m_currentAnimation[m_currentFrame];
		mp_currentSprite->setTextureRect(mp_frame->iRect);
	}
	mp_currentSprite->setPosition(position);
}

void AnimatedSprite::Update() {
	m_time += Settings::ms_deltatime;
	if(m_time >= m_currentAnimation[m_currentFrame].duration) {
		m_time = 0.0f;
		m_currentFrame = ++m_currentFrame % m_currentAnimation.size();
		mp_frame = &m_currentAnimation[m_currentFrame];
		mp_currentSprite->setTextureRect(mp_frame->iRect);
	}
}

bool AnimatedSprite::Init(const std::string& name) {
	if(m_allFrames.size() > 0) {
		auto it = m_allFrames.find(name);
		if(it != m_allFrames.end()) {
			m_currentAnimation = it->second;
		}
		else {
			return false;
		}

		auto it2 = m_allSprites.find(name);
		if(it2 != m_allSprites.end()) {
			mp_currentSprite = it2->second;
		}
		else {
			return false;
		}

		mp_frame = &m_currentAnimation[0];
		mp_currentSprite->setTextureRect(mp_frame->iRect);
		m_currentAnimationName = name;
		return true;
	}

	return false;
}

bool AnimatedSprite::ChangeAnimation(const std::string& name) {

	if(m_currentAnimationName == name) {
		return false;
	}

	std::cout << name << std::endl;

	auto itFrame = m_allFrames.find(name);
	auto itSprite = m_allSprites.find(name);
	
	if(itFrame == m_allFrames.end() || itSprite == m_allSprites.end()) {
		return false;
	}

	m_currentAnimation = itFrame->second;
	mp_currentSprite = itSprite->second;
	m_currentFrame = 0;
	mp_frame = &m_currentAnimation[0];
	mp_currentSprite->setTextureRect(mp_frame->iRect);
	mp_currentSprite->setOrigin(mp_currentSprite->getLocalBounds().width / 2, mp_currentSprite->getLocalBounds().height / 2);

	m_currentAnimationName = name;
	
	return true;
}

sf::Sprite* AnimatedSprite::getSprite() {
	return mp_currentSprite;
}