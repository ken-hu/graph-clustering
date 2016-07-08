/*
 * =====================================================================================
 *
 *       Filename:  lanczos.h
 *
 *    Description:  The interface of lanczos algorithm
 *        Created:  06/11/2016 21:22:58
 *
 *         Author:  Ken Hu, xnchnhu@gmail.com
 *
 * =====================================================================================
 */

#ifndef LANCZOS_H_
#define LANCZOS_H_

#include <vector>
#include <map>
#include "graph.h"

template<typename Vector, typename T>
class Lanczos {
    private:
        Vector& initialise(Vector& vec);
        Vector multGraphVec(const Graph& g, const Vector& vec);
        inline T dot(const Vector& v1, const Vector& v2);
        inline T norm(const Vector& vec);
        inline Vector& normalise(Vector& vec);
        inline void gramSchmidt(int& iter, int& size);
        inline T l2norm(const Vector& alpha, const Vector& beta);

    public:
        Lanczos(const Graph& g, bool reorthogonalisation);

        Vector alpha;
        Vector beta;
        std::unordered_map<int, Vector> lanczos_vecs;
        void print_tri_mat();
};

#include "../src/lanczos.cc"
#endif
