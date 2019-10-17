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
int counter;


pthread_mutex_t lock;
atomic<bool> flag_tas;

pthread_barrier_t bar;

atomic<int> next_num;
atomic<int> now_serving;

void empty(int);
void print(int);

void cnt_tas_lock(int);
void cnt_ttas_lock(int);
void cnt_ticket_lock(int);
void cnt_mcs_lock(int);
void cnt_pthread_lock(int);

void cnt_bar_sense(int);
void cnt_bar_pthread(int);

const int NUM_FUNCS = 9;
void (*funcs[NUM_FUNCS])(int)  = {
	empty,
  cnt_tas_lock,
	cnt_ttas_lock,
  cnt_ticket_lock,
	cnt_mcs_lock,
	cnt_pthread_lock,
	cnt_bar_sense,
	cnt_bar_pthread,
  print
};


class Node
{
public:
  atomic<Node*> next;
  atomic<bool> wait;
};

atomic<Node*> tail{NULL};

class MCSLock
{
public:


  void acquire(Node*);
  void release(Node*);

};

void MCSLock::acquire(Node* myNode)
{
  Node* oldTail = tail.load();
  myNode->next.store(NULL);
  while(!tail.compare_exchange_strong(oldTail,myNode))
  {
    oldTail = tail.load();
  }

  // if oldTail== NULL, we’ve
  // acquired the lock
  // otherwise, wait for it

  if(oldTail != NULL)
  {
    myNode->wait.store(true);
    oldTail->next.store(myNode);
    while( myNode->wait.load()){}
  }
}

void MCSLock::release(Node* myNode)
{
  Node* temp_node = myNode;
  if(tail.compare_exchange_strong(temp_node, NULL))
  {
    // no one is waiting, and we just
    // freed the lock
  }
  else
  {
    // hand lock to next waiting thread
    while(myNode->next.load() == NULL) {}
    myNode->next.load()->wait.store(false);
  }
}

MCSLock mcs_lock;


class Barrier
{
public:
  atomic<int> count;
  atomic<int> sense;
  int N = NUM_THREADS;

  void wait(void);

};

void Barrier::wait()
{
  thread_local bool my_sense=0;
  if(my_sense==0)
  {
    my_sense = 1;
  }
  else
  {
    my_sense = 0;
  }
  count.fetch_add(1);
  int cnt_cpy = count;
  if(cnt_cpy == N)
  {
    //cnt.store(0,RELAXED);
    count.store(0,std::memory_order_relaxed);
    sense.store(my_sense);
  }
  else
  {
    while(sense.load()!=my_sense)
    {}
  }
}


Barrier bar_sense;

