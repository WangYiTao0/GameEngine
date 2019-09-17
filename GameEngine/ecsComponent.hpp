#pragma once
#include <vector>
#include <tuple>

/*typedef void(*F)(int)
定义了一个指向函数的指针F，其返回值 void 类型，参数是后面的(int).
*/
typedef void* EntityHandle;
struct BaseECSComponent;
typedef unsigned int (*ECSComponentCreateFuction)(std::vector<unsigned int>& memory,
	EntityHandle entity, BaseECSComponent* comp);
typedef void(*ECSComponentFreeFunction)(BaseECSComponent* comp);

#define NULL_ENTITY_HANDLE nullptr;

struct BaseECSComponent
{
public:
	static unsigned int RegisterComponentType(ECSComponentCreateFuction createfn,
		ECSComponentFreeFunction freefn, size_t size);
	EntityHandle entityID = NULL_ENTITY_HANDLE;

	inline static ECSComponentCreateFuction GetTypeCreateFunction(unsigned int id)
	{
		return std::get<0>(componentTypes[id]);
	}
	inline static ECSComponentFreeFunction GetTypeFreeFunction(unsigned int id)
	{
		return std::get<1>(componentTypes[id]);
	}
	inline static size_t getTypeSize(unsigned int id)
	{
		return std::get<2>(componentTypes[id]);
	}
private:
	static std::vector<std::tuple<ECSComponentCreateFuction, ECSComponentFreeFunction, size_t>> componentTypes;
};

template<typename T>
struct ECSComponent : public BaseECSComponent
{
	static const ECSComponentCreateFuction CREATE_FUNCTION;
	static const ECSComponentFreeFunction FREE_FUNCTION;

	static const unsigned int ID;
	static const size_t SIZE;
};

template<typename Component>
unsigned int ECSComponentCreate(std::vector<unsigned char>& memory,
	EntityHandle entity, BaseECSComponent* comp)
{
	unsigned int index = memory.size();
	memory.resize(index + Component::SIZE);
	//new[] where to construct
	Component* component = new(&memory[index])Component(*(Component*)comp);
	component->entity = entity;
	return index;
}

template<typename Component>
void ECSComponentFree(BaseECSComponent* comp)
{
	Component* component = (Component*)comp;
	component->~Component();
}

template<typename T>
const unsigned int ECSComponent<T>::ID(BaseECSComponent::RegisterComponentType
(ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T)));

template<typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

template<typename T>
const ECSComponentCreateFuction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate <T>);

template<typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);

struct  TestComponent:public ECSComponent<TestComponent>
{
	float x;
	float y;
	//etc
};