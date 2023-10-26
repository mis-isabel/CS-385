/*******************************************************************************
 * Filename: sqrt.cpp
 * Author  : Isabel Sutedjo
 * Version : 1.0
 * Date    : September 8, 2022
 * Description: Computes the SQRT.
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>
using namespace std;

double sqrt(double num, double epsilon)
{
    double last_guess, next_guess;
    last_guess = 1;
    next_guess = (last_guess + num / last_guess) / 2;
    if (num < 0)
    {
        return numeric_limits<double>::quiet_NaN();
    }
    if ((num == 0) || (num == 1))
    {
        return num;
    }

    while (abs(last_guess - next_guess) > epsilon)
    {
        last_guess = next_guess;
        next_guess = (last_guess + num / last_guess) / 2;
    }
    return next_guess;
}

int main(int argc, char *argv[])
{
    double num, epsilon;
    epsilon = 1e-7;
    istringstream iss;
    if (!(argc == 3 || argc == 2))
    {
        cerr << "Usage: " << argv[0] << " <value> [epsilon]" << endl;
        return 1;
    }
    iss.str(argv[1]);
    if (!(iss >> num))
    {
        cerr << "Error: Value argument must be a double." << endl;
        return 1;
    }
    iss.clear();
    if (argc == 3)
    {
        iss.str(argv[2]);
        if (!(iss >> epsilon) || (epsilon <= 0))
        {
            cerr << "Error: Epsilon argument must be a positive double." << endl;
            return 1;
        }
    }

    cout << fixed << setprecision(8) << sqrt(num, epsilon) << endl;
    return 0;
}