#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>


using namespace std;
using namespace std::chrono;

// Trim whitespaces from start and end of string
string trim(const string &s) {
    regex pattern("^\\s+|\\s+$");
    return regex_replace(s, pattern, "");
}


int travelAroundTheWorld(const vector<int>& a, const vector<int>& b, const long capacity) {

    int total = 0;
    int n = static_cast<int>(a.size());
    int num_cities_to_validate = n;

    // start from the end and work backwards
    // appending new start cities to already
    // validated sub-paths.
    // Worst case is when all start cities except the last
    // one fail at the first transition
    for (int i = n-1; i > -1; i--) {
        long tank = 0;
        bool is_valid = true;

        for (int j = 0; j < num_cities_to_validate; j++) {
            int curr_index = (i + j) % n;
            tank += a[curr_index];
            if (tank > capacity) {
                tank = capacity;
            }
            tank -= b[curr_index];
            if (tank < 0) {
                is_valid = false;
                break;
            }
        }
        if (is_valid) {
            total++;
            // This validated circuit contains a valid sub-path for the
            // next start city, so we just need to validate the
            // connection to this sub path
            num_cities_to_validate = 1;
        } else if (num_cities_to_validate < n) {
            // this city transition was not possible
            // so, we will need to revalidate it for the
            // next start candidate.
            num_cities_to_validate += 1;
        }
    }

    return total;
}


vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end;

    while ((end = str.find(' ', start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}


int main()
{
    ifstream fin("test_case_large.txt");

    if (!fin.is_open()) {
        cout << "Unable to open test case data" << endl;
        return 1;
    }

    // first line contains N (number of cities) and C (car fuel tank capacity)
    string first_multiple_input_temp;
    getline(fin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(trim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    long c = stol(first_multiple_input[1]);


    cout << "N = " << n << " C = " << c << " gallons" << endl;

    // second line contains a fuel availability at each city
    string a_temp_temp;
    getline(fin, a_temp_temp);

    vector<string> a_temp = split(trim(a_temp_temp));

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        a[i] = stoi(a_temp[i]);
    }

    cout << "a[] contains " << a.size() << " entries" << endl;


    // Third line contains the fuel cost in travelling from city to city
    string b_temp_temp;
    getline(fin, b_temp_temp);

    vector<string> b_temp = split(trim(b_temp_temp));

    bool valid = true;
    vector<int> b(n);
    for (int i = 0; i < n; i++) {
        b[i] = stoi(b_temp[i]);
        if ( b[i] > c ) {
            valid = false;
        }
    }

    if (!valid) { // cost is more than car's fuel capacity
        cout << 0 << endl;
        return 0;
    }

    cout << "b[] contains " << a.size() << " entries" << endl;
    cout << "Computing start city candidates .." << endl;

    auto start = high_resolution_clock::now();
    int result = travelAroundTheWorld(a, b, c);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);


    cout << "Duration: " << duration.count() << " seconds" << endl;
    cout << result << "\n";

    return 0;
}
