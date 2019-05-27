#include <iostream>
#include "ManageFile.hpp"

//Permet de lire/écrire des fichiers
ManageFile::ManageFile() {}

ManageFile::ManageFile(string path) {
    this->path = path;
}

//retourne les lignes vector<string>
vector<string> ManageFile::readFile() {
    string line;
    vector<string> out;
    ifstream file;


    file.open(path);
    if(!file) {
        cout << "Unable to open file : " << path << endl;
    }
    while(std::getline(file, line)) {
        out.push_back(line);
    }
    file.close();
    return out;
}

void ManageFile::writeFile(string text) {
    ofstream file;
    file.open(path);
    file << text;
    file.close();
}

void ManageFile::setFile(string path){
    this->path = path;
}
