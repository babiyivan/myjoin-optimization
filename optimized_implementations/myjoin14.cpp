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
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

// Read a CSV file into a vector of pairs using mmap
vector<pair<string, string>> read_file(const string &filename) {
    vector<pair<string, string>> data;
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        cerr << "Error opening file: " << filename << "\n";
        return data;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        cerr << "Error getting file size: " << filename << "\n";
        close(fd);
        return data;
    }

    size_t file_size = sb.st_size;
    if (file_size == 0) {
        close(fd);
        return data;
    }

    char *file_in_memory = static_cast<char*>(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (file_in_memory == MAP_FAILED) {
        cerr << "Error mapping file: " << filename << "\n";
        close(fd);
        return data;
    }

    size_t estimated_lines = file_size / 50; // Rough estimate
    data.reserve(estimated_lines);

    size_t start = 0;
    for (size_t i = 0; i < file_size; ++i) {
        if (file_in_memory[i] == '\n') {
            size_t line_length = i - start;
            string line(file_in_memory + start, line_length);
            size_t delim = line.find(',');
            if (delim != string::npos) {
                data.emplace_back(
                    line.substr(0, delim),
                    line.substr(delim + 1)
                );
            }
            start = i + 1;
        }
    }

    // Handle last line if it doesn't end with a newline
    if (start < file_size) {
        string line(file_in_memory + start, file_size - start);
        size_t delim = line.find(',');
        if (delim != string::npos) {
            data.emplace_back(
                line.substr(0, delim),
                line.substr(delim + 1)
            );
        }
    }

    munmap(file_in_memory, file_size);
    close(fd);
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

    // Use a single output buffer to minimize I/O operations
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
                    output_buffer << it3->second << "," << key1 << "," << value1 << ","
                                  << it2->second << "," << it4->second << "\n";
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