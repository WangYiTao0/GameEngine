#pragma once
#include "..\\Common.h"
#include <vector>
#include <tuple>

#define NULL_ENTITY_HANDLE nullptr

/*typedef void(*F)(int)
defines a pointer f to a function, whose return value is void type and the parameter is (int)
*/
struct BaseECSComponent;

typedef void* EntityHandle;

typedef uint32(*ECSComponentCreateFunction)(std::vector<uint8>& memory, EntityHandle entity, BaseECSComponent* comp);

typedef void (*ECSComponentFreeFunction)(BaseECSComponent* comp);

struct BaseECSComponent
{
	static uint32 nextID();
	EntityHandle entity = NULL_ENTITY_HANDLE;
};

template<typename T>
struct  ECSComponent : public BaseECSComponent
{
	//CreateFunction
	static const ECSComponentCreateFunction CREATE_FUNCTION;
	//FreeFunction
	static const ECSComponentFreeFunction FREE_FUNCTION;
	//ID
	static const uint32 ID;
	//Type size
	static const size_t SIZE;
};
template<typename Component>
uint32 ECSComponentCreate(std::vector<uint8>& memory, EntityHandle entity, BaseECSComponent* comp)
{
	uint32 index = memory.size();
	memory.resize(index + Component::SIZE);
	//                 new() set memory addr      *dereference to call copy construct  Cast to pointer Type name T 
	Component* component = new(&memory[index]) Component(*(Component*)comp);
	component->entity = entity;
	return index;
}

template<typename Component>
void ECSComponentFree(BaseECSComponent* comp)
{
	Component* component = (Component*)comp;
	//manually calling the destructor
	component->~Component();
}

template<typename T>
const uint32 ECSComponent<T>::ID(BaseECSComponent::nextID());

template<typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

template<typename T>
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

template<typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);

struct TestComponent : public ECSComponent<TestComponent>
{
	//get static num
	float x;
	float y;
	//etc
};