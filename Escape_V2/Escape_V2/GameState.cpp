// StartMenuState.cpp

#include "GameState.h"
#include "PlayerObject.h"
#include "Settings.h"
#include "Guard.h"
#include "LightManager.h"
#include "WallManager.h"
#include "CollisionManager.h"
#include "HullManager.h"
#include "stdafx.h"
#include "AnimatedSprite.h"
#include "SpriteManager.h"

GameState::GameState() {
	m_nextState = "";
	mp_player = nullptr;
	m_timer = 0.0f;
}

GameState::~GameState() {
	for(unsigned int i = 0; i < m_guards.size(); i++) {
		if(m_guards.at(i) != nullptr) {
			delete m_guards.at(i);
			m_guards.at(i) = nullptr;
		}
	}
}

void GameState::Enter() {
	m_nextState = "";

	m_spriteManager.Init("../data/");
	
	mp_player = new PlayerObject(m_spriteManager.Load("male.txt"));

	for(unsigned int i = 0; i < Settings::ms_guards.size(); i++) {
		m_guards.push_back(new Guard(i, m_spriteManager.Load("maleGuardWalking.txt")));
	}

	mp_view = new sf::View();

	mp_view->setCenter(mp_player->GetPosition());
	mp_view->setSize(Settings::ms_window->getSize().x, Settings::ms_window->getSize().y);

	
	ls = new ltbl::LightSystem(AABB(Vec2f(0, 0), Vec2f(static_cast<float>(3657), static_cast<float>(5651))), Settings::ms_window, "../data/lightFin.png", "../data/shaders/lightAttenuationShader.frag");
    ls->SetView(*mp_view);

	/*ls2 = new ltbl::LightSystem(AABB(Vec2f(0, 0), Vec2f(static_cast<float>(3657), static_cast<float>(5651))), Settings::ms_window, "../data/lightFin.png", "../data/shaders/lightAttenuationShader.frag");
    ls2->SetView(*mp_view);*/
    
    lm = new LightManager(ls);
    hl = new HullManager(ls);
    wl = new WallManager(hl);
    cl = new CollisionManager(wl);

	/*lm2 = new LightManager(ls2);
    hl2 = new HullManager(ls2);
    wl2 = new WallManager(hl2);
    cl2 = new CollisionManager(wl2);*/

	ltbl::Light_Point* testLight = new ltbl::Light_Point();
	testLight->m_intensity = 1.0f;
	testLight->m_center = Vec2f(200.0f, 200.0f);
	testLight->m_radius = 800.0f;
	testLight->m_size = 1.0f;
	testLight->m_color.b = 0.8f;
	testLight->m_color.r = 0.8f;
	testLight->m_color.g = 0.8f;
	testLight->m_spreadAngle = ltbl::pifTimes2;
	testLight->m_softSpreadAngle = 0.0f;
	testLight->m_bleed = 0.0f;
	testLight->m_linearizeFactor = 0.8f;
	testLight->CalculateAABB();
    
	lm->AddLight(testLight, mp_player);
	testLight->SetAlwaysUpdate(true);

	/*for( int i = 0; i < m_guards.size(); i++) {
		ltbl::Light_Point* guardLight = new ltbl::Light_Point();
		guardLight->m_intensity = 1.5f;
		guardLight->m_center = Vec2f(100.0f, 100.0f);
		guardLight->m_radius = 800.0f;
		guardLight->m_size = 48.0f;
		guardLight->m_color.b = 1.0f;
		guardLight->m_color.r = 1.0f;
		guardLight->m_color.g = 1.0f;
		guardLight->m_spreadAngle = 1.0f;
		guardLight->m_softSpreadAngle = 0.0f;
		guardLight->m_bleed = 0.3f;

		guardLight->CalculateAABB();
	
    
		lm2->AddLight(guardLight, m_guards.at(i));
		guardLight->SetAlwaysUpdate(true);
	}*/

	
	ltbl::Light_Point* testLight2 = new ltbl::Light_Point();
	testLight2->m_center = Vec2f(Settings::ms_enter.x, -4150);
	testLight2->m_radius = 1000.0f;
	testLight2->m_size = 10.0f;
	testLight2->m_color.r = 5.0f;
    testLight2->m_color.g = 5.0f;
    testLight2->m_color.b = 5.0f;
	testLight2->m_intensity = 1.0f;
	//testLight2->m_bleed = 0.0f;
	testLight2->m_spreadAngle = ltbl::pifTimes2;
	testLight2->m_softSpreadAngle = 0.0f;
	testLight2->CalculateAABB();
    
	lm->AddLight(testLight2, lm);
	
	testLight2->SetAlwaysUpdate(false);

	/*testLight3 = new ltbl::Light_Point();
	testLight3->m_center = Vec2f(Settings::ms_enter.x, -4000);
	testLight3->m_radius = 1000.0f;
	testLight3->m_size = 10.0f;
	testLight3->m_color.r = 5.0f;
    testLight3->m_color.g = 5.0f;
    testLight3->m_color.b = 5.0f;
	testLight3->m_intensity = 1.0f;
	testLight3->m_bleed = 0.0f;
	testLight3->m_spreadAngle = ltbl::pifTimes2;
	testLight3->m_softSpreadAngle = 0.0f;
	testLight3->CalculateAABB();
    
	lm2->AddLight(testLight3, lm2);

	testLight3->SetAlwaysUpdate(false);*/
	
	if(!wl->LoadFromFile("../data/EnterLevelNameSquareWalls.txt")) {
		abort();
	}

	/*if(!wl2->LoadFromFile("../data/EnterLevelNameSquareWalls.txt")) {
		abort();
	}*/

	if (!m_music.openFromFile("../data/I Knew a Guy - Stealth.wav")) {
		std::cout << "Failed loading music for GameState!\n";
	}

	m_music.play();
}

