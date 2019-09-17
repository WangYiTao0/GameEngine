#include "ecsComponent.hpp"

std::vector<std::tuple<ECSComponentCreateFuction,
	ECSComponentFreeFunction, size_t>> BaseECSComponent::componentTypes;

unsigned int BaseECSComponent::RegisterComponentType(ECSComponentCreateFuction createfn, ECSComponentFreeFunction freefn, size_t size)
{
	unsigned int componentID = componentTypes.size();
	componentTypes.push_back(std::tuple<ECSComponentCreateFuction,
		ECSComponentFreeFunction, size_t>(createfn, freefn, size));
	return componentID++;
}
