// stdafx.h

#pragma once

#include "targetver.h"

#include <stdio.h>
#ifndef __APPLE__
#include <tchar.h>
#endif
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <LTBL/Light/LightSystem.h>
#include <LTBL/Constructs.h>
#include <LTBL/Light/Light_Point.h>
#include <LTBL/Utils.h>


#pragma warning(disable:4099)
//#define SFML_STATIC

#if defined(_DEBUG)
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#pragma comment(lib, "OpenGL32.lib")
#else
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-audio.lib")
#pragma comment(lib, "OpenGL32.lib")
#endif
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
