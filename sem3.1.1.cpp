#include "stack.h"
#include "list.h"
#include <fstream>

using namespace std;

const int IDENTIFIER_MAX_NAME = 5;
const int MAX_NUMBER_LENGTH = 5;
const int MAX_OPERATORS = 5;
static int current_operators = 0;

inline bool isLetter(char c) {
	return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z';
}

inline bool isOperator(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}

inline bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

int state = 0;

el* global = new el{ nullptr, {"", 0} };

string errorVariable;

void handleException(int n) {
	switch (state) {
	case -1:
		cout << "Wrong identifier name in assigment operator";
		break;
	case -2:
		cout << "Identifier too long";
		break;
	case -3:
		cout << "Unexpected symbol in assignment operator. Should be :=";
		break;
	case -4:
		cout << "Expression should end with ;";
		break;
	case -5:
		cout << "Wrond number form";
		break;
	case -6:
		cout << "Too much operations";
		break;
	case -7:
		cout << "Wrong operand in expression";
		break;
	case -8:
		cout << "Got no operator after left operand";
		break;
	case -9:
		cout << "Expression ended wrong";
		break;
	case -10:
		cout << "Variable {" << errorVariable << "} not found";
		break;
	case -100:
		cout << "Unexpected EOF";
		break;
	case -99:
		cout << "Runtime error : Zero division" << endl; return;
	default:
		cout << "Unexpected error(possibly EOF)";
	}
	cout << " in instruction " << n << endl;
	cout << "[DEBUG] state = " << state << endl;
}

bool isOperand(ifstream& in, char& c, int& value) {
	string s = "";
	if (c == '+' || c == '-') {
		int m = c == '+' ? 1 : -1;
		int t = 0, mult = pow(10, MAX_NUMBER_LENGTH - 1);
		for (int i = 0; i < MAX_NUMBER_LENGTH; i++) {
			if (in.get(c)) {
				if (!isDigit(c)) {
					state = -5;
					return false;
				}
				t += (c - '0') * mult;
				mult /= 10;
			}
			else {
				state = -100;
				return false;
			}
		}
		if (!in.get(c)) {
			state = -100;
			return false;
		}
		value = t * m;
		return true;
	}
	else if (isLetter(c)) {
		s += c;
		int a = 1;
		while (a <= IDENTIFIER_MAX_NAME) {
			if (in.get(c)) {
				if (isLetter(c) || isDigit(c)) {
					a++; s += c;
				}
				else {
					break;
				}
			}
			else {
				state = -100;
				return false;
			}
		}
		if (a > IDENTIFIER_MAX_NAME) {
			state = -2;
			return false;
		}
		if (findX(global, s)) {
			value = getX(global, s).value;
			return true;
		}
		else {
			state = -10;
			errorVariable = s;
			return false;
		}
	}
	state = -7;
	return false;
}

//bool isExpression(ifstream& in, char& c) {
//	int temp;
//	if (c == '(') {
//
//		if (in.get(c)) {
//
//			if (c == '(') {
//				if (isExpression(in, c)) {
//					if (in.get(c)) {
//						if (isOperator(c)) {
//							if (in.get(c)) {
//								if (c == '(') {
//									if (isExpression(in, c)) {
//										if (in.get(c)) {
//											if (c == ')') {
//												return true;
//											}
//											else {
//												return false;
//											}
//										}
//										else {
//											return false;
//										}
//									}
//									else {
//										return false;
//									}
//								}
//								else if (isOperand(in, c, temp)) {
//									if (c == ')') {
//										return true;
//									}
//									else {
//										return false;
//									}
//								}
//								else return false;
//							}
//							else {
//								return false;
//							}
//						}
//						else {
//							return false;
//						}
//					}
//					else {
//						return false;
//					}
//				}
//			}
//			else if (isOperand(in, c, temp)) {
//				if (isOperator(c)) {
//					if (in.get(c)) {
//						if (c == '(') {
//							if (isExpression(in, c)) {
//								if (in.get(c)) {
//									if (c == ')') {
//										return true;
//									}
//									else {
//										return false;
//									}
//								}
//								else {
//									return false;
//								}
//							}
//							else {
//								return false;
//							}
//						}
//						else if (isOperand(in, c, temp)) {
//							if (c == ')') {
//								return true;
//							}
//							else {
//								return false;
//							}
//						}
//						else return false;
//					}
//					else {
//						return false;
//					}
//				}
//				else {
//					return false;
//				}
//			}
//			else {
//				return false;
//			}
//
//		}
//
//		else {
//			return false;
//		}
//	}
//}

