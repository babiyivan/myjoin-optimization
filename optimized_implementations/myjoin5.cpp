//
// Created by Jakob Petermandl on 23/01/25.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Read a CSV file into an unordered_multimap for faster lookups
vector<pair<string, string>> read_file(const string &filename) {
    vector<pair<string, string>> data;
    ifstream file(filename);
    string line;

    file.seekg(0, ios::end); // Estimate file size for pre-allocation
    size_t estimated_lines = file.tellg() / 50; // Rough guess: 50 bytes per line
    file.seekg(0, ios::beg);

    data.reserve(estimated_lines); // Reserve space

    while (getline(file, line)) {
        //reduce scope of std::string
        size_t delim = line.find(',');
        if (delim != string::npos) {
            data.emplace_back(
                line.substr(0, delim),
                line.substr(delim + 1)
            );
        }
    }

    return data;
}

void my_join(const string &file1, const string &file2, const string &file3, const string &file4) {
    auto data1 = read_file(file1);
    auto data2 = read_file(file2);
    auto data3 = read_file(file3);
    auto data4 = read_file(file4);

    unordered_multimap<string, string> map1, map2, map3, map4;

    map1.reserve(data1.size());
    map2.reserve(data2.size());
    map3.reserve(data3.size());
    map4.reserve(data4.size());

    map1.insert(data1.begin(), data1.end());
    map2.insert(data2.begin(), data2.end());
    map3.insert(data3.begin(), data3.end());
    map4.insert(data4.begin(), data4.end());

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
