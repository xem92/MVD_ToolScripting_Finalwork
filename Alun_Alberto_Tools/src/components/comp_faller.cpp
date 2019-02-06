#pragma once
#include "comp_faller.h"
#include "../EntityComponentStore.h"
#include "../extern.h"

//////////////////////////////////////////////////////////
////////////////// CUSTOM COMPONENTS /////////////////////
//////////////////////////////////////////////////////////


// Important function, this causes the rotation.
void Faller::update(float dt) {

	if (ECS.entities.size() != 0) {
		Transform& transform = ECS.getComponentFromEntity<Transform>(owner);
		if (ECS.getComponentFromEntity<Collider>(owner).colliding) {
			is_falling = true;
		}
		if (is_falling) {
			timer += dt * 10;
			if (timer > 25) {
				transform.translate(lm::vec3(0.0, -1.0 * dt * speed, 0.0));
			}
		}
	}
}

// Save to json
void Faller::Save(rapidjson::Document & json, rapidjson::Value & entity)
{
	rapidjson::Value obj(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = json.GetAllocator();

	// Set translation
	{
		rapidjson::Value naxis(rapidjson::kArrayType);
		obj.AddMember("speed", speed, allocator);
	}

	entity.AddMember("faller", obj, allocator);
}

// Load method from json
void Faller::Load(rapidjson::Value & entity, int ent_id) {

	entity_id = ent_id;
	auto json_sp = entity["faller"]["speed"].GetFloat();

	speed = json_sp;
}

// Debug on ImGui
void Faller::debugRender() {

	ImGui::AddSpace(0, 5);
	if (ImGui::TreeNode("Faller")) {
		ImGui::AddSpace(0, 5);
		ImGui::DragFloat("Speed", &speed);
		ImGui::TreePop();
	}

	ImDrawList*   draw_list = ImGui::GetWindowDrawList();
	ImVec2 p = ImGui::GetCursorScreenPos();
	draw_list->AddLine(ImVec2(p.x - 9999, p.y), ImVec2(p.x + 9999, p.y), ImGui::GetColorU32(ImGuiCol_Border));
}