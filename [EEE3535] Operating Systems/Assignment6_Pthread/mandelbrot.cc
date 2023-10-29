#include <pthread.h>
#include "mandelbrot.h"
#include <cstdlib>
#include <cassert>

// Mandelbrot convergence map
unsigned mandelbrot[resolution*resolution];

// Lock and cond variables for thread_exit() and thread_join()
pthread_mutex_t lock;
pthread_cond_t  cond;

//number of running threads
unsigned running = 0;

//argument structure for a thread
struct thread_arg {
  unsigned thread_complex;	//# of complex number for the thread
  unsigned thread_id;		//thread id
};

// This must be called at the end of thread function
void thread_exit(void) {
  pthread_mutex_lock(&lock);
  //when a thread finishes, # of running thread -1
  running --;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&lock);
}

// This is called by the main thread.
void thread_join(void) {
  pthread_mutex_lock(&lock);
  //wait until all the children thread finish
  while(running != 0)
    pthread_cond_wait(&cond, &lock);
  pthread_mutex_unlock(&lock);
}

// Thread function to calculate a part of Mandelbrot set.
void* thread_mandelbrot(void* arg) {
  //bring argument structure or the thread
  struct thread_arg *arguments = (struct thread_arg *)arg;
  //# of complex number to calculate
  unsigned numcom = arguments->thread_complex;
  //bring thread number
  unsigned t = arguments->thread_id;

  unsigned h, w;	//location - height, width
  float a, b;		//complex number a+bi

  for(unsigned j = 0; j < numcom; j++){
    //find the pixel location i
    unsigned i = j*num_threads+t;

    //set values
    w = i%768;			//width
    h = i/768;			//height
    a = -2.2+3.2*w/768;		//real value part
    b = -1.5+3.1*h/768;		//imaginary value part

    Complex c=Complex(a,b);	//c
    Complex Zn=c;		//Z0=c

    //iteration
    unsigned iteration = 0;
    while(iteration < max_iterations){	  //assumption 2
      Zn=(Zn.operator*(Zn)).operator+(c); //Z_n value calculation(def)
      if(Zn.magnitude2()>2.0) break;	  //assumption 1
      iteration++;
    }

    //pixel color value = # of iterations are made
    //until c is proven to escape from Mandelbrot set
    mandelbrot[i]=iteration;
  }
  thread_exit();  // Wake up a thread waiting on the condition variable.
  return 0;
}

// Calculate the Mandelbrot convergence map.
void calc_mandelbrot(void) {

  //set number of running thread
  running = num_threads;

  //to handle arbitrary number of threads evenly
  unsigned quotient = (resolution*resolution)/num_threads;	
  unsigned remainder = (resolution*resolution) - quotient*num_threads;
  
  //pthreads structure array and arguments structure array
  pthread_t *threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
  struct thread_arg *arg = (thread_arg*)malloc(num_threads * sizeof(thread_arg));

  //put value(thread_id, and # of complex numbers in the thread)
  //in argument array and create&assert pthread
  for(unsigned t = 0; t < num_threads; t++){
    
    //balancing # of calculation(pixels) between threads
    arg[t].thread_complex = quotient;	
    if(t < remainder) arg[t].thread_complex++;

    //put thread_id
    arg[t].thread_id=t;

    //pthread create assert
    assert(!pthread_create(&threads[t],NULL,&thread_mandelbrot,(void*)&arg[t]));
  }
  thread_join();  		// Check the condition variable.
  free(threads);  free(arg);	//free malloc allocation
  return;
}

