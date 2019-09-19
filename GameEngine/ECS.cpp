#include "ECS.hpp"

ECS::~ECS()
{
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		size_t typeSize = BaseECSComponent::getTypeSize(it->first);
		auto freefn = BaseECSComponent::GetTypeFreeFunction(it->first);
		for (unsigned int i = 0; i < it->second.size(); i += typeSize)
		{
			freefn((BaseECSComponent*)& it->second[i]);
		}
	}

	for (unsigned int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}
}

//EntityHandle ECS::makeEntity(BaseECSComponent* component, const unsigned int* componentIDs, size_t numComponents)
//{
//	auto *newEntity = new std::pair<unsigned int, 
//		std::vector<unsigned int,std::pair<unsigned int, unsigned int>>>();
//	EntityHandle handle = (EntityHandle)newEntity;
//	for (unsigned int i = 0; i < numComponents; i++)
//	{
//		if (!BaseECSComponent::isTypeValid(componentIDs[i]))
//		{
//			//DeBugLog
//			delete newEntity;
//			return NULL_ENTITY_HANDLE;
//		}
//
//		auto createfn = BaseECSComponent::GetTypeCreateFunction(componentIDs[i]);
//		std::pair<unsigned int, unsigned int> newPair;
//		newPair.first = componentIDs[i];
//		newPair.second = createfn(components[componentIDs[i]], handle, &entityComponents[i]);
//		newEntity->second.push_back(newPair);
//	}
//
//	return handle;
//}
//
//void ECS::removeEntity(EntityHandle handle)
//{
//	std::vector<std::pair<unsigned int,unsigned int>>& entity = handleToEntity(handle)
//}