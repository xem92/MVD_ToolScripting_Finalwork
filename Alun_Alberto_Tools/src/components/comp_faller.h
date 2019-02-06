#pragma once
#include "../includes.h"
#include "../Components.h"

//////////////////////////////////////////////////////////
////////////////// CUSTOM COMPONENTS /////////////////////
//////////////////////////////////////////////////////////

// Method used to rotate object around axis given an speed
struct Faller : public Component {

	float speed = 1;
	int entity_id;
	float timer = 0.0f;
	bool is_falling = false;

	void update(float dt);
	void Save(rapidjson::Document& json, rapidjson::Value & entity);
	void Load(rapidjson::Value & entity, int ent_id);
	void debugRender();
};
