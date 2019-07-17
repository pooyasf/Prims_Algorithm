#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include "primsserial.h"
#include "primsomp.h"

using namespace std;

// Here we should set number of vertices.
// It should not exceed some level.
#define V 9000

// This function return true if two vertices are in same community!
// otherwise it returns false
bool same_community(int p_sum_c[] , int i , int j){
    int num = sizeof(p_sum_c)/sizeof(p_sum_c[0]);
    int i_index = -1 , j_index = -1;
    for (int x = 0 ; x < num ; x++){
        if( (p_sum_c[x] <= i) && (i_index == -1) ){
            i_index = x+1;
        }
        if( (p_sum_c[x] <= j) && (j_index == -1) ){
            j_index = x+1;
        }
    }
    return i_index == j_index;
}

// Symmetric SBM Class
class SSBM {
  public:
        // This is static so we avoid segmentation fault error
        // TO Do: USE boost library for graph storage.
        static int graph[V][V];
        void AddCommunity(int c_size)
        {
			communities.push_back(c_size);
		}

        void set_probability(double P_i , double P_o)
        {
            P_inside = P_i;
            P_outside = P_o;
        }
        void create();


   private:
        double P_inside , P_outside ;
        int vertices = V;
        vector<int> communities;
};
int SSBM::graph[V][V] = {};

// This function make a SSBM based on setuped parameters
void SSBM::create(){

    default_random_engine generator;

    // Set random seed for reproducible results
    //generator.seed(34);

    // set probabilities for bernoulli random number
    bernoulli_distribution ran_inside(P_inside);
    bernoulli_distribution ran_outside(P_outside);

    // calculate cum sum of communities size
    // This help us figure out vertices positions
    int p_sum_c[communities.size()];
    partial_sum(communities.begin() , communities.end() , p_sum_c );


    // here we fill out graph with 0 and 1 based on prob. parameters
    // this is not the most effiecint way to create this graph
    // but it works fine for now
    for( int i = 0 ; i < vertices ; i++){

        for( int j = i ; j < vertices ; j++){

            // check if the both vertices are inside common community or not.
            if ( same_community(p_sum_c,i,j) ){
                graph[i][j] = ran_inside(generator);
                graph[j][i] = graph[i][j];
            } else {
                graph[i][j] = ran_outside(generator);
                graph[j][i] = graph[i][j];
            }

        }

    }


}

int main()
{
    // Array for keeping MST edges
    int mstSet[V];
    // Create SBM object
    SSBM graph1;

    // Add necessary communities(size) to graph
    // Sum of all sizes should equal to number of Vertices ( V )
    // ** Set number of Vertices at top of this file
    graph1.AddCommunity(3000);
    graph1.AddCommunity(3000);
    graph1.AddCommunity(3000);

    // Set probability parameters. ( inside community prob. , outside community prob. )
    graph1.set_probability( 0.8 , 0.5 );

    // create SBM with above parameters !
    graph1.create();

    // Here we can know when SBM created.
    cout << "SBM graph created!\n";

    // Keep track of serial prims time
    double start = omp_get_wtime();
    // call serial prims to find MST.
    PrimsSerial(graph1.graph , mstSet);
    double end = omp_get_wtime();
    printf("Serial Time = %f\n\n", end - start);


    start = omp_get_wtime();
    // call parallel prims to find MST
    PrimsOMP(graph1.graph , mstSet);
    end = omp_get_wtime();
    printf("Parallel Time = %f\n\n", end - start);




    return 0;
}
