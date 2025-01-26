/**
 * Author: Aleman Mihnea 25.01.2025
 *
 * This file contains an optimized implementation of a CSV join operation that reads four input files,
 * processes them, and outputs the joined results. The following optimizations have been applied:
 *
 * 1. Use `std::string_view` instead of `std::string` to avoid unnecessary memory allocations.
 *    - `std::string_view` is a non-owning view of a string, which means it does not allocate memory
 *      or copy the string data. This reduces memory usage and improves performance by avoiding
 *      unnecessary allocations and copies.
 *
 * 2. Use `robin_hood::unordered_map` instead of `std::unordered_map` for faster hash table lookups.
 *
 * 3. Read the entire file into memory at once to reduce I/O operations, instead of reading line by line.
 *
 * 4. Use a single output buffer to minimize I/O operations.
 *    - Using a single output buffer and writing to it in chunks reduces the number of I/O operations,
 *      which can improve performance when writing large amounts of data. 
 *    - ostringstream is not used because it can be less efficient.
 *
 * 5. Reserve map sizes to reduce reallocations.
 *    - Reserving the size of the maps in advance reduces the number of reallocations needed as elements
 *      are inserted, which can improve performance.
 *
 * 6. Use `emplace_back` instead of `push_back` for better performance.
 *    - `emplace_back` constructs the element in place directly in the container, which can avoid
 *      unnecessary copies or moves. This can lead to more efficient code by reducing temporary object
 *      creation and destruction.
 *
 * 7. No need to use map for the first file, as we iterate over it only once.
 */
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

    //open the file in binary mode and positions the pointer at the end
    ifstream file(filename, ios::ate | ios::binary);
    
    // Read the entire file into memory
    size_t file_size = file.tellg();
    file.seekg(0, ios::beg);
    file_content.resize(file_size);
    file.read(file_content.data(), file_size);

    // Parse the file content into key-value pairs
    vector<pair<string_view, string_view>> data;
    data.reserve(file_size / 50); // Rough estimate of average line size

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

    // Reserve map sizes to reduce reallocations
    robin_hood::unordered_map<string_view, vector<string_view>> map2(data2.size());
    robin_hood::unordered_map<string_view, vector<string_view>> map3(data3.size());
    robin_hood::unordered_map<string_view, vector<string_view>> map4(data4.size());

    for (const auto &[key, value] : data2) {
        map2[key].emplace_back(value);
    }
    for (const auto &[key, value] : data3) {
        map3[key].emplace_back(value);
    }
    for (const auto &[key, value] : data4) {
        map4[key].emplace_back(value);
    }

    // Use a single output buffer to minimize I/O operations
    string output_buffer;
    output_buffer.reserve(65536);


    for (const auto &[key1, value1] : data1) {
        auto it2 = map2.find(key1);
        auto it3 = map3.find(key1);

        if (it2 == map2.end() || it3 == map3.end())
            continue;

        const auto &values2 = it2->second;
        const auto &values3 = it3->second;

        for (const auto &value2 : values2) {
            for (const auto &value3 : values3) {
                auto it4 = map4.find(value3);
                if (it4 != map4.end()) {
                    const auto &values4 = it4->second;
                    for (const auto &value4 : values4) {
                        // Construct output line
                        output_buffer.append(value3).append(",").append(key1).append(",")
                                      .append(value1).append(",").append(value2).append(",")
                                      .append(value4).append("\n");
                        
                        if (output_buffer.size() >= 65536) {
                            cout << output_buffer;
                            output_buffer.clear();
                            output_buffer.reserve(65536); 
                        }
                    }
                }
            }
        }
    }

    // Write the output buffer to standard output
    cout << output_buffer;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <file1> <file2> <file3> <file4>\n";
        return 1;
    }

    my_join(argv[1], argv[2], argv[3], argv[4]);
    return 0;
}
