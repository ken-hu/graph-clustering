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
#include <vector>
#include <boost/mpi.hpp>

class Graph {
    private:
        boost::mpi::communicator world;
        typedef std::unordered_set<int> SetOfNeighbours;
        std::unordered_map<int, SetOfNeighbours> G;

        int local_size_;
        int global_size_;
        int rank_;
        std::vector<int> global_index_;
        std::unordered_map<int, int> local_index_;

        mutable std::unordered_map<int, int> Colour;
        void addEdge(int src, int dest);

    public:
        Graph() {}
        Graph(int n); // Construct a random graph with n vertices

        const std::unordered_map<int, std::unordered_set<int>>::const_iterator find(int vertex) const;
        const int edgesNum() const;
        const int subgraphsNum() const;
        const int size() const;

        void setColour(int vertex, int colour) const;
        const int getColour(int vertex) const;

        void readDotFormat(const std::string& filename, const int& global_size);
        void readDotFormatWithColour(const std::string& filename);
        void readDotFormatByColour(const std::string& filename, const int& global_size);
        void printDotFormat() const;
        void outputDotFormat(const std::string& filename) const;
        void outputResult(const std::string& filename) const;
        void printLaplacianMat() const;

        const int globalSize() const;
        const int localSize() const;
        const int rank() const;
        const int globalIndex(int local_index) const;
        const int localIndex(int global_index) const;
        std::vector<int> global_rank_map;
};

#endif