void global_init(){
	threads = (pthread_t*) malloc(NUM_THREADS*sizeof(pthread_t));
	args = (size_t*) malloc(NUM_THREADS*sizeof(size_t));
	pthread_barrier_init(&bar, NULL, NUM_THREADS);
	counter = 0;
	// atomic_counter.store(0);
  //
	pthread_mutex_init(&lock,NULL);
	// tas_lock.store(false);
	// array = (int*) malloc(NUM_THREADS*sizeof(int));
	// array_padded = (padded_int*) malloc(NUM_THREADS*sizeof(padded_int));



  bar_sense.count = 0;
  bar_sense.sense = 0;
  bar_sense.N = NUM_THREADS;

  next_num = 0;
  now_serving = 0;

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



// LOCKS

//tas
void tas_lock()
{
  bool expected, changed;
  do{
    expected = false;
    changed = true;
  }while(!flag_tas.compare_exchange_strong(expected,changed));
}

void tas_unlock()
{
  flag_tas.store(false);
}


//ttas
void ttas_lock()
{
  bool expected, changed;
  do{
    expected = false;
    changed = true;
  }while(flag_tas.load()==true || !flag_tas.compare_exchange_strong(expected,changed));
}

void ttas_unlock()
{
  flag_tas.store(false);
}

//ticket

void ticket_lock()
{
  int my_num = next_num.fetch_add(1);
  while(now_serving.load() != my_num)
  {}
}

void ticket_unlock()
{
  now_serving.fetch_add(1);
}


void empty(int tid)
{
    cout<<"In the empty function";
}

void print(int tid)
{
	printf("Thread %d reporting for duty.\n",tid);
}

void cnt_tas_lock(int)
{
  for(int i=0; i<ITERATIONS; i++)
  {
    tas_lock();
    //cout<<"\n Locked";
    //cout<<"\n Incrementing counter from "<<tid;
    counter++;
    //cout<<"\n Unlocked";
    tas_unlock();
  }
}

void cnt_ttas_lock(int)
{
  for(int i=0; i<ITERATIONS; i++)
  {
    ttas_lock();
    //cout<<"\n Locked";
    //cout<<"\n Incrementing counter from "<<tid;
    counter++;
    //cout<<"\n Unlocked";
    ttas_unlock();
  }
}

void cnt_ticket_lock(int)
{
  for(int i=0; i<ITERATIONS; i++)
  {
    ticket_lock();
    //cout<<"\n Locked";
    //cout<<"\n Incrementing counter from "<<tid;
    counter++;
    //cout<<"\n Unlocked";
    ticket_unlock();
  }
}

void cnt_mcs_lock(int tid)
{

  for(int i=0; i<ITERATIONS; i++)
  {
    Node* myNode = new(Node);
    mcs_lock.acquire(myNode);
    //cout<<"\n Locked";
    //cout<<"\n Incrementing counter from "<<tid;
    counter++;
    //cout<<"\n Unlocked";
    mcs_lock.release(myNode);
    free(myNode);
  }
}

void cnt_pthread_lock(int tid)
{
  for(int i=0; i<ITERATIONS; i++)
  {
    pthread_mutex_lock(&lock);
    //cout<<"\n Locked";
    //cout<<"\n Incrementing counter from "<<tid;
    counter++;
    //cout<<"\n Unlocked";
    pthread_mutex_unlock(&lock);
  }
}

void cnt_bar_sense(int tid)
{
  tid--;
  for(int i=0; i<NUM_THREADS*ITERATIONS; i++)
  {
    if(i%NUM_THREADS==tid)
    {
      counter++;
    }
    bar_sense.wait();
  }
}

void cnt_bar_pthread(int tid)
{
  tid--;
  for(int i=0; i<NUM_THREADS*ITERATIONS; i++)
  {
    if(i%NUM_THREADS==tid)
    {
      counter++;
    }
    pthread_barrier_wait(&bar);
  }
}




void* thread_main(void* args)
{
  size_t tid = *((size_t*)args);

  void (*test)(int) = funcs[TEST_NUM];

//  cout<<"Hello World "<<tid<<endl;
  test(tid);
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

    if(strcmp(argv[i],"--lock=ttas") == 0)
    {
      TEST_NUM = 2;
    }

    if(strcmp(argv[i],"--lock=ticket") == 0)
    {
      TEST_NUM = 3;
    }

    if(strcmp(argv[i],"--lock=mcs") == 0)
    {
      TEST_NUM = 4;
    }

    if(strcmp(argv[i],"--lock=pthread") == 0)
    {
      TEST_NUM = 5;
    }

    if(strcmp(argv[i],"--bar=sense") == 0)
    {
      TEST_NUM = 6;
    }

    if(strcmp(argv[i],"--bar=pthread") == 0)
    {
      TEST_NUM = 7;
    }

    if(strcmp(argv[i],"--empty") == 0)
    {
      TEST_NUM = 0;
    }

    if(strcmp(argv[i],"--print") == 0)
    {
      TEST_NUM = 8;
    }
  }

  cout<<"\n Threads : "<<NUM_THREADS;
  cout<<"\n Iterations : "<<ITERATIONS;
  cout<<"\n Lock : "<<TEST_NUM;
  cout<<endl;


  global_init();

  for( i=1; i < NUM_THREADS; i++)
  {
    args[i] = i+1;
    pthread_create(&threads[i], NULL, &thread_main, &args[i]);
  }

  i=1;
  thread_main(&i);

  for (int i=1; i<NUM_THREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }

  cout<<"\n Counter = "<<counter<<endl;

}
