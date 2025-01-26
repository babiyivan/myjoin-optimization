//
// Aleman Mihnea 25/01/25
// use string_view instead of string to avoid unnecessary allocations
// source: https://jaredmil.medium.com/c-std-string-view-explained-89df5ba6b25e
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <string_view>

using namespace std;

// Function to split a string into tokens using a delimiter
vector<string_view> split(const string_view &line, char delimiter) {
    vector<string_view> tokens;
    size_t start = 0;
    size_t end;

    while ((end = line.find(delimiter, start)) != string_view::npos) {
        tokens.emplace_back(line.substr(start, end - start));
        start = end + 1;
    }

    if (start < line.size()) {
        tokens.emplace_back(line.substr(start));
    }

    return tokens;
}

// Read a CSV file and return its contents as a vector of pairs
vector<pair<string_view, string_view>> read_file(const string &filename, string &file_content) {
    vector<pair<string_view, string_view>> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << "\n";
        return data;
    }

    // Read entire file content to a string
    stringstream buffer;
    buffer << file.rdbuf();
    file_content = buffer.str();

    string_view content_view(file_content);
    size_t line_start = 0;
    size_t line_end;

    while ((line_end = content_view.find('\n', line_start)) != string_view::npos) {
        string_view line = content_view.substr(line_start, line_end - line_start);
        auto tokens = split(line, ',');
        if (tokens.size() == 2) {
            data.emplace_back(tokens[0], tokens[1]);
        }
        line_start = line_end + 1;
    }

    // Handle the last line if not empty
    if (line_start < content_view.size()) {
        string_view line = content_view.substr(line_start);
        auto tokens = split(line, ',');
        if (tokens.size() == 2) {
            data.emplace_back(tokens[0], tokens[1]);
        }
    }

    return data;
}

// Optimized my_join function using string_view
void my_join(const string &file1, const string &file2, const string &file3, const string &file4) {
    string file_content1, file_content2, file_content3, file_content4;

    auto data1 = read_file(file1, file_content1);
    auto data2 = read_file(file2, file_content2);
    auto data3 = read_file(file3, file_content3);
    auto data4 = read_file(file4, file_content4);

    multimap<string_view, string_view> map1(data1.begin(), data1.end());
    multimap<string_view, string_view> map2(data2.begin(), data2.end());
    multimap<string_view, string_view> map3(data3.begin(), data3.end());
    multimap<string_view, string_view> map4(data4.begin(), data4.end());

    for (const auto &[key1, value1] : map1) {
        auto range2 = map2.equal_range(key1);
        auto range3 = map3.equal_range(key1);

        for (auto it2 = range2.first; it2 != range2.second; ++it2) {
            for (auto it3 = range3.first; it3 != range3.second; ++it3) {
                auto range4 = map4.equal_range(it3->second);

                for (auto it4 = range4.first; it4 != range4.second; ++it4) {
                    cout << it3->second << "," << key1 << "," << value1 << "," << it2->second << "," << it4->second << "\n";
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

