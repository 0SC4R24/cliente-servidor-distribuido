#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <map>
#include <functional>
#include <vector>
#include "../../include/utils/socket.h"
#include "../../include/utils/tipos.h"
#include "../../include/filemanager/filemanager_stub.h"

//
// Created by Oscar on 7/11/2023
//

// Command handlers
void handleHelp() {
    cout << "Available commands:" << endl;
    cout << "  help     - Display available commands" << endl; // Done
    cout << "  ls       - Display all files inside file manager. Usage: ls -[l | r]" << endl; // Done
    cout << "  download - Downloads a file from the file manager. Usage: download <fileName>" << endl; // Done
    cout << "  upload   - Uploads a file to the file manager. Usage: upload <fileName>" << endl; // Done
    cout << "  pwd      - Displays the current path of the file manager, note it is in the server system." << endl; // Done
    cout << "  cat      - Displays the contents of a file. Usage: cat -[l | r] <fileName>" << endl; // Done
    cout << "  greet    - Greet the user. Usage: greet <name>" << endl; // Done
    cout << "  exit     - Exit the terminal" << endl; // Done
}

void handleGreet(vector<string>& args) {
    if (args.size() != 2) {
        cout << "Usage: greet <name>" << endl;
    } else {
        cout << "Hello, " << args[1] << "!" << endl;
    }
}

void handleExit() {
    cout << "Exiting the terminal..." << endl;
    exit(0);
}

void handlePwd(vector<string>& args) {
    cout << "/home/user/" << args[0] << endl;
}

void handleLs(vector<string>& args) {
    if (args.size() != 2) {
        cout << "Usage: ls -[l | r]" << endl;
    } else {
        if (args[1] == "-r") {
            FileManager_stub *fm = new FileManager_stub(args[0]);
            vector<string *> *vfiles = fm->listFiles();

            cout << "Files list in the remote directory " << args[0] << ":" << endl;
            for (unsigned int i = 0; i < vfiles->size(); i++)
            {
                cout << "  File: " << vfiles->at(i)->c_str() << endl;
            }

            fm->freeListedFiles(vfiles);

            delete fm;

            cout << endl;
        } else if (args[1] == "-l") {
            DIR *dir;
            struct dirent *ent;
            string path = ".";

            if ((dir = opendir(path.c_str())) != nullptr) {
                /* print all the files and directories within directory */
                cout << "Files list in the local directory " << path << ":" << endl;
                while ((ent = readdir(dir)) != nullptr) {
                    if (ent->d_type == DT_REG) {
                        string *f = new string(ent->d_name);
                        cout << "  File: " << f->c_str() << endl;
                    }
                }
                closedir(dir);
            } else {
                /* could not open directory */
                cout << "ERROR: No existe el fichero o directorio" << endl;
            }

            cout << endl;
        } else {
            cout << "Argument " << args[1] << " not found. Usage: ls -[l | r]" << endl;
        }
    }
}

void download(string path, string fileName) {
    FileManager_stub *fm = new FileManager_stub(path);
    vector<string *> *vfiles = fm->listFiles();
    bool exists = false;

    for (unsigned int i = 0; i < vfiles->size(); i++) {
        if (vfiles->at(i)->c_str() == fileName) {
            exists = true;
            break;
        }
    }
    fm->freeListedFiles(vfiles);

    if (!exists) {
        cout << "File doesn't exist" << endl;
        return;
    }

    char* file = (char*) fileName.c_str();
    char* data;
    unsigned long int dataLength = 0;
    
    fm->readFile(file, data, dataLength);

    string localPath = "./" + fileName;
    FILE *f = fopen(localPath.c_str(), "w");
    fwrite(data, dataLength, 1, f);
    fclose(f);

    delete fm;
    delete[] data;
}

void handleDownload(vector<string>& args) {
    if (args.size() != 2) {
        cout << "Usage: download <fileName>" << endl;
    } else {
        download(args[0], args[1]);
    }
}

