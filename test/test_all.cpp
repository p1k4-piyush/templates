#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

int main()
{
    string base_dir = fs::exists("test debug") ? "test debug" : ".";
    vector<string> tests;
    for (const auto& entry : fs::directory_iterator(base_dir)) {
        string path = entry.path().string();
        if (path.find("test_all.cpp") != string::npos)
            continue;
        if (entry.path().extension() == ".cpp") {
            tests.push_back(entry.path().stem().string());
        }
    }

    // Sort to have consistent order
    sort(tests.begin(), tests.end());

    vector<string> failed_tests;

    for (const string& test : tests) {
        cout << "\n============================================\n";
        cout << "Running test: " << test << "...\n";
        cout << "============================================\n";

        string compile_path = base_dir + "/" + test + ".cpp";
        string cmd = "g++ -std=c++20 -DLOCAL -DGRACIE \"" + compile_path + "\" -o /tmp/test_runner && /tmp/test_runner 2> /tmp/test_log.txt";
        int res = system(cmd.c_str());
        if (res != 0) {
            cerr << "Test " << test << " failed to compile or run.\n";
            failed_tests.push_back(test);
            cout << "\nPress Enter to continue to the next test...";
            string dummy;
            getline(cin, dummy);
            continue;
        }

        ifstream in("/tmp/test_log.txt");
        string line;
        string last_title = "";
        while (getline(in, line)) {
            // Strip ANSI codes
            line = regex_replace(line, regex("\x1B\\[[0-9;]*[a-zA-Z]"), "");

            // Capture preceding literal titles directly if they exist
            smatch title_match;
            if (regex_search(line, title_match, regex(R"(^\[\s*\d+\]\s+\"[^\"]+\"\s+=\s*\"([^\"]+)\")"))) {
                last_title = title_match[1];
            }

            smatch match;
            regex rgx(R"((?:<graphviz:([^>]+)>))");
            if (regex_search(line, match, rgx)) {
                if (match.size() >= 2) {
                    string title = last_title.empty() ? "Struct_" + to_string(rand() % 1000) : last_title;
                    string old_file = match[1];

                    // Force the old file path to implicitly include base_dir if needed
                    string full_old_file = base_dir + "/" + old_file;
                    if (!fs::exists(full_old_file))
                        full_old_file = old_file;

                    string new_file = title + ".png";
                    for (char& c : new_file)
                        if (c == ' ')
                            c = '_';
                    string full_new_file = base_dir + "/" + new_file;

                    string full_old_dot = full_old_file.substr(0, full_old_file.find_last_of('.')) + ".dot";
                    string new_dot = title + ".dot";
                    for (char& c : new_dot)
                        if (c == ' ')
                            c = '_';
                    string full_new_dot = base_dir + "/" + new_dot;

                    rename(full_old_file.c_str(), full_new_file.c_str());
                    rename(full_old_dot.c_str(), full_new_dot.c_str());

                    regex replace_rgx(old_file);
                    line = regex_replace(line, replace_rgx, new_file);

                    // Automatically open the image
                    system(("open \"" + full_new_file + "\"").c_str());

                    // Reset title for following structs
                    last_title = "";
                }
            }
            cerr << line << "\n";
        }

        cout << "\nPress Enter to continue to the next test...";
        string dummy;
        getline(cin, dummy);
    }

    if (!failed_tests.empty()) {
        cout << "\n============================================\n";
        cout << "Failed Tests (RTE/TLE/Compile Error):\n";
        for (const string& ft : failed_tests) {
            cout << " - " << ft << "\n";
        }
        cout << "============================================\n";
    } else {
        cout << "\n============================================\n";
        cout << "All tests passed successfully!\n";
        cout << "============================================\n";
    }

    return 0;
}
