#include <set>

#include <iostream>
#include <fstream>
#include <vector>

#include <cstring>

#include <sys/stat.h>
#include <stdio.h>

using namespace std;

string DEFAULT_CONFIG_PATH = string(getenv("HOME")) + string("/.ijcp/ijcp.conf");
string DEFAULT_CLIPBOARD_PATH = string(getenv("HOME")) + string("/.ijcp/clipboard");
string DEFAULT_IJCP_PATH = string(getenv("HOME")) + string("/.ijcp/");

bool VERBOSE = false;

struct stat sb;

bool isDirectory(const char*);
set<char> getArgs(int, char**);
vector<string> getParams(int, char**);
string resolvePath(string);

int main(int argc, char* argv[]) {
    // Configure arguments
    set<char> args = getArgs(argc, argv);

    // Detect parameters (filepaths)
    vector<string> params = getParams(argc, argv);

    // Verbose output triggered by 'v' or 'V'
    VERBOSE = args.count('v') || args.count('V');

    if (VERBOSE) {
        cout << "It's Just Copy-Paste (C) Langston Chadler 2018" << endl;
    }

    // Ensure that IJCP has a folder for configuration and copy-paste
    if (!isDirectory(DEFAULT_IJCP_PATH.c_str())) {
        if (VERBOSE) {
            cout << "\nInitializing default clipboard path at " << DEFAULT_IJCP_PATH << endl;
        }
        system((string("mkdir ") + string(DEFAULT_IJCP_PATH)).c_str());
        if (!isDirectory(DEFAULT_IJCP_PATH.c_str())) {
            cerr << "Initialization failed for default clipboard path: " << DEFAULT_IJCP_PATH << endl;
            return -3;
        }
    }

    // If input is piped, copy
    if (args.count('c') || args.count('C')) {
        unsigned int size = 0;
        ofstream output;
        output.open(DEFAULT_CLIPBOARD_PATH);
        if (!output.is_open()) {
            cerr << "Unable to open IJCP clipboard" << endl;
            return -2;
        }
        string word;
        if (getline(cin, word, '\0')) {
            output << word;
            size += word.length();
        }
        output.close();
        cout << "\nCopied " << size << " bytes to clipboard" << endl;
    }
    else if (args.count('p') || args.count('P')) {
        if (args.count('o') || args.count('O')) {
            if (params.empty()) {
                cerr << "Output filename required" << endl;
                return -1;
            }
            for (string fileName : params) {
                if (VERBOSE) {
                    cout << "Outputting to " << fileName << "... ";
                }
                ifstream input;
                input.open(DEFAULT_CLIPBOARD_PATH);
                if (!input.is_open()) {
                    cerr << "Unable to open " << DEFAULT_CLIPBOARD_PATH << endl;
                }
                ofstream output;
                output.open(fileName);
                if (!output.is_open()) {
                    cerr << "Unable to open " << fileName << endl;
                }
                string word;
                if (getline(input, word, '\0')) {
                    output << word;
                }
                output.close();
                if (VERBOSE) {
                    cout << "Done." << endl;
                }
            }
        }
        else {
            ifstream input;
            input.open(DEFAULT_CLIPBOARD_PATH);
            if (!input.is_open()) {
                cerr << "Unable to open " << DEFAULT_CLIPBOARD_PATH << endl;
            }
            string word;
            if (getline(input, word, '\0')) {
                cout << word;
            }
            input.close();
        }
    }
    else {
        if (params.empty()) {
            cerr << "Input filename required" << endl;
            return -1;
        }
        ifstream input;
        input.open(params[0]);
        if (!input.is_open()) {
            cerr << "Unable to open " << params[0] << endl;
        }
        ofstream output;
        output.open(DEFAULT_CLIPBOARD_PATH.c_str());
        if (!output.is_open()) {
            cerr << "Unable to open IJCP clipboard" << endl;
            return -2;
        }
        int size;
        string word;
        if (getline(input, word, '\0')) {
            output << word;
            size += word.length();
        }
        input.close();
        output.close();
        cout << "\nCopied " << size << " bytes to clipboard" << endl;
    }

    return 0;
}

bool isDirectory(const char* pathName) {
    return stat(pathName, &sb) == 0 && S_ISDIR(sb.st_mode);
}

set<char> getArgs(int argc, char** argv) {
    set<char> s;
    int l;
    for (int i = 1; i < argc; i++) {
        l = strlen(argv[i]);
        if (l > 0 && argv[i][0] == '-') {
            for (int j = 1; j < l; j++) {
                s.insert(argv[i][j]);
            }
        }
    }
    return s;
}

vector<string> getParams(int argc, char** argv) {
    vector<string> v;
    int l;
    for (int i = 1; i < argc; i++) {
        l = strlen(argv[i]);
        if (l > 0 && argv[i][0] != '-') {
            v.push_back(string(argv[i]));
        }
    }
    return v;
}

string resolvePath(string path) {
    if (path.length() < 2) return "";
    if (path[0] == '.') return string("pwd") + path.substr(1);
    if (path[0] == '~') return string(getenv("HOME")) + path.substr(1);
}