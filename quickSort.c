#include <stdio.h>
#include <string.h>

void quickSort(int * ,int ,int );
int  partition(int * ,int ,int );
void swap(int * ,int ,int );

int main(int ac,char **ag)
{
    if(ac != 2) printf("Notice arguments!\n");
   
    int i = 0;
    int size = 0;
    int num = 0;
    int array[100] = {0};
    char *p = ag[1];

    while(size < strlen(ag[1])){
        while(*p >= '0' && *p <= '9'){
            num = num * 10 + *p - '0';
            p ++;
            size ++;
        }
        array[i++] = num;
        num = 0;
        p ++;
        size ++;
   }

   quickSort(array ,0 , i-1);
   
   int j = 0;
   printf("Sorted array:");
   for(;j <= i-1; j ++) printf("%d  ", array[j]);
   printf("\n");
   return 0;
}

void quickSort(int *array,int low, int high)
{
    int pivotposition;
    if(low < high){
        pivotposition = partition(array ,low , high);
        quickSort(array ,low ,pivotposition -1);
        quickSort(array ,pivotposition +1 ,high);
    }
}

int partition(int *array, int low, int high)
{
    int i ;
    int j = low;
    int pivot = array[low];
    for(i = low + 1; i <= high; i ++)
    {
        if(array[i] < pivot){
            j ++;
            swap(array, j, i);
        }
    }
    swap(array, low, j);
    return j;
}

void swap(int *array, int i, int j)
{
    int temp;
    temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}



  
    
     
