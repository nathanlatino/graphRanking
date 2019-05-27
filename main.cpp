#include <string>
#include <fstream>
#include <iostream>

#include <set>
#include "project/graph.hpp"
#include "project/generator/ManageFile.hpp"
#include "project/generator/GraphBuilder.hpp"

int main() {
    auto* bigGraph = new  Graph<string, float>(true);
    auto* littleGraph = new Graph<string, float>(true);

    GraphBuilder bigGraphBuilder(bigGraph);
    GraphBuilder littleGraphBuilder(littleGraph);

    bigGraphBuilder.buildFromFile("../project/generator/data");
    littleGraphBuilder.buildFromFile("../project/generator/rawdata");




    //ranking on 2608 vertex
    std::ofstream pageRank("../output/pageRanking.txt", std::ios::out);
    bigGraph->calculatePageRank(pageRank);
    pageRank.close();

    std::ofstream fileGraph("../output/graph.dot", std::ios::out);
    littleGraph->printGraphViz(fileGraph);
    fileGraph.close();

    //graphviz
    system("cd ../output; circo -Tpdf graph.dot > graph.pdf");




    return 0;
}