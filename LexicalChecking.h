/*
	Members: Vinh Tran
			 Quang Nguyen
	Course: CPSC 323 - 04
	Professor: Anthony Le
	Assignment 2: Syntax Analyzer
*/

#include <iostream>
#include <string>

using namespace std;

/*
	Q = { 0 (ENTRY), 1 (IDENTIFIER), 2 (INTEGER), 3 (REAL) }
	q0 = 0 (ENTRY)
*/
enum FINITE_STATE_MACHINE {
	ENTRY = 0,
	IDENTIFIER = 1,
	INTEGER = 2,
	REAL = 3,
};

/*
		  |			    LETTER			        DIGIT				OTHER
	N:	  | Input:  [a...z][A...Z][$]		   [0...9]				 [.]
__________|______________________________________________________________
-->   0   |               1                       2                   0
      1   |				  1					      1					  0
	  2   |				  0					      2					  3
	  3   |				  0					      3					  0
*/
int state[4][3] = { {IDENTIFIER, INTEGER, ENTRY},
					{IDENTIFIER, IDENTIFIER, ENTRY},
					{ENTRY, INTEGER, REAL},
					{ENTRY, REAL , ENTRY} };

string checkState(int state) {
	if (state == IDENTIFIER) {
		return "IDENTIFIER";
	}
	else if (state == INTEGER) {
		return "INTEGER";
	}
	else {
		return "REAL";
	}
}

// Check the token of a character if it is an OPERATOR
bool checkOperator(char ch) {
	if (ch == '*' || ch == '+' || ch == '-' || ch == '=' || ch == '/' || ch == '>' || ch == '<' || ch == '%') {
		return true;
	}
	return false;
}

// Check the token of a character if it is a SEPARATOR
bool checkSeparator(char ch) {
	if (ch == '\'' || ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == ',' ||
		ch == '.' || ch == ':' || ch == ';') {
		return true;
	}
	return false;
}

// Check the token of a word if it is an IDENTIFIER, INTEGER, REAL, or KEYWORD
string lexer(string word)
{	
	// Check if 'word' is a KEYWORD
	if (word == "int" || word == "float" || word == "bool" || word == "true" || word == "false" || word == "if" || word == "else" || word == "then" ||
		word == "endif" || word == "while" || word == "do" || word == "for" || word == "input" || word == "output" || word == "and" || word == "or" ||
		word == "not" || word == "whileend" || word == "double") {
		return "KEYWORD";
	}
	// Check if 'word' is an IDENTIFIER, INTEGER, REAL
	else {

		// Initialize the entry state for FSM
		int currentState = 0;

		for (int c = 0; c < word.length(); c++) {
			int col;

			if (word[c] == '.') {
				col = 2;
			}
			else if (isdigit(word[c])) {
				col = 1;
			}
			else {
				col = 0;
			}
			currentState = state[currentState][col];
		}
		return checkState(currentState);
	}
}