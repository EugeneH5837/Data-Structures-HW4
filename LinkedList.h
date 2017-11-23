#pragma once
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <string.h>
#include <sstream>
using namespace std;
template <class T>
struct Node {
public:
	T info;
	Node<T> *next;
	Node<T> *prev;
	Node();
	Node(T data) : info(data) {
		next = NULL;
		prev = NULL;
	}
};

template <class T>
class LinkedList {
	
public:
	
	std::string name;
	std::string getname();
	Node<T> *head;
	Node<T> *tail;
	int count = 0;
	void insertion(T a);
	LinkedList();
	void destroylist();
	void putwordsintolist(std::string filename);
	void overwriteLL(LinkedList<T> &otherlist);
	void display();
	void writetofile(std::string filename);
	void copylist(LinkedList<T> &otherlist);
	LinkedList<T>& operator=(LinkedList<T> &otherlist);
	
};

template<class T>
inline Node<T>::Node()
{
	next = NULL;
	prev = NULL;
	
}
