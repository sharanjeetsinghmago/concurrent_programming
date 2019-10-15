#include <iostream>
#include <atomic>
#include <pthread.h>
#include <string.h>

using namespace std;


pthread_t* threads;
size_t* args;
size_t NUM_THREADS = 7 ;
int TEST_NUM = 0;
int ITERATIONS = 10;

atomic<bool> flag_tas;
atomic<bool> flag_tatas;

void empty(int);
void print(int);
// void cnt_lock(int);
// void cnt_tas_lock(int);
// void cnt_test_and_tas_lock(int);
// void cnt_fai(int);
// void cnt_cas(int);
// void cnt_array(int);
// void cnt_array_padded(int);
// void cnt_thread_local(int);
// void cnt_stack(int);

const int NUM_FUNCS = 2;
void (*funcs[NUM_FUNCS])(int)  = {
	empty,
  print
	// print,
	// cnt_lock,
	// cnt_tas_lock,
	// cnt_test_and_tas_lock,
	// cnt_fai,
	// cnt_cas,
	// cnt_array,
	// cnt_array_padded,
	// cnt_thread_local,
	// cnt_stack
};

void global_init(){
	threads = (pthread_t*) malloc(NUM_THREADS*sizeof(pthread_t));
	args = (size_t*) malloc(NUM_THREADS*sizeof(size_t));
	// pthread_barrier_init(&bar, NULL, NUM_THREADS);
	// counter = 0;
	// atomic_counter.store(0);
  //
	// pthread_mutex_init(&lock,NULL);
	// tas_lock.store(false);
	// array = (int*) malloc(NUM_THREADS*sizeof(int));
	// array_padded = (padded_int*) malloc(NUM_THREADS*sizeof(padded_int));
}


void global_cleanup(){
	// if(atomic_counter.load()!=0){counter = atomic_counter.load();}
  //
	// printf("Counter:%d\n",counter);
  //
	// free(array);
	// free(array_padded);
	// free(threads);
	// free(args);
	// pthread_barrier_destroy(&bar);
	// pthread_mutex_destroy(&lock);
}



void empty(int tid){}

void print(int tid){
	printf("Thread %d reporting for duty.\n",tid);
}

//tas
bool tas()
{
  if(flag_tas == false)
  {
    flag_tas = true;
    return true;
  }
  else
  {
    return false;
  }
}

//
// //tatas
// void lock()
// {
//   while(flag.load()==true || flag.tas() == false)
//   {}
// }
//
// void unlock()

void* thread_main(void* args)
{
  size_t tid = *((size_t*)args);

  void (*test)(int) = funcs[TEST_NUM];

  cout<<"Hello World "<<tid<<endl;



}

int main(int argc, char **argv)
{
  size_t i;


// checking for inputs in arguments
  for (i = 0; i < argc; i++)
  {

    if(strcmp(argv[i],"--name") == 0)
    {
      //printf("Sharanjeet Singh Mago\n");
      cout<<"Sharanjeet Singh Mago\n";
      return 0;
    }

    if(i+1 != argc)
    {
      if(strcmp(argv[i],"-o") == 0)
      {
        //out_file = argv[i+1];
        i++;
      }
    }

    if(i+1 != argc)
    {
      if(strcmp(argv[i],"-t") == 0)
      {
        NUM_THREADS = atoi(argv[i+1]);
        i++;
      }
    }

    if(i+1 != argc)
    {
      if(strcmp(argv[i],"-i") == 0)
      {
        ITERATIONS = atoi(argv[i+1]);
        i++;
      }
    }

    if(strcmp(argv[i],"--lock=tas") == 0)
    {
      TEST_NUM = 1;
    }

    if(strcmp(argv[i],"--alg=ttas") == 0)
    {
      TEST_NUM = 2;
    }

    if(strcmp(argv[i],"--alg=mcs") == 0)
    {
      TEST_NUM = 3;
    }

    if(strcmp(argv[i],"--alg=pthread") == 0)
    {
      TEST_NUM = 4;
    }

    if(strcmp(argv[i],"--bar=sense") == 0)
    {
      TEST_NUM = 5;
    }

    if(strcmp(argv[i],"--bar=pthread") == 0)
    {
      TEST_NUM = 6;
    }
  }




  global_init();

  for( i=1; i < NUM_THREADS; i++)
  {
    args[i] = i+1;
    pthread_create(&threads[i], NULL, &thread_main, &args[i]);
  }

  i=1;
  thread_main(&i);

}
