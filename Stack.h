#pragma once
#include <iostream>
using namespace std;
#define thesize 500
template <class T>
class MyStack
{
	int top;
	T *arr = new T[thesize];
public:

	MyStack() { top = -1; }
	void push(T val);
	T pop();
	T gettop();
	bool isEmpty();
	bool isFull();

};


