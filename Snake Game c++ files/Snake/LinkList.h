#pragma once
#include "LinkListNode.h"
#include <iostream>

template<typename T>
class LinkList
{
private:
	LinkListNode<T>* head;
	LinkListNode<T>* tail;
	int length;
public:
	LinkList();
	~LinkList();
	void addFront(T data);
	void addEnd(T data);
	void removeEnd();
	LinkListNode<T>* const getEnd();
	LinkListNode<T>* const getFront();
	int const getLength();
	void emptyList();
};

template<typename T>
inline LinkList<T>::LinkList()
{
	head = nullptr;
	tail = nullptr;
	length = 0;
}

template<typename T>
inline LinkList<T>::~LinkList()
{
	LinkListNode<T>* current = head;
	LinkListNode<T>* next;
	while (current != nullptr)
	{
		next = current->next;
		delete current;
		current = next;
	}
}

template<typename T>
inline void LinkList<T>::addFront(T data)
{
	LinkListNode<T>* node = new LinkListNode<T>(data, head);
	if (head == nullptr)
	{
		tail = node;
	}
	head = node;
	length++;
}

template<typename T>
inline void LinkList<T>::addEnd(T data)
{
	LinkListNode<T>* node = new LinkListNode<T>(data, nullptr);
	if (head == nullptr)
	{
		head = node;
	}
	else
	{
		tail->next = node;
	}
	tail = node;
	length++;
}

template<typename T>
inline void LinkList<T>::removeEnd()
{
	if (head == nullptr)
	{
		std::cout << "Cannot remove from empty link list";
	}
	else if (head->next == nullptr) //Remove last element, making list empty
	{
		delete head;
		head = nullptr;
		tail = nullptr;
		length = 0;
	}
	else
	{
		LinkListNode<T>* current = head;
		while (current->next != tail)
		{
			current = current->next;
		}
		delete tail;
		current->next = nullptr;
		tail = current;
		length--;
	}
}

template<typename T>
inline LinkListNode<T>* const LinkList<T>::getEnd()
{
	return tail;
}

template<typename T>
inline LinkListNode<T>* const LinkList<T>::getFront()
{
	return head;
}

template<typename T>
inline int const LinkList<T>::getLength()
{
	return length;
}

template<typename T>
inline void LinkList<T>::emptyList()
{
	if (length > 0)
	{
		LinkListNode<T>* current = head;
		LinkListNode<T>* next = current->next;

		while (next != nullptr)
		{
			delete current;
			current = next;
			next = current->next;
		}
		delete current;
		head = nullptr;
		tail = nullptr;
		length = 0;
	}
}
