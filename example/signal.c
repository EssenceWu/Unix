
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler_fun( int signal_num );


int main( void )
{
	signal( SIGINT, signal_handler_fun );
	for (;; )
		;
	return(0);
}


void signal_handler_fun( int signal_num )
{
	printf( "Opps! I catch the %d signal!\n", signal_num );
}
