// AnimatedSprite.h

#pragma once

#include "stdafx.h"

class AnimatedSprite {
public:
	struct Frame{
		float duration;
		sf::IntRect iRect;
	};

public:
	AnimatedSprite();
	~AnimatedSprite();

	void Update(sf::Vector2f iRect);
	void Update();
	void AddAnimation(const std::string& name, std::vector<Frame> frame, sf::Sprite* psprites);
	bool Init(const std::string& name);
	bool ChangeAnimation(const std::string& name);

	sf::Sprite* getSprite();

private:
	
	sf::Sprite* mp_currentSprite;
	std::vector<Frame> m_currentAnimation;
	int m_currentFrame;
	float m_time;
	Frame* mp_frame;

	std::map<std::string, sf::Sprite*> m_allSprites;
	std::map<std::string, std::vector<Frame>> m_allFrames;
};

