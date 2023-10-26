/*******************************************************************************
 * Name        : shortestpaths.cpp
 * Author      : Lili Fortes and Isabel Sutedjo
 * Version     : 1.0
 * Date        : 12/7/2022
 * Description : Finds the shortest path between each vertex.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

int num_vertices;
long INF = numeric_limits<long>::max();
vector<vector<long>> distancematrix;
vector<vector<long>> pathlengths;
vector<vector<long>> intermediatevertices;

int len(long num)
{
    int digits = 0;
    while (num > 0)
    {
        num = num / 10;
        digits++;
    }
    return digits;
}

// adjusted from textbook
void floyd(vector<vector<long>> distancematrix)
{
    long newPath, original;
    for (int k = 0; k < num_vertices; k++)
    {
        for (int i = 0; i < num_vertices; i++)
        {
            for (int j = 0; j < num_vertices; j++)
            {
                original = pathlengths[i][j];
                if (pathlengths[i][k] == INF || pathlengths[k][j] == INF)
                {
                    newPath = INF;
                }
                else
                {
                    newPath = pathlengths[i][k] + pathlengths[k][j];
                }
                if (newPath < original)
                {
                    intermediatevertices[i][j] = k;
                    pathlengths[i][j] = newPath;
                }
            }
        }
    }
}

bool creatematrix(int argc, char *const argv[])
{
    int weight;
    string edge1, edge2, weightstring;
    vector<string> lines;

    // Make sure the right number of command line arguments exist.
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return false;
    }

    // Create an ifstream object.
    ifstream input_file(argv[1]);
    // If it does not exist, print an error message.
    if (!input_file)
    {
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return false;
    }

    // read vertices
    unsigned int line_number = 1;
    string line;
    getline(input_file, line); // reading line 1 from file
    stringstream value(line);  // read line
    if (!(value >> num_vertices && (num_vertices >= 1) && (num_vertices <= 26)))
    {
        cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
        return false;
    }
    else
    {
        lines.push_back(line);
    }
    ++line_number;

    while (getline(input_file, line))
    { // reading the rest of the lines
        value.clear();
        stringstream value(line); // read edges
        if (!(value >> edge1 >> edge2 >> weightstring))
        {
            cerr << "Error: Invalid edge data '" << line << "' on line " << line_number << "." << endl;
            return false;
        }
        else
            lines.push_back(line);
        ++line_number;
    }

    string allowededges;

    for (int ch = 'A'; ch <= 'A' + num_vertices - 1; ch++)
    {
        allowededges.push_back((char)ch);
    }

    int startindex, endindex;

    distancematrix.resize(num_vertices, vector<long>(num_vertices));
    pathlengths.resize(num_vertices, vector<long>(num_vertices));
    intermediatevertices.resize(num_vertices, vector<long>(num_vertices));

    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = 0; j < num_vertices; j++)
        {
            if (i - j != 0)
            {
                distancematrix[i][j] = INF;
                pathlengths[i][j] = INF;
            }
            else
            {
                distancematrix[i][j] = 0;
                pathlengths[i][j] = 0;
            }
            intermediatevertices[i][j] = INF;
        }
    }

    for (size_t i = 1; i < lines.size(); i++)
    {
        value.clear();
        stringstream value(lines[i]);
        value >> edge1 >> edge2 >> weightstring;
        stringstream weightvalue(weightstring);
        if (allowededges.find(edge1) == string::npos)
        {
            cerr << "Error: Starting vertex '" << edge1 << "' on line " << i + 1 << " is not among valid values " << allowededges.front() << "-" << allowededges.back() << "." << endl;
            return false;
        }
        else if (allowededges.find(edge2) == string::npos)
        {
            cerr << "Error: Ending vertex '" << edge2 << "' on line " << i + 1 << " is not among valid values " << allowededges.front() << "-" << allowededges.back() << "." << endl;
            return false;
        }
        else if (!(weightvalue >> weight) || weight <= 0)
        {
            cerr << "Error: Invalid edge weight '" << weightstring << "' on line " << i + 1 << "." << endl;
            return false;
        }
        else
        { // create matrix
            startindex = edge1[0];
            endindex = edge2[0];
            distancematrix[startindex - 'A'][endindex - 'A'] = weight;
            pathlengths[startindex - 'A'][endindex - 'A'] = weight;
        }
        weightvalue.clear();
    }
    return true;
}

/**
 * Displays the matrix on the screen formatted as a table.
 */
void display_table(vector<vector<long>> matrix, const string &label, const bool use_letters = false)
{
    cout << label << endl;
    long max_val = 0;
    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = 0; j < num_vertices; j++)
        {
            long cell = matrix[i][j];
            if (cell < INF && cell > max_val)
            {
                max_val = matrix[i][j];
            }
        }
    }
    long max_cell_width;
    if (use_letters)
    {
        max_cell_width = len(max_val);
        if (max_val == 0)
        {
            max_cell_width = 1;
        }
    }
    else
    {
        max_cell_width = len(max(static_cast<long>(num_vertices), max_val));
    }
    cout << ' ';
    for (int j = 0; j < num_vertices; j++)
    {
        cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
    }
    cout << endl;
    for (int i = 0; i < num_vertices; i++)
    {
        cout << static_cast<char>(i + 'A');
        for (int j = 0; j < num_vertices; j++)
        {
            cout << " " << setw(max_cell_width);
            if (matrix[i][j] == INF)
            {
                cout << "-";
            }
            else if (use_letters)
            {
                cout << static_cast<char>(matrix[i][j] + 'A');
            }
            else
            {
                cout << matrix[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
}

string getShortestPath(int start, int end, vector<vector<long>> intermediatevertices)
{
    string path, to;
    path.push_back((char)(start + 'A'));
    if (start - end != 0)
    {
        to.push_back(end + 'A');
        if (intermediatevertices[start][end] != INF)
        {
            return getShortestPath(start, intermediatevertices[start][end], intermediatevertices) +
            getShortestPath(intermediatevertices[start][end], end, intermediatevertices);
        }
        return to;
    }
    return "";
}

void printshortestpath()
{
    for (int i = 0; i < num_vertices; i++)
    {
        char start = i + 'A';
        for (int j = 0; j < num_vertices; j++)
        {
            char end = j + 'A';
            cout << start << " -> " << end << ", distance: ";
            if (pathlengths[i][j] == INF)
            {
                cout << "infinity";
            }
            else
            {
                cout << pathlengths[i][j];
            }
            cout << ", path: ";

            if (i - j == 0)
            {
                cout << start;
            }
            else
            {
                string shortestPath = getShortestPath(i, j, intermediatevertices);
                string path;
                if (pathlengths[i][j] == INF)
                {
                    cout << "none";
                }
                else
                {
                    path.push_back(start);
                    path += shortestPath;
                    for (size_t i = 0; i < path.length(); i++)
                    {
                        cout << path[i];
                        if (path[i] != path.back())
                        {
                            cout << " -> ";
                        }
                    }
                }
            }
            cout << endl;
        }
    }
}

int main(int argc, char *const argv[])
{
    if (creatematrix(argc, argv))
    {
        floyd(distancematrix);
        display_table(distancematrix, "Distance matrix:");
        display_table(pathlengths, "Path lengths:");
        display_table(intermediatevertices, "Intermediate vertices:", true);
        printshortestpath();
    }
    return 1;
}
