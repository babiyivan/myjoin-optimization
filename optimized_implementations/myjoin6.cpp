#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " file1 file2 file3 file4" << endl;
        return 1;
    }

    string file1_name = argv[1];
    string file2_name = argv[2];
    string file3_name = argv[3];
    string file4_name = argv[4];

    // Use unordered_multimap for all files to handle duplicates
    unordered_multimap<string, string> file1_data, file2_data, file3_data, file4_data;

    // Helper function to read data into an unordered_multimap (reuse for all files)
    auto read_file = [](const string& filename, unordered_multimap<string, string>& data) {
        ifstream file_stream(filename);
        string line;
        while (getline(file_stream, line)) {
            stringstream ss(line);
            string field1, field2;
            if (getline(ss, field1, ',') && getline(ss, field2, ',')) {
                data.insert({field1, field2});
            }
        }
        file_stream.close();
    };

    read_file(file1_name, file1_data);
    read_file(file2_name, file2_data);
    read_file(file3_name, file3_data);
    read_file(file4_name, file4_data);

    // Perform the join using structured bindings
    for (const auto& [a, b] : file1_data) {  // Structured bindings
        auto file2_range = file2_data.equal_range(a);
        for (auto it2 = file2_range.first; it2 != file2_range.second; ++it2) {
            auto& [_,c] = *it2; //discard _
            auto file3_range = file3_data.equal_range(a);
            for (auto it3 = file3_range.first; it3 != file3_range.second; ++it3) {
                auto& [_,d] = *it3;
                auto file4_range = file4_data.equal_range(d);
                for (auto it4 = file4_range.first; it4 != file4_range.second; ++it4) {
                    auto& [_, e] = *it4; //discard first
                    cout << d << "," << a << "," << b << "," << c << "," << e << endl;
                }
            }
        }
    }

    return 0;
}