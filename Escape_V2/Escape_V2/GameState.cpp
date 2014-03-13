// StartMenuState.cpp

#include "GameState.h"
#include "PlayerObject.h"
#include "Settings.h"
#include "Guard.h"
#include "LightManager.h"
#include "WallManager.h"
#include "CollisionManager.h"
#include "FurnitureManager.h"
#include "DoorManager.h"
#include "KeyManager.h"
#include "HullManager.h"
#include "stdafx.h"
#include "AnimatedSprite.h"
#include "SpriteManager.h"
#include "Grid2D.h"
#include "PathFinding.h"
#include "Key.h"
#include "GUI.h"
#include "GuardFootSteps.h"

GameState::GameState() {
	m_nextState = "";
	mp_player = nullptr;
	m_timerGuards = 0.0f;
	m_timerPlayer = 0.0f;
	m_gameOverTimer = 0.0f;
	mp_grid = nullptr;
	mp_gui = nullptr;
	mp_guardFootSteps = nullptr;
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

	m_timerPlayer = 0.0f;
	m_timerGuards = 0.0f;
	m_gameOverTimer = 0.0f;

	mp_player = new PlayerObject(m_spriteManager.Load("Player.txt"));

	mp_view = new sf::View();

	mp_view->setCenter(mp_player->GetPosition());
	mp_view->setSize(static_cast<float>(Settings::ms_window->getSize().x), static_cast<float>(Settings::ms_window->getSize().y));

	mp_gui = new GUI();

	ls = new ltbl::LightSystem(AABB(Vec2f(0, 0), Vec2f(static_cast<float>(3657), static_cast<float>(5651))), Settings::ms_window, "../data/lightFin.png", "../data/shaders/lightAttenuationShader.frag");
	ls->SetView(*mp_view);

	//ls2 = new ltbl::LightSystem(AABB(Vec2f(0, 0), Vec2f(static_cast<float>(3657), static_cast<float>(5651))), Settings::ms_window, "../data/lightFin.png", "../data/shaders/lightAttenuationShader.frag");
	//ls2->SetView(*mp_view);

	lm = new LightManager(ls);
	hl = new HullManager(ls);
	wl = new WallManager(hl);
	km = new KeyManager(lm, ls);
	dm = new DoorManager(hl, lm, ls);
	fm = new FurnitureManager(hl);
	cl = new CollisionManager(wl, km, dm);

	if(!wl->LoadFromFile("../data/Walls.txt")) {
		abort();
	}

	/*lm2 = new LightManager(ls2);
	hl2 = new HullManager(ls2);
	wl2 = new WallManager(hl2);
	cl2 = new CollisionManager(wl2, km, dm);*/

	mp_grid = new Grid2D();
	mp_grid->Init(&m_level, cl);

	for(unsigned int i = 0; i < Settings::ms_guards.size(); i++) {
		m_guards.push_back(new Guard(i, m_spriteManager.Load("Guard1.txt"), mp_grid));
	}

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
	//lm2->AddLight(testLight, mp_player);
	testLight->SetAlwaysUpdate(true);

	for(unsigned int i = 0; i < m_guards.size(); i++) {
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


		lm->AddLight(guardLight, m_guards.at(i));
		guardLight->SetAlwaysUpdate(true);
	}


	ltbl::Light_Point* testLight2 = new ltbl::Light_Point();
	testLight2->m_center = Vec2f(Settings::ms_enter.x, -4150);
	testLight2->m_radius = 1000.0f;
	testLight2->m_size = 10.0f;
	testLight2->m_color.r = 0.0f;
	testLight2->m_color.g = 0.0f;
	testLight2->m_color.b = 0.0f;
	testLight2->m_intensity = 1.0f;
	//testLight2->m_bleed = 0.0f;
	testLight2->m_spreadAngle = ltbl::pifTimes2;
	testLight2->m_softSpreadAngle = 0.0f;
	testLight2->CalculateAABB();

	lm->AddLight(testLight2, lm);

	testLight2->SetAlwaysUpdate(false);
	/*
	testLight3 = new ltbl::Light_Point();
	testLight3->m_center = Vec2f(Settings::ms_enter.x, -4000);
	testLight3->m_radius = 1000.0f;
	testLight3->m_size = 10.0f;
	testLight3->m_color.r = 0.0f;
	testLight3->m_color.g = 0.0f;
	testLight3->m_color.b = 0.0f;
	testLight3->m_intensity = 1.0f;
	testLight3->m_bleed = 0.0f;
	testLight3->m_spreadAngle = ltbl::pifTimes2;
	testLight3->m_softSpreadAngle = 0.0f;
	testLight3->CalculateAABB();

	lm2->AddLight(testLight3, lm2);

	testLight3->SetAlwaysUpdate(false);
	*/

	/*if(!wl2->LoadFromFile("../data/Walls.txt")) {
	abort();
	}*/
	if(!fm->LoadFromFile("../data/Furniture.txt",ls)) {
		abort();
	}
	if(!km->LoadFromFile("../data/Keys.txt")) {
		abort();
	}
	if(!dm->LoadFromFile("../data/Doors.txt")) {
		abort();
	}

	if (!m_music.openFromFile("../data/I Knew a Guy - Stealth.wav")) {
		std::cout << "Failed loading music for GameState!\n";
	}

	mp_guardFootSteps = new GuardFootSteps();
	m_music.setLoop(true);
	m_music.play();
}

