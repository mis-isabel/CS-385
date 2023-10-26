/*******************************************************************************
* Name        : waterjugpuzzle.cpp
* Author      : Jedd Alcalde and Isabel Sutedjo
* Date        : 10/06/2022
* Description : Given 3 water jugs(A, B, C) and amounts of water to reach(a, b, c) this program uses BFS to find the most optimal way to reach that goal.
* Pledge      : "I pledge my honor that I have abided by the Stevens Honor System"
******************************************************************************/ 
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <queue>
#include <stack>
#include <string>
using namespace std;
//Process Explanation
/*
State is a "point in time" containing all of the waterjugs
Each time a transaction made with any cup, the state is "recorded" by creating another state object
We will have an organized matrix of these cases aswell as an empty queue
There will be a defined "initial" and "final" state in the matrix
What we will be looking at (and possibly deleting) are the intermediate states that are the same
Example)Filling and then Emptying all jugs doesn't make sense as they are redundant, we cant progress the problem with those steps
The only difference between states is +/- capA/B/C or transferring
Not all values are solvable
Since we are using 3 jugs instead of 2, and all 3 jugs have a goal, once one jug has reached its goal we should treat the problem as normal
We will be using every possible combination of operations to find all possible states we could get in
From there we use breadth first search to find the shortest way to get to our final state
Since we will only have the final state when we find it, we need to be able to backtrack using the parent pointer of the state class
As we backtrack its a good idea to push_back() all the state objects into a vector and then print that as our result
Use a dynamic 2D array to confirm whether we have visited a state already in our tree
Use pointers to share states rather than copy everywhere (this can result in memory leaks)
Create a 1D array of pointers of size A+1, each pointing to a 1D array of size B+1, dynamic 2D arrays aren't possible in c++
Each value in the pointed 1D array should start as false
*/

struct State {
    int a, b, c;
    string directions;
    State *parent;

    //parent denotes a pointer to the predecessor of this state
    //it will be the way you "remmeber" how you got to your final state
    
    State(int _a, int _b, int _c, string _directions) : 
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { }
    
    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }
};