bool checkIfFileExists(char* fileName, string path) {
    string file = string(fileName);
    file = path + "/" + file;
    FILE* f = fopen(file.c_str(), "r");
    bool exists = false;

    if (f != nullptr) {
        exists = true;
        fclose(f);
    } else {
        exists = false;
    }

    return exists;
}

void handleUpload(vector<string>& args) {
    if (args.size() != 2) {
        cout << "Usage: upload <fileName>" << endl;
    } else {
        char* fileName = (char*) args[1].c_str();
        if (!checkIfFileExists(fileName, ".")) {
            cout << "File doesn't exist" << endl;
            return;
        }

        string file = string(fileName);
        file = "./" + file;
        FILE *f = fopen(file.c_str(), "r");

        char* data;
        unsigned long int dataLength = 0;

        fseek(f, 0L, SEEK_END);
        dataLength = ftell(f);
        fseek(f, 0L, SEEK_SET);
        data = new char[dataLength];
        fread(data, dataLength, 1, f);

        FileManager_stub *fm = new FileManager_stub(args[0]);
        fm->writeFile(fileName, data, dataLength);

        delete fm;
        delete[] data;
        fclose(f);
    }
}

void handleCat(vector<string>& args) {
    if (args.size() != 3) {
        cout << "Usage: cat -[l | r] <fileName>" << endl;
        return;
    }
    
    if (args[1] == "-l") {
        char* fileName = (char*) args[2].c_str();
        if (!checkIfFileExists(fileName, ".")) {
            cout << "File doesn't exist" << endl;
            return;
        }

        string file = string(fileName);
        file = "./" + file;
        FILE *f = fopen(file.c_str(), "r");

        char* data;
        unsigned long int dataLength = 0;
        fseek(f, 0L, SEEK_END);
        dataLength = ftell(f);
        fseek(f, 0L, SEEK_SET);
        data = new char[dataLength];

        fread(data, dataLength, 1, f);
        string content = string(data);
        cout << content << endl;
        fclose(f);
    } else if (args[1] == "-r") {
        FileManager_stub *fm = new FileManager_stub(args[0]);
        char* file = (char*) args[2].c_str();
        char* data;
        unsigned long int dataLength = 0;
        
        fm->readFile(file, data, dataLength);
        string content = string(data);
        cout << content << endl;

        delete fm;
        delete[] data;
    } else {
        cout << "Argument " << args[1] << " not found. Usage: cat -[l | r] <fileName>" << endl;
    }
}

void handleCd(vector<string>& args) {
    if (args.size() != 2) {
        cout << "Usage: cd <path>" << endl;
    } else {
        args[0] = args[1];
    }
}

int main(int argc, char **argv)
{
    map<string, function<void(vector<string>&)>> commandHandlers = {
        {"help", [] (vector<string>&) { handleHelp(); }},
        {"greet", [] (vector<string>& args) { handleGreet(args); }},
        {"exit", [] (vector<string>&) { handleExit(); }},
        {"ls", [] (vector<string>& args) { handleLs(args); }},
        {"pwd", [] (vector<string>& args) { handlePwd(args); }},
        {"download", [] (vector<string>& args) { handleDownload(args); }},
        {"upload", [] (vector<string>& args) { handleUpload(args); }},
        {"cat", [] (vector<string>& args) { handleCat(args); }}
    };

    string path = "dirprueba";
    string input;
    while (true) {
        cout << "terminal:$ ";

        getline(cin, input);

        if (input.empty()) {
            continue;
        }

        istringstream iss(input);
        string command;
        iss >> command;

        vector<string> args;
        string arg;
        args.push_back(path);
        while (iss >> arg) {
            args.push_back(arg);
        }

        auto it = commandHandlers.find(command);
        if (it != commandHandlers.end()) {
            it->second(args);
        } else {
            cout << "Unknown command. Type 'help' for a list of commands." << endl;
        }
    }

    return 0;
}
