#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void merge(int a[], int left, int mid, int right)
{



  int x = mid - left + 1;
  int y = right - mid;

  int L[x],R[y];


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

  while(i<x)
  {
    a[k] = L[i];
    i++;
    k++;
  }

  while(j<y)
  {
    a[k] = R[j];
    j++;
    k++;
  }



}

void merge_sort(int a[], int left, int right)
{
  if(left < right)
  {
    int mid = left + (right - left)/2;

    merge_sort(a, left, mid);
    merge_sort(a, mid+1, right);

    merge(a, left, mid, right);
  }


}


int main(int argc, char **argv)
{

  int a[10];
  int num = 0;
  int n = 0;
  int i = 0;
  char* out_file;


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


  merge_sort(a,0,n);



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

  printf("Sorted Array\n");

  for(int i=0;i<=n;i++)
    printf("%d\n",a[i]);


}
