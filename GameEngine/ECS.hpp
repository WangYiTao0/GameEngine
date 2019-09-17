#pragma once
#include "ecsComponent.hpp"
#include "ecsSystem.hpp"
#include <map>


class ECS
{
public:
	ECS(){}
	~ECS();

	//entity methods
	EntityHandle makeEntity(BaseECSComponent* component, const unsigned int* componentIDs,
		size_t numComponents);
	//component methods
	template<class Component>
	void addComponent(EntityHandle entity, Component* component);

	template <class Component>
	void removeComponent(EntityHandle entity);

	template <class Component>
	void getComponent(EntityHandle entity);

	//system methods;
	inline void addSystem(BaseECSSystem& system)
	{
		systems.push_back(&system);
	}
	void updateSystems(float delta);
	void removeSystem(BaseECSSystem& system);


private:
	//no copy and assign
	ECS(const ECS& other) = delete;
	ECS operator=(const ECS& other) = delete;

	inline std::pair<unsigned int, std::vector < std::pair<unsigned int, unsigned int>>>* handleToRawType(EntityHandle handle)
	{
		(std::pair<unsigned int, std::vector<std::pair<unsigned int, unsigned int>>>*)handle;
	}

	inline unsigned int handleToEntityIndex(EntityHandle handle)
	{
		return handleToRawType(handle)->first;
	}

	inline std::vector<std::pair<unsigned int, unsigned int>>& handleToEnetity(EntityHandle handle)
	{
		return handleToRawType(handle)->second;
	}

private:
	std::vector<BaseECSSystem*> systems;
	std::map<unsigned int, std::vector<unsigned char>> component;
	std::vector<std::pair<unsigned int, std::vector<
		std::pair<unsigned int, unsigned int>>>*> entities;
};