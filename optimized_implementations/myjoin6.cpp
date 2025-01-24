//
// Created by Jakob Petermandl on 23/01/25.
//
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
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

    unordered_multimap<string, string> map1;
    unordered_multimap<string, string> map2;
    unordered_multimap<string, string> map3;
    unordered_multimap<string, string> map4;

    map1.reserve(data1.size());
    map2.reserve(data2.size());
    map3.reserve(data3.size());
    map4.reserve(data4.size());

    map1.insert(data1.begin(), data1.end());
    map2.insert(data2.begin(), data2.end());
    map3.insert(data3.begin(), data3.end());
    map4.insert(data4.begin(), data4.end());

    std::ostringstream buffer; // Use a buffer for efficient output

    for (const auto &[key1, value1] : map1) {
        auto range2 = map2.equal_range(key1);
        auto range3 = map3.equal_range(key1);
        if (range2.first == range2.second || range3.first == range3.second) continue; // Skip if no matches in map2 or map3

        for (auto it2 = range2.first; it2 != range2.second; ++it2) {
            auto it3 = range3.first;
            while (it3 != range3.second) {
                // Unroll by 2 for it3 loop
                if (it3 != range3.second) {
                    auto range4 = map4.equal_range(it3->second);
                    auto it4 = range4.first;
                    while (it4 != range4.second) {
                        // Unroll by 2 for it4 loop
                        if (it4 != range4.second) {
                            buffer << it3->second << "," << key1 << "," << value1 << ","
                                   << it2->second << "," << it4->second << "\n";
                            ++it4;
                        }
                        if (it4 != range4.second) {
                            buffer << it3->second << "," << key1 << "," << value1 << ","
                                   << it2->second << "," << it4->second << "\n";
                            ++it4;
                        }
                    }
                    ++it3;
                }
                if (it3 != range3.second) {
                    auto range4 = map4.equal_range(it3->second);
                    auto it4 = range4.first;
                    while (it4 != range4.second) {
                        // Unroll by 2 for it4 loop
                        if (it4 != range4.second) {
                            buffer << it3->second << "," << key1 << "," << value1 << ","
                                   << it2->second << "," << it4->second << "\n";
                            ++it4;
                        }
                        if (it4 != range4.second) {
                            buffer << it3->second << "," << key1 << "," << value1 << ","
                                   << it2->second << "," << it4->second << "\n";
                            ++it4;
                        }
                    }
                    ++it3;
                }
            }
        }
    }

    // Flush the buffer to standard output in one go
    std::cout << buffer.str();
}


int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <file1> <file2> <file3> <file4>\n";
        return 1;
    }

    my_join(argv[1], argv[2], argv[3], argv[4]);
    return 0;
}
