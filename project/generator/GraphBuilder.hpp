
#pragma once
#include <string>
#include <sstream>
#include <bits/stdc++.h>
#include "ManageFile.hpp"
#include "../graph.hpp"


class GraphBuilder {
public:
    GraphBuilder(Graph<std::string,float>* graph);
    void buildFromFile(std::string path);
    void buildInvertFromFile(std::string path);
private:
    Graph<std::string,float> *graph;
    unordered_map<std::string,int>* vertex;

    vector<std::string> split(std::string text, char split);
    void createEdge(vector<std::string> tab, int index);
    void createInvertEdge(vector<std::string> tab, int index);
    void createVertex(vector<std::string> tab);
};
