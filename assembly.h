#pragma once
#include "Syntax.h"
#include <stack>

int memory_address = 2000;	// Global starting address variable
TokenType save;				// For instruction generation
bool stdinflag = false;		// flag for if ID should be pushed or popped from stack

class Symbol {
public:
	string identifier;
	int memLocation;
	string type;

	Symbol(string a, int b, string c) {
		identifier = a;
		memLocation = b;
		type = c;
	}
	Symbol() {
		identifier = "";
		memLocation = 0;
		type = "";
	}
};

class Instr{
public:
	string Op;
	int operand;
	Instr(string a, int b) {
		Op = a;
		operand = b;
	}
	Instr() {
		Op = "";
		operand = 0;
	}
};

vector<Symbol> SymbolTable;		// Global Symbol Table
vector<Instr> InstrTable;		// Global Instruction Table
stack<int> jumpstack;			// Stack for back patching jump instructions

int get_address(string x) {
	if (SymbolTable.size() == 0) {
		cout << "Error: table empty" << endl;
		system("pause");
		exit(1);
	}
	for (int i = 0; i < SymbolTable.size(); i++) {
		if (SymbolTable[i].identifier == x) {
			return SymbolTable[i].memLocation;
		}
	}
	// If it reaches below code, id was not found in table
	cout << "Error: identifier not declared." << endl;
	system("pause");
	exit(1);
}

void printST() {
	if (SymbolTable.size() == 0) return;
	cout << "\n----------- Symbol Table -----------" << endl;
	for (int i = 0; i < SymbolTable.size(); i++) {
		cout << "Identifier: " << SymbolTable[i].identifier << "\t\tAddress: " << SymbolTable[i].memLocation << "\t\tType: " << SymbolTable[i].type << endl;
	}
}

void printIT() {
	if (InstrTable.size() == 0) return;
	cout << "\n--------- Instruction Table ---------" << endl;
	for (int i = 0; i < InstrTable.size(); i++) {
		cout << i + 1 << "\t" << InstrTable[i].Op << "\t";
		if (InstrTable[i].operand == -1)
			cout << "null" << endl;
		else
			cout << InstrTable[i].operand << endl;
	}
}

void add_symbol(string ins, int mem, string t) {
	Symbol input(ins, mem, t);
	bool found = false;
	int i = 0;
	while (!found && i < SymbolTable.size())
	{
		if (SymbolTable[i].identifier == ins)
		{
			found = true;
		}
		i++;
	}
	if (!found)
	{
		SymbolTable.push_back(input);
		memory_address++;
	}
}

void add_instr(string ins, int oprnd) {
	Instr input(ins, oprnd);
	InstrTable.push_back(input);
}

void back_patch() {
	int patch = jumpstack.top();
	jumpstack.pop();
	InstrTable[patch].operand = InstrTable.size() + 1;
}

void Ptf_ST() //Print to file ST
{
	if (SymbolTable.size() == 0) return;
	fout << "\n----------- Symbol Table -----------" << endl;
	for (int i = 0; i < SymbolTable.size(); i++) {
		fout << "Identifier: " << SymbolTable[i].identifier << "\t\tAddress: " << SymbolTable[i].memLocation << "\t\tType: " << SymbolTable[i].type << endl;
	}
}

void Ptf_IT() //Print to file IT
{
	if (InstrTable.size() == 0) return;
	fout << "\n--------- Instruction Table ---------" << endl;
	for (int i = 0; i < InstrTable.size(); i++) {
		fout << i + 1 << "\t" << InstrTable[i].Op << "\t";
		if (InstrTable[i].operand == -1)
			fout << "null" << endl;
		else
			fout << InstrTable[i].operand << endl;
	}
}