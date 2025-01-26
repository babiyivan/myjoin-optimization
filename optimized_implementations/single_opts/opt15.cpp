//
// Aleman Mihnea 25/01/25
// use of robin hood hashing for the maps
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "robin_hood.h" // Include the Robin Hood unordered_map library

using namespace std;

// Function to split a line into tokens based on a delimiter
vector<string> split(const string &line, char delimiter) {
    vector<string> tokens;
    stringstream ss(line);
    string token;

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

// Read a CSV file and return its contents as a robin_hood::unordered_map
robin_hood::unordered_map<string, vector<string>> read_file(const string &filename) {
    robin_hood::unordered_map<string, vector<string>> data;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        auto tokens = split(line, ',');
        if (tokens.size() == 2) {
            data[tokens[0]].push_back(tokens[1]);
        }
    }

    return data;
}

void my_join(const string& file1, const string& file2, const string& file3, const string& file4) {
    auto map1 = read_file(file1);
    auto map2 = read_file(file2);
    auto map3 = read_file(file3);
    auto map4 = read_file(file4);

    for (const auto& [key1, values1] : map1) {
        // Check if the key exists in both map2 and map3
        if (map2.find(key1) != map2.end() && map3.find(key1) != map3.end()) {
            const auto& values2 = map2[key1];
            const auto& values3 = map3[key1];

            for (const auto& value1 : values1) {
                for (const auto& value2 : values2) {
                    for (const auto& value3 : values3) {
                        // Check if value3 exists as a key in map4
                        if (map4.find(value3) != map4.end()) {
                            const auto& values4 = map4[value3];
                            for (const auto& value4 : values4) {
                                // Print the joined result
                                cout << value3 << "," << key1 << "," << value1 << "," << value2 << "," << value4 << "\n";
                            }
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <file1> <file2> <file3> <file4>\n";
        return 1;
    }

    my_join(argv[1], argv[2], argv[3], argv[4]);

    return 0;
}
