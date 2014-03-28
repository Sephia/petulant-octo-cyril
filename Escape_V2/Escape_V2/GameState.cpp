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
#include "FoV.h"
#include "GuardFootSteps.h"
#include "Level.h"
#include "Door.h"
#include "SoundManager.h"

GameState::GameState() {
	m_nextState = "";
	mp_player = nullptr;
	m_timerGuards = 0.0f;
	m_timerPlayer = 0.0f;
	m_gameOverTimer = 0.0f;
	mp_grid = nullptr;
	mp_gui = nullptr;
	mp_guardFootSteps = nullptr;
	mp_level = nullptr;
	m_levelToLoad = 0;

	mp_music_undetected = nullptr;
	mp_music_detected = nullptr;
	mp_keySound = nullptr;
	mp_unlock = nullptr;
	mp_death = nullptr;
	mp_jokeDeath = nullptr;
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
	loadingScreenTexture.loadFromFile("../Data/Sprites/0procent.png");
	loadingScreenSprite = new sf::Sprite(loadingScreenTexture);

	Settings::ms_window->clear();
	Settings::ms_window->draw(*loadingScreenSprite);
	Settings::ms_window->display();

	m_nextState = "";
	m_deathSoundPlayed = false;

	if(m_levelToLoad == 0) {
		m_pathToLevel = "Tutorial/";
	}
	else if(m_levelToLoad == 1) {
		m_pathToLevel = "level01/";
	}
	else if(m_levelToLoad == 2) {
		m_pathToLevel = "level02/";
	}
	else if(m_levelToLoad == 3) {
		m_pathToLevel = "level03/";
	}
	else if(m_levelToLoad == 4) {
		m_pathToLevel = "level04/";
	}
	else {
		std::cout << "Failed to load level\n";
	}

	Settings::Load(m_pathToLevel);
	mp_level = new Level(m_pathToLevel);

	m_spriteManager.Init("../data/sprites/");

	m_timerPlayer = 0.0f;
	m_timerGuards = 0.0f;
	m_gameOverTimer = 0.0f;

	mp_player = new PlayerObject(m_spriteManager.Load("Player.txt"));

	mp_view = new sf::View();

	mp_view->setCenter(mp_player->GetPosition());
	mp_view->setSize(static_cast<float>(Settings::ms_window->getSize().x), static_cast<float>(Settings::ms_window->getSize().y));

	mp_gui = new GUI();

	ls = new ltbl::LightSystem(AABB(Vec2f(0, 0), Vec2f(static_cast<float>(Settings::ms_window->getSize().x), static_cast<float>(Settings::ms_window->getSize().y))), Settings::ms_window, "../data/sprites/lightFin.png", "../data/shaders/lightAttenuationShader.frag");
	ls->SetView(*mp_view);

	lm = new LightManager(ls);
	hl = new HullManager(ls);
	wl = new WallManager(hl);
	km = new KeyManager(lm, ls);
	dm = new DoorManager(hl, lm, ls);
	fm = new FurnitureManager(hl);
	cl = new CollisionManager(wl, km, dm);

	loadingScreenTexture.loadFromFile("../Data/Sprites/30procent.png");
	loadingScreenSprite->setTexture(loadingScreenTexture);
	Settings::ms_window->clear();
	Settings::ms_window->draw(*loadingScreenSprite);
	Settings::ms_window->display();

	if(!wl->LoadFromFile("../data/" + m_pathToLevel + "Walls.txt")) {
		abort();
	}
	if(!fm->LoadFromFile("../data/" + m_pathToLevel + "Furniture.txt",ls)) {
		abort();
	}
	loadingScreenTexture.loadFromFile("../Data/Sprites/60procent.png");
	loadingScreenSprite->setTexture(loadingScreenTexture);
	Settings::ms_window->clear();
	Settings::ms_window->draw(*loadingScreenSprite);
	Settings::ms_window->display();

	if(!km->LoadFromFile("../data/" + m_pathToLevel + "Keys.txt")) {
		abort();
	}
	if(!dm->LoadFromFile("../data/" + m_pathToLevel + "Doors.txt")) {
		abort();
	}

	mp_fov = new FoV(wl, mp_level->GetSpriteSize().x, mp_level->GetSpriteSize().y);

	mp_grid = new Grid2D();
	mp_grid->Init(mp_player->GetSprite(), mp_level, cl, fm);

	loadingScreenTexture.loadFromFile("../Data/Sprites/90procent.png");
	loadingScreenSprite->setTexture(loadingScreenTexture);
	Settings::ms_window->clear();
	Settings::ms_window->draw(*loadingScreenSprite);
	Settings::ms_window->display();

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
		guardLight->m_softSpreadAngle = 0.3f;
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
	testLight2->m_spreadAngle = ltbl::pifTimes2;
	testLight2->m_softSpreadAngle = 0.0f;
	testLight2->CalculateAABB();

	lm->AddLight(testLight2, lm);

	testLight2->SetAlwaysUpdate(false);

	Settings::ms_soundManager.newSong("../data/music/I Knew a Guy - Stealth.wav", true);
	Settings::ms_soundManager.newSong("../data/music/Simplex-Menu_Music.wav", true);

	Settings::ms_soundManager.newSound("../data/sound/RUN_1.ogg", false, 500, 0.1f);
	Settings::ms_soundManager.newSound("../data/sound/RUN_2.ogg", false, 500, 0.1f);
	Settings::ms_soundManager.newSound("../data/sound/DOOR_OPEN.ogg", false, 800, 0.9f);
	Settings::ms_soundManager.newSound("../data/sound/DOOR_CLOSE.ogg", false, 800, 0.9f);
	Settings::ms_soundManager.newSound("../data/sound/PICKUP_KEY.ogg", false, 800, 0.9f);
	Settings::ms_soundManager.newSound("../data/sound/UNLOCK_DOOR.ogg", false, 800, 0.9f);
	Settings::ms_soundManager.newSound("../data/sound/DEATH_SOUND-REAL.ogg", false, 800, 0.9f);
	Settings::ms_soundManager.newSound("../data/sound/DEATH_SOUND.ogg", false, 800, 0.9f);
	Settings::ms_soundManager.newSound("../data/sound/STEP_1.ogg", false, 0.1f, 10.0f);
	Settings::ms_soundManager.newSound("../data/sound/STEP_2.ogg", false, 0.1f, 10.0f);
	Settings::ms_soundManager.newSound("../data/sound/FURNITURE.ogg", false, 0.1f, 10.0f);

	Settings::ResetShot();

	mp_guardFootSteps = new GuardFootSteps();
	mp_keySound = Settings::ms_soundManager.GetSound("../data/sound/PICKUP_KEY.ogg")->CreateSound(sf::Vector2f(0,0));
	mp_unlock = Settings::ms_soundManager.GetSound("../data/sound/UNLOCK_DOOR.ogg")->CreateSound(sf::Vector2f(0,0));
	mp_death = Settings::ms_soundManager.GetSound("../data/sound/DEATH_SOUND-REAL.ogg")->CreateSound(sf::Vector2f(0,0));
	mp_jokeDeath = Settings::ms_soundManager.GetSound("../data/sound/DEATH_SOUND.ogg")->CreateSound(sf::Vector2f(0,0));

	Settings::ms_soundManager.GetSong("../data/music/Simplex-Menu_Music.wav")->Stop();
	Settings::ms_soundManager.GetSong("../data/music/I Knew a Guy - Stealth.wav")->Play();
}

