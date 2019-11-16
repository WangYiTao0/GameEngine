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

		AddComponentInternal(handle, newEntity->second, componentIDs[i], &entityComponents[i]);
	}
	newEntity->first = entities.size();
	entities.push_back(newEntity);

	return handle;
}

void ECS::RemoveEntity(EntityHandle handle)
{
	auto& entity = HandleToEntity(handle);
	for (uint32 i = 0; i < entity.size(); i++) {
		DeleteComponent(entity[i].first, entity[i].second);
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

void ECS::DeleteComponent(uint32 componentID, uint32 index)
{
	std::vector<uint8>& array = components[componentID];

	ECSComponentFreeFunction freefn = BaseECSComponent::GetTypeFreeFunction(componentID);
	size_t typeSize = BaseECSComponent::GetTypeSize(componentID);
	uint32 srcIndex = array.size() - typeSize;

	BaseECSComponent* destComponent = (BaseECSComponent*) & array[index];
	BaseECSComponent* srcComponent = (BaseECSComponent*) & array[srcIndex];
	freefn(destComponent);

	if (index == srcIndex) {
		array.resize(srcIndex);
		return;
	}
	//update entity
	memcpy(destComponent, srcComponent, typeSize);

	std::vector<std::pair<uint32, uint32> >& srcComponents = HandleToEntity(srcComponent->entity);

	for (uint32 i = 0; i < srcComponents.size(); i++) {
		if (componentID == srcComponents[i].first && srcIndex == srcComponents[i].second) {
			srcComponents[i].second = index;
			break;
		}
	}

	array.resize(srcIndex);
}

bool ECS::RemoveComponentInternal(EntityHandle handle, uint32 componentID)
{
	std::vector<std::pair<uint32, uint32> >& entityComponents = HandleToEntity(handle);
	for (uint32 i = 0; i < entityComponents.size(); i++) {
		if (componentID == entityComponents[i].first) {
			DeleteComponent(entityComponents[i].first, entityComponents[i].second);
			uint32 srcIndex = entityComponents.size() - 1;
			uint32 destIndex = i;
			entityComponents[destIndex] = entityComponents[srcIndex];
			entityComponents.pop_back();
			return true;
		}
	}
	return false;
}

void ECS::AddComponentInternal(EntityHandle handle, std::vector<std::pair<uint32, uint32>>& entity, uint32 componentID, BaseECSComponent* component)
{
	ECSComponentCreateFunction createfn = BaseECSComponent::GetTypeCreateFunction(componentID);
	std::pair<uint32, uint32> newPair;
	newPair.first = componentID;
	newPair.second = createfn(components[componentID], handle, component);
	entity.push_back(newPair);
}


BaseECSComponent* ECS::GetComponentInternal(std::vector<std::pair<uint32, uint32> >& entityComponents, uint32 componentID)
{
	for (uint32 i = 0; i < entityComponents.size(); i++) {
		if (componentID == entityComponents[i].first) {
			return (BaseECSComponent*)&components[componentID][entityComponents[i].second];
		}
	}
	return nullptr;
}

