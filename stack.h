#pragma once
#include <iostream>
using namespace std;

struct elem {
	elem* prev;
	int c;
};

typedef elem* stack;

bool isEmpty(stack s) {
	return s == nullptr;
}

stack create() {
	return nullptr;
}

void push(stack& top, int a) {
	stack t = new elem{ top, a };
	top = t;
}

int pop(stack& top) {
	stack q = top;
	top = top->prev;
	int info = q->c;
	delete q;
	return info;
}

int peek(stack top) {
	if (top) {
		return top->c;
	}
	else {
		cout << "stack is empty";
		return 0;
	}
}

void del(stack& a) {
	if (a) del(a->prev);
	else return;
}

int getCount(stack top) {
	stack p = top;
	int c = 0;
	while (p) {
		c++;
		p = p->prev;
	}
	return c;
}