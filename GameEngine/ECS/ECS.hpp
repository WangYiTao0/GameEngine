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
	void addComponent(EntityHandle entity, Component* component);

	template<class Component>
	void removeComponent(EntityHandle entity);

	template<class Component>
	void getComponent(EntityHandle entity);


	// System methods
	inline void addSystem(BaseECSSystem& system)
	{
		systems.push_back(&system);
	}
	void updateSystems(float delta);

	void removeSystem(BaseECSSystem& system);
private:
	

private:
	std::vector<BaseECSSystem*> systems;

	std::map<uint32,std::vector<uint8>> components;

	std::vector <std::pair<uint32, std::vector<std::pair<uint32, uint32>>>*> entities;

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

};


