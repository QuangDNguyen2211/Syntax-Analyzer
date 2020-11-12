/*
	Members: Vinh Tran
			 Quang Nguyen
	Course: CPSC 323 - 04
	Professor: Anthony Le
	Assignment 2: Syntax Analyzer
*/

#include <iostream>
#include "SyntaxAnalyzer.h"
#include "Tools.h"

using namespace std;

int main() {
	// Variable for reading user's input for selecting the mode of the syntax analyzer
	int choice;
	// Design the menu driven for the program
		// Mode 1: Let user input a partial code and do the syntax analyzer
		// Mode 2: Do the syntax analyzer from a source file
	cout << "\t=================================\n"
		<< "\t   WELCOME TO SYNTAX ANALYZER\n"
		<< "\t=================================\n"
		<< "\t           MAIN MENU\n"
		<< "   1. Manual\n"
		<< "   2. Read from a file\n"
		<< "   3. Exit\n\n";

	// Get and check user's input
	// If user inputted '1', the program will be in Mode 1
	// If user inputted '2', the program will be in Mode 2
	// If user inputted '3', the program will be terminal
	// If user's input is bad, ask them to input again
	choice = getBoundedInt("Select your choice [Integer Only]: ", 1, 3);

	switch (choice) {
		case 1:
		{
			string line;
			cout << "\nInput your code: ";
			// Get user's input
			getline(cin, line);
			cout << endl;
			// Call function "syntax_analyzer()" to do the syntax analyzer
			syntax_analyzer(line);
			cout << "SYNTAX ANALYZER COMPLETED.\n";
		} break;
		case 2:
		{
			string fileName;
			while (true) {
				cout << "\nInput the file's name: ";
				// Get user's input
				getline(cin, fileName);
				// Open the file
				fstream myFile(fileName);

				if (myFile.is_open()) {
					// Read line by line from a file
					// Then call function "syntax_analyzer()" to do the syntax analyzer of each line
					string readFile;
					while (getline(myFile, readFile)) {
						cout << endl;
						syntax_analyzer(readFile);
					}
					cout << "SYNTAX ANALYZER COMPLETED.\n";
				} else {
					// Show the error and ask user's input again
					cout << "Could not open \"" << fileName << "\" file or file does not exist.\n";
					continue;
				}
			// Close the file
			myFile.close();
			break;
			}
		} break;
		default:
		{
			// The program will be terminal when entered '3'
			cout << "The program was terminated.\n";
			return 0;
		}
	}
	return 0;
}

