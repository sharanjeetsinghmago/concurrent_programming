#include <stdio.h>

void main()
{
  int x[10] = {3,5,1,76,34,23,87,27,11,77};
  int temp;

  for(int i=0;i<10;i++)
  {
    for(int j=0;j<i;j++)
    {
      if(x[j]>x[i])
      {
        temp = x[i];
        x[i] = x[j];
        x[j] = temp;
      }
    }
  }


  for(int a=0;a<10;a++)
  {
      printf(" %d ",x[a]);
  }

  printf("\n");

}
