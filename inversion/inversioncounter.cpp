/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Isabel Sutedjo
 * Version     : 1.0
 * Date        : 11/5/22
 * Description : Counts the number of inversions in an array.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);

/**
 * Counts the number of inversions in an array in Theta(n^2) time.
 */
long count_inversions_slow(int array[], int length) {
    // TODO
    // double for loop, outer loop goes through array
    // inner loop checks if i is greater than j, if it is then for every i that's greater is +1 inversions
    long inversions = 0;
    for (int i = 0; i < length; i++){
        for (int j = i + 1; j < length; j++){
            if (array[i] > array[j]){
                inversions++;
            }
        }
    }
    return inversions;
}

/**
 * Counts the number of inversions in an array in Theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) {
    // TODO
    // Hint: Use mergesort!
    // define scratch (empty array with length of array)
    // have to store mergesort in long inversions 
    // low = 0, high = length - 1
    int* scratch = new int[length];
    long inversions = mergesort(array, scratch, 0, length - 1);
    delete []scratch;
    return inversions;
}

static long mergesort(int array[], int scratch[], int low, int high) {
    // TODO
    // declaring variables
    long inversions = 0;
    long count1 = 0;
    long count2 = 0;
    if (low < high){
        int mid = low + (high - low) / 2;
        int L = low;
        int H = mid + 1;
        //have to add each iteration to inversion counter
        count1 += mergesort(array, scratch, low, mid);
        count2 += mergesort(array, scratch, mid + 1, high);
        inversions += count1 + count2;
        for (int k = low; k <= high; k++){
            if ((L <= mid) && (H > high || array[L] <= array[H])){
                scratch[k] = array[L];
                L = L + 1;
            }
            else{
                scratch[k] = array[H];
                H = H + 1;
                // for inclusivity
                inversions += (mid - L) + 1;
            }
        }
        // scratch array goes back into the original array
        for (int k = low; k <= high; k++){
            array[k] = scratch[k];
        }  
    }
    return inversions;
}

int main(int argc, char *argv[]) {
    // TODO: parse command-line argument
    // error checking  
    if (argc > 2){
        cerr << "Usage: ./inversioncounter [slow]" << endl;
        return 1;
    }
    //if there are 2 arguments given but it is not "slow" 
    if((argc == 2) && (strcmp(argv[1], "slow"))){
         cerr << "Error: Unrecognized option '" << argv[1] << "'." << endl;
         return 1;
    }
    cout << "Enter sequence of integers, each followed by a space: " << flush;
    
    istringstream iss;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }

    // TODO: produce output
    // error checking for if integers are not given
    int num_values = values.size();
    if(num_values == 0){
        cerr << "Error: Sequence of integers not received." << endl;
        return 1;
    }
    // argc -> has 2 arguments ("slow" and the numbers)
    if ((argc == 2)){
        long slow = count_inversions_slow(&values[0], num_values);
        cout << "Number of inversions: " << slow << endl;
    }
    // all else should be default using the fast algorithm
    else {
        long fast = count_inversions_fast(&values[0], num_values);
        cout << "Number of inversions: " << fast << endl;
    }
    return 0;
}
