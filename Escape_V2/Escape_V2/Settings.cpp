// Settings.cpp

#include "Settings.h"
#include "stdafx.h"

std::map<int, std::vector<sf::Vector2f>> Settings::m_allGuardWaypoints;
std::vector<sf::Vector2f> Settings::ms_guards;
std::vector<int> Settings::ms_guardsRotation;
std::vector<sf::Vector2f> Settings::ms_roomWaypoints;
sf::RenderWindow *Settings::ms_window = nullptr;
float Settings::ms_deltatime = 0.1f;
bool Settings::ms_gameOver = false;
bool Settings::ms_shoot = false;
sf::Vector2f Settings::ms_enter = sf::Vector2f(0.0f, 0.0f);
sf::Vector2f Settings::ms_exit = sf::Vector2f(0.0f, 0.0f);
sf::Vector2f Settings::ms_shootFrom = sf::Vector2f(0.0f, 0.0f);
sf::Vector2f Settings::ms_shootTo = sf::Vector2f(0.0f, 0.0f);
clock_t Settings::ms_newTime = clock();
clock_t Settings::ms_oldTime = clock();
InputManager Settings::ms_inputManager;
SoundManager Settings::ms_soundManager;

void Settings::Load(const std::string& c_path) {
	{
		std::string path = "../data/" + c_path;
		std::ifstream stream(path + "Guards.txt");
		std::string row;
		std::string split;
		std::vector<sf::Vector2f> waypoints;
		sf::Vector2f vect;

		m_allGuardWaypoints.clear();
		ms_guards.clear();
		ms_guardsRotation.clear();


		if(stream.is_open()) {
			while(!stream.eof()) {
				std::getline(stream, row);
				if(row.find("§") != std::string::npos) {

					if(ms_guards.size() > 0) {
						m_allGuardWaypoints.insert(std::pair<int, std::vector<sf::Vector2f>>(ms_guards.size() - 1, waypoints));
					}

					waypoints.clear();

					std::getline(stream, row);
					{
						std::istringstream sstream(row);

						std::vector<std::string> tokens;
						copy(std::istream_iterator<std::string>(sstream),
							std::istream_iterator<std::string>(),
							std::back_inserter<std::vector<std::string> >(tokens));

						ms_guards.push_back(sf::Vector2f((float)atof(tokens.at(0).c_str()), (float)atof(tokens.at(1).c_str())));
						ms_guardsRotation.push_back(atoi(tokens.at(2).c_str()));
					}
					std::istringstream sstream(row);
					std::vector<std::string> tokens;
					copy(std::istream_iterator<std::string>(sstream),
						std::istream_iterator<std::string>(),
						std::back_inserter<std::vector<std::string> >(tokens));

					vect.x = static_cast<float>(atof(tokens.at(0).c_str()));
					vect.y = static_cast<float>(atof(tokens.at(1).c_str()));
					waypoints.push_back(vect);
				}
				else if(row.length() > 0) {
					std::istringstream sstream(row);

					std::vector<std::string> tokens;
					copy(std::istream_iterator<std::string>(sstream),
						std::istream_iterator<std::string>(),
						std::back_inserter<std::vector<std::string> >(tokens));

					vect.x = static_cast<float>(atof(tokens.at(1).c_str()));
					vect.y = static_cast<float>(atof(tokens.at(2).c_str()));
					waypoints.push_back(vect);


				}
			}
		}
		m_allGuardWaypoints.insert(std::pair<int, std::vector<sf::Vector2f>>(ms_guards.size() - 1, waypoints));
	}
	{
		std::string path = "../data/" + c_path;
		std::ifstream stream(path + "EnterExit.txt");
		std::string row;
		std::string split;
		std::vector<sf::Vector2f> waypoints;
		sf::Vector2f vect;


		if(stream.is_open()) {
			while(!stream.eof()) {
				std::getline(stream, row);
				if(row.length() > 0) {
					std::istringstream sstream(row);

					std::vector<std::string> tokens;
					copy(std::istream_iterator<std::string>(sstream),
						std::istream_iterator<std::string>(),
						std::back_inserter<std::vector<std::string> >(tokens));

					vect.x = static_cast<float>(atof(tokens.at(0).c_str()));
					vect.y = static_cast<float>(atof(tokens.at(1).c_str()));
					ms_enter = vect;
				}
				std::getline(stream, row);
				if(row.length() > 0) {
					std::istringstream sstream(row);

					std::vector<std::string> tokens;
					copy(std::istream_iterator<std::string>(sstream),
						std::istream_iterator<std::string>(),
						std::back_inserter<std::vector<std::string> >(tokens));

					vect.x = static_cast<float>(atof(tokens.at(0).c_str()));
					vect.y = static_cast<float>(atof(tokens.at(1).c_str()));
					ms_exit = vect;
				}
			}
		}
	}
	{
		std::string path = "../data/" + c_path + "RoomWaypoints.txt";
		std::ifstream stream(path);
		std::string row;
		sf::Vector2f newPosition;
		ms_roomWaypoints.clear();

		if(stream.is_open()) {
			while(!stream.eof()) {
				std::getline(stream, row);
				if(row.length() > 0) {
					std::stringstream sstream(row);
					std::string part;
					sstream >> part;
					newPosition.x = std::stof(part);
					sstream >> part;
					newPosition.y = std::stof(part);
					ms_roomWaypoints.push_back(newPosition);
				}
			}
		}
	}
	ms_inputManager.ClearPresses();
}

