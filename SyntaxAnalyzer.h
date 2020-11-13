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
void statement(LinkedList<string>&);
void expression(LinkedList<string>&);
bool term(LinkedList<string>&);
void expression_prime(LinkedList<string>&);
bool factor(LinkedList<string>&);
void term_prime(LinkedList<string>&);

// Analyze the declarative statement or the assignment statement
void statement(LinkedList<string>& data) {
	// Case 1: Analyze the assignment statement
	if (data.showToken() == "IDENTIFIER") {
		cout << "Token: " << data.showToken() << "\t|\t" << "Lexeme: " << data.showLexeme() << endl;
		cout << "   <Statement> -> <Assign>" << endl;
		cout << "   <Assign> -> <Identifier> = <Expression>;" << endl << endl;
		data.pop_front();
		if (data.isEmpty()) {
			cout << "SYNTAX ERROR!! EXPECT IDENTIFIER" << endl;
			return;
		}
		if (data.showLexeme() == "=") {
			cout << "Token: " << data.showToken() << "\t\t|\t" << "Lexeme: " << data.showLexeme() << endl << endl;
			data.pop_front();
			expression(data);
			if (data.isEmpty()) {
				cout << "WARNING!! EXPECT ';' TO END STATEMENT" << endl;
				return;
			}
			if (data.showLexeme() == ";") {
				cout << "Token: " << data.showToken() << "\t|\t" << "Lexeme: " << data.showLexeme() << endl;
				cout << "   <TermPrime> -> Epsilon" << endl;
				cout << "   <ExpressionPrime> -> Epsilon" << endl << endl;
			}
		}
		else
			cout << "SYNTAX ERROR!! EXPECT OPERATOR '='" << endl;
	}
	// Case 2: Analyze the declarative statement
	else if (data.showToken() == "KEYWORD") {
		cout << "Token: " << data.showToken() << "\t\t|\t" << "Lexeme: " << data.showLexeme() << endl;
		cout << "   <Statement> -> <Declarative>" << endl;
		cout << "   <Declarative> -> <Type> <ID>" << endl;
		cout << "   <Type> -> int | bool | float" << endl << endl;
		data.pop_front();
		if (data.isEmpty()) {
			cout << "SYNTAX ERROR!! EXPECT IDENTIFIER OR ID" << endl;
			return;
		}
		if (data.showToken() == "IDENTIFIER") {
			cout << "Token: " << data.showToken() << "\t|\t" << "Lexeme: " << data.showLexeme() << endl;
			cout << "   <Statement> -> <Assign>" << endl;
			cout << "   <Assign> -> <Identifier> = <Expression>;" << endl << endl;
			data.pop_front();
			if (data.isEmpty()) {
				cout << "WARNING!! EXPECT ';' TO END STATEMENT" << endl;
				return;
			}
			if (data.showLexeme() == "=") {
				cout << "Token: " << data.showToken() << "\t\t|\t" << "Lexeme: " << data.showLexeme() << endl << endl;
				data.pop_front();
				expression(data);
				if (data.isEmpty()) {
					cout << "WARNING!! EXPECT ';' TO END STATEMENT" << endl;
					return;
				}
			}
			if (data.showLexeme() == ";") {
				cout << "Token: " << data.showToken() << "\t|\t" << "Lexeme: " << data.showLexeme() << endl;
				cout << "   <TermPrime> -> Epsilon" << endl;
				cout << "   <ExpressionPrime> -> Epsilon" << endl << endl;
			}
		}
	}
	// Case 3: Display the error
	else {
		cout << "SYNTAX ERROR!! EXPECT A DECLARACTIVE OR ASSIGNMENT STATEMENT" << endl;
	}
}

// Analyze the expression in the statement
void expression(LinkedList<string>& data) {
	cout << "Token: " << data.showToken() << "\t|\t" << "Lexeme: " << data.showLexeme() << endl;
	cout << "   <Expression> -> <Term> <ExpressionPrime>" << endl;
	if (term(data)) {
		if (data.isEmpty()) {
			return;
		}
		expression_prime(data);
	}
	else {
		cout << "SYNTAX ERROR!! EXPECT IDENTIFIER" << endl;
	}
}

// Analyze the term in the statment
bool term(LinkedList<string>& data) {
	if (factor(data)) {
		data.pop_front();
		term_prime(data);
		return true;
	}
	return false;
}

// Analyze the expression using left recursion in the statement
void expression_prime(LinkedList<string>& data) {
	if (data.isEmpty()) {
		return;
	}
	else if (data.showLexeme() == "+" || data.showLexeme() == "-") {
		cout << "Token: " << data.showToken() << "\t\t|\t" << "Lexeme: " << data.showLexeme() << endl;
		cout << "   <TermPrime> -> Epsilon" << endl;
		cout << "   <ExpressionPrime> -> " << data.showLexeme() << " <Term> <ExpressionPrime>" << endl << endl;
		data.pop_front();
		cout << "Token: " << data.showToken() << "\t\t|\t" << "Lexeme: " << data.showLexeme() << endl;
		if (term(data)) {
			expression_prime(data);
		}
		else {
			cout << "SYNTAX ERROR!! EXPECT IDENTIFIER" << endl;
			return;
		}
	}
	else if (data.showLexeme() == ";") {
		return;
	}
	else {
		cout << "SYNTAX ERROR!! EXPECT OPERATOR '+' OR '-'" << endl;
	}
}

// Analyze the factor in the statement
bool factor(LinkedList<string>& data) {
	if (data.isEmpty()) {
		return false;
	}
	else if (data.showToken() == "IDENTIFIER") {
		cout << "   <Term> -> <Factor> <TermPrime>" << endl;
		cout << "   <Factor> -> <ID> | <Num>" << endl;
		cout << "   <ID> -> id" << endl << endl;
	}
	else if (data.showToken() == "INTEGER") {
		cout << "   <Term> -> <Factor> <TermPrime>" << endl;
		cout << "   <Factor> -> <ID> | <Num>" << endl;
		cout << "   <Num> -> int" << endl << endl;
	}
	else if (data.showToken() == "REAL") {
		cout << "   <Term> -> <Factor> <TermPrime>" << endl;
		cout << "   <Factor> -> <ID> | <Num>" << endl;
		cout << "   <Num> -> REAL" << endl << endl;
	}
	return true;
}

// Analye the term using left recursion in the statement
void term_prime(LinkedList<string>& data) {
	if (data.isEmpty()) {
		return;
	}
	else if (data.showLexeme() == "*" || data.showLexeme() == "/") {
		cout << "Token: " << data.showToken() << "\t\t|\t" << "Lexeme: " << data.showLexeme() << endl;
		cout << "   <TermPrime> -> " << data.showLexeme() << " <Factor> <TermPrime>" << endl << endl;
		data.pop_front();
		if (data.isEmpty()) {
			return;
		}
		cout << "Token: " << data.showToken() << "\t\t|\t" << "Lexeme: " << data.showLexeme() << endl;
		if (factor(data)) {
			data.pop_front();
			term_prime(data);
		}
	}
}

// Start analyzing the syntax based on user's input or a input file
void syntax_analyzer(LinkedList<string> data) {
	statement(data);
}
