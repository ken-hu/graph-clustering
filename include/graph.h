/*
 * =====================================================================================
 *
 *       Filename:  graph.h
 *
 *    Description:  The interface of a graph
 *        Created:  06/09/2016 16:22:24
 *
 *         Author:  Ken Hu, xnchnhu@gmail.com
 *
 * =====================================================================================
 */


/*
 * =====================================================================================
 *        Class:  Graph
 *  Description:  Class to create a new graph object
 * =====================================================================================
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <string>

class Graph {
    private:
        typedef std::unordered_set<int> SetOfNeighbours;
        std::unordered_map<int, SetOfNeighbours> G;
        mutable std::unordered_map<int, int> Colour;

    public:
        Graph() {}
        Graph(int n); // Construct a random graph with n vertices

        void addEdge(int src, int dest);
        const int edgesNum() const;
        const int subgraphsNum() const;
        const int size() const;

        void printDotFormat() const;
        void outputDotFormat(const std::string& filename) const;
        void printLaplacianMat() const;
        void setColour(int vertex, int colour) const;
        const int getColour(int vertex) const;
        void readDotFormat(std::ifstream& In);
        void readDotFormatWithColour(std::ifstream& In);
        const std::unordered_map<int, std::unordered_set<int>>::const_iterator find(int vertex) const;
};

#endif
