/*
	Members: Vinh Tran
			 Quang Nguyen
	Course: CPSC 323 - 04
	Professor: Anthony Le
	Assignment 2: Syntax Analyzer
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "Tools.h"
using namespace std;

// Functions' Prototype
void statement(LinkedList<string>&, ofstream& writeFile);
void expression(LinkedList<string>&, ofstream& writeFile);
bool term(LinkedList<string>&, ofstream& writeFile);
void expression_prime(LinkedList<string>&, ofstream& writeFile);
bool factor(LinkedList<string>&, ofstream& writeFile);
void term_prime(LinkedList<string>&, ofstream& writeFile);
void print(LinkedList<string> data, ofstream& writeFile, string cases, string on_off);
void print_error(ofstream& writeFile, string cases);

// Analyze the declarative statement or the assignment statement
void statement(LinkedList<string>& data, ofstream& writeFile) {
	// Case 1: Analyze the assignment statement
	if (data.showToken() == "IDENTIFIER") {
		print(data, writeFile, "assign", "on");
		data.pop_front();
		if (data.isEmpty() || data.showLexeme() != "=") {
			print_error(writeFile, "error =");
			return;
		}
		else if (data.showLexeme() == "=") {
			print(data, writeFile, "", "on");
			data.pop_front();
			expression(data, writeFile);
			if (data.isEmpty() || data.showLexeme() != ";") {
				print_error(writeFile, "error ;");
				return;
			}
			print(data, writeFile, ";", "on");
		}
	}
	// Case 2: Analyze the declarative statement
	else if (data.showToken() == "KEYWORD") {
		if (data.showLexeme() == "int" || data.showLexeme() == "float") {
			print(data, writeFile, "declarative", "on");
			data.pop_front();
			if (data.isEmpty() || data.showToken() != "IDENTIFIER") {
				print_error(writeFile, "error id");
				return;
			}
			else {
				print(data, writeFile, "id", "on");
				data.pop_front();
				if (data.isEmpty() || data.showLexeme() != "=" && data.showLexeme() != ";") {
					print_error(writeFile, "error ; or =");
					return;
				}
				else if (data.showLexeme() == "=") {
					print(data, writeFile, "assign", "off");
					print(data, writeFile, "", "on");
					data.pop_front();
					if (data.isEmpty()) {
						print_error( writeFile, "error id");
						return;
					}
					expression(data, writeFile);
				}
				if (data.isEmpty() || data.showLexeme() != ";") {
					print_error(writeFile, "error ;");
					return;
				}
				print(data, writeFile, ";", "on");
			}
		}
		else
			print_error(writeFile, "error declarative");
	}
	// Case 3: Display the error
	else {
		print_error(writeFile, "error");
	}
}

// Analyze the expression in the statement
void expression(LinkedList<string>& data, ofstream& writeFile) {
	print(data, writeFile, "expression", "on");
	if (term(data, writeFile)) {
		if (data.isEmpty()) {
			return;
		}
		expression_prime(data, writeFile);
	}
	else {
		print_error(writeFile, "error factor");
	}
}

// Analyze the term in the statment
bool term(LinkedList<string>& data, ofstream& writeFile) {
	if (factor(data, writeFile)) {
		data.pop_front();
		term_prime(data, writeFile);
		return true;
	}
	return false;
}

// Analyze the expression using left recursion in the statement
void expression_prime(LinkedList<string>& data, ofstream& writeFile) {
	if (data.isEmpty()) {
		return;
	}
	else if (data.showLexeme() == "+" || data.showLexeme() == "-") {
		print(data, writeFile, "expression prime", "on");
		data.pop_front();
		print(data, writeFile, "", "on");
		if (term(data, writeFile)) {
			expression_prime(data, writeFile);
		}
		else {
			print_error(writeFile, "error id");
			return;
		}
	}
	else if (data.showLexeme() == ";") {
		return;
	}
	else {
		print_error(writeFile, "error ep");
	}
}

// Analyze the factor in the statement
bool factor(LinkedList<string>& data, ofstream& writeFile) {
	if (data.isEmpty()) {
		return false;
	}
	else if (data.showToken() == "IDENTIFIER") {
		print(data, writeFile, "identifier", "off");
		return true;
	}
	else if (data.showToken() == "INTEGER" || data.showToken() == "REAL") {
		print(data, writeFile, "num", "off");
		return true;
	}
	return false;
}

// Analye the term using left recursion in the statement
void term_prime(LinkedList<string>& data, ofstream& writeFile) {
	if (data.isEmpty()) {
		return;
	}
	else if (data.showLexeme() == "*" || data.showLexeme() == "/") {
		print(data, writeFile, "term prime", "on");
		data.pop_front();
		if (data.isEmpty()) {
			return;
		}
		print(data, writeFile, "", "on");
		if (factor(data, writeFile)) {
			data.pop_front();
			term_prime(data, writeFile);
		}
	}
}

void print_error(ofstream& writeFile, string cases) {
	if (cases == "error") {
		writeFile << "EXPECT AN IDENTIFIER OR INT OR FLOAT FOR ASSIGMENT OR DECLARATIVE STATEMENT" << endl;
		return;
	}
	else if (cases == "error =") {
		writeFile << "SYNTAX ERROR!! = NEEDED" << endl;
		return;
	}
	else if (cases == "error ;") {
		writeFile << "SYNTAX ERROR!! ; NEEDED TO END STATEMENT" << endl;
		return;
	}
	else if (cases == "error declarative") {
		writeFile << "TYPE ERROR!! INT OR FLOAT NEEDED" << endl;
		return;
	}
	else if (cases == "error id") {
		writeFile << "SYNTAX ERROR!! IDENTIFIER NEEDED" << endl;
		return;
	}
	else if (cases == "error ; or =") {
		writeFile << "SYNTAX ERROR!! ; OR = NEEDED" << endl;
		return;
	}
	else if (cases == "error factor") {
		writeFile << "SYNTAX ERROR!! IDENTIFIER OR INTEGER OR FLOAT NEEDED" << endl;
		return;
	}
	else if (cases == "error ep") {
		writeFile << "SYNTAX ERROR!! + or - NEEDED" << endl;
		return;
	}

}

void print(LinkedList<string> data, ofstream& writeFile, string cases, string on_off)
{
	if (on_off == "on") {
		writeFile << "Token: " << setw(20) << data.showToken() << setw(11) << "|" << "Lexeme: " << setw(20) << data.showLexeme() << endl;
	}

	if(cases == "assign") {
		writeFile << "   <Statement> -> <Assign>" << endl
			<< "   <Assign> -> <Identifier> = <Expression>;" << endl;
	}
	else if (cases == "declarative") {
		writeFile << "   <Statement> -> <Declarative>" << endl
			<< "   <Declarative> -> <Type> <ID>" << endl
			<< "   <Type> -> int | bool | float" << endl;
	}
	else if (cases == "id") {
		writeFile << "   <ID> -> id" << endl;
	}
	else if (cases == "expression") {
		writeFile << "   <Expression> -> <Term> <ExpressionPrime>" << endl;
	}
	else if (cases == "expression prime") {
		writeFile << "   <TermPrime> -> Epsilon" << endl
			<< "   <ExpressionPrime> -> " << data.showLexeme() << " <Term> <ExpressionPrime>" << endl;
	}
	else if (cases == "term prime") {
		writeFile << "   <TermPrime> -> " << data.showLexeme() << " <Factor> <TermPrime>" << endl;
	}
	else if (cases == "identifier") {
		writeFile << "   <Term> -> <Factor> <TermPrime>" << endl 
			<< "   <Factor> -> <ID> | <Num>" << endl 
			<< "   <ID> -> id" << endl;
	}
	else if (cases == "num") {
		writeFile << "   <Term> -> <Factor> <TermPrime>" << endl
			<< "   <Factor> -> <ID> | <Num>" << endl
			<< "   <Num> -> int | float" << endl;
	}
	else if (cases == ";") {
		writeFile << "   <TermPrime> -> Epsilon" << endl
			<< "   <ExpressionPrime> -> Epsilon" << endl;
	}
}

// Start analyzing the syntax based on user's input or a input file
void syntax_analyzer(LinkedList<string> data, ofstream &writeFile) {
	statement(data, writeFile);
}
