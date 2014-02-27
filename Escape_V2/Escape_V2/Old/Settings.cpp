// Settings.cpp

#include "Settings.h"

std::map<int, std::vector<sf::Vector2f>> Settings::m_allGuardWaypoints;
std::vector<sf::Vector2f> Settings::ms_guards;
sf::RenderWindow *Settings::ms_window = nullptr;
float Settings::ms_deltatime = 0.1f;
bool Settings::ms_gameOver = false;
sf::Vector2f Settings::ms_enter = sf::Vector2f(0.0f, 0.0f);
sf::Vector2f Settings::ms_exit = sf::Vector2f(0.0f, 0.0f);
clock_t Settings::ms_newTime = clock();
clock_t Settings::ms_oldTime = clock();
InputManager Settings::ms_inputManager;

Settings::Settings() {
	
}


Settings::~Settings() {

}

void Settings::Load() {
	{
		std::string path = "../data/";
		std::ifstream stream(path + "Guards.txt");
		std::string row;
		std::string split;
		std::vector<sf::Vector2f> waypoints;
		sf::Vector2f vect;
	

		if(stream.is_open()) {
			while(!stream.eof()) {
				std::getline(stream, row);
				if(row.find("§") != std::string::npos) {

					if(ms_guards.size() > 0) {
						m_allGuardWaypoints.insert(std::pair<int, std::vector<sf::Vector2f>>(ms_guards.size() - 1, waypoints));
					}

					waypoints.clear();

					std::getline(stream, row);

					std::istringstream sstream(row);

					std::vector<std::string> tokens;
					copy(std::istream_iterator<std::string>(sstream),
					std::istream_iterator<std::string>(),
					std::back_inserter<std::vector<std::string> >(tokens));

					ms_guards.push_back(sf::Vector2f((float)atof(tokens.at(0).c_str()), (float)atof(tokens.at(1).c_str())));
				}
				else if(row.length() > 0) {
					std::istringstream sstream(row);

					std::vector<std::string> tokens;
					copy(std::istream_iterator<std::string>(sstream),
					std::istream_iterator<std::string>(),
					std::back_inserter<std::vector<std::string> >(tokens));

					vect.x = atof(tokens.at(1).c_str());
					vect.y = atof(tokens.at(2).c_str());
					waypoints.push_back(vect);

				
				}
			}
		}
		m_allGuardWaypoints.insert(std::pair<int, std::vector<sf::Vector2f>>(ms_guards.size() - 1, waypoints));
	}
	{
		std::string path = "../data/";
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

					vect.x = atof(tokens.at(0).c_str());
					vect.y = atof(tokens.at(1).c_str());
					ms_enter = vect;
				}
				std::getline(stream, row);
				if(row.length() > 0) {
					std::istringstream sstream(row);

					std::vector<std::string> tokens;
					copy(std::istream_iterator<std::string>(sstream),
					std::istream_iterator<std::string>(),
					std::back_inserter<std::vector<std::string> >(tokens));

					vect.x = atof(tokens.at(0).c_str());
					vect.y = atof(tokens.at(1).c_str());
					ms_exit = vect;
				}
			}
		}
	}
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
            //std::cout << "found: " << it->width << "*" << it->height << std::endl;
            break;
        }
        it++;
    }

	sf::ContextSettings setting;
	setting.antialiasingLevel = 8;
	ms_window->close();
	ms_window->create(*it, "SFML shapes", sf::Style::Fullscreen, setting);
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
            //std::cout << "found: " << it->width << "*" << it->height << std::endl;
            break;
        }
        it++;
    }
	
	sf::ContextSettings setting;
	setting.antialiasingLevel = 8;
	ms_window->close();
	ms_window->create(video, "SFML shapes", sf::Style::Default, setting);
}