#pragma once
#include <fstream>
#include <vector>

using namespace std;

class ManageFile {
public :
    ManageFile(string path);
    ManageFile();
    vector<string> readFile();
    void writeFile(string text);
    void setFile(string path);
private :
    string path;
};