string bfs(State *cap, State *init, State *fin){
	queue<State*> que;            //stores States that have already been visited
	bool **boolarr = new bool*[cap -> a + 1];        //stores values for if we have already visited a state or not, its a double pointer because its a 2D array

	for(int i = 0;i < (cap -> a + 1);i++){            //iterate through boolean arrays so we can set all of the bools to false
		boolarr[i] = new bool[cap -> b + 1];        //instantiate the boolean
		fill(boolarr[i], boolarr[i] + cap -> b + 1, false);      //start out all booleans as false
	}

    que.push(init);           // add initial state to queue

    State* currentstate;        //iterator for states

    vector<State*> statearr={};       //used to see if you've seen the state already

    string result = "";             //what we will print out later

    int sol = 0;
   
   	while (que.size() > 0){      //bfs keeps going until the queue is empty
        currentstate = que.front();            //current is the iterating State of the bfs
        que.pop();                        //reduce the stack we already have it

        stack<string> strstack;
        //if we got to our target State
		if ((currentstate->a == fin->a) && (currentstate->b == fin->b)){
            sol = 1;
            //delete must be different
			while (currentstate != NULL){
                strstack.push(currentstate -> directions + " " + currentstate -> to_string());       //add the iterated State to the string
				currentstate = currentstate -> parent;              //iterate the current State backward
			}

            while(strstack.size() != 1){              //idk y it isnt >= otherwise it goes one too far
                result += strstack.top()+"\n";       //append the top of string to result
                strstack.pop();                      //pop the first in the stack
            }
            result += strstack.top();
            delete currentstate;
               
            for(int i = 0; i < cap -> a + 1; i++){
                delete [] boolarr[i];       //delete the 1D array
            }
        
            delete [] boolarr;          //delete the the 2nd dimension of the array

            for(size_t i = 0; i < statearr.size(); i++){
                delete statearr[i];       //delete the spot in memory
            }
            break;
            //return result;
        }

		if (boolarr[currentstate -> a][currentstate -> b] == 0)
		{
            
            boolarr[currentstate -> a][currentstate -> b] = 1; //set in arr that node has been visited for future searches

            //Instantiations done, time to pour

            int transferred;            //represents the water moved from one jug to another
            //duplicates currently equal to the current values
            //used for instantiating new states post pour
            int dupA = currentstate->a;
            int dupB = currentstate->b;
            int dupC = currentstate->c;
            //Pour from C to A(1)
            if ((cap -> a > currentstate -> a) && (currentstate -> c > 0)){
                string addstr = "Pour ";
                dupB = currentstate -> b;            //set dupB to currentb

                if ((currentstate -> a + currentstate -> c) <= (cap -> a)){        //a can hold all of c's water
                    transferred = currentstate->c;           //transferred is all that was in c
                    dupA = currentstate -> c + currentstate -> a;         //a is what was in a and c
                }
                else
                {
                    transferred = cap -> a - currentstate->a;          //transferred water is difference
                    dupA = cap -> a;          //a is filled
                }
                //operation complete string it up
                addstr +=  to_string(transferred);
                if(transferred > 1){
                    addstr += " gallons from C to A.";
                }
                else{
                    addstr += " gallon from C to A.";
                }

                //these are added after strings are made, if they were before we would be skipping a step
                dupC = currentstate -> c - transferred;        //c is inverse difference

                //insantiate the new state
                State *child = new State(dupA, dupB, dupC, addstr);
                //set parent
                child -> parent = currentstate;

                //push instantiated state to queue
                que.push(child);

                //push instantiated state to memory
                statearr.push_back(child);
            }
            
            //Pour from B to A(2)
            if ((cap -> a > currentstate -> a) && (currentstate -> b > 0)){
                string addstr = "Pour ";
                dupC = currentstate->c;          //set dupC to currentc

                if ((currentstate -> a + currentstate -> b) <= (cap -> a)){        //a can hold all of b's water
                    transferred = currentstate->b;           //transferred is all that was in b
                    dupA = currentstate -> b + currentstate -> a;         //a is what was in a and b
                }
                else
                {
                    transferred = cap->a - currentstate->a;          //transferred water is difference
                    dupA = cap -> a;          //a is filled
                }
                //operation complete string it up
                addstr +=  to_string(transferred);
                if(transferred > 1){
                    addstr += " gallons from B to A.";;
                }
                else{
                    addstr += " gallon from B to A.";
                }

                //these are added after strings are made, if they were before we would be skipping a step
                dupB = currentstate->b - transferred;        //b is inverse difference

                //insantiate the new state
                State *child = new State(dupA, dupB, dupC, addstr);
                //set parent
                child -> parent = currentstate;

                //push instantiated state to queue
                que.push(child);

                //push instantiated state to memory
                statearr.push_back(child);
            }

            //Pour from C to B(3)
            if ((cap -> b > currentstate -> b) && (currentstate -> c > 0)){
                string addstr = "Pour ";
                dupA = currentstate->a;         //set dupB to currentb

                if ((currentstate -> b + currentstate -> c) <= (cap -> b)){        //b can hold all of c's water
                    transferred = currentstate -> c;           //transferred is all that was in c
                    dupB = currentstate -> c + currentstate -> b;         //b is what was in b and c
                }
                else
                {
                    transferred = cap -> b - currentstate -> b;          //transferred water is difference
                    dupB = cap -> b;          //b is filled
                }
                //operation complete string it up
                addstr +=  to_string(transferred);
                if(transferred > 1){
                    addstr += " gallons from C to B.";
                }
                else{
                    addstr +=" gallon from C to B.";
                }

                //these are added after strings are made, if they were before we would be skipping a step
                dupC = currentstate->c - transferred;        //c is inverse difference

                //insantiate the new state
                State *child = new State(dupA, dupB, dupC, addstr);
                //set parent
                child -> parent = currentstate;

                //push instantiated state to queue
                que.push(child);

                //push instantiated state to memory
                statearr.push_back(child);
            }

            //Pour from A to B(4)
            if ((cap -> b > currentstate -> b) && (currentstate -> a > 0)){
                string addstr = "Pour ";
                dupC = currentstate->c;          //set dupC to currentc

                if ((currentstate -> b + currentstate -> a) <= (cap -> b)){        //b can hold all of a's water
                    transferred = currentstate->a;           //transferred is all that was in a
                    dupB = currentstate -> a + currentstate -> b;         //a is what was in b and a
                }
                else
                {
                    transferred = cap->b - currentstate->b;          //transferred water is difference
                    dupB = cap -> b;          //b is filled
                }
                //operation complete string it up
                addstr +=  to_string(transferred);
                if(transferred > 1){
                    addstr += " gallons from A to B.";
                }
                else{
                    addstr += " gallons from A to B.";
                }

                //these are added after strings are made, if they were before we would be skipping a step
                dupA = currentstate->a - transferred;        //a is inverse difference
                dupC = currentstate->c;          //set dupC to currentc

                //insantiate the new state
                State *child = new State(dupA, dupB, dupC, addstr);
                //set parent
                child -> parent = currentstate;

                //push instantiated state to queue
                que.push(child);

                //push instantiated state to memory
                statearr.push_back(child);
            }
            
            //Pour from B to C(5)
            if ((cap -> c > currentstate -> c) && (currentstate -> b > 0)){
                string addstr = "Pour ";
                dupA = currentstate->a;          //set dupA to currenta

                if ((currentstate -> c + currentstate -> b) <= (cap -> c)){        //c can hold all of b's water
                    transferred = currentstate -> b;           //transferred is all that was in b
                    dupC = currentstate -> b + currentstate -> c;         //c is what was in c and b
                }
                else
                {
                    transferred = cap -> b - currentstate -> b;          //transferred water is difference
                    dupC = cap -> c;          //c is filled
                }
                //operation complete string it up
                addstr +=  to_string(transferred);
                if(transferred > 1){
                    addstr += " gallons from B to C.";
                }
                else{
                    addstr += " gallon from B to C.";
                }

                //these are added after strings are made, if they were before we would be skipping a step
                dupB = currentstate->b - transferred;        //b is inverse difference

                //insantiate the new state
                State *child = new State(dupA, dupB, dupC, addstr);
                //set parent
                child -> parent = currentstate;

                //push instantiated state to queue
                que.push(child);

                //push instantiated state to memory
                statearr.push_back(child);
            }

            //Pour from A to C(6)
            if ((cap -> c > currentstate -> c) && (currentstate -> a > 0)){
                string addstr = "Pour ";
                dupB = currentstate->b;          //set dupB to currentb

                if ((currentstate -> c + currentstate -> a) <= (cap -> c)){        //c can hold all of a's water
                    transferred = currentstate->a;           //transferred is all that was in a
                    dupC = currentstate -> a + currentstate -> c;         //c is what was in c and a
                }
                else
                {
                    transferred = cap -> c - currentstate->c;          //transferred water is difference
                    dupC = cap -> c;          //c is filled
                }
                //operation complete string it up
                addstr +=  to_string(transferred);
                if(transferred> 1){
                    addstr += " gallons from A to C.";
                }
                else{
                    addstr += " gallon from A to C.";
                }

                //these are added after strings are made, if they were before we would be skipping a step
                dupA = currentstate -> a - transferred;        //a is inverse difference

                //insantiate the new state
                State *child = new State(dupA, dupB, dupC, addstr);
                //set parent
                child -> parent = currentstate;

                //push instantiated state to queue
                que.push(child);

                //push instantiated state to memory
                statearr.push_back(child);
            }
            //else we've already seen the state
		}
    }

    //why did the while loop stop?
    if(sol == 1){//stopped due to target state found
        return result;
    }

    //defeat memory leaks by deleting everything

	for(int i = 0; i < (cap->a+1); i++){
		delete [] boolarr[i];       //boolean array deleted
	}
   
    for (size_t i = 0; i < statearr.size(); i++){
        delete statearr[i];         //state vector deleted
    }
    delete [] boolarr;
    //if solution condition was never reached, then just return no solution
    return "No solution.";
}



