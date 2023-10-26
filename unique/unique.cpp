/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Isabel Sutedjo
 * Date        : 9/28/22
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
    // TODO: returns true if all characters in string are lowercase
    // letters in the English alphabet; false otherwise.

    //check if letters are lowercase
    for (int j : s){
        if (islower(j) == false){
            return false;
        }
    }
    return true;
}

bool all_unique_letters(const string &s) {
    // TODO: returns true if all letters in string are unique, that is
    // no duplicates are found; false otherwise.

    // You MUST use only a single int for storage and work with bitwise
    // and bitshifting operators.  Using any other kind of solution will
    // automatically result in a grade of ZERO for the whole assignment.

    // initialize int for storage
    int storage = 0;

    //loop through the string
    for (int x = 0; x < (int)s.size(); x++){
	//compare distance of current character to the value of a and shift 1 by that amount
        if ((storage & (1 << (s[x] - 97))) > 0){
            return false;
        }
        storage = storage | (1 << (s[x] - 97));
    } 
    return true;

}

int main(int argc, char * const argv[]) {
    // TODO: reads and parses command line arguments.
    // Calls other functions to produce correct output.

    //check for too little or too many input arguments
    if (argc !=2){
        cout << "Usage: ./unique <string>" << endl;
        return 1;
    }

    //check for bad input
    else if (is_all_lowercase(argv[1]) == false){
        cout << "Error: String must contain only lowercase letters." << endl;
        return 1;
    }

    //check for unique letters
    else if (all_unique_letters(argv[1]) == true){
        cout << "All letters are unique." << endl;
        return 1;
    }

    //check for duplicate letters
    else if (all_unique_letters(argv[1]) == false){
        cout << "Duplicate letters found." << endl;
        return 1;
    }
    return 0;
}
