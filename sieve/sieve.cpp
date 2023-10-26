/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Isabel Sutedjo
 * Date        : September 20, 2022
 * Description : Sieve of Eratosthenes
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve
{
public:
    PrimesSieve(int limit);

    ~PrimesSieve()
    {
        delete[] is_prime_;
    }

    int num_primes() const
    {
        return num_primes_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool *const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    // Method declarations
    int count_num_primes() const;
    void sieve();
    static int num_digits(int num);
};

PrimesSieve::PrimesSieve(int limit) : is_prime_{new bool[limit + 1]}, limit_{limit}
{
    sieve();
}

void PrimesSieve::display_primes() const
{
    // TODO: write code to display the primes in the format specified in the
    // requirements document.
    int max_prime_;
    for (int j = 2; j <= limit_; j++)
    {
        if (is_prime_[j] == true)
        {
            max_prime_ = j;
        }
    }
    const int max_prime_width = num_digits(max_prime_);
    const int primes_per_row = 80 / (max_prime_width + 1);
    int nums_in_row = 0;
    cout << endl;
    cout << "Number of primes found: " << count_num_primes() << endl;
    cout << "Primes up to " << limit_ << ":" << endl;
    for (int i = 2; i <= limit_; i++)
    {
        if (is_prime_[i] == true)
        {
            nums_in_row++;
            // handles last prime
            if (i == max_prime_)
            {
                cout << i << endl;
                break;
            }
            // handles one line
            else if (num_primes_ <= primes_per_row)
            {
                cout << i << " ";
            }
            // handles last number in a row
            else if (nums_in_row == primes_per_row)
            {
                cout << setw(max_prime_width);
                cout << i << endl;
                nums_in_row = 0;
            }
            else if (nums_in_row < primes_per_row)
            {
                cout << setw(max_prime_width);
                cout << i << " ";
            }
        }
    }
}

int PrimesSieve::count_num_primes() const
{
    // TODO: write code to count the number of primes found
    int total_primes = 0;
    for (int i = 2; i <= limit_; i++)
    {
        if (is_prime_[i] == true)
        {
            total_primes++;
        }
    }
    return total_primes;
}

void PrimesSieve::sieve()
{
    // TODO: write sieve algorithm
    for (int j = 2; j <= limit_; j++)
    {
        is_prime_[j] = true;
    }
    for (int i = 2; i <= sqrt(limit_); i++)
    {
        if (is_prime_[i] == true)
        {
            for (int j = i * i; j <= limit_; j += i)
            {
                is_prime_[j] = false;
            }
        }
    }
    num_primes_ = count_num_primes();
}

int PrimesSieve::num_digits(int num)
{
    // TODO: write code to determine how many digits are in an integer
    // Hint: No strings are needed. Keep dividing by 10.
    int x = 1;
    while (num >= 10)
    {
        num /= 10;
        x++;
    }
    return x;
}

int main()
{
    cout << "**************************** "
         << "Sieve of Eratosthenes"
         << " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if (!(iss >> limit))
    {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2)
    {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }

    // TODO: write code that uses your class to produce the desired output.
    PrimesSieve z(limit);
    z.display_primes();
    return 0;
}
