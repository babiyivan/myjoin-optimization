#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

vector<string> split(const string &line, char delimiter) {
    vector<string> tokens;
    stringstream ss(line);
    string token;

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

// Read a CSV file and store its contents in an unordered_map
unordered_map<string, vector<string>> read_file(const string &filename) {
    unordered_map<string, vector<string>> data;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        auto tokens = split(line, ',');
        if (tokens.size() == 2) {
            data[tokens[0]].push_back(tokens[1]);
        }
    }

    return data;
}

void my_join(const string &file1, const string &file2, const string &file3, const string &file4) {
    auto map1 = read_file(file1);
    auto map2 = read_file(file2);
    auto map3 = read_file(file3);
    auto map4 = read_file(file4);

    for (const auto &pair1 : map1) {
        const string &key1 = pair1.first;
        const vector<string> &values1 = pair1.second;

        if (map2.find(key1) != map2.end() && map3.find(key1) != map3.end()) {
            const vector<string> &values2 = map2[key1];
            const vector<string> &values3 = map3[key1];

            for (const auto &value1 : values1) {
                for (const auto &value2 : values2) {
                    for (const auto &value3 : values3) {
                        if (map4.find(value3) != map4.end()) {
                            const vector<string> &values4 = map4[value3];
                            for (const auto &value4 : values4) {
                                cout << value3 << "," << key1 << "," << value1 << "," << value2 << "," << value4 << "\n";
                            }
                        }
                    }
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
