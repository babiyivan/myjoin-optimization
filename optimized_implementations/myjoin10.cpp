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

void my_join(const string &file1, const string &file2, const string &file3, const string &file4) {
    auto data1 = read_file(file1);
    auto data2 = read_file(file2);
    auto data3 = read_file(file3);
    auto data4 = read_file(file4);

    // Initialize unordered_multimap with reserved space to reduce rehashing
    unordered_multimap<string, string> map2(data2.begin(), data2.end());
    unordered_multimap<string, string> map3(data3.begin(), data3.end());
    unordered_multimap<string, string> map4(data4.begin(), data4.end());

    // Define a buffer size threshold for flushing output
    const size_t BUFFER_THRESHOLD = 1 << 20; // 1 MB buffer size
    ostringstream output_buffer;
    size_t current_buffer_size = 0;

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
                    output_buffer << it3->second << "," << key1 << "," << value1 << ","
                                  << it2->second << "," << it4->second << "\n";

                    // Keep track of the buffer size
                    current_buffer_size += output_buffer.str().size();

                    // Flush the buffer to stdout if it exceeds the threshold
                    if (current_buffer_size >= BUFFER_THRESHOLD) {
                        cout << output_buffer.str();
                        output_buffer.str("");    // Clear the buffer
                        output_buffer.clear();    // Reset error state
                        current_buffer_size = 0;  // Reset buffer size counter
                    }
                }
            }
        }
    }

    // Flush any remaining content in the buffer
    if (current_buffer_size > 0) {
        cout << output_buffer.str();
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
