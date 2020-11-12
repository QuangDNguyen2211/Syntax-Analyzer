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
#include "LexicalChecking.h"
#include "Tools.h"

void statement(string test, int& index);
void expression(string test, int& index);
bool term(string test, int& cont);
void expression_prime(string test, int& index);
bool factor(string test, int index);
void term_prime(string test, int & index);

void statement(string test, int& index) {
	string testID;
	testID = test[index];
	if (lexer(testID) == "IDENTIFIER") {
		cout << "Token: Identifier\t" << "|\t" << "Lexeme: " << test[index] << endl;
		cout << "   <Statement> -> <Assign>" << endl;
		cout << "   <Assign> -> <Identifier> = <Expression>;" << endl << endl;
		index++;
		if (checkOperator(test[index]) && test[index] == '=') {
			cout << "Token: Operator\t\t" << "|\t" << "Lexeme: " << test[index] << endl << endl;
			index++;
			expression(test, index);
			if (checkSeparator(test[index]) && test[index] == ';') {
				cout << "Token: Operator\t\t" << "|\t" << "Lexeme: " << test[index] << endl;
				cout << "   <TermPrime> -> Epsilon" << endl;
				cout << "   <ExpressionPrime> -> Epsilon" << endl << endl;
			}
		}
		else
			cout << "SYNTAX ERROR!! EXPECT OPERATOR '='" << endl;
	}
	else
		cout << "SYNTAX ERROR!! EXPECT IDENTIFIER" << endl;
}

void expression(string test, int& index) {
	cout << "Token: Identifier\t" << "|\t" << "Lexeme: " << test[index] << endl;
	cout << "   <Expression> -> <Term> <ExpressionPrime>" << endl;
	if (term(test, index)) {
		if (index >= test.length()) {
			return;
		} else {
			expression_prime(test, index);
		}
	} else {
		cout << "SYNTAX ERROR!! EXPECT IDENTIFIER" << endl;
	}
}

bool term(string test, int & index) {
	if (factor(test, index)) {
		index++;
		term_prime(test, index);
		return true;
	}
	return false;
}

void expression_prime(string test, int & index) {
	if (checkOperator(test[index]) && (test[index] == '+' || test[index] == '-')) {
		cout << "Token: Operator\t\t" << "|\t" << "Lexeme: " << test[index] << endl;
		cout << "   <TermPrime> -> Epsilon" << endl;
		cout << "   <ExpressionPrime> -> " << test[index] << " <Term> <ExpressionPrime>" << endl << endl;
		index++;
		cout << "Token: Identifier\t" << "|\t" << "Lexeme: " << test[index] << endl;
		if (term(test, index))
			expression_prime(test, index);
		else
		{
			cout << "SYNTAX ERROR!! EXPECT IDENTIFIER" << endl;
			return;
		}
	}
	else if (index >= test.length() || test[index] == ';')
		return;
	else
		cout << "SYNTAX ERROR!! EXPECT OPERATOR '+' OR '-'" << endl;
}

bool factor(string test, int cont) {
	if (cont >= test.length()) {
		return false;
	}
	string testID;
	testID = test[cont];
	if (lexer(testID) == "IDENTIFIER") {
		cout << "   <Term> -> <Factor> <TermPrime>" << endl;
		cout << "   <Factor> -> ( <Expression> ) | <ID> | <Num>" << endl;
		cout << "   <ID> -> id" << endl << endl;
		return true;
	}
}

void term_prime(string test, int & index) {
	if (checkOperator(test[index]) && (test[index] == '*' || test[index] == '/')) {
		cout << "Token: Operator\t\t" << "|\t" << "Lexeme: " << test[index] << endl;
		cout << "   <TermPrime> -> " << test[index] << " <Factor> <TermPrime>" << endl << endl;
		index++;
		cout << "Token: Identifier\t" << "|\t" << "Lexeme: " << test[index] << endl;
		if (factor(test, index)) {
			++index;
			term_prime(test, index);
		}
	} else if (index >= test.length()) {
		return;
	}
}

bool is_white_space(char c) {
	return string::npos != WHITESPACE.find(c);
}

void delete_white_space(string& test) {
	for (int c = 0; c < test.length(); c++) {
		if (is_white_space(test[c])) {
			test.erase(c, 1);
		}
	}
}

void syntax_analyzer(string test) {
	delete_white_space(test);
	int index = 0;
	statement(test, index);
}


