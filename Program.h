#pragma once
#include <SFML/Audio.hpp>
#include "PerfectCollision.h"
#include <iostream>
#include <unordered_map>
#include <deque>

using namespace sf;
using namespace std;

const float WINDOW_WIDTH = (float)VideoMode::getDesktopMode().width;
const float WINDOW_HEIGHT = (float)VideoMode::getDesktopMode().height;