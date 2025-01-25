//
// Created by Ivan Babiy on 25/01/2025.
//
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// Interning Helper: Maps strings to unique integer IDs
class StringInterner {
private:
    unordered_map<string, int> string_to_id;
    vector<string> id_to_string;
    int next_id = 0;

public:
    // Get or assign an ID for a string
    int intern(const string &s) {
        auto it = string_to_id.find(s);
        if (it != string_to_id.end()) {
            return it->second;
        }
        string_to_id[s] = next_id;
        id_to_string.push_back(s);
        return next_id++;
    }

    // Get the original string from an ID
    const string &resolve(int id) const {
        return id_to_string[id];
    }
};

// Read a CSV file and intern its strings
vector<pair<int, int>> read_file_and_intern(const string &filename, StringInterner &interner) {
    vector<pair<int, int>> data;
    ifstream file(filename);
    string line;

    file.seekg(0, ios::end);
    size_t estimated_lines = file.tellg() / 50; // Rough estimate
    file.seekg(0, ios::beg);

    data.reserve(estimated_lines);

    while (getline(file, line)) {
        size_t delim = line.find(',');
        if (delim != string::npos) {
            int key_id = interner.intern(line.substr(0, delim));
            int value_id = interner.intern(line.substr(delim + 1));
            data.emplace_back(key_id, value_id);
        }
    }

    return data;
}

void my_join(const string &file1, const string &file2, const string &file3, const string &file4) {
    StringInterner interner;

    // Read files and intern strings
    auto data1 = read_file_and_intern(file1, interner);
    auto data2 = read_file_and_intern(file2, interner);
    auto data3 = read_file_and_intern(file3, interner);
    auto data4 = read_file_and_intern(file4, interner);

    // Initialize unordered_multimap for efficient lookups
    unordered_multimap<int, int> map2(data2.begin(), data2.end());
    unordered_multimap<int, int> map3(data3.begin(), data3.end());
    unordered_multimap<int, int> map4(data4.begin(), data4.end());

    ostringstream output_buffer;

    for (const auto &[key1, value1] : data1) {
        // Check matches in map2 and map3
        auto range2 = map2.equal_range(key1);
        auto range3 = map3.equal_range(key1);

        if (range2.first == range2.second || range3.first == range3.second)
            continue;

        for (auto it2 = range2.first; it2 != range2.second; ++it2) {
            for (auto it3 = range3.first; it3 != range3.second; ++it3) {
                // Look up map4 matches for the current value from map3
                auto range4 = map4.equal_range(it3->second);
                for (auto it4 = range4.first; it4 != range4.second; ++it4) {
                    output_buffer << interner.resolve(it3->second) << ","
                                  << interner.resolve(key1) << ","
                                  << interner.resolve(value1) << ","
                                  << interner.resolve(it2->second) << ","
                                  << interner.resolve(it4->second) << "\n";
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
