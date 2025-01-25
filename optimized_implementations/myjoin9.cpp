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
#include <thread>
#include <mutex>

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

// Parallelized join worker
void process_chunk(const vector<pair<string, string>> &chunk,
                   const unordered_multimap<string, string> &map2,
                   const unordered_multimap<string, string> &map3,
                   const unordered_multimap<string, string> &map4,
                   ostringstream &output_buffer,
                   mutex &output_mutex) {
    ostringstream local_buffer;

    for (const auto &[key1, value1] : chunk) {
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
                    local_buffer << it3->second << "," << key1 << "," << value1 << ","
                                 << it2->second << "," << it4->second << "\n";
                }
            }
        }
    }

    // Safely add the local buffer to the shared output buffer
    lock_guard<mutex> lock(output_mutex);
    output_buffer << local_buffer.str();
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

    // Prepare multithreading
    size_t num_threads = thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4; // Default to 4 threads if hardware_concurrency is unavailable
    size_t chunk_size = data1.size() / num_threads;

    vector<thread> threads;
    vector<vector<pair<string, string>>> chunks;

    // Divide `data1` into chunks
    for (size_t i = 0; i < num_threads; ++i) {
        auto start = data1.begin() + i * chunk_size;
        auto end = (i == num_threads - 1) ? data1.end() : start + chunk_size;
        chunks.emplace_back(start, end);
    }

    ostringstream output_buffer;
    mutex output_mutex;

    // Launch threads to process chunks
    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back(process_chunk, ref(chunks[i]), ref(map2), ref(map3), ref(map4),
                             ref(output_buffer), ref(output_mutex));
    }

    // Wait for all threads to complete
    for (auto &t : threads) {
        t.join();
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
