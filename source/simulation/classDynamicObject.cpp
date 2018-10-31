#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "../../headers/classDynamicObject.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/integrators.h"
#include "../../headers/stringConversion.h"

DynamicObject::~DynamicObject(){}

void DynamicObject::addSolvedVelocity(sf::Vector2f cStep, sf::Vector2f nStep) {}

void DynamicObject::updatePosition(float dt) {}

//void DynamicObject::applyExternalImpulse(sf::Vector2f force, float dt) {}
