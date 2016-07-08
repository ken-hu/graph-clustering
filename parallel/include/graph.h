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
        int local_size_;
        int global_size_;
        int rank_;
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
        void readDotFormat(const std::string& filename);
        void readDotFormatWithColour(const std::string& filename);
        const std::unordered_map<int, std::unordered_set<int>>::const_iterator find(int vertex) const;

        void init(int rank, int global_size, int local_size);
        const int globalSize() const;
        const int localSize() const;
        const int rank() const;
        const int globalIndex(int local_index) const;
        const int localIndex(int global_index) const;
};

#endif