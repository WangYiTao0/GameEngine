#pragma once

#include "ecsComponent.hpp"

class BaseECSSystem
{
public:
	BaseECSSystem(const std::vector<unsigned int>& componentTypesIn)
		:componentTypes(componentTypesIn){}
	virtual void updateComponents(float delta,BaseECSComponent** component){}
	const std::vector<unsigned int>& getComponentTypes()
	{
		return componentTypes;
	}
private:
	std::vector<unsigned int> componentTypes;
};