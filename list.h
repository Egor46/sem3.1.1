#pragma once
#include <string>

using namespace std;

struct variable {
	string name = "";
	int value = 0;
};

bool operator==(variable v1, variable v2) {
	return v1.name == v2.name;
}

struct el {
	el* next;
	variable var;
};

void add(el* top, variable v) {
	while (top->next != nullptr) {
		top = top->next;
		if (top->var == v) {
			top->var = v;
			return;
		}
	}
	top->next = new el{ nullptr, v };
}

void printl(el* top) {
	while (top->next) {
		top = top->next;
		cout << top->var.name << ' ' << top->var.value << endl;
	}
}

bool findX(el* top, string x) {
	while (top) {
		if (top->var.name == x) return true;
		top = top->next;
	}
	return false;
}

variable getX(el* top, string x) {
	while (top->var.name != x) top = top->next;
	return top->var;
}

void delL(el* top) {
	if (!top) return;
	delL(top->next);
	delete top;
}