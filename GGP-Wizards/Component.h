#pragma once
class GameObject;
#include "Object.h"

class Component : public Object
{
public:
	Component();
	~Component();

	GameObject* getGameObject();
	void setGameObject(GameObject* gameObject);

private:
	GameObject* gameObject;
};