void GameState::Exit() {
	m_nextState = "";
	m_timerGuards = 0.0f;
	m_timerPlayer = 0.0f;
	m_gameOverTimer = 0.0f;

	if(mp_player != nullptr) {
		delete mp_player;
		mp_player = nullptr;
	}

	if(mp_grid != nullptr) {
		delete mp_grid;
		mp_grid = nullptr;
	}
	if(mp_gui != nullptr) {
		delete mp_gui;
		mp_gui = nullptr;
	}

	for(unsigned int i = 0; i < m_guards.size(); i++) {
		if(m_guards.at(i) != nullptr) {
			delete m_guards.at(i);
			m_guards.at(i) = nullptr;
			m_guards.erase(m_guards.begin() + i);
			i--;
		}
	}

	if(mp_guardFootSteps != nullptr) {
		mp_guardFootSteps->Cleanup();
		delete mp_guardFootSteps;
		mp_guardFootSteps = nullptr;
	}

	if(fm != nullptr) {
		delete fm;
		fm = nullptr;
	}
	if(km != nullptr) {
		delete km;
		km = nullptr;
	}
	if(lm != nullptr) {
		delete lm;
		lm = nullptr;
	}
	if(wl != nullptr) {
		delete wl;
		wl = nullptr;
	}
	if(hl != nullptr) {
		delete hl;
		hl = nullptr;
	}
	if(cl != nullptr) {
		delete cl;
		cl = nullptr;
	}
	if(ls != nullptr) {
		delete ls;
		ls = nullptr;
	}

	m_soundRippleManager.Cleanup();

	m_spriteManager.Cleanup();

	m_music.stop();
}

bool GameState::Update() {
	//Updates the timer for the guards
	m_timerGuards += Settings::ms_deltatime;
	m_timerPlayer += Settings::ms_deltatime;

	//Updates keypresses. returns true if the game is exiting
	if(UpdateEvents()) {
		return false;
	}

	//Updates the player
	mp_player->Update(cl, fm);
	if(mp_player->IsRunning() && m_timerPlayer > 0.5f) {
		m_timerPlayer = 0.0f;
		m_soundRippleManager.CreateSoundRipple(mp_player->GetPosition(), 4, true, m_spriteManager.Load("Ripple.txt"));
		/*if(mp_player->IsRightFoot()) {
			int s = Settings::ms_soundManager.newSound("../data/RUN_1.wav", false);
			Settings::ms_soundManager.Sounds.at(s)->Play(false);
		}
		else {
			int s = Settings::ms_soundManager.newSound("../data/RUN_2.wav", false);
			Settings::ms_soundManager.Sounds.at(s)->Play(false);
		}*/
	}

	//Checks and fixes the collision detection
	//ToDo: Check with furnitures and doors.
	//ToDo: fix the actions that is taken when a collision is detected. The player can't move at all as it is now.
	/*int tries = 0;
	while(cl->Circle_WallCollision(*(mp_player->GetSprite()))) {
		if(mp_player->CollisionDetected(tries)) {
			break;
		}
		tries++;
	}*/

	for(unsigned int i = 0; i < m_guards.size(); i++) {
		//checks to see if a guard is within hearing range of a playercreated sound
		sf::Vector2f pos(m_soundRippleManager.GuardNotice(m_guards.at(i)->GetPosition()));
		if(pos.x > 1.0f && pos.y > 1.0f) {
			m_guards.at(i)->AddWaypointToFront(pos);
		}

		//updates the guard
		m_guards.at(i)->Update(mp_player->GetPosition(), cl);

		//checks to see if a foot step is to be created. Move or change so it is in sync with the walk animation
		if(m_guards.at(i)->IsWalking() && m_timerGuards > 0.5f) {
			AnimatedSprite* footPrint = m_spriteManager.Load("GuardFootStep.txt");
			if(m_guards.at(i)->GetFoot() == 0) {
				footPrint->ChangeAnimation("Footstep_left.png");
			}
			else {
				footPrint->ChangeAnimation("Footstep_right.png");
			}
			mp_guardFootSteps->AddRipple(m_guards.at(i)->GetPosition(), m_guards.at(i)->GetSprite()->getSprite()->getRotation(), footPrint);
		}

		//updates the guards flashlight position and direction
		Vec2f vecG(m_guards.at(i)->GetPosition().x, -m_guards.at(i)->GetPosition().y + mp_view->getSize().y);
		float angle = (static_cast<int>(m_guards.at(i)->GetSprite()->getSprite()->getRotation() - 90) % 360)  * (3.141592 / 180);
		lm->GetLight(m_guards.at(i))->m_directionAngle = -angle;
		lm->GetLight(m_guards.at(i))->SetCenter(vecG);
		lm->GetLight(m_guards.at(i))->CalculateAABB();
	}

	//resets the timer
	if(m_timerGuards > 0.5f) {
		m_timerGuards = 0.0f;
	}

	//updates the footsteps and the soundripples
	mp_guardFootSteps->Update();
	m_soundRippleManager.UpdateSounds();

	//updates the light around the player
	Vec2f vec(mp_player->GetPosition().x, -mp_view->getCenter().y + mp_view->getSize().y);
	lm->GetLight(mp_player)->SetCenter(vec);
	
	//checks if the player has won or lost.
	//ToDo: Fix so it does the right thing. Separate win and loss.
	if(Settings::ms_gameOver && m_gameOverTimer > 1.0f) {
		m_nextState = "StartMenuState";
		Settings::ms_gameOver = false;
		return false;
	}
	else if(mp_player->GetPosition().x > Settings::ms_exit.x && mp_player->GetPosition().x < Settings::ms_exit.x + 100 && mp_player->GetPosition().y > Settings::ms_exit.y && mp_player->GetPosition().y < Settings::ms_exit.y + 50) {
		m_nextState = "StartMenuState";
		return false;
	}
	else if(Settings::ms_gameOver) {
		m_gameOverTimer += Settings::ms_deltatime;
	}

	return true;
}

