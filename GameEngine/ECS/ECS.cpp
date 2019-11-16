#include "ECS.hpp"
#include <algorithm>

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

void ECS::UpdateSystems(float delta)
{
	std::vector<BaseECSComponent*> componentParam;
	std::vector<std::vector<uint8>*> componentArrays;
	for (uint32 i = 0; i < systems.size(); i++) {
		const std::vector<uint32>& componentTypes = systems[i]->getComponentTypes();
		if (componentTypes.size() == 1) {
			size_t typeSize = BaseECSComponent::GetTypeSize(componentTypes[0]);
			std::vector<uint8>& array = components[componentTypes[0]];
			for (uint32 j = 0; j < array.size(); j += typeSize) {
				BaseECSComponent* component = (BaseECSComponent*) & array[j];
				systems[i]->updateComponents(delta, &component);
			}
		}
		else {
			UpdateSystemWithMultipleComponents(i, delta, componentTypes, componentParam, componentArrays);
		}
	}
}

uint32 ECS::FindLeastCommonComponent(const std::vector<uint32>& componentTypes)
{
	uint32 minSize = components[componentTypes[0]].size()
		/ BaseECSComponent::GetTypeSize(componentTypes[0]);
	uint32 minIndex = 0;
	for (uint32 i = 1; i < componentTypes.size(); i++) {
		size_t typeSize = BaseECSComponent::GetTypeSize(componentTypes[i]);
		uint32 size = components[componentTypes[i]].size() / typeSize;
		if (size < minSize) {
			minSize = size;
			minIndex = i;
		}
	}

	return minIndex;
}

void ECS::UpdateSystemWithMultipleComponents(uint32 index, float delta,
	const std::vector<uint32>& componentTypes, std::vector<BaseECSComponent*>& componentParam,
	std::vector<std::vector<uint8>*>& componentArrays)
{
	componentParam.resize(std::max(componentParam.size(), componentTypes.size()));
	componentArrays.resize(std::max(componentArrays.size(), componentTypes.size()));
	for (uint32 i = 0; i < componentTypes.size(); i++) {
		componentArrays[i] = &components[componentTypes[i]];
	}
	uint32 minSizeIndex = FindLeastCommonComponent(componentTypes);

	size_t typeSize = BaseECSComponent::GetTypeSize(componentTypes[minSizeIndex]);
	std::vector<uint8>& array = *componentArrays[minSizeIndex];
	for (uint32 i = 0; i < array.size(); i += typeSize) {
		componentParam[minSizeIndex] = (BaseECSComponent*) & array[i];
		std::vector<std::pair<uint32, uint32> >& entityComponents =
			HandleToEntity(componentParam[minSizeIndex]->entity);

		bool isValid = true;
		for (uint32 j = 0; j < componentTypes.size(); j++) {
			if (j == minSizeIndex) {
				continue;
			}

			componentParam[j] = GetComponentInternal(entityComponents,
				*componentArrays[j], componentTypes[j]);
			if (componentParam[j] == nullptr) {
				isValid = false;
				break;
			}
		}

		if (isValid) {
			systems[index]->updateComponents(delta, &componentParam[0]);
		}
	}
}

bool ECS::RemoveSystem(BaseECSSystem& system)
{
	for (uint32 i = 0; i < systems.size(); i++) {
		if (&system == systems[i]) {
			systems.erase(systems.begin() + i);
			return true;
		}
	}
	return false;
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

BaseECSComponent* ECS::GetComponentInternal(std::vector<std::pair<uint32, uint32> >& entityComponents, std::vector<uint8>& array, uint32 componentID)
{
	for (uint32 i = 0; i < entityComponents.size(); i++) {
		if (componentID == entityComponents[i].first) {
			return (BaseECSComponent*) & array[entityComponents[i].second];
		}
	}
	return nullptr;
}

