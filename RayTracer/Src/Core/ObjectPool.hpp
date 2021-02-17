#pragma once
#include <map>
#include <stack>

template<class T>
class ObjectPool
{
public:
	ObjectPool()
	{
		m_objectID = 0;
	}

	~ObjectPool()
	{
		for (auto kvp : m_objects)
		{
			T* obj = kvp.second;
			if (obj)
			{
				delete obj;
			}
		}
		m_objects.clear();
	}

	int Add(T* object)
	{
		int id = m_objectID;
		if (m_objects.find(id) == m_objects.end())
		{
			m_objects.insert(std::pair<int, T*>(id, object));
			m_objectID++;
			return id;
		}
		return -1;
	}

	T* Get(int objectID)
	{
		if (m_objects.find(objectID) == m_objects.end())
			return nullptr;
		return m_objects[objectID];
	}

	int GetID(T* object)
	{
		for (auto kvp : m_objects)
		{
			T* obj = kvp.second;
			if (obj && obj == object)
			{
				return kvp.first;
			}
		}
		return -1;
	}

	bool Delete(int objectID)
	{
		if (m_objects.find(objectID) == m_objects.end())
			return false;
		T* object = m_objects[objectID];
		m_objects.erase(objectID);
		delete object;
	}

private:
	std::map<int, T*> m_objects;

	int m_objectID;
};

//template<class T>
//class ObjectPoolStack
//{
//public:
//	ObjectPoolStack()
//	{
//	}
//
//	~ObjectPoolStack()
//	{
//		while (m_objects.size() > 0)
//		{
//			T* obj = m_objects.top();
//			if (obj)
//				delete obj;
//			m_objects.pop();
//		}
//	}
//
//	void Push(T* object)
//	{
//		m_objects.push(object);
//	}
//
//	T* Pop()
//	{
//		if (m_objects.size() == 0)
//			return nullptr;
//		T* obj = m_objects.top();
//		m_objects.pop();
//		return obj;
//	}
//
//private:
//	std::stack<T*> m_objects;
//};
//
//template<class T>
//class ObjectPoolForCache
//{
//public:
//	ObjectPoolForCache()
//	{
//	}
//
//	~ObjectPoolForCache()
//	{
//		for (auto kvp : m_objects)
//		{
//			T* obj = kvp.second;
//			if (obj)
//			{
//				delete obj;
//			}
//		}
//		m_objects.clear();
//	}
//
//	void Push(T* object, unsigned long key)
//	{
//		ObjectPoolStack<T>* stack = nullptr;
//		if (m_objects.find(key) == m_objects.end())
//		{
//			m_objects.insert(std::pair<unsigned long, ObjectPoolStack<T>*>(key, new ObjectPoolStack<T>()));
//		}
//		stack = m_objects[key];
//		stack->Push(object);
//	}
//
//	T* Pop(unsigned long key)
//	{
//		if (m)
//	}
//
//private:
//	std::map<unsigned long, ObjectPoolStack<T>*> m_objects;
//};