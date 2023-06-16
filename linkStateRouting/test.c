#include <stdio.h>
#include <limits.h>


void main(){
    int n = 6;
    // int arr[n][n];
    int i,j,k;
    int visited[n];
    for(i=0;i<n;i++)
        visited[i] = 0;
    // for(i=0;i<n;i++){
    //     for(j=0;j<n;j++){

    //     }
    // }
    int arr[6][6] = {-1,2,4,-1,-1,-1,
                    -1,-1,1,7,-1,-1,
                    -1,-1,-1,-1,3,-1,
                    -1,-1,-1,-1,-1,1,
                    -1,-1,-1,2,-1,5,
                    -1,-1,-1,-1,-1,-1};
    // for(i=0;i<n;i++){
    //     for(j=0;j<n;j++){
    //         printf("%d ",arr[i][j]);
    //     }
    //     printf("\n");
    // }

    int root = 0;
    int visited_count = 0;
    visited[root] = 1;
    visited_count++;

    i = root;
    while (visited_count < n) {
        int min = INT_MAX;
        int index_node = i;
        for(i=0;i<n;i++){
            if(arr[root][i] != -1 && visited[i] != 1 && i!=root && arr[root][i] < min){
                min = arr[root][i];
                index_node = i;
            }
        }

        visited[index_node] = 1;
        visited_count++;

        for(int i=0;i<n;i++){
            if(visited[i] == 1)
                continue;
            
            int dist_AC = arr[root][i] == -1 ? INT_MAX : arr[root][i];
            int dist_BC = arr[index_node][i] == -1 ? INT_MAX : arr[index_node][i];

            if(dist_BC != INT_MAX && min+dist_BC < dist_AC){
                arr[root][i] = dist_BC+min;
            }
        }
    }

    printf("After link state routing \n");
    printf("Shortest path from node %d to other nodes\n", root);
    printf("Node\t Shortestdist\n");
    for(int i=0;i<n;i++){
        if(i != root)
            printf("%d\t %d\n",i,arr[root][i]);
    }
    
}