bool isExpression(ifstream& in, char& c) {
	int temp;
	if (c == '(') {
		if (in.get(c)) {
			if (isExpression(in, c)) {
				if (isOperator(c)) {
					current_operators++;
					if (current_operators > MAX_OPERATORS) {
						state = -6;
						return false;
					}
					if (in.get(c)) {
						{
							if (c == '(') {
								if (in.get(c)) {
									if (isExpression(in, c)) {
										if (c == ')') {
											if (in.get(c)) {
												return true;
											}
											else {
												state = -100;
												return false;
											}
										}
										else {
											state = -9;
											return false;
										}
									}
									else {
										return false;
									}
								}
								else {
									state = -100;
									return false;
								}
							}
							else if (isOperand(in, c, temp)) {
								if (c == ')') { if (in.get(c)) return true; else { state = -100; return false; }; }
								else {
									state = -9;
									return false;
								}
							}
							else {
								state = -7;
								return false;
							}
						}
					}
					else {
						state = -100;
						return false;
					}
				}
				else state = -8;
			}
		}
		else {
			state = -100;
			return false;
		}
	}
	else if (isOperand(in, c, temp)) {
		if (isOperator(c)) {
			current_operators++;
			if (current_operators > MAX_OPERATORS) {
				state = -6;
				return false;
			}
			if (in.get(c)) {
				if (c == '(') {
					if (in.get(c)) {
						if (isExpression(in, c)) {
							if (c == ')') {
								if (in.get(c)) {
									return true;
								}
								else {
									state = -100;
									return false;
								}
							}
							else {
								state = -9;
								return false;
							}
						}
						else {
							return false;
						}
					}
					else {
						state = -100;
						return false;
					}
				}
				else if (isOperand(in, c, temp)) {
					if (c == ')') {
						if (in.get(c)) return true;
						else {
							state = -100;
							return false;
						}
					}
					else {
						state = -9;
						return false;
					}
				}
				else {
					state = -7;
					return false;
				}
			}
			else {
				state = -100;
				return false;
			}
		}
		else state = -8;
		return false;
	}
	else {
		state = -7;
		return false;
	}
}

bool checkProgram(ifstream& in) {
	char c = in.get();
	int instruction = 1;
	int temp;
	string s = ""; int a = 0;
	while (!in.eof() && state >= 0) {
		switch (state) {
		case 0:
			if (!isLetter(c)) {
				state = -1;
				break;
			}
			else {
				s += c; a++;
				state = 1;
				break;
			}
		case 1:
			if (isLetter(c) || isDigit(c)) {
				if (a > IDENTIFIER_MAX_NAME) {
					state = -2;
					break;
				}
				a++;
				s += c;
				break;
			}
			if (c == ':') {
				state = 2;
				break;
			}
			else {
				state = -2;
				break;
			}
		case 2:
			if (c == '=') {
				state = 3;
				add(global, { s,0 });
				s = ""; a = 0;
				break;
			}
			else {
				state = -3;
				break;
			}
		case 3:
			if (c == '(') {
				if (!in.get(c)) {
					state = -100;
					break;
				}
				if (isExpression(in, c)) {
					if (c == ';') {
						instruction++;
						state = 0;
						break;
					}
					else {
						state = -4;
						break;
					}
				}
				else {
					break;
				}
			}
			else if (isOperand(in, c, temp)) {
				if (c == ';') {
					instruction++;
					state = 0;
					break;
				}
				else {
					state = -4;
					break;
				}
			}
			else {
				break;
			}
		}
		in.get(c);
	}
	if (state == 0) return true;
	handleException(instruction);
	return false;
}

void calculateExpression(ifstream& in, char& c, stack& vs, stack& os) {
	in.get(c); int value;
	if (c == '(') {
		calculateExpression(in, c, vs, os);
		in.get(c);
		push(os, c);
		in.get(c);
	}
	else {
		isOperand(in, c, value);
		push(vs, value);
		push(os, c);
		in.get(c);
	}
	if (c == '(') {
		calculateExpression(in, c, vs, os);
		in.get(c);
	}
	else {
		isOperand(in, c, value);
	}
	if (c == ')') {
		int op = pop(os);
		if (op == '+') push(vs, pop(vs) + value);
		else if (op == '-') push(vs, pop(vs) - value);
		else if (op == '*') push(vs, pop(vs) * value);
		else if (op == '/') {
			if (value == 0)
			{
				state = -99;
				handleException(-1);
				return;
			}
			push(vs, pop(vs) / value);
		}
	}
}

void executeProgram(ifstream& in) {
	variable thisVar; char c = in.get();
	stack var_stack = new elem{ nullptr, 0 }; int value;
	stack op_stack = new elem{ nullptr, 0 };
	string s = "";
	while (!in.eof()) {
		switch (state) {
		case 0:
			s += c;
			state = 1;
			break;
		case 1:
			if (c == ':') { state = 2; break; }
			s += c;
			break;
		case 2:
			thisVar = { s, 0 };
			s = "";
			state = 3;
			break;
		case 3:
			if (c == '(') {
				state = 4;
			}
			else {
				isOperand(in, c, value);
				thisVar.value = value;
				add(global, thisVar);
				state = 0; break;
			}
		case 4:
			calculateExpression(in, c, var_stack, op_stack);
			state = 6;
			break;
		case 6:
			thisVar.value = pop(var_stack);
			add(global, thisVar);
			state = 0;
			break;
		}
		in.get(c);
	}
	del(var_stack);
	del(op_stack);
}

int main() {
	ifstream in("in.txt");
	if (checkProgram(in)) {
		delL(global);
		global = new el{ nullptr, {"",0} };
		in.close();
		in.open("in.txt");
		executeProgram(in);
		printl(global);
	}
	delL(global);
	return 0;
}