/**
 * Real Time Embedded Systems
 * Project 1
 * 
 * Ⓒ 2021 K. Chatzis
 * kachatzis <at> ece.auth.gr
 **/

#include <stdio.h>
#include <pthread_fifo.h>


#define DEFAULT_PRODUCERS 2     // Default Number of Producer Threads
#define DEFAULT_CONSUMERS 2     // Default Number of Consumer Threads
#define DEFAULT_QUEUE_SIZE 1    // Default Size of Function Queue
#define DEFAULT_FUNCTIONS_T 3   // Default Functions per Thread


/**
 * Producer Thread Function
 **/
void *_f_producer (void *q);

/**
 * Consumer Thread Function
 **/
void *_f_consumer (void *q);

/**
 * Work Function
 **/
void *work(void){ printf("Work"); return NULL; }


/**
 * Thread Function Arguments
 * 
 * @param fifo Function Queue
 * @param loop Function Created per Thread
 * @param p Number of Producers
 * @param q Number of Consumers
 * @param idx Thread Index
 **/
typedef struct t_args {
    queue* fifo;
    int loop;
    int p;
    int q;
    int idx;
} t_args;


int main(int argc, char** argv)
{


    /***********************
     * Read Arrguments
     ***********************/

    // If no arguments
    if (argc <= 1)
	{
        // Show usage info
		printf( "Usage: \n" );
		printf( " -p <int>  : Producers\n" );
		printf( " -q <int>  : Consumers\n" );
        printf( " -s <int>  : Queue Size\n" );
        printf( " -l <int>  : Functions per Producer (Thread)\n" );
        // exit
        return 0;
	}


    int p = DEFAULT_PRODUCERS;  // Producers
    int q = DEFAULT_CONSUMERS;  // Consumers
    int s = DEFAULT_QUEUE_SIZE; // Queue Size
    int l = DEFAULT_FUNCTIONS_T;// Functions per Thread

    // Read Arguments
	for (int i = 1; i < argc; i++)
	{
		if ( strcmp(argv[i], "-p") == 0 ) {
			i++;
			p = (i < argc) ? atoi(argv[i]) : DEFAULT_PRODUCERS;
		}

        else if ( strcmp(argv[i], "-q") == 0 ) {
			i++;
			q = (i < argc) ? atoi(argv[i]) : DEFAULT_CONSUMERS;
		}

        else if ( strcmp(argv[i], "-s") == 0 ) {
			i++;
			s = (i < argc) ? atoi(argv[i]) : DEFAULT_QUEUE_SIZE;
		}

        else if ( strcmp(argv[i], "-l") == 0 ) {
			i++;
			l = (i < argc) ? atoi(argv[i]) : DEFAULT_FUNCTIONS_T;
		}

	}

    printf( "Producers: %d\nConsumers: %d\nQueue Size: %d\nFunctions per Producer: %d\n", p, q, s, l );


    /***********************
     * Create & Run Queue
     ***********************/

    queue *fifo;
    // Initialize Producers and Consumers
    pthread_t* pro = (pthread_t*)malloc(p * sizeof(pthread_t));
    pthread_t* con = (pthread_t*)malloc(p * sizeof(pthread_t));

    // Producer And Consumer FUnctions
    void *producer (void *args);
    void *consumer (void *args);

    // Initialize Queue
    fifo = queueInit (s);
    if (fifo ==  NULL) {
        fprintf (stderr, "main: Queue Init failed.\n");
        exit (1);
    }


    

    // Start Producer Threads
    for(int i=0; i<p; i++)
    {
        t_args* _t_args = (t_args*) malloc(sizeof(t_args));
        _t_args->fifo = fifo;
        _t_args->loop = l;
        _t_args->p = p;
        _t_args->q = q;
        _t_args->idx = i;
        pthread_create (&(pro[i]), NULL, _f_producer, (void*)_t_args);
    }
        

    // Start Consumer Threads
    for(int i=0; i<q; i++)
    {    
        t_args* _t_args = (t_args*) malloc(sizeof(t_args));
        _t_args->fifo = fifo;
        _t_args->loop = l;
        _t_args->p = p;
        _t_args->q = q;
        _t_args->idx = i;
        pthread_create (&(con[i]), NULL, _f_consumer, (void*)_t_args);
    }

    // Join Threads
    for(int i=0; i<p; i++)
        pthread_join (pro[i], NULL);

    for(int i=0; i<q; i++)  
        pthread_join (con[i], NULL);

    // Clean Up
    queueDelete(fifo);
    free(pro);
    free(con); 


    return 0;
}


void *_f_producer (void *inArgs)
{
    queue *fifo;
    int loop;
    int i;
    int idx;

    fifo = (queue*) ((struct t_args*)inArgs)-> fifo;
    loop = (int)    ((struct t_args*)inArgs)-> loop;
    idx  = (int)    ((struct t_args*)inArgs)-> idx;

    for (i = 0; i < loop; i++) {
        pthread_mutex_lock (fifo->mut);

        while (fifo->full) {
            // printf ("producer %d: queue FULL.\n", idx);
            pthread_cond_wait (fifo->notFull, fifo->mut);
        }

        workFunction* wf;
        wf->work = (void *) (work);
        void* arg = (void*)malloc(sizeof(int));
        (wf->arg)  = (int)(idx*loop + i);

        enqueue (fifo, wf);
        pthread_mutex_unlock (fifo->mut);
        pthread_cond_signal (fifo->notEmpty);
        usleep (100000);
    }

    // printf("producer %d: exit\n", idx);

    return (NULL);
}


void *_f_consumer (void *inArgs)
{
    queue *fifo;
    int loop;
    int i, d;
    int p, q;
    int idx;

    fifo = (queue*) ((struct t_args*)inArgs)-> fifo;
    loop = (int)    ((struct t_args*)inArgs)-> loop;
    p    = (int)    ((struct t_args*)inArgs)-> p;
    q    = (int)    ((struct t_args*)inArgs)-> q;
    idx  = (int)    ((struct t_args*)inArgs)-> idx;

    int rec_count = (idx<q-1)? (loop*p)/q : ((loop*p)/q) + (loop*p)%q ;
    // printf("   Consumer %d To Receive %d\n", idx, rec_count);


    for (i = 0; i < rec_count ; i++) {
        pthread_mutex_lock (fifo->mut);

        while (fifo->empty) {
            // printf ("consumer %d: queue EMPTY.\n", idx);
            pthread_cond_wait (fifo->notEmpty, fifo->mut);
        }

        dequeue (fifo, &d);
        pthread_mutex_unlock (fifo->mut);
        pthread_cond_signal (fifo->notFull);
        printf ("consumer %d: rec %d.\n", idx, d);
        usleep(200000);
    }

    // printf("consumer %d: exit\n", idx);

    return (NULL);
}

