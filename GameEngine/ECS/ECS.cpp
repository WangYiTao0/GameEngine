#include "ECS.hpp"


ECS::~ECS()
{
	for (std::map < uint32, std::vector<uint8>>::iterator it = components.begin(); it != components.end(); ++it)
	{
		size_t typeSize = BaseECSComponent::GetTypeSize(it->first);
		//free
		ECSComponentFreeFunction freefn = BaseECSComponent::GetTypeFreeFunction(it->first);

		for (uint32 i = 0; i < it->second.size(); i += typeSize) 
		{
			freefn((BaseECSComponent*)&it->second[i]);
		}
		
	}
	for (uint32 i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
}

EntityHandle ECS::MakeEntity(BaseECSComponent* entityComponents, const uint32* componentIDs, size_t numComponents)
{
	std::pair<uint32, std::vector<std::pair<uint32, uint32> > >* newEntity = new std::pair<uint32, std::vector<std::pair<uint32, uint32> > >();

	EntityHandle handle = (EntityHandle)newEntity;

	for (uint32 i = 0; i < numComponents; i++) {
		if (!BaseECSComponent::IsTypeValid(componentIDs[i])) 
		{
			DEBUG_LOG("ECS", LOG_ERROR, "'%u' is not a valid component type.", componentIDs[i]);
			delete newEntity;
			return NULL_ENTITY_HANDLE;
		}
		ECSComponentCreateFunction createfn = BaseECSComponent::GetTypeCreateFunction(componentIDs[i]);
		std::pair<uint32, uint32> newPair;
		newPair.first = componentIDs[i];
		newPair.second = createfn(components[componentIDs[i]], handle, &entityComponents[i]);
		newEntity->second.push_back(newPair);
	}
	newEntity->first = entities.size();
	entities.push_back(newEntity);

	return handle;
}

void ECS::RemoveEntity(EntityHandle handle)
{
	auto& entity = HandleToEntity(handle);
	for (uint32 i = 0; i < entity.size(); i++) {
		removeComponentInternal(entity[i].first, entity[i].second);
	}

	uint32 destIndex = HandleToEntityIndex(handle);
	uint32 srcIndex = entities.size() - 1;
	delete entities[destIndex];
	entities[destIndex] = entities[srcIndex];
	entities.pop_back();
}

void ECS::updateSystems(float delta)
{
}

void ECS::removeSystem(BaseECSSystem& system)
{
}
