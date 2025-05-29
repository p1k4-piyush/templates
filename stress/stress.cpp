
//	https://github.com/p1k4-piyush/templates/

#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

string getBinaryName(const string& filename)
{
    size_t slash = filename.find_last_of("/\\");
    string base = (slash == string::npos) ? filename : filename.substr(slash + 1);
    size_t dot = base.rfind('.');
    if (dot != string::npos) {
        return base.substr(0, dot);
    }
    return base;
}

string readFile(const string& filename)
{
    ifstream file(filename);
    stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

int main(int argc, char* argv[])
{
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " generator.cpp brute.cpp sol.cpp n" << endl;
        return 1;
    }

    string genSource = argv[1];
    string bruteSource = argv[2];
    string solSource = argv[3];
    int testCount = atoi(argv[4]);

    string genBinary = getBinaryName(genSource);
    string bruteBinary = getBinaryName(bruteSource);
    string solBinary = getBinaryName(solSource);

    string compileGen = " g++ -std=c++23 -O3 -fsanitize=undefined,address " + genSource + " -o " + genBinary;

    string compileBrute = "g++ -std=c++23 -O3 -fsanitize=undefined,address " + bruteSource + " -o " + bruteBinary;

    string compileSol = "g++ -std=c++23 -O3 -fsanitize=undefined,address " + solSource + " -o " + solBinary;

    if (system(compileGen.c_str()) != 0) {
        cerr << "Compilation failed for " << genSource << endl;
        return 1;
    }

    if (system(compileBrute.c_str()) != 0) {
        cerr << "Compilation failed for " << bruteSource << endl;
        return 1;
    }

    if (system(compileSol.c_str()) != 0) {
        cerr << "Compilation failed for " << solSource << endl;
        return 1;
    }

    double maxSolTime = 0.0;
    int maxTestCaseNumber = 0;
    string maxTestInput = "";
    string maxBruteOutput = "";
    string maxSolOutput = "";

    int ttt = testCount / 10;
    for (int i = 1; i <= testCount; i++) {
        auto loopStart = high_resolution_clock::now();
        auto genStart = high_resolution_clock::now();

        string genCmd = "./" + genBinary + " > input.txt";

        if (system(genCmd.c_str()) != 0) {
            cerr << "Error running generator" << endl;
            return 1;
        }

        auto genStop = high_resolution_clock::now();
        auto genDuration = duration_cast<duration<double>>(genStop - genStart);
        auto bruteStart = high_resolution_clock::now();

        string runBrute = "./" + bruteBinary + " < input.txt > out_brute.txt";

        if (system(runBrute.c_str()) != 0) {
            cerr << "Error running brute solution" << endl;
            return 1;
        }

        auto bruteStop = high_resolution_clock::now();
        auto bruteDuration = duration_cast<duration<double>>(bruteStop - bruteStart);
        auto solStart = high_resolution_clock::now();

        string runSol = "./" + solBinary + " < input.txt > out_sol.txt";

        if (system(runSol.c_str()) != 0) {
            cerr << "Error running optimized solution" << endl;
            return 1;
        }

        auto solStop = high_resolution_clock::now();
        auto solDuration = duration_cast<duration<double>>(solStop - solStart);

        string inputContent = readFile("input.txt");
        string bruteOutput = readFile("out_brute.txt");
        string solOutput = readFile("out_sol.txt");

        auto loopStop = high_resolution_clock::now();
        auto loopDuration = duration_cast<duration<double>>(loopStop - loopStart);

        if (i == 1 || (i % ttt) == 0) {
            cout << "Test case " << i << " timings:" << endl;
            cout << "  Generator: " << genDuration.count() << " seconds" << endl;
            cout << "  Brute-force: " << bruteDuration.count() << " seconds" << endl;
            cout << "  Optimized: " << solDuration.count() << " seconds" << endl;
            cout << "  Total loop: " << loopDuration.count() << " seconds" << endl;
        }

        if (bruteOutput != solOutput) {
            // cout << "Test case " << i << " failed." << endl;

            // cout << "Input:" << endl;
            // cout << inputContent << endl;

            // cout << "Brute-force output:" << endl;
            // cout << bruteOutput << endl;

            // cout << "Optimized solution output:" << endl;
            // cout << solOutput << endl;

            cout << "Incorrect solution" << endl;
            return 0;
        } else {
            if (i == 1 || (i % ttt) == 0)
                cout << "Test case " << i << " passed." << endl;
        }

        if (solDuration.count() > maxSolTime) {
            maxSolTime = solDuration.count();
            maxTestCaseNumber = i;
            maxTestInput = inputContent;
            maxBruteOutput = bruteOutput;
            maxSolOutput = solOutput;
        }
    }

    cout << "----------------------------------------" << endl;
    cout << "Maximum optimized solution time was " << maxSolTime << " seconds" << endl;
    // cout << "Occurred at test case " << maxTestCaseNumber << " with input:" << endl;
    // cout << maxTestInput << endl;
    // cout << "Brute-force output:" << endl;
    // cout << maxBruteOutput << endl;
    // cout << "Optimized solution output:" << endl;
    // cout << maxSolOutput << endl;
    {
        ofstream inFile("input.txt");
        if (inFile.is_open()) {
            inFile << maxTestInput;
            inFile.close();
        } else {
            cerr << "Failed to open input.txt for writing." << endl;
        }
    }
    {
        ofstream outFile("output.txt");
        if (outFile.is_open()) {
            outFile << maxSolOutput;
            outFile.close();
        } else {
            cerr << "Failed to open output.txt for writing." << endl;
        }
    }
    cout << "----------------------------------------" << endl;

    cout << "All test cases passed." << endl;

    return 0;
}
