/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Isabel Sutedjo
 * Date        : 10/5/22
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

vector< vector<int> > get_ways(int num_stairs) {
    // TODO: Return a vector of vectors of ints representing
    // the different combinations of ways to climb num_stairs
    // stairs, moving up either 1, 2, or 3 stairs at a time.
    
    //based off of given python script in class
    vector<vector <int>> combinations;
    
    //hard code for when there is no stairs, defaults to 0
    if (num_stairs <= 0){
        return {{}};
    }

    //since there can only be 1, 2, or 3 stairs taken at a time loop from 1-3
    for (int x = 1; x < 4; x++){
        if (num_stairs >= x){
            vector<vector <int>> result = get_ways(num_stairs - x);
            //prepends x to the final solution (combinations) in result:
            for (long unsigned int y = 0; y < result.size(); y++){
                result.at(y).push_back(x);
            }
            combinations.insert(combinations.end(), result.begin(), result.end());
        }
    }
    return combinations;
}
void display_ways(const vector< vector<int> > &ways) {
    // TODO: Display the ways to climb stairs by iterating over
    // the vector of vectors and printing each combination.
    
    //declaring variables
    int length = ways.size();
    int buffer = 1;
    int store = length;

    //calculates how many digits for spacing buffer
    while (store >= 10){
        store /= 10;
        buffer++;
    }

    //hard code in 1 stair as to not confuse spacing
    if (length == 1){
        cout << "1 way to climb 1 stair." << endl;
    }

    //prints out amt of ways to climb the amt of stairs given
    else{
        cout << length << " ways to climb " << ways[0].size() << " stairs." << endl;
    }

    //loops through the outer array of 1 to length of array
    for (long unsigned int x = 0; x < ways.size(); x++){
        //right aligns 
        cout << setw(buffer);
        cout << x + 1 << ". [";
        //loops through inner arrays that shows the solutions of stair sets
        for (long unsigned int y = ways[x].size() - 1; y > 0; y--){
            cout << ways[x][y] << ", "; 
        }
        cout << ways[x][0] << "]" << endl;
   }   
}

int main(int argc, char * const argv[]) {
    int x;
    istringstream iss;

    //check for too little or too many input arguments
    if (argc != 2){
        cerr << "Usage: ./stairclimber <number of stairs>" << endl;
        return 1;
    }
    
    //check for bad input 
    iss.str(argv[1]);
    if ((!(iss >> x)) || x <= 0){
        cerr << "Error: Number of stairs must be a positive integer." << endl;
        return 1;

    //runs and displays the ways
    display_ways(get_ways(x));
    return 0;
}
