#include <stdio.h>
#include <limits.h>
#include <unistd.h>

void main(){
    int n;
    printf("enter the number of nodes : ");
    scanf("%d",&n);
    int arr[n][n];
    printf("Enter the cost matrix\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            scanf("%d",&arr[i][j]);
    }

    int change;
    int dist_AB,dist_BC,dist_AC;
    do{
        change = 0;
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(arr[i][j] != -1 && i != j){
                    for(int k=0;k<n;k++){
                        dist_AB = arr[i][j];
                        dist_AC = arr[i][k] == -1 ? INT_MAX : arr[i][k];
                        dist_BC = arr[j][k] == -1 ? INT_MAX : arr[j][k];

                        if(dist_BC == INT_MAX)
                            continue;
                        if(dist_AB+dist_BC < dist_AC){
                            arr[i][k] = dist_AB+dist_BC;
                            change++;
                        }
                    }
                }
            }
        }
    }while(change!=0);

    printf("After Distance Vector Routing:\n");
    for(int i=0;i<n;i++)
    {
        printf("\n");
        for(int j=0;j<n;j++)
        {
            printf("%d ",arr[i][j]);
        }
    }

}