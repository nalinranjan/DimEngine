#pragma once

//#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "GameComponent.h"
#include "Transform.h"

namespace DimEngine
{
	class __declspec(dllexport) GameObject final : public Transform, private IUpdatable
	{
		friend class Scene;


	private:
		bool isActive;
		bool isStatic;
		unordered_map<type_index, GameComponent*> components;


	public:
		GameObject();
		GameObject(float x, float y, float z);
		GameObject(XMVECTOR position, XMVECTOR rotation, XMVECTOR scale);
		~GameObject();

		bool IsActive();
		bool IsActiveSelf();
		bool IsStatic();
		GameObject* GetParent();
		GameObject* GetChild(size_t index);
		template<class T> T* GetComponent();

		void SetParent(GameObject* other);
		template<class T, class... argTs> T* AddComponent(argTs... args);
		template<class T> void RemoveComponent();

		void SendMessage_(const Message& message);
		void SendMessageUp(const Message& message, u32 level = UINT_MAX);
		void SendMessageDown(const Message& message, u32 level = UINT_MAX);


	protected:
		void Update(f32 deltaTime, f32 currentTime);
		void LateUpdate(f32 deltaTime, f32 currentTime);


	private:
		void HandleMessage(const Message& message) { }
	};


	template<class T> __inline T* GameObject::GetComponent()
	{
		type_index id = typeid(T);

		return static_cast<T*>(components.find(id) == components.end() ? nullptr : components[id]);
	}

	template<class T, class ...argTs> __inline T* GameObject::AddComponent(argTs ...args)
	{
		//assert(std::is_base_of<GameComponent, T>::value)

		type_index id = typeid(T);

		if (components.find(id) != components.end())
			throw "Cannot add duplicated components!";

		T* component = new T(args...);
		static_cast<GameComponent*>(component)->isActive = true;
		static_cast<GameComponent*>(component)->gameObject = this;

		components[id] = component;

		return component;
	}

	template<class T> __inline void GameObject::RemoveComponent()
	{
		type_index id = typeid(T);

		auto it = components.find(id);

		if (it != components.end())
			components.erase(it);
	}
}
