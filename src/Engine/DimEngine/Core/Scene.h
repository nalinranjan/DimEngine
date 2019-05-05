#ifndef SCENE_H
#define SCENE_H
#pragma once

#include <vector>

#include "../Common/LinkedList.hpp"
#include "../Common/Stack.hpp"

#include "GameObject.h"
#include "IUpdatable.h"


namespace DimEngine
{
	class __declspec(dllexport) Scene : IUpdatable
	{
		friend class GameObject;


	private:
		static Scene* currentScene;
	

	public:
		static Scene* GetCurrentScene();

		static void UnloadAll();


	private:
		vector<GameObject*> roots;
		union
		{
			i32 flag;
			struct
			{
				bool isActive : 1;
				bool : 1;
				bool : 1;
				bool : 1;

				bool : 1;
				bool : 1;
				bool : 1;
				bool : 1;

				bool : 1;
				bool : 1;
				bool : 1;
				bool : 1;

				bool : 1;
				bool : 1;
				bool : 1;
				bool : 1;

				bool : 1;
				bool : 1;
				bool : 1;
				bool : 1;

				bool : 1;
				bool : 1;
				bool : 1;
				bool : 1;

				bool : 1;
				bool : 1;
				bool : 1;
				bool : 1;

				bool : 1;
				bool : 1;
				bool : 1;
				bool isDestroyed : 1;
			};
		};

		Scene();
		~Scene();

		size_t AddRootObject(GameObject* gameObject);
		bool RemoveRootObject(GameObject* gameObject);
	

	public:
		void Update(f32 deltaTime, f32 currentTime);
		void LateUpdate(f32 deltaTime, f32 currentTime);
	};
}
#endif