void Settings::UpdateDeltatime() {
	ms_oldTime = ms_newTime;
	ms_newTime = clock();

	ms_deltatime = float(ms_newTime - ms_oldTime) / CLOCKS_PER_SEC;
	if(ms_deltatime > 0.1f) {
		ms_deltatime = 0.1f;
	}
}

void Settings::SetFullscreen() {
	sf::VideoMode video(1280, 720);
	std::vector<sf::VideoMode> supportedModes;
	supportedModes = video.getFullscreenModes();
	auto it = supportedModes.begin();
	while (it!=supportedModes.end())
	{
		if(it->width/16 == it->height/9)
		{
			video = *it;
			break;
		}
		it++;
	}

	sf::ContextSettings setting;
	setting.antialiasingLevel = 8;
	ms_window->close();
	ms_window->create(*it, "SFML shapes", sf::Style::Fullscreen, setting);
	Settings::ms_window->setFramerateLimit(60);
}

void Settings::SetWindowed() {
	sf::VideoMode video(1280, 720);
	std::vector<sf::VideoMode> supportedModes;
	supportedModes = video.getFullscreenModes();
	auto it = supportedModes.begin();
	while (it!=supportedModes.end())
	{
		if(it->width/16 == it->height/9)
		{
			video = *it;
			break;
		}
		it++;
	}

	sf::ContextSettings setting;
	setting.antialiasingLevel = 8;
	ms_window->close();
	ms_window->create(video, "SFML shapes", sf::Style::Default, setting);
	Settings::ms_window->setFramerateLimit(60);
}

void Settings::Shoot(sf::Vector2f from, sf::Vector2f to) {
	if(!ms_shoot)
	{
		ms_shootFrom = from;
		ms_shootTo = to;
		ms_shoot = true;
	}
}

void Settings::DrawShot() {
	if(ms_shoot) {
		sf::VertexArray lines(sf::Lines, 2);
		lines[0].position = ms_shootFrom;
		lines[0].color = sf::Color(128, 128, 128);
		lines[1].position = ms_shootTo;
		lines[1].color = sf::Color(192, 192, 192);
		ms_window->draw(lines);
	}
}

void Settings::ResetShot() {
	ms_shoot = false;
}

bool Settings::IsShot() {
	return ms_shoot;
}