#pragma once

#include "ecsComponent.hpp"
#include "ecsSystem.hpp"
#include <map>
#include <unordered_map>


class ECS
{
public:
	ECS(){}
	~ECS();
	//nocopy and assign for ecs
	ECS(const ECS&) = delete;
	ECS& operator=(const ECS&) = delete;
	ECS(const ECS&&) = delete;
	ECS& operator=(const ECS&&) = delete;

	// Entity methods
	EntityHandle MakeEntity(BaseECSComponent** components, const uint32* componentIDs, size_t numComponents);

	void RemoveEntity(EntityHandle handle);


	template<class A>
	EntityHandle makeEntity(A& c1)
	{
		BaseECSComponent* components[] = { &c1 };
		uint32 componentIDs[] = { A::ID };
		return makeEntity(components, componentIDs, 1);
	}

	template<class A, class B>
	EntityHandle makeEntity(A& c1, B& c2)
	{
		BaseECSComponent* components[] = { &c1, &c2 };
		uint32 componentIDs[] = { A::ID, B::ID };
		return makeEntity(components, componentIDs, 2);
	}

	template<class A, class B, class C>
	EntityHandle makeEntity(A& c1, B& c2, C& c3)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID };
		return makeEntity(components, componentIDs, 3);
	}

	template<class A, class B, class C, class D>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID };
		return makeEntity(components, componentIDs, 4);
	}

	template<class A, class B, class C, class D, class E>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID };
		return makeEntity(components, componentIDs, 5);
	}

	template<class A, class B, class C, class D, class E, class F>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID };
		return makeEntity(components, componentIDs, 6);
	}

	template<class A, class B, class C, class D, class E, class F, class G>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID };
		return makeEntity(components, componentIDs, 7);
	}

	template<class A, class B, class C, class D, class E, class F, class G, class H>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID };
		return makeEntity(components, componentIDs, 8);
	}

	template<class A, class B, class C, class D, class E, class F, class G, class H, class I>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8, I& c9)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID, I::ID };
		return makeEntity(components, componentIDs, 9);
	}

	template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8, I& c9, J& c10)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9, &c10 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID, I::ID, J::ID };
		return makeEntity(components, componentIDs, 10);
	}



	// Component methods
	template<class Component>
	inline void AddComponent(EntityHandle entity, Component* component)
	{
		AddComponentInternal(entity, HandleToEntity(entity), Component::ID, component);
	}

	template<class Component>
	void RemoveComponent(EntityHandle entity)
	{
		return RemoveComponentInternal(entity, Component::ID);
	}

	template<class Component>
	Component* GetComponent(EntityHandle entity)
	{
		return (Component*)GetComponentInternal(HandleToEntity(entity), components[Component::ID], Component::ID);
	}


	// System methods
	void UpdateSystems(ECSSystemList& systems, float delta);
private:
	

private:
	//system
	std::vector<BaseECSSystem*> systems;
	//component  map  ID   array of component
	std::unordered_map<uint32,std::vector<uint8>> components;
	//entity               index  ,   entity           ID componenttype,index
	std::vector <std::pair<uint32, std::vector<std::pair<uint32, uint32>>>* > entities;

	inline std::pair<uint32, std::vector<std::pair<uint32, uint32>>>* HandleToRawType(EntityHandle handle)
	{
		return static_cast<std::pair<uint32, std::vector<std::pair<uint32, uint32>>>*> (handle);
	}
	//entity index
	inline uint32 HandleToEntityIndex(EntityHandle handle)
	{
		return HandleToRawType(handle)->first;
	}
	//entity
	inline std::vector<std::pair<uint32, uint32> >& HandleToEntity(EntityHandle handle)
	{
		return HandleToRawType(handle)->second;
	}

	void DeleteComponent(uint32 componentID, uint32 index);

	bool RemoveComponentInternal(EntityHandle handle, uint32 componentID);

	void AddComponentInternal(EntityHandle handle,
		std::vector<std::pair<uint32, uint32> >& entity, 
		uint32 componentID, BaseECSComponent* component);

	//BaseECSComponent* GetComponentInternal(std::vector<std::pair<uint32, uint32> >& entityComponents, uint32 componentID);

	BaseECSComponent* GetComponentInternal(
		std::vector<std::pair<uint32, uint32> >& entityComponents,
		std::vector<uint8>& array, uint32 componentID);

	void UpdateSystemWithMultipleComponents(uint32 index, 
		ECSSystemList& systems,
		float delta, 
		const std::vector<uint32>& componentTypes,
		std::vector<BaseECSComponent*>& componentParam, 
		std::vector<std::vector<uint8>*>& componentArrays);

	uint32 FindLeastCommonComponent(
		const std::vector<uint32>& componentTypes,
		const std::vector<uint32>& componentFlags);
};


