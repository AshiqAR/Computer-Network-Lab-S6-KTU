#include <stdio.h>
#include <unistd.h>
#include <limits.h>

void main(){
    printf("Enter the numebr of nodes : ");
    int n; scanf("%d",&n);
    int arr[n][n];
    int i,j;
    printf("Enter the cost matrix");
    for(i=0;i<n;i++){
        for(j=0;j<n;j++ ){
            scanf("%d",&arr[i][j]);
        }
    }

    printf("Enter the source node: ");
    int root;scanf("%d",&root);
    
    int visited[n];
    for(i=0;i<n;i++)
        visited[i] = 0;
    visited[root] = 1;
    int visited_count = 1;

    while(visited_count < n){
        int index_node = root;
        int min = INT_MAX;
        for(i=0;i<n;i++){
            if ( i!=root && arr[root][i] != -1 && visited[i] != 1 && min > arr[root][i] ){
                min = arr[root][i];
                index_node = i;
            }
        }

        visited[index_node] = 1;
        visited_count++;

        for(i=0;i<n;i++){
            if(visited[i] == 1)
                continue;

            int dist_AC = arr[root][i] == -1 ? INT_MAX : arr[root][i];
            int dist_BC = arr[index_node][i] == -1 ? INT_MAX : arr[index_node][i];

            if(dist_BC != INT_MAX && dist_AC > dist_BC+min){
                arr[root][index_node] = dist_BC+min;
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