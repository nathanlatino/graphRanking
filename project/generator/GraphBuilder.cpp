//
// Created by nath on 02.05.19.
//

#include "GraphBuilder.hpp"
#include <iostream>

GraphBuilder::GraphBuilder(Graph<std::string, float> *graph) {
    this->graph = graph;
    vertex = new unordered_map<string,int>();
}

void GraphBuilder::buildFromFile(string path){

    ManageFile file(path);

    vector<string> tab = file.readFile();

    createVertex(tab);

    int idx = 0;
    Graph<std::string, float>::Iterator* it = graph->all_vertex_iterator();
    while (it->valid()) {
        vector<string> line = split(tab.at(idx++), ' ');
        createEdge(line, it->get_vertex());
        it->next();
    }
}

void GraphBuilder::buildInvertFromFile(string path){
    ManageFile file(path);
    vector<string> tab = file.readFile();

    createVertex(tab);

    int idx = 0;
    Graph<std::string, float>::Iterator* it = graph->all_vertex_iterator();
    while (it->valid()) {
        vector<string> line = split(tab.at(idx++), ' ');
        createInvertEdge(line, it->get_vertex());
        it->next();
    }
}

void GraphBuilder::createInvertEdge(vector<std::string> line, int index) {
    for(int i = 1; i < line.size(); i++){
        graph->add_edge(vertex->at(line.at(i)), index,1);
    }
}
void GraphBuilder::createEdge(vector<string> line, int index){
    for(int i = 1; i < line.size(); i++){
        if(vertex->find(line.at(i)) != vertex->end()){
            graph->add_edge(index,vertex->at(line.at(i)),1);
        }
    }
}

void GraphBuilder::createVertex(vector<string> tab) {
    for(int i = 0; i < tab.size(); i++) {
        vector<string> line = split(tab.at(i), ' ');
        vertex->insert(pair<string,int>(line.at(0),i));
        graph->add_vertex(line.at(0));
    }
}

vector<string> GraphBuilder::split(string text, char split){
    vector<string> elems;
    stringstream ss(text);
    string item;
    while(getline(ss, item, split)) {
        elems.push_back(item);
    }
    return elems;
}

