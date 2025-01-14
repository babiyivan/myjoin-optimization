//
// Created by Alex Ploner on 14/01/25.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>


using namespace std;

// Split a string into tokens by a given delimiter
vector<string> split(const string &line, char delimiter) {
    vector<string> tokens;
    string token;
    size_t start = 0, end = 0;

    while ((end = line.find(delimiter, start)) != string::npos) {
        tokens.emplace_back(line.substr(start, end - start));
        start = end + 1;
    }
    tokens.emplace_back(line.substr(start)); // Add the last token
    return tokens;
}

// Read a CSV file into an unordered_multimap for faster lookups
vector<pair<string, string> > read_file(const string &filename) {
    vector<pair<string, string> > data;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        auto tokens = split(line, ',');
        if (tokens.size() == 2) {
            data.emplace_back(std::move(tokens[0]), std::move(tokens[1]));
        }
    }

    return data;
}

void my_join(const string &file1, const string &file2, const string &file3, const string &file4) {
    auto data1 = read_file(file1);
    auto data2 = read_file(file2);
    auto data3 = read_file(file3);
    auto data4 = read_file(file4);

    multimap<string, string> map1(data1.begin(), data1.end());
    multimap<string, string> map2(data2.begin(), data2.end());
    multimap<string, string> map3(data3.begin(), data3.end());
    multimap<string, string> map4(data4.begin(), data4.end());

    for (const auto &[key1, value1] : map1) {
        // Directly use the ranges of matching elements
        auto range2 = map2.equal_range(key1);
        auto range3 = map3.equal_range(key1);

        for (auto it2 = range2.first; it2 != range2.second; ++it2) {
            for (auto it3 = range3.first; it3 != range3.second; ++it3) {
                auto range4 = map4.equal_range(it3->second);

                for (auto it4 = range4.first; it4 != range4.second; ++it4) {
                    cout << it3->second << "," << key1 << "," << value1 << ","
                         << it2->second << "," << it4->second << "\n";
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
