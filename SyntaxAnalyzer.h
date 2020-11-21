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
bool checkConditional(LinkedList<string>&, ofstream& writeFile);
void print(LinkedList<string> data, ofstream& writeFile, string cases, string on_off);
void print_error(ofstream& writeFile, string cases);

// Start analyzing the syntax based on user's input or a input file
void syntax_analyzer(LinkedList<string> data, ofstream& writeFile) {
	statement(data, writeFile);
}

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
			} else if (data.showLexeme() == "whileend" || data.showLexeme() == "endif") {
				return;
			}
			print(data, writeFile, ";", "on");
		}
	}
	// Case 2: Analyze the declarative, while, and if else statement
	else if (data.showToken() == "KEYWORD") {
		if (data.showLexeme() == "int" || data.showLexeme() == "float" || data.showLexeme() == "bool") {
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
					if (data.isEmpty() || data.showLexeme() == ";") {
						print_error( writeFile, "error id");
						return;
					}
					expression(data, writeFile);
				} 
				if (data.isEmpty() || data.showLexeme() != ";") {
					print_error(writeFile, "error ;");
					return;
				} else if (data.showLexeme() == "whileend" || data.showLexeme() == "endif") {
					return;
				}
				print(data, writeFile, ";", "on");
			}
		}
		else if (data.showLexeme() == "while" || data.showLexeme() == "if") {
			print(data, writeFile, "declarative", "on");
			data.pop_front();
			if (data.isEmpty()) {
				return;
			}
			else {
				if (checkConditional(data, writeFile)) {
					data.pop_front();
					if (data.isEmpty()) {
						return;
					} else {
						if (data.showLexeme() == "then" || data.showLexeme() == "do") {
							print(data, writeFile, "", "on");
							data.pop_front();
							if (data.isEmpty()) {
								return;
							} else {
								statement(data, writeFile);
								if (data.showLexeme() == "else") {
									print(data, writeFile, "", "on");
									data.pop_front();
									if (data.isEmpty()) {
										return;
									} else {
										statement(data, writeFile);
										if (data.showLexeme() == "endif") {
											print(data, writeFile, "", "on");
											return;
										}
									}
								}
								else if (data.showLexeme() == "whileend") {
									print(data, writeFile, "", "on");
									return;
								}
							}
						} else {
							cout << "\nSYNTAX ERROR: \"" << data.showLexeme() << "\"" << endl;
							writeFile << "\nSYNTAX ERROR: \"" << data.showLexeme() << "\"" << endl;
						}
					}
				} else {
					cout << "\nSYNTAX ERROR: LACK OF \"(\" AND \")\"\n";
					writeFile << "\nSYNTAX ERROR: LACK OF \"(\" AND \")\"\n";
					cout << "EXPECTED: (";
					while (!data.isEmpty()) {
						cout << data.showLexeme();
						writeFile << data.showLexeme();
						data.pop_front();
					}
					cout << ")\n";
					writeFile << ")\n";
				}
			}
		}
		else {
			print_error(writeFile, "error declarative");
		}
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
	else if (data.showLexeme() == ";" || data.showLexeme() == ")" || data.showLexeme() == "else" || data.showLexeme() == "whileend" || data.showLexeme() == "endif") {
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
	else if (data.showToken() == "KEYWORD" && (data.showLexeme() == "true" || data.showLexeme() == "false")) {
		print(data, writeFile, "bool", "off");
		return true;
	}
	return false;
}

