#include <iostream>
#include "primsserial.h"
#include "primsomp.h"
#include <fstream>
#include <cmath>


#define V 150

// Calculate euclidean distance between two data point
double twoPointDistance(int i,int j,double datapoint[V][4]){
    return sqrt( pow(  datapoint[i][0] - datapoint[j][0] ,2) +
                                pow( datapoint[i][1] - datapoint[j][1] ,2) +
                                    pow( datapoint[i][2] - datapoint[j][2] , 2) +
                                        pow( datapoint[i][3] - datapoint[j][3] , 2) );
}

// Make Iris graph from csv file
void construct_graph_csv(string filePath , double graph[V][V]){

    ifstream infile;
    infile.open(filePath);
    string p1,p2,p3,p4;
    double datapoint[V][4];
    int i = 0;
    // read csv file line by line
    while( getline(infile, p1 , ',') ){

        datapoint[i][0] = stod(p1);

        getline(infile, p2, ',');
        datapoint[i][1] = stod(p2);

        getline(infile, p3, ',');
        datapoint[i][2] = stod(p3);

        getline(infile, p4);
        datapoint[i][3] = stod(p4);

        i++;
        if(i == V){
            break;
        }

    }
    // fill adj. matrix with euclidean distance
    for(int i = 0 ; i < V ; ++i){
        for(int j = 0 ; j < V ; ++j){
            graph[i][j] = twoPointDistance(i,j,datapoint);
        }
    }


    infile.close();
}
// count edges with both sides in same cluster
void IRIS_Cluster_Summary(int mstEdge[]){
    int cluster1 = 0 , cluster2 = 0 , cluster3 = 0;
    for(int i = 0 ; i < V ; ++i){
        if( (mstEdge[i] < 50) && i < 50 ){
            cluster1++;
        }
        if( (mstEdge[i] < 100) && (mstEdge[i] > 49) && i > 49 && i < 100  ){
            cluster2++;
        }
        if( (mstEdge[i] > 99) && (mstEdge[i] < 150) && i > 99 && i < 150  ){
            cluster3++;
        }
    }
    cout << "Cluster1:" << cluster1 << "\n";
    cout << "Cluster2:" << cluster2 << "\n";
    cout << "Cluster3:" << cluster3 << "\n";
}


int main()
{
    double graph[V][V] ;
    int mstEdge[V];

    construct_graph_csv("iris.data" , graph);

    // keep track of function execution time
    double start = omp_get_wtime();
    PrimsSerial(graph,mstEdge);
    double end = omp_get_wtime();
    printf("Serial Time = %f\n\n", end - start);

    //IRIS_Cluster_Summary(mstEdge);

    start = omp_get_wtime();
    PrimsOMP(graph,mstEdge);
    end = omp_get_wtime();
    printf("Parallel Time = %f\n\n", end - start);

    //IRIS_Cluster_Summary(mstEdge);


    return 0;
}
