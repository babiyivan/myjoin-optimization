//
// Created by Artur Ohanian on 26/01/2025.
//
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <future>
#include <mutex>
#include <thread>
#include "robin_hood.h"

using namespace std;

const size_t BUFFER_SIZE = 1 << 20; // 1MB buffer per thread

struct FileData {
    string content;
    vector<pair<string_view, string_view>> pairs;
};

FileData read_file(const string& filename) {
    FileData fd;
    ifstream file(filename, ios::binary | ios::ate);
    const size_t size = file.tellg();
    file.seekg(0);
    fd.content.resize(size);
    file.read(fd.content.data(), size);

    size_t start = 0;
    while (start < fd.content.size()) {
        size_t end = fd.content.find('\n', start);
        if (end == string::npos) end = fd.content.size();
        
        size_t delim = fd.content.find(',', start);
        if (delim < end) {
            fd.pairs.emplace_back(
                string_view(fd.content.data() + start, delim - start),
                string_view(fd.content.data() + delim + 1, end - delim - 1)
            );
        }
        start = end + 1;
    }
    return fd;
}

void my_join(const string& file1, const string& file2, 
             const string& file3, const string& file4) {
    // Parallel file reading
    auto future1 = async(launch::async, read_file, file1);
    auto future2 = async(launch::async, read_file, file2);
    auto future3 = async(launch::async, read_file, file3);
    auto future4 = async(launch::async, read_file, file4);

    auto f1 = future1.get();
    auto f2 = future2.get();
    auto f3 = future3.get();
    auto f4 = future4.get();

    // Parallel map building
    robin_hood::unordered_map<string_view, vector<string_view>> map2, map3, map4;
    
    auto build_map2 = [&] {
        map2.reserve(f2.pairs.size());
        for (const auto& [k,v] : f2.pairs) map2[k].emplace_back(v);
    };
    
    auto build_map3 = [&] {
        map3.reserve(f3.pairs.size());
        for (const auto& [k,v] : f3.pairs) map3[k].emplace_back(v);
    };
    
    auto build_map4 = [&] {
        map4.reserve(f4.pairs.size());
        for (const auto& [k,v] : f4.pairs) map4[k].emplace_back(v);
    };

    thread t2(build_map2);
    thread t3(build_map3);
    thread t4(build_map4);
    t2.join(); t3.join(); t4.join();

    // Parallel processing
    mutex cout_mutex;
    const size_t num_threads = thread::hardware_concurrency();
    const size_t chunk_size = f1.pairs.size() / num_threads;
    vector<thread> workers;

    auto process_chunk = [&](size_t start, size_t end) {
        string buffer;
        buffer.reserve(BUFFER_SIZE);

        for (size_t i = start; i < end; ++i) {
            const auto& [key1, value1] = f1.pairs[i];
            auto it2 = map2.find(key1);
            auto it3 = map3.find(key1);

            if (it2 == map2.end() || it3 == map3.end()) continue;

            for (const auto& value2 : it2->second) {
                for (const auto& value3 : it3->second) {
                    if (auto it4 = map4.find(value3); it4 != map4.end()) {
                        for (const auto& value4 : it4->second) {
                            buffer.append(value3).append(",")
                                  .append(key1).append(",")
                                  .append(value1).append(",")
                                  .append(value2).append(",")
                                  .append(value4).append("\n");

                            if (buffer.size() >= BUFFER_SIZE) {
                                lock_guard lock(cout_mutex);
                                cout << buffer;
                                buffer.clear();
                            }
                        }
                    }
                }
            }
        }

        if (!buffer.empty()) {
            lock_guard lock(cout_mutex);
            cout << buffer;
        }
    };

    for (size_t t = 0; t < num_threads; ++t) {
        size_t start = t * chunk_size;
        size_t end = (t == num_threads-1) ? f1.pairs.size() : start + chunk_size;
        workers.emplace_back(process_chunk, start, end);
    }

    for (auto& t : workers) t.join();
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <file1> <file2> <file3> <file4>\n";
        return 1;
    }

    my_join(argv[1], argv[2], argv[3], argv[4]);
    return 0;
}