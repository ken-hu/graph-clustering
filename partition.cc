/*
 * =====================================================================================
 *
 *       Filename:  partition.cc
 *
 *    Description:  Partition the graph according to the eigenvectors
 *        Created:  06/17/2016 14:15:15
 *
 *         Author:  Ken Hu, xnchnhu@gmail.com
 *
 * =====================================================================================
 */

#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>

#include "partition.h"
#include "graph.h"
#include "lanczos.h"
#include "tqli.h"

#define Sign(a) (a >= 0.0 ? 1:0)

using namespace std;

void printSparseMatrix(std::map<std::pair<int,int>, double>& sparse_matrix, int size);
void printDenseMatrix(std::unordered_map<int, std::vector<double>>& dense_matrix);
void printVector(const std::vector<double>& vec);
std::vector<double> getEigenVec(const Graph& g);
std::unordered_map<int, std::vector<double>> getEigenMatrix(const Graph& g);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  utils
 *  Description:  Print vector, matrix for debug
 * =====================================================================================
 */

void printSparseMatrix(map<pair<int,int>, double>& sparse_matrix, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++)
			cout << sparse_matrix[make_pair(i,j)] << "\t";
		cout << endl;
	}
}

void printDenseMatrix(unordered_map<int, vector<double>>& dense_matrix) {
	int size = dense_matrix.size();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << dense_matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void printVector(const vector<double>& vec) {
	for (const double& x:vec)
		cout << x <<  " ";
	cout << endl;
}

void printEigenvalues(const Graph& g) {
	int size = g.size();

	// Define the input arguments for Lanczos to construct tridiagonal matrix
	vector<double> alpha, beta;

	unordered_map<int, vector<double>> lanczos_vecs;

	// Construct tridiagonal matrix using Lanczos algorithm
	map<pair<int,int>, double> trimat = constructTriMat(g, alpha, beta, lanczos_vecs);
	beta.push_back(0);

	// Define an identity matrix as the input for TQLI algorithm
	unordered_map<int, vector<double>> tri_eigen_vecs;
	vector<double> vinitial(size,0);
	for(int i = 0; i < size; i++)	tri_eigen_vecs[i] = vinitial;
	for(int i = 0; i < size; i++)	tri_eigen_vecs[i][i] = 1;

	// Calculate the eigenvalues and eigenvectors of the tridiagonal matrix
	tqli(alpha, beta, size, tri_eigen_vecs);

	ofstream Output;
	Output.open("eigenvalues.dat");

	for (int i = 0; i < size; i++)	{
		Output << i << " " << alpha[i] << endl;
	}

	Output.close();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getEigenVec
 *  Description:  Return the fiedler vector using lanczos and tqli functions
 * =====================================================================================
 */

vector<double> getEigenVec(const Graph& g) {

	int size = g.size();

	// Define the input arguments for Lanczos to construct tridiagonal matrix
	vector<double> alpha, beta;

	unordered_map<int, vector<double>> lanczos_vecs;

	// Construct tridiagonal matrix using Lanczos algorithm
	map<pair<int,int>, double> trimat = constructTriMat(g, alpha, beta, lanczos_vecs);
	beta.push_back(0);

#ifdef Debug
	cout << endl;
	cout << "triangular matrix: " << endl;
	printSparseMatrix(trimat, size);

#endif

	// Define an identity matrix as the input for TQLI algorithm
	unordered_map<int, vector<double>> tri_eigen_vecs;
	vector<double> vinitial(size,0);
	for(int i = 0; i < size; i++)	tri_eigen_vecs[i] = vinitial;

	for(int i = 0; i < size; i++)	tri_eigen_vecs[i][i] = 1;

	// Calculate the eigenvalues and eigenvectors of the tridiagonal matrix
	tqli(alpha, beta, size, tri_eigen_vecs);

	// Find the index of the second smallest eigenvalue (fiedler vector) of the eigenvalues vector "alpha" 
	int index_of_second_vec = 0;
	unordered_multimap<double, int> hashmap;
	for (unsigned int i = 0; i < alpha.size(); i++)
		hashmap.insert({alpha[i], i});

	vector<double> auxiliary_vec = alpha;
	sort(auxiliary_vec.begin(), auxiliary_vec.end());
	auto it = hashmap.find(auxiliary_vec[1]);
	index_of_second_vec = it->second;
	
#ifdef Debug
	cout << "index_of_second_vec = " << index_of_second_vec << endl;
#endif

	// Calculate the fiedler vector of original Laplacian matrix using 
	// the fiedler vector of the tridiagonal matrix computed by TQLI 
	vector<double> tri_second_vec(size, 0);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (j == index_of_second_vec)
				tri_second_vec[i] = tri_eigen_vecs[i][j];
		}
	}
	//cout << "Second tri eigenvec: " << endl;
	//printVector(tri_second_vec);
	
	vector<double> fiedler_vector(size, 0);
	for	(int i = 0; i < size; i++) {
		for	(int j = 0; j < size; j++) {
			fiedler_vector[i] += lanczos_vecs[j][i] * tri_second_vec[j];
		}
	}

#ifdef Debug
	cout << "fiedler vector: " << endl;
	printVector(fiedler_vector);
#endif

	return fiedler_vector;
} 


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getEigenMatrix
 *  Description:  Get all the eigenvectors of the original matrix.
 * =====================================================================================
 */

