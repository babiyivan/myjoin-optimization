#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "robin_hood.h"

using namespace std;

// Read a CSV file into a vector of pairs
vector<pair<string, string>> read_file(const string &filename) {
    vector<pair<string, string>> data;
    ifstream file(filename);
    string line;

    file.seekg(0, ios::end);
    size_t estimated_lines = file.tellg() / 50; // Rough estimate
    file.seekg(0, ios::beg);

    data.reserve(estimated_lines); // Preallocate memory for efficiency

    while (getline(file, line)) {
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

    // Initialize maps with preallocated size
    size_t data2_size = data2.size();
    size_t data3_size = data3.size();
    size_t data4_size = data4.size();

    robin_hood::unordered_map<string, vector<string>> map2;
    robin_hood::unordered_map<string, vector<string>> map3;
    robin_hood::unordered_map<string, vector<string>> map4;

    map2.reserve(data2_size);
    map3.reserve(data3_size);
    map4.reserve(data4_size);

    // Populate maps
    for (const auto& [key, value] : data2) {
        map2[key].push_back(value);
    }
    for (const auto& [key, value] : data3) {
        map3[key].push_back(value);
    }
    for (const auto& [key, value] : data4) {
        map4[key].push_back(value);
    }

    // Output results directly to cout in manageable chunks
    string output_chunk;
    output_chunk.reserve(8192); // Reserve a large buffer for each chunk

    for (const auto &[key1, value1] : data1) {
        // Perform all lookups once and reuse iterators
        auto it2 = map2.find(key1);
        auto it3 = map3.find(key1);

        if (it2 == map2.end() || it3 == map3.end())
            continue;

        for (const auto& value2 : it2->second) {
            for (const auto& value3 : it3->second) {
                auto it4 = map4.find(value3);
                if (it4 != map4.end()) {
                    for (const auto& value4 : it4->second) {
                        output_chunk.append(value3).append(",")
                                    .append(key1).append(",")
                                    .append(value1).append(",")
                                    .append(value2).append(",")
                                    .append(value4).append("\n");

                        // Flush chunk if it exceeds a certain size
                        if (output_chunk.size() > 65536) { // Flush every 64 KB
                            cout << output_chunk;
                            output_chunk.clear();
                        }
                    }
                }
            }
        }
    }

    // Flush remaining output
    if (!output_chunk.empty()) {
        cout << output_chunk;
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
