#pragma once
#include "..\\Common.hpp"
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
public:
	static uint32 registerComponentType(ECSComponentCreateFunction createfn,
		ECSComponentFreeFunction freefn, size_t size);
	EntityHandle entity = NULL_ENTITY_HANDLE;

	inline static ECSComponentCreateFunction GetTypeCreateFunction(uint32 id)
	{
		return std::get<0>((*componentTypes)[id]);
	}

	inline static ECSComponentFreeFunction GetTypeFreeFunction(uint32 id)
	{
		return std::get<1>((*componentTypes)[id]);
	}

	inline static size_t GetTypeSize(uint32 id)
	{
		return std::get<2>((*componentTypes)[id]);
	}

	inline static bool IsTypeValid(uint32 id)
	{
		return id < componentTypes->size();
	}

private:
	static std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>*  componentTypes;
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
const uint32 ECSComponent<T>::ID(BaseECSComponent::registerComponentType(ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T)));

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