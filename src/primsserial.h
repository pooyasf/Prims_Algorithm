#include <iostream>
#include <string>
#include <bits/stdc++.h>


using namespace std;

// template function for finding edge with minimum weight
// connecting current MST with rest of Graph
template <typename A ,typename B , typename C>
int minKey(A key[], B mstSet[],C V)
{

    int min_index;
    A min = INT_MAX;

    for (int v = 0; v < V; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}


template <typename T , size_t M, size_t N>
void PrimsSerial(T (&graph)[M][N] , int Parent[])
{
    int V = M  ;


    T key[V];


    bool mstSet[V];


    for (int i = 0; i < V; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    key[0] = 0;
    Parent[0] = -1;


    for (int count = 0; count < V - 1; count++)
    {

        int u = minKey(key, mstSet , V);


        mstSet[u] = true;

        for (int v = 0; v < V; v++)

            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])
                Parent[v] = u, key[v] = graph[u][v];
    }

}




