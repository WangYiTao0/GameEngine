#pragma once

#include "ecsComponent.hpp"
#include "ecsSystem.hpp"
#include <map>


class ECS
{
public:
	ECS(){}
	~ECS();
	//nocopy and assign for ecs
	ECS(const ECS&) = delete;
	ECS& operator=(const ECS&) = delete;
	ECS(const ECS&&) = delete;
	ECS& operator=(const ECS&&) = delete;

	// Entity methods
	EntityHandle MakeEntity(BaseECSComponent* components, const uint32* componentIDs, size_t numComponents);

	void RemoveEntity(EntityHandle handle);

	// Component methods
	template<class Component>
	inline void AddComponent(EntityHandle entity, Component* component)
	{
		AddComponentInternal(entity, HandleToEntity(entity), Component::ID, component);
	}

	template<class Component>
	void RemoveComponent(EntityHandle entity)
	{
		return RemoveComponentInternal(entity, Component::ID);
	}

	template<class Component>
	void GetComponent(EntityHandle entity)
	{
		GetComponentInternal(HandleToEntity(entity), Component::ID);
	}


	// System methods
	inline void addSystem(BaseECSSystem& system)
	{
		systems.push_back(&system);
	}
	void updateSystems(float delta);

	void removeSystem(BaseECSSystem& system);
private:
	

private:
	//system
	std::vector<BaseECSSystem*> systems;
	//component  map  ID   array of component
	std::map<uint32,std::vector<uint8>> components;
	//entity               index  ,   entity           ID componenttype,index
	std::vector <std::pair<uint32, std::vector<std::pair<uint32, uint32>>>* > entities;

	inline std::pair<uint32, std::vector<std::pair<uint32, uint32>>>* HandleToRawType(EntityHandle handle)
	{
		return static_cast<std::pair<uint32, std::vector<std::pair<uint32, uint32>>>*> (handle);
	}
	//entity index
	inline uint32 HandleToEntityIndex(EntityHandle handle)
	{
		return HandleToRawType(handle)->first;
	}
	//entity
	inline std::vector<std::pair<uint32, uint32> >& HandleToEntity(EntityHandle handle)
	{
		return HandleToRawType(handle)->second;
	}

	void DeleteComponent(uint32 componentID, uint32 index);

	bool RemoveComponentInternal(EntityHandle handle, uint32 componentID);

	void AddComponentInternal(EntityHandle handle,
		std::vector<std::pair<uint32, uint32> >& entity, 
		uint32 componentID, BaseECSComponent* component);

	BaseECSComponent* GetComponentInternal(std::vector<std::pair<uint32, uint32> >& entityComponents, uint32 componentID);


};