void GameState::Exit() {
	m_spriteManager.Cleanup();
	m_music.stop();
}

bool GameState::Update() {
	m_timer += Settings::ms_deltatime;

	if(UpdateEvents()) {
		return false;
	}

	mp_player->Update();
	int tries = 0;
	while(cl->Circle_WallCollision(mp_player->GetPosition(), 46)) {
		if(mp_player->CollisionDetected(tries)) {
			break;
		}
		tries++;
	}

	for(int i = 0; i < m_guards.size(); i++) {
		m_guards.at(i)->Update(mp_player->GetPosition(), cl);
		if(m_timer > 0.5f) {
			m_soundRippleManager.CreateSoundRipple(m_guards.at(i)->GetPosition(), 2, false, m_spriteManager.Load("ripple.txt"));
			
		}

		/*Vec2f vecG(m_guards.at(i)->GetPosition().x, -m_guards.at(i)->GetPosition().y + mp_view->getSize().y);
		float angle = (m_guards.at(i)->GetSprite()->getSprite()->getRotation() - 90) * (3.141592 / 180);
		lm2->GetLight(m_guards.at(i))->m_directionAngle = -angle;
		lm2->GetLight(m_guards.at(i))->SetCenter(vecG);
		lm2->GetLight(m_guards.at(i))->CalculateAABB();*/
	}

	if(m_timer > 0.5f) {
		m_timer = 0.0f;
	}

	m_soundRippleManager.UpdateSounds();

	Vec2f vec(mp_player->GetPosition().x, -mp_view->getCenter().y + mp_view->getSize().y);
	//lm->GetLight(mp_player)->CalculateAABB();
	lm->GetLight(mp_player)->SetCenter(vec);

	

	if(Settings::ms_gameOver || 
		(	mp_player->GetPosition().x > Settings::ms_exit.x && mp_player->GetPosition().x < Settings::ms_exit.x + 100 &&
			mp_player->GetPosition().y > Settings::ms_exit.y && mp_player->GetPosition().y < Settings::ms_exit.y + 50
		)) {
		m_nextState = "CreditState";
		return false;
	}
	return true;
}

void GameState::Draw() {
	Settings::ms_window->clear(sf::Color(0, 0, 0, 255));
	m_level.Draw();
	mp_player->Draw();
	for(unsigned int i = 0; i < m_guards.size(); i++) {
		m_guards.at(i)->Draw();
	}
	
	mp_view->setCenter(mp_player->GetPosition());
	Settings::ms_window->setView(*mp_view);

	ls->SetView(*mp_view);
	//ls2->SetView(*mp_view);

	/*ls2->RenderLights();
	ls2->RenderLightTexture();*/

	ls->RenderLights();
	ls->RenderLightTexture();

	
	


	

	m_soundRippleManager.Draw();
	Settings::ms_window->display();
}

std::string GameState::Next() {
	return m_nextState;
}

bool GameState::IsType(const std::string &c_type) {
	return c_type.compare("GameState") == 0;
}

void GameState::Reset() {
	m_nextState = "";
}

bool GameState::UpdateEvents() {
	bool quit = false;
	sf::Event event;
	while (Settings::ms_window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			Settings::ms_window->close();
			quit = true;
		}

		else if(event.type == sf::Event::KeyPressed) {
			if(event.key.code == sf::Keyboard::A) {
				Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::A] = true;
			}
			if(event.key.code == sf::Keyboard::D) {
				Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::D] = true;
			}
			if(event.key.code == sf::Keyboard::W) {
				Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::W] = true;
			}
			if(event.key.code == sf::Keyboard::S) {
				Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::S] = true;
			}
			if(event.key.code == sf::Keyboard::LShift) {
				Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::LShift] = true;
			}
		}

		else if(event.type == sf::Event::KeyReleased) {
			if(event.key.code == sf::Keyboard::A) {
				Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::A] = false;
			}
			if(event.key.code == sf::Keyboard::D) {
				Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::D] = false;
			}
			if(event.key.code == sf::Keyboard::W) {
				Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::W] = false;
			}
			if(event.key.code == sf::Keyboard::S) {
				Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::S] = false;
			}
			if(event.key.code == sf::Keyboard::LShift) {
				Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::LShift] = false;
			}
		}
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		Settings::SetFullscreen();
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
		Settings::SetWindowed();
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		Settings::ms_window->close();
		quit = true;
	}

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		m_soundRippleManager.CreateSoundRipple(mp_player->GetPosition(), 2, true, m_spriteManager.Load("ripple.txt"));
		for(int i = 0; i < m_guards.size(); i++) {
			sf::Vector2f pos(m_soundRippleManager.GuardNotice(m_guards.at(i)->GetPosition()));
			if(pos.x > 0.1f && pos.y > 0.1f) {
				m_guards.at(i)->AddWaypointToFront(pos);
			}
		}
	}
	else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		m_soundRippleManager.CreateSoundRipple(mp_player->GetPosition(), 4, true, m_spriteManager.Load("ripple.txt"));
		for(int i = 0; i < m_guards.size(); i++) {
			sf::Vector2f pos(m_soundRippleManager.GuardNotice(m_guards.at(i)->GetPosition()));
			if(pos.x > 0.1f && pos.y > 0.1f) {
				m_guards.at(i)->AddWaypointToFront(pos);
			}
		}
	}

	

	return quit;
}