// Analyze the term using left recursion in the statement
void term_prime(LinkedList<string>& data, ofstream& writeFile) {
	if (data.isEmpty()) {
		return;
	}
	else if (data.showLexeme() == "*" || data.showLexeme() == "/" || data.showLexeme() == ">" || data.showLexeme() == "<") {
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

// Analyze the condition for while and if else statement
bool checkConditional(LinkedList<string>& data, ofstream& writeFile) {
	if (data.showLexeme() == "(") {
		print(data, writeFile, "", "on");
		data.pop_front();
		if (data.isEmpty()) {
			return false;
		}
		else {
			expression(data, writeFile);
			if (data.showLexeme() == ")") {
				print(data, writeFile, "", "on");
				return true;
			}
		}
	}
	return false;
}

// Analyze the error of the source code and display on the screen and write to the file
void print_error(ofstream& writeFile, string cases) {
	if (cases == "error") {
		// Display on the screen
		cout << "EXPECT AN IDENTIFIER OR INT OR FLOAT FOR ASSIGMENT OR DECLARATIVE STATEMENT" << endl;
		// Write into the destination file
		writeFile << "EXPECT AN IDENTIFIER OR INT OR FLOAT FOR ASSIGMENT OR DECLARATIVE STATEMENT" << endl;
		return;
	}
	else if (cases == "error =") {
		// Display on the screen
		cout << "SYNTAX ERROR!! = NEEDED" << endl;
		// Write into the destination file
		writeFile << "SYNTAX ERROR!! = NEEDED" << endl;
		return;
	}
	else if (cases == "error ;") {
		// Display on the screen
		cout << "SYNTAX ERROR!! ; NEEDED TO END STATEMENT" << endl;
		// Write into the destination file
		writeFile << "SYNTAX ERROR!! ; NEEDED TO END STATEMENT" << endl;
		return;
	}
	else if (cases == "error declarative") {
		// Display on the screen
		cout << "TYPE ERROR!! INT OR FLOAT NEEDED" << endl;
		// Write into the destination file
		writeFile << "TYPE ERROR!! INT OR FLOAT NEEDED" << endl;
		return;
	}
	else if (cases == "error id") {
		// Display on the screen
		cout << "SYNTAX ERROR!! IDENTIFIER NEEDED" << endl;
		// Write into the destination file
		writeFile << "SYNTAX ERROR!! IDENTIFIER NEEDED" << endl;
		return;
	}
	else if (cases == "error ; or =") {
		// Display on the screen
		cout << "SYNTAX ERROR!! ; OR = NEEDED" << endl;
		// Write into the destination file
		writeFile << "SYNTAX ERROR!! ; OR = NEEDED" << endl;
		return;
	}
	else if (cases == "error factor") {
		// Display on the screen
		cout << "SYNTAX ERROR!! IDENTIFIER OR INTEGER OR FLOAT NEEDED" << endl;
		// Write into the destination file
		writeFile << "SYNTAX ERROR!! IDENTIFIER OR INTEGER OR FLOAT NEEDED" << endl;
		return;
	}
	else if (cases == "error ep") {
		// Display on the screen
		cout << "SYNTAX ERROR!! + or - NEEDED" << endl;
		// Write into the destination file
		writeFile << "SYNTAX ERROR!! + or - NEEDED" << endl;
		return;
	}

}

// Analyze the appropriate rules of the source code and display on the screen and write to the file
void print(LinkedList<string> data, ofstream& writeFile, string cases, string on_off) {	
	cout << left;
	if (on_off == "on") {
		// Display on the screen
		cout << "Token: " << setw(20) << data.showToken() << setw(11) << "|" << "Lexeme: " << setw(20) << data.showLexeme() << endl;
		// Write into the destination file
		writeFile << "Token: " << setw(20) << data.showToken() << setw(11) << "|" << "Lexeme: " << setw(20) << data.showLexeme() << endl;
	}

	if(cases == "assign") {
		// Display on the screen
		cout << "   <Statement> -> <Assign>" << endl
			 << "   <Assign> -> <Identifier> = <Expression>;" << endl;
		// Write into the destination file
		writeFile << "   <Statement> -> <Assign>" << endl
			      << "   <Assign> -> <Identifier> = <Expression>;" << endl;
	}
	else if (cases == "declarative") {
		// Display on the screen
		cout << "   <Statement> -> <Declarative> | if <Conditional> then <Statement> else <Statement> endif" << endl
			 << "   <Statement> -> while <Conditional> do <Statement> whileend" << endl
			 << "   <Declarative> -> <Type> <ID>" << endl
			 << "   <Conditional> -> ( <Expression> <Relop> <Expression> ) | ( <Expression> )" << endl
			 << "   <Relop> -> < | > " << endl
			 << "   <Type> -> int | float | bool" << endl
			 << "   <ID> -> id" << endl;
		// Write into the destination file
		writeFile << "   <Statement> -> <Declarative> | if <Conditional> then <Statement> else <Statement> endif" << endl
				  << "   <Statement> -> while <Conditional> do <Statement> whileend" << endl
			      << "   <Declarative> -> <Type> <ID>" << endl
				  << "   <Conditional> -> ( <Expression> <Relop> <Expression> ) | ( <Expression> )" << endl
				  << "   <Relop> -> < | > " << endl
			      << "   <Type> -> int | float | bool" << endl
				  << "   <ID> -> id" << endl;
	}			
	else if (cases == "id") {
		// Display on the screen
		cout << "   <ID> -> id" << endl;
		// Write into the destination file
		writeFile << "   <ID> -> id" << endl;
	}
	else if (cases == "expression") {
		// Display on the screen
		cout << "   <Expression> -> <Term> <ExpressionPrime>" << endl;
		// Write into the destination file
		writeFile << "   <Expression> -> <Term> <ExpressionPrime>" << endl;
	}
	else if (cases == "expression prime") {
		// Display on the screen
		cout << "   <TermPrime> -> Epsilon" << endl
			 << "   <ExpressionPrime> -> " << data.showLexeme() << " <Term> <ExpressionPrime>" << endl;
		// Write into the destination file
		writeFile << "   <TermPrime> -> Epsilon" << endl
			      << "   <ExpressionPrime> -> " << data.showLexeme() << " <Term> <ExpressionPrime>" << endl;
	}
	else if (cases == "term prime") {
		// Display on the screen
		cout << "   <TermPrime> -> " << data.showLexeme() << " <Factor> <TermPrime>" << endl;
		// Write into the destination file
		writeFile << "   <TermPrime> -> " << data.showLexeme() << " <Factor> <TermPrime>" << endl;
	}
	else if (cases == "identifier") {
		// Display on the screen
		cout << "   <Term> -> <Factor> <TermPrime>" << endl
			 << "   <Factor> -> <ID> | <Num> | <BoolType>" << endl
			 << "   <ID> -> id" << endl;
		// Write into the destination file
		writeFile << "   <Term> -> <Factor> <TermPrime>" << endl 
			      << "   <Factor> -> <ID> | <Num> | <BoolType>" << endl 
			      << "   <ID> -> id" << endl;
	}
	else if (cases == "num") {
		// Display on the screen
		cout << "   <Term> -> <Factor> <TermPrime>" << endl
			 << "   <Factor> -> <ID> | <Num> | <BoolType>" << endl
			 << "   <Num> -> int | float" << endl
			 << "   <BoolType> -> true | false" << endl;
		// Write into the destination file
		writeFile << "   <Term> -> <Factor> <TermPrime>" << endl
			      << "   <Factor> -> <ID> | <Num> | <BoolType>" << endl
			      << "   <Num> -> int | float" << endl
				  << "   <BoolType> -> true | false" << endl;
	}
	else if (cases == "bool") {
		// Display on the screen
		cout << "   <Term> -> <Factor> <TermPrime>" << endl
			 << "   <Factor> -> <ID> | <Num> | <BoolType>" << endl
			 << "   <Num> -> int | float" << endl
			 << "   <BoolType> -> true | false" << endl;
		// Write into the destination file
		writeFile << "   <Term> -> <Factor> <TermPrime>" << endl
			      << "   <Factor> -> <ID> | <Num> | <BoolType>" << endl
			      << "   <Num> -> int | float" << endl
				  << "   <BoolType> -> true | false" << endl;
	}
	else if (cases == ";") {
		// Display on the screen
		cout << "   <TermPrime> -> Epsilon" << endl
			 << "   <ExpressionPrime> -> Epsilon" << endl;
		// Write into the destination file
		writeFile << "   <TermPrime> -> Epsilon" << endl
			      << "   <ExpressionPrime> -> Epsilon" << endl;
	}
}
