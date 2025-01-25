#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "robin_hood.h"

using namespace std;

// Read a CSV file into a vector of pairs using std::string_view
vector<pair<string_view, string_view>> read_file(const string &filename, string &file_content) {
    ifstream file(filename, ios::ate | ios::binary);
    if (!file) {
        cerr << "Error: Unable to open file " << filename << "\n";
        exit(1);
    }

    // Read the entire file into memory
    size_t file_size = file.tellg();
    file.seekg(0, ios::beg);
    file_content.resize(file_size);
    file.read(file_content.data(), file_size);

    // Parse the file content into key-value pairs
    vector<pair<string_view, string_view>> data;
    size_t estimated_lines = file_size / 50; // Rough estimate of average line size
    data.reserve(estimated_lines);

    size_t start = 0;
    size_t end = 0;
    while ((end = file_content.find('\n', start)) != string::npos) {
        size_t delim = file_content.find(',', start);
        if (delim != string::npos && delim < end) {
            data.emplace_back(
                string_view(file_content.data() + start, delim - start),
                string_view(file_content.data() + delim + 1, end - delim - 1));
        }
        start = end + 1;
    }

    return data;
}

void my_join(const string &file1, const string &file2, const string &file3, const string &file4) {
    // Read files into memory
    string file1_content, file2_content, file3_content, file4_content;
    auto data1 = read_file(file1, file1_content);
    auto data2 = read_file(file2, file2_content);
    auto data3 = read_file(file3, file3_content);
    auto data4 = read_file(file4, file4_content);

    // Initialize robin_hood::unordered_map with vectors to handle multiple values
    robin_hood::unordered_map<string_view, vector<string_view>> map2;
    robin_hood::unordered_map<string_view, vector<string_view>> map3;
    robin_hood::unordered_map<string_view, vector<string_view>> map4;

    for (const auto &[key, value] : data2) {
        map2[key].push_back(value);
    }
    for (const auto &[key, value] : data3) {
        map3[key].push_back(value);
    }
    for (const auto &[key, value] : data4) {
        map4[key].push_back(value);
    }

    // Use a single output buffer to minimize I/O operations
    ostringstream output_buffer;

    for (const auto &[key1, value1] : data1) {
        // Check matches in map2 and map3
        auto it2 = map2.find(key1);
        auto it3 = map3.find(key1);

        if (it2 == map2.end() || it3 == map3.end())
            continue;

        for (const auto &value2 : it2->second) {
            for (const auto &value3 : it3->second) {
                // Look up map4 matches for the current value from map3
                auto it4 = map4.find(value3);
                if (it4 != map4.end()) {
                    for (const auto &value4 : it4->second) {
                        output_buffer << value3 << "," << key1 << "," << value1 << ","
                                      << value2 << "," << value4 << "\n";
                    }
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
