#pragma once
#include <vector>
template<typename T>
class ObjectPool
{
private:
	std::vector<T> objects;
public:
	ObjectPool();
	T& createObject();
	std::vector<T>& getObjects();
};

template<typename T>
inline ObjectPool<T>::ObjectPool()
{
	objects = std::vector<T>();
}

template<typename T>
inline T& ObjectPool<T>::createObject()
{
	objects.emplace_back();
	return objects.back();
}

template<typename T>
inline std::vector<T>& ObjectPool<T>::getObjects()
{
	return objects;
}
