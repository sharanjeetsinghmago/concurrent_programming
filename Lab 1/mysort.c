#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#define MAX 1000000

int part=0;

int a[MAX];

int num_threads = 5;

// merging two parts
void merge( int left, int mid, int right)
{

  int x = mid - left + 1;   // size of left part
  int y = right - mid;      // size of right part

  int L[x],R[y];


  // inserting values in left and right parts

  for(int i=0;i<x+y;i++)
  {
    if(i<x)
    {
      L[i] = a[left+i];

    }
    else
    {
      R[i-x] = a[left+i];

    }
  }


  int i=0,j=0,k=left;


  // merge left and right in ascending order
  while(i<x && j<y)
  {

    if(L[i]<R[j])
    {
      a[k] = L[i];
      i++;
    }
    else
    {
      a[k] = R[j];
      j++;
    }
    k++;
  }

  // insert remaining values from left
  while(i<x)
  {
    a[k] = L[i];
    i++;
    k++;
  }


  // inserting remaining values from right
  while(j<y)
  {
    a[k] = R[j];
    j++;
    k++;
  }
}



void merge_sort(int left, int right)
{
  if(left < right)
  {
    int mid = left + (right - left)/2;

    merge_sort(left, mid);
    merge_sort(mid+1, right);

    merge(left, mid, right);
  }
}


// void* merge_sort1(int arr[])
// {
//   int thread_part = part++;
//
//   int left = thread_part * (10/4);
//   int right = (thread_part + 1) * (10/4) - 1;
//
//   int mid = left + (right - left) / 2;
//   if(left < right)
//   {
//
//     printf("\n<< TH %d : Step 1>>\n", thread_part);
//     merge_sort(arr,left, mid);
//
//     printf("\n<< TH %d : Step 2>>\n", thread_part);
//     merge_sort(arr,mid + 1, right);
//
//     printf("\n<< TH %d : Step 3>>\n", thread_part);
//     merge(arr,left, mid, right);
//   }
//
// }


int main(int argc, char **argv)
{

  int num = 0;
  int n = 0;
  int i = 0;
  char* out_file;


  // checking for inputs in arguments
  for (i = 0; i < argc; i++)
  {
  //  printf("argv[%d] = %s\n", i, argv[i]);

    if(i+1 != argc)
    {
      if(strcmp(argv[i],"-o") == 0)
      {
        out_file = argv[i+1];
        i++;
      }
    }

    if(strcmp(argv[i],"--name") == 0)
    {
      printf("Sharanjeet Singh Mago\n");
    }
  }

  printf("\n");

  FILE *f;

  char data[20];

  char* file_name = argv[1];

  f = fopen(file_name, "r");

  i=0;

  if(f == NULL)
  {
    printf(" Failed to open the input file\n");
    return -1;
  }
  else
  {
    //printf(" File Opened");


    while( fgets( data, 10, f) != NULL )
    {
      if(isdigit(data[0]))
      {

        num = atoi(data);

        a[i] = num;
        i++;
        num = 0;
      }

    }

  }

  fclose(f);

  n=i-1;

  printf("Unsorted Array\n");

  for(int i=0;i<=n;i++)
    printf("%d\n",a[i]);


  // Sorting code starts here
/*
  pthread_t threads[num_threads];

  for(int i=0; i < num_threads; i++)
  {
    printf("\n\n<<Creating Thread %d>>",i);
    pthread_create(&threads[i], NULL, merge_sort1, a);
  }
  */
  merge_sort(0,n);

/*
  for (int i=0; i<num_threads; i++)
  {
    printf("\n\n<<Joining thread %d>>",i);
    pthread_join(threads[i], NULL);
  }
*/
  // Sorting code ends here

  f = fopen(out_file, "w");

  char buffer[200000] = "";

  int x;


  if(f == NULL)
  {
    printf(" Failed to open the output file\n");
    return -1;
  }
  else
  {

    for(int i=0;i<=n;i++)
    {
      x = snprintf(buffer,10, "%d\n", a[i]);
      fwrite(buffer,x,1,f);
    }


  }

  printf("\n\nSorted Array\n");

  for(int i=0;i<=n;i++)
    printf("%d\n",a[i]);


}
