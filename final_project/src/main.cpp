#include "trees_lock.h"

using namespace std;

ofstream out_file;

int num_threads;

int num_iterations;

pthread_mutex_t tree_lock;

Node *groot;



int main()
{

  srand(time(NULL));

  cout << "\n Enter the number of threads...";

  cin >> num_threads;

  cout << "\n Enter the number of iterations...";

  cin >> num_iterations;

  pthread_mutex_init(&tree_lock,NULL);

  pthread_t threads[num_threads];

  for(int i=0;i<num_threads;i++)
  {
    pthread_create(&threads[i], NULL, thread_func, NULL);
  }

  for (int i=0; i<num_threads; i++)
  {
    pthread_join(threads[i], NULL);
  }

  out_file.open("../output/out.txt");

  if(!out_file)
  {
    cout<<"file nhi bani";
  }

  out_file<<"\n In order traversal of tree => ";

  cout<<"\n In order traversal of tree => ";

  //inOrder(groot);

  range(groot, 0, MAX_KEY);

  cout<<endl;



  return 0;
}
