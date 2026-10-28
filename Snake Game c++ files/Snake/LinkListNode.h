#pragma once
template<typename T>
class LinkListNode
{
public:
	T data;
	LinkListNode* next;
	LinkListNode(T data, LinkListNode<T>* next);
};

template<typename T>
inline LinkListNode<T>::LinkListNode(T data, LinkListNode<T>* next)
{
	this->data = data;
	this->next = next;
}
