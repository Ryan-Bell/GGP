#include "Component.h"
#include "GameObject.h"

Component::Component()
{
	gameObject = nullptr;
}

Component::~Component() {}

GameObject * Component::getGameObject()
{
	return gameObject;
}

void Component::setGameObject(GameObject * gameObject)
{
	this->gameObject = gameObject;
}