void GameState::Draw() {

	//mp_view->setCenter(sf::Vector2f(Settings::ms_exit.x, Settings::ms_exit.y));
	//mp_view->setCenter(m_guards.at(0)->GetPosition());

	mp_view->setCenter(mp_player->GetPosition());

	Settings::ms_window->setView(*mp_view);

	m_level.Draw();
	mp_player->Draw();
	for(unsigned int i = 0; i < m_guards.size(); i++) {
		m_guards.at(i)->Draw();
	}

	dm->Draw(Settings::ms_window);
	fm->Draw(Settings::ms_window);


	ls->SetView(*mp_view);

	km->Draw(Settings::ms_window);

	ls->RenderLights();
	ls->RenderLightTexture();

	m_soundRippleManager.Draw();
	mp_guardFootSteps->Draw();

	Settings::DrawShot();

	mp_gui->Draw(Settings::ms_window);
	//mp_grid->Draw();
	Settings::ms_window->display();
	Settings::ms_window->clear(sf::Color(0, 0, 0, 255));
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
	Settings::ms_inputManager.PostUpdate();

	bool quit = false;
	sf::Event event;
	while (Settings::ms_window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			Settings::ms_window->close();
			quit = true;
		}

		else if(event.type == sf::Event::KeyPressed) {
			Settings::ms_inputManager.m_keyboard_current[event.key.code] = true;
			/*if(event.key.code == sf::Keyboard::A) {
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
			}*/
			if(event.key.code == sf::Keyboard::Space) {
				Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::Space] = true;
				sf::CircleShape* circle_key = cl->Circle_KeyPickup(*mp_player->GetSprite());
				if(circle_key != nullptr) {
					Key* key = km->PickUpKey(circle_key);
					sf::Color color = key->GetPickUpRadius()->getFillColor();
					color.a = 255;
					key->setColor(color);
					mp_gui->AddItem(key);
				}
			}
		}

		else if(event.type == sf::Event::KeyReleased) {
			Settings::ms_inputManager.m_keyboard_current[event.key.code] = false;
			/*if(event.key.code == sf::Keyboard::A) {
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
			if(event.key.code == sf::Keyboard::Space) {
			Settings::ms_inputManager.m_keyboard_current[sf::Keyboard::Space] = false;
			}*/
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		Settings::ms_window->close();
		quit = true;
	}

	//if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
	//	m_soundRippleManager.CreateSoundRipple(mp_player->GetPosition(), 2, true, m_spriteManager.Load("ripple.txt"));
	//	for(unsigned int i = 0; i < m_guards.size(); i++) {
	//		/*sf::Vector2f pos(m_soundRippleManager.GuardNotice(m_guards.at(i)->GetPosition()));
	//		if(pos.x > 0.1f && pos.y > 0.1f) {
	//		m_guards.at(i)->AddWaypointToFront(pos);
	//		}*/
	//	}
	//}
	//else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
	//	m_soundRippleManager.CreateSoundRipple(mp_player->GetPosition(), 4, true, m_spriteManager.Load("ripple.txt"));
	//	for(unsigned int i = 0; i < m_guards.size(); i++) {
	//		/*sf::Vector2f pos(m_soundRippleManager.GuardNotice(m_guards.at(i)->GetPosition()));
	//		if(pos.x > 0.1f && pos.y > 0.1f) {
	//		m_guards.at(i)->AddWaypointToFront(pos);
	//		}*/
	//	}
	//}



	return quit;
}
