#pragma once
#include "Components.h"
#include "components/comp_rotator.h"
#include "components/comp_faller.h"
#include "components/comp_tag.h"
#include <vector>
#include <unordered_map>
#include <map>

using namespace std;

/**** ENTITY COMPONENT STORE ****/

//the entity component manager is a global struct that contains an array of
//all the entities, and an array to store each of the component types
struct EntityComponentStore {
    
    //vector of all entities
    vector<Entity> entities;
    
    ComponentArrays components; // defined at bottom of Components.h
    
    //create Entity and add transform component by default
    //return array id of new entity
    int createEntity(string name) {
        entities.emplace_back(name);
        createComponentForEntity<Transform>((int)entities.size() - 1);
        return (int)entities.size() - 1;
    }

	//returns id of entity
	int getEntity(string name) {
		for (size_t i = 0; i < entities.size(); i++)
			if (entities[i].name == name) return (int)i;
		return -1;
	}

	// delete entity
	void deleteEntity(int entity_id) {
		deleteComponent<Transform>(entity_id);
		for (auto& trans : getAllComponents<Transform>()) {
			if (trans.parent > entity_id && trans.parent > -1) {
				--trans.parent;
			}
		}
		deleteComponent<Mesh>(entity_id);
		deleteComponent<Camera>(entity_id);
		deleteComponent<Light>(entity_id);
		deleteComponent<Collider>(entity_id);
		deleteComponent<GUIElement>(entity_id);
		deleteComponent<GUIText>(entity_id);
		deleteComponent<Rotator>(entity_id);
		deleteComponent<Tag>(entity_id);
		deleteComponent<Faller>(entity_id);

		for (auto& entity : entities) {
			for (int i = 0; i < NUM_TYPE_COMPONENTS; i++) { 
				if (entity.components[i] > entity_id) {
					entity.components[i] -= 1;
				}
			}
		}

		entities.erase(entities.begin() + entity_id);
	}

	template<typename T>
	void deleteComponent(int entity_id) {
		int i = 0, del_pos = 0;
		bool has_deleted = false;
		auto& components = getAllComponents<T>();
		for (auto& comp : components) {
			if (comp.owner == entity_id) {
				del_pos = i;
				has_deleted = true;
			} else if (comp.owner > entity_id) {
				comp.owner--;
			}
		}
		if (has_deleted) {
			components.erase(components.begin() + del_pos);
		}
	}

    //returns id of entity
    void update(float dt) {

        auto& meshes = getAllComponents<Mesh>();
        auto& lights = getAllComponents<Light>();
        auto& colliders = getAllComponents<Collider>();
        auto& rotators = getAllComponents<Rotator>();
        auto& fallers = getAllComponents<Faller>();
        auto& tags = getAllComponents<Tag>();

        for (auto& light : lights) light.update(dt);
        for (auto& col : colliders) col.update(dt);
        for (auto& rot : rotators) rot.update(dt);
        for (auto& fal : fallers) fal.update(dt);
        for (auto& tag : tags) tag.update(dt);
    }

    void render() {

        auto& meshes = getAllComponents<Mesh>();
        auto& lights = getAllComponents<Light>();
        auto& colliders = getAllComponents<Collider>();
        auto& rotators = getAllComponents<Rotator>();
        auto& fallers = getAllComponents<Faller>();
        auto& tags = getAllComponents<Tag>();

        for (auto& light : lights) light.render();
        for (auto& col : colliders) col.render();
        for (auto& rot : rotators) rot.render();
        for (auto& fal : fallers) fal.render();
        for (auto& tag : tags) tag.render();
    }

    void renderEntity(int entity_id)
    {
        debugRender<Transform>(entity_id);
        debugRender<Mesh>(entity_id);
        debugRender<Light>(entity_id);
        debugRender<Collider>(entity_id);
        debugRender<Rotator>(entity_id);
        debugRender<Faller>(entity_id);
        debugRender<Tag>(entity_id);
    }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if < I < sizeof...(Tp), void>::type
        renderComponents(std::tuple<Tp...>& t)
    {
        for (auto& c : std::get<I>(t))
            c.debugRender();
    }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if < I < sizeof...(Tp), void>::type
        updateComponents(std::tuple<Tp...>& t, float dt)
    {
        for (auto& c : std::get<I>(t))
            c.update(dt);
    }

    //creates a new component with no entity parent
    template<typename T>
    void debugRender(int entity_id) {

        if (getComponentID<T>(entity_id) != -1) {
            T & comp = getComponentFromEntity<T>(entity_id);
            comp.debugRender();
        }
    }

    //creates a new component with no entity parent
    template<typename T>
    int createComponent(){
        // get reference to vector
        vector<T>& the_vec = get<vector<T>>(components);
        // add a new object at back of vector
        the_vec.emplace_back();
        // return index of new object in vector
        return (int)the_vec->size() - 1;
    }
    
    //creates a new component and associates it with an entity
    template<typename T>
    T& createComponentForEntity(int entity_id){
        // get reference to vector
        vector<T>& the_vec = get<vector<T>>(components);
        // add a new object at back of vector
        the_vec.emplace_back();
        
        //get index type of ComponentType
        const int type_index = type2int<T>::result;
        
        //set index of entity component array to index of newly added component
        entities[entity_id].components[type_index] = (int)the_vec.size() - 1;
        
        //set owner of component to entity
        Component& new_comp = the_vec.back();
        new_comp.owner = entity_id;
        
        return the_vec.back(); // return pointer to new component
    }
    
    //return reference to component at id in array
    template<typename T>
    T& getComponentInArray(int an_id) {
        return get<vector<T>>(components)[an_id] ;
    }
    
    //return reference to component stored in entity
    template<typename T>
    T& getComponentFromEntity(int entity_id) {
        //get index for type
        const int type_index = type2int<T>::result;
        //get index for component
        const int comp_index = entities[entity_id].components[type_index];
        //return component from vector in tuple
        return get<vector<T>>(components)[comp_index];
    }

	//return reference to component stored in entity, accessed by name
	template<typename T>
	T& getComponentFromEntity(std::string entity_name) {
		//get entity id
		const int entity_id = getEntity(entity_name);
		//get index for type
		const int type_index = type2int<T>::result;
		//get index for component
		const int comp_index = entities[entity_id].components[type_index];
		//return component from vector in tuple
		return get<vector<T>>(components)[comp_index];
	}

    template<typename T>
    T& getSafeComponentFromEntity(std::string entity_name) {
        //get entity id
        const int entity_id = getEntity(entity_name);
        //get index for type
        const int type_index = type2int<T>::result;
        //get index for component
        const int comp_index = entities[entity_id].components[type_index];

        if (comp_index != -1)
            return get<vector<T>>(components)[comp_index];
        else {
            T* t = new T{};
            return *t;
        }
    }
    
    //return id of component in relevant array
    template<typename T>
    int getComponentID(int entity_id) {
        //get identifier of type
        const int type_index = type2int<T>::result;
        //return id of this component type for this
        return entities[entity_id].components[type_index];
    }
    
    //returns a const (i.e. non-editable) reference to vector of Type
    //i.e. array will not be editable
    template<typename T>
    std::vector<T>& getAllComponents() {
        return get<vector<T>>(components);
    }
    //stores main camera id
    int main_camera = -1;
    
};