void GameState::Exit() {
	m_nextState = "";
	m_timerGuards = 0.0f;
	m_timerPlayer = 0.0f;
	m_gameOverTimer = 0.0f;

	Settings::ms_window->setView(Settings::ms_window->getDefaultView());

	if(mp_level != nullptr) {
		delete mp_level;
		mp_level = nullptr;
	}

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

	if(loadingScreenSprite != nullptr) {
		delete loadingScreenSprite;
		loadingScreenSprite = nullptr;
	}

	m_soundRippleManager.Cleanup();

	m_spriteManager.Cleanup();

	Settings::ms_soundManager.GetSong("../data/music/I Knew a Guy - Stealth.wav")->Stop();

	if(mp_keySound != nullptr) {
		delete mp_keySound;
		mp_keySound = nullptr;
	}
	if(mp_unlock != nullptr) {
		delete mp_unlock;
		mp_unlock = nullptr;
	}
	if(mp_death != nullptr) {
		delete mp_death;
		mp_death = nullptr;
	}
	if (mp_jokeDeath != nullptr) {
		delete mp_jokeDeath;
		mp_jokeDeath = nullptr;
	}
}

bool GameState::Update() {
	//Updates the timers
	m_timerGuards += Settings::ms_deltatime;
	m_timerPlayer += Settings::ms_deltatime;
	m_bumpTimer += Settings::ms_deltatime;

	//Updates keypresses. returns true if the game is exiting
	if(UpdateEvents()) {
		return false;
	}

	//Updates the player
	int noise = mp_player->Update(cl, fm, mp_fov);
	if(noise != -1 && m_bumpTimer > 0.91f) {
		sf::Sound* bump = nullptr;
		if(!SoundEntity::IsMuted()) {
			bump = Settings::ms_soundManager.GetSound("../data/sound/FURNITURE.ogg")->CreateSound(mp_player->GetPosition());
			if(bump != nullptr) {
				bump->play();
			}
		}
		m_soundRippleManager.CreateSoundRipple(mp_player->GetPosition(), noise, true, m_spriteManager.Load("Ripple.txt"), bump);
		m_bumpTimer = 0.0f;
	}

	//Update the listener
	Settings::ms_soundManager.Update(mp_player->GetPosition());
	if(mp_player->IsRunning() && m_timerPlayer > 0.5f) {
		m_timerPlayer = 0.0f;
		sf::Sound* footstep = nullptr;
		if(mp_player->IsRightFoot()) {
			if(!SoundEntity::IsMuted()) {
				footstep = Settings::ms_soundManager.GetSound("../data/sound/RUN_1.ogg")->CreateSound(mp_player->GetPosition());
			}
		}
		else {
			if(!SoundEntity::IsMuted()) {
				footstep = Settings::ms_soundManager.GetSound("../data/sound/RUN_2.ogg")->CreateSound(mp_player->GetPosition());
			}
		}
		m_soundRippleManager.CreateSoundRipple(mp_player->GetPosition(), 4, true, m_spriteManager.Load("Ripple.txt"), footstep);
	}

	for(unsigned int i = 0; i < m_guards.size(); i++) {
		//checks to see if a guard is within hearing range of a playercreated sound
		sf::Vector2f pos(m_soundRippleManager.GuardNotice(m_guards.at(i)->GetPosition()));
		if(pos.x > 1.0f && pos.y > 1.0f) {
			m_guards.at(i)->AddWaypointToFront(pos);
		}

		//updates the guard
		m_guards.at(i)->Update(mp_player->GetPosition(), cl, fm);

		//checks to see if a foot step is to be created. Move or change so it is in sync with the walk animation
		if(m_guards.at(i)->IsWalking() && m_timerGuards > 0.5f) {
			AnimatedSprite* footPrint = m_spriteManager.Load("GuardFootStep.txt");
			sf::Sound* sound = nullptr;
			if(m_guards.at(i)->GetFoot() == 0) {
				footPrint->ChangeAnimation("Footstep_left.png");
				if(!SoundEntity::IsMuted())
				{
					sound = Settings::ms_soundManager.GetSound("../data/sound/STEP_1.ogg")->CreateSound((m_guards.at(i)->GetPosition()));
				}
			}
			else {
				footPrint->ChangeAnimation("Footstep_right.png");
				if(!SoundEntity::IsMuted())
				{
					sound = Settings::ms_soundManager.GetSound("../data/sound/STEP_2.ogg")->CreateSound((m_guards.at(i)->GetPosition()));
				}
			}
			mp_guardFootSteps->AddRipple(m_guards.at(i)->GetPosition(), m_guards.at(i)->GetSprite()->getSprite()->getRotation(), footPrint, sound);
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

	//Check if door is open
	if(Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::E) || Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::Space)) {
		sf::CircleShape* door = cl->Circle_DoorUse(*mp_player->GetSprite());
		if(door != nullptr) {
			if(!dm->OpenDoor(door)) {
				for(int i = 0; i < mp_gui->GetItemCount(); i++) {
					sf::Color color = mp_gui->GetItem(i)->getColor();
					if(dm->OpenDoor(door, color)) {
						mp_gui->RemoveItem(i);
						if(!SoundEntity::IsMuted())
						{
							mp_unlock->play();
							mp_unlock->setPosition(door->getPosition().x, 0, door->getPosition().y);
						}
						break;
					}
				}
			}
			else
			{
				if(dm->GetDoor(door)->IsOpen())
				{
					if(!SoundEntity::IsMuted())
					{
						sf::Sound* mp_closeDoor = Settings::ms_soundManager.GetSound("../data/sound/DOOR_CLOSE.ogg")->CreateSound(door->getPosition());
						m_soundRippleManager.CreateSoundRipple(mp_player->GetPosition(), 1, true, m_spriteManager.Load("Ripple.txt"), mp_closeDoor);
					}
				}
				else
				{
					if(!SoundEntity::IsMuted())
					{
						sf::Sound* mp_openDoor = Settings::ms_soundManager.GetSound("../data/sound/DOOR_OPEN.ogg")->CreateSound(door->getPosition());
						m_soundRippleManager.CreateSoundRipple(mp_player->GetPosition(), 1, true, m_spriteManager.Load("Ripple.txt"), mp_openDoor);
					}
				}
			}
		}
	}

	//check if key is close enough to pick up
	sf::CircleShape* circle_key = cl->Circle_KeyPickup(*mp_player->GetSprite());
	if(circle_key != nullptr) {
		Key* key = km->PickUpKey(circle_key);
		sf::Color color = key->GetPickUpRadius()->getFillColor();
		color.a = 255;
		key->setColor(color);
		mp_gui->AddItem(key);
		if (!SoundEntity::IsMuted())
		{
			mp_keySound->play();
			mp_keySound->setPosition(key->getPosition().x, 0, key->getPosition().y);
		}
	}

	//updates the footsteps and the soundripples
	mp_guardFootSteps->Update();
	m_soundRippleManager.UpdateSounds();

	//updates the light around the player
	Vec2f vec(mp_player->GetPosition().x, -mp_view->getCenter().y + mp_view->getSize().y);
	lm->GetLight(mp_player)->SetCenter(vec);

	//checks if the player has won or lost.
	if(Settings::ms_gameOver && m_gameOverTimer > 3.2f) {
		m_nextState = "GameOverState";
		Settings::ms_gameOver = false;
		return false;
	}
	else if(mp_player->GetPosition().x > Settings::ms_exit.x - 100 && mp_player->GetPosition().x < Settings::ms_exit.x + 100 && mp_player->GetPosition().y > Settings::ms_exit.y - 100 && mp_player->GetPosition().y < Settings::ms_exit.y + 100) {
		m_levelToLoad++;
		m_levelToLoad = m_levelToLoad % 5;
		if(m_levelToLoad == 0) {
			m_nextState = "VictoryState";
			return false;
		}
		else {
			Exit();
			Enter();
		}
	}
	else if(Settings::ms_gameOver) {
		if(m_gameOverTimer > 0.5f && !m_deathSoundPlayed)
		{
			int random = rand()%10;
			if(random == 0)
			{
				mp_jokeDeath->play();
			}
			else
			{
				mp_death->play();
			}
			m_deathSoundPlayed = true;
		}
		m_gameOverTimer += Settings::ms_deltatime;
	}

	if(Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::F1) && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LControl) && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LShift)) {
		m_levelToLoad = 1;
		Exit();
		Enter();
	}
	else if(Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::F2) && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LControl) && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LShift)) {
		m_levelToLoad = 2;
		Exit();
		Enter();
	}
	else if(Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::F3) && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LControl) && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LShift)) {
		m_levelToLoad = 3;
		Exit();
		Enter();
	}
	else if(Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::F4) && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LControl) && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LShift)) {
		m_levelToLoad = 4;
		Exit();
		Enter();
	}
	else if(Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::F5) && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LControl) && Settings::ms_inputManager.IsDownKeyboard(sf::Keyboard::LShift)) {
		m_levelToLoad = 0;
		Exit();
		Enter();
	}
	else if(Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::F11)) {
		Settings::SetFullscreen();
	}
	else if(Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::F12)) {
		Settings::SetWindowed();
	}
	else if(Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::Escape)) {
		m_nextState = "StartMenuState";
		return false;
	}

	return true;
}

void GameState::Draw() {
	//mp_view->setCenter(m_guards.at(1)->GetPosition());
	mp_view->setCenter(mp_player->GetPosition());

	Settings::ms_window->setView(*mp_view);

	mp_level->Draw();
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

	mp_fov->Draw();

	m_soundRippleManager.Draw();
	mp_guardFootSteps->Draw();

	Settings::DrawShot();

	mp_gui->Draw(Settings::ms_window);

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
		}

		else if(event.type == sf::Event::KeyReleased) {
			Settings::ms_inputManager.m_keyboard_current[event.key.code] = false;
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		m_nextState = "StartMenuState";
		quit = true;
	}

	return quit;
}