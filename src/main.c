/**
 * Real Time Embedded Systems
 * Project 1
 * 
 * Ⓒ 2021 K. Chatzis
 * kachatzis <at> ece.auth.gr
 **/

#include <stdio.h>
#include <pthread_fifo.h>


#define DEFAULT_PRODUCERS 2
#define DEFAULT_CONSUMERS 2


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
        // exit
        return 0;
	}


    int p = DEFAULT_PRODUCERS;  // Producers
    int q = DEFAULT_CONSUMERS;  // Consumers

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

	}

    printf( "Producers: %d\nConsumers: %d\n", p, q );


    /***********************
     * Create & Run Queue
     ***********************/

    queue *fifo;
    pthread_t pro, con;

    void *producer (void *args);
    void *consumer (void *args);

    fifo = queueInit ();
    if (fifo ==  NULL) {
        fprintf (stderr, "main: Queue Init failed.\n");
        exit (1);
    }
    pthread_create (&pro, NULL, producer, fifo);
    pthread_create (&con, NULL, consumer, fifo);
    pthread_join (pro, NULL);
    pthread_join (con, NULL);
    queueDelete (fifo);

    
    return 0;
}