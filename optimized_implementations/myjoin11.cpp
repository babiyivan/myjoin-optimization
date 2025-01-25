//
// Created by Ivan Babiy on 25/01/2025.
//
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

// Read a CSV file into a vector of pairs
vector<pair<string, string>> read_file(const string &filename) {
    vector<pair<string, string>> data;
    ifstream file(filename);
    string line;

    file.seekg(0, ios::end);
    size_t estimated_lines = file.tellg() / 50; // Rough estimate
    file.seekg(0, ios::beg);

    data.reserve(estimated_lines);

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

// Binary search helper: Find all matches for a key in sorted data
vector<pair<string, string>> find_all_matches(const vector<pair<string, string>> &sorted_data, const string &key) {
    vector<pair<string, string>> matches;

    auto range = equal_range(sorted_data.begin(), sorted_data.end(), make_pair(key, ""),
                             [](const pair<string, string> &a, const pair<string, string> &b) {
                                 return a.first < b.first;
                             });

    for (auto it = range.first; it != range.second; ++it) {
        matches.push_back(*it);
    }

    return matches;
}

void my_join(const string &file1, const string &file2, const string &file3, const string &file4) {
    auto data1 = read_file(file1);
    auto data2 = read_file(file2);
    auto data3 = read_file(file3);
    auto data4 = read_file(file4);

    // Sort data2, data3, and data4 by key for binary search
    sort(data2.begin(), data2.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });

    sort(data3.begin(), data3.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });

    sort(data4.begin(), data4.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });

    ostringstream output_buffer;

    for (const auto &[key1, value1] : data1) {
        // Binary search for matches in data2 and data3
        auto matches2 = find_all_matches(data2, key1);
        auto matches3 = find_all_matches(data3, key1);

        if (matches2.empty() || matches3.empty())
            continue;

        for (const auto &[key2, value2] : matches2) {
            for (const auto &[key3, value3] : matches3) {
                // Binary search for matches in data4
                auto matches4 = find_all_matches(data4, value3);

                for (const auto &[key4, value4] : matches4) {
                    output_buffer << value3 << "," << key1 << "," << value1 << ","
                                  << value2 << "," << value4 << "\n";
                }
            }
        }
    }

    // Flush the buffer to standard output
    cout << output_buffer.str();
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <file1> <file2> <file3> <file4>\n";
        return 1;
    }

    my_join(argv[1], argv[2], argv[3], argv[4]);
    return 0;
}
