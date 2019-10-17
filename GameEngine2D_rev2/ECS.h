#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static_assert (std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID(); //Code expands here and results in the currently last known ID being assigned to typeID
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32; //Layers; collision, render, whatever.

using ComponentBitSet = std::bitset<maxComponents>; //Allows us to find if an entity has a particular component out of a possible selection
using GroupBitset = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>; //Array of component pointers, size of 32

class ECS
{
public:
	ECS();
	~ECS();
};

class Component
{
public:
	Entity* entity;

	virtual void Init() {}
	virtual void Update() {}
	virtual void Draw() {}

	virtual ~Component() {}
};

class Entity
{
private:
	const char* tag;
	bool active = true;
	std::vector<std::unique_ptr<Component>> componentList;

	ComponentArray componentRA;
	ComponentBitSet componentBitset;
	GroupBitset groupBitset;

public:
	Manager& manager;

	Entity(Manager& mManager) : manager(mManager) {}

	void Update()
	{
		for (auto& c : componentList)
		{
			c->Update();
		}
	}

	void Draw()
	{
		for (auto& c : componentList) c->Draw();
	}

	bool isActive() { return active; }
	void Destroy()	{ active = false; }

	bool hasGroup(Group mGroup)
	{
		return groupBitset[mGroup];
	}

	void addToGroup(Group mGroup);

	void removeFromGroup(Group mGroup)
	{
		groupBitset[mGroup] = false;
	}

	template <typename T>
	bool hasComponent() const
	{
		return componentBitset[getComponentTypeID<T>()]; //Does this entity have this component attached?
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		componentList.emplace_back(std::move(uPtr));

		//When adding a component, will always put the component of a type in the same position for every entity which has it.
		componentRA[getComponentTypeID<T>()] = c;
		componentBitset[getComponentTypeID<T>()] = true;

		c -> Init();
		return *c;
	}

	//Sample usage: GameObject.getComponent()<PositionComponent>().setXpos(25);
	template <typename T>
	T& getComponent() const
	{
		auto ptr(componentRA[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	void setActive(bool state)
	{
		active = state;
	}

	void setTag(const char* t)
	{
		tag = t;
	}

	const char* getTag()
	{
		return tag;
	}

	bool isTagged()
	{
		return tag != NULL;
	}
};

class Manager
{
private:
	std::list<std::unique_ptr<Entity>> entitiesList; //Changed to list to eliminate access violation when vector reallocates after getting too large.
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
	void Update()
	{
		for (auto& e : entitiesList)
		{
			e->Update();
		}
	}

	void Draw()
	{
		for (auto& e : entitiesList)
		{
			e->Draw();
		}
	}

	///Removes all inactive entities when called
	void Refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity)
			{
				return !mEntity->isActive() || !mEntity->hasGroup(i);
			}),
				std::end(v));
		}

		entitiesList.erase(std::remove_if(std::begin(entitiesList), std::end(entitiesList), [](const std::unique_ptr<Entity> &mEntity)
		{
			return !mEntity->isActive();
		}),
			std::end(entitiesList));
	}

	void AddToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	///Instantiates a new object of type entity, adds it to the entitesList Vector object and returns a pointer to the new
	///Entity object for use by the calling function
	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entitiesList.emplace_back(std::move(uPtr));

		return *e;
	}
};