unordered_map<int, vector<double>> getEigenMatrix(const Graph& g, vector<double>& eigenvalues) {

	int size = g.size();

	// Define the input arguments for Lanczos to construct tridiagonal matrix
	vector<double> alpha, beta;
	unordered_map<int, vector<double>> lanczos_vecs;

	// Construct tridiagonal matrix using Lanczos algorithm
	map<pair<int,int>, double> trimat = constructTriMat(g, alpha, beta, lanczos_vecs);
	beta.push_back(0);

#ifdef Debug
	cout << endl;
	cout << "vector alpha: " << endl;
	printVector(alpha);

	cout << "vector beta: " << endl;
	printVector(beta);

	cout << "triangular matrix: " << endl;
	printSparseMatrix(trimat, size);
#endif

	// Define an identity matrix as the input for TQLI algorithm
	unordered_map<int, vector<double>> tri_eigen_vecs;
	unordered_map<int, vector<double>> laplacian_eigen_vecs;
	vector<double> vinitial(size,0);
	for(int i = 0; i < size; i++) {
		tri_eigen_vecs[i] = vinitial;	
		laplacian_eigen_vecs[i] = vinitial;
	}
	for(int i = 0; i < size; i++) {
		tri_eigen_vecs[i][i] = 1;
	}

	// Calculate the eigenvalues and eigenvectors of the tridiagonal matrix
	tqli(alpha, beta, size, tri_eigen_vecs);
	eigenvalues = alpha;

	// Calculate all the eigenvectors of original Laplacian matrix using 
	// the eigenvectors of the tridiagonal matrix computed by TQLI 
	for (int k = 0; k < size; k++) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
			laplacian_eigen_vecs[k][i] += lanczos_vecs[j][i] * tri_eigen_vecs[j][k];
			}
		}	
	}

#ifdef Debug
	// Print all the eigenvalues of the tridiagonal/laplacian matrix
	cout << "laplacian eigenvalues: " << endl;
	printVector(alpha);

	// Print all the eigenvectors in row
	cout << "laplacian_eigen_vecs: " << endl;
	printDenseMatrix(laplacian_eigen_vecs);

#endif
	return laplacian_eigen_vecs;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  partition
 *  Description:  Partition the graph into two subgraphs according to the fiedler vector
 * =====================================================================================
 */
void partition(const Graph& g) {

    int numofvertex = g.size();
    vector<double> fiedler_vector = getEigenVec(g);

	for (int vertex = 0; vertex < numofvertex; vertex++) {
		g.setColour(vertex, Sign(fiedler_vector[vertex]));	
	}
	cout << "Partitioning is done." << endl;
}

/*-----------------------------------------------------------------------------
 *  Modified partition function to for multiple partitioning
 *-----------------------------------------------------------------------------*/

void multiPartition(const Graph& g) {
	int size = g.size();

	vector<double> eigenvalues;
	unordered_map<int, vector<double>> laplacian_eigen_vecs = getEigenMatrix(g, eigenvalues);

#ifdef Debug
	// Print all the eigenvalues of the tridiagonal/laplacian matrix
	cout << "laplacian eigenvalues: " << endl;
	printVector(eigenvalues);

	// Print all the eigenvectors in row
	cout << "laplacian_eigen_vecs: " << endl;
	printDenseMatrix(laplacian_eigen_vecs);
#endif

	// eigenvalues_index_sort stores the original index of the sorted eigenvalues 
	vector<int> eigenvalues_index_sort;
	unordered_multimap<double, int> hashmap;
	for (int i = 0; i < size; i++)
		hashmap.insert({eigenvalues[i], i});

	vector<double> auxiliary_vec = eigenvalues;
	sort(auxiliary_vec.begin(), auxiliary_vec.end());
	for (int i = 0; i < size; i++) {
		auto it = hashmap.find(auxiliary_vec[i]);
		eigenvalues_index_sort.push_back(it->second);
		hashmap.erase(it);
	}

#ifdef Debug
	cout << endl;
	cout << "Original index of eigenvalues in sorted order: "; 
	for (const int& x:eigenvalues_index_sort)
		cout <<	x << " "; 
	cout << endl;

	cout << "eigenvalues in sorted order: " << endl; 
	for (const int& x:eigenvalues_index_sort)
		cout <<	eigenvalues[x] << " "; 
	cout << endl;

	cout << "Laplacian eigenvectors in sorted order: " << endl;
	for (const int& row:eigenvalues_index_sort) {
		for (int col = 0; col < size; col++) {
			cout << laplacian_eigen_vecs[row][col] << " ";
		}
		cout << endl;
	}

	cout << "Laplacian eigenvectors in sorted order: " << endl;
	for (const int& row:eigenvalues_index_sort) {
		for (int col = 0; col < size; col++) {
			cout << Sign(laplacian_eigen_vecs[row][col])<< " ";
		}
		cout << endl;
	}
#endif

	int num = 2; // number of eigenvectors to partition the graph, start from the second smallest.
	for (int vertex = 0; vertex < size; vertex++) {
		int colour = 0;
		for (int eigenvec_index = 1; eigenvec_index <= num; eigenvec_index++) {
			int row = eigenvalues_index_sort[eigenvec_index];
			colour += pow(2, eigenvec_index - 1) * Sign(laplacian_eigen_vecs[row][vertex]);
		}
		g.setColour(vertex, colour);
	}
}