int main(int argc, char * const argv[]){

	istringstream iss;
    //max capacities for corresponding jugs
    int capA = 0;
    int capB = 0;
    int capC = 0;
    //end goals for corresponding jugs
    int goalA = 0;
    int goalB = 0;
    int goalC = 0;

    if(argc != 7){
        cout << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
        return 1;
    }
    iss.str(argv[1]);
    if((!(iss >> capA)) || capA <= 0){
        cout << "Error: Invalid capacity \'" << argv[1] << "\' for jug A." << endl;
        return 1;
    }
    iss.clear();

    iss.str(argv[2]);
    if((!(iss >> capB)) || capB <= 0){
        cout << "Error: Invalid capacity \'" << argv[2] << "\' for jug B." << endl;
        return 1;
    }
    iss.clear();

    iss.str(argv[3]);
    if((!(iss >> capC)) || capC <= 0){
        cout << "Error: Invalid capacity \'" << argv[3] << "\' for jug C." << endl;
        return 1;
    }
    iss.clear();

    iss.str(argv[4]);
    if((!(iss >> goalA)) || goalA < 0){
        cout << "Error: Invalid goal \'" << argv[4] << "\' for jug A." << endl;
        return 1;
    }
    iss.clear();

    iss.str(argv[5]);
    if((!(iss >> goalB)) || goalB < 0){
        cout << "Error: Invalid goal \'" << argv[5] << "\' for jug B." << endl;
        return 1;
    }
    iss.clear();

    iss.str(argv[6]);
    if((!(iss >> goalC)) || goalC < 0){
        cout << "Error: Invalid goal \'" << argv[6] << "\' for jug C." << endl;
        return 1;
    }
    iss.clear();
    if(capA < goalA){
        cout << "Error: Goal cannot exceed capacity of jug A." << endl;
        return 1;
    }
    if(capB < goalB){
        cout << "Error: Goal cannot exceed capacity of jug B." << endl;
        return 1;
    }
    if(capC < goalC){
        cout << "Error: Goal cannot exceed capacity of jug C." << endl;
        return 1;
    }
    if(capC != goalA + goalB + goalC){
        //sum of goals must be equal to the capC
        cout << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
        return 1;
    }
    else{
        State *initial = new State(0, 0, capC, "Initial state.");//initial state of jugs, all empty except jug C
        State *fin = new State(goalA, goalB, goalC, "Final state.");//the final state we want to reach
        State *capacity = new State(capA, capB, capC, "Max state.");//used to control max volume of jugs
        cout << bfs(capacity, initial, fin) << endl;
        delete initial;
        delete fin;
        delete capacity;
    }
    return 0;
}
