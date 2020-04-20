
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main( void )
{
	int	count	= 0;
	pid_t	pid	= fork();
	if ( pid < 0 )
	{
		printf( "Fork error\n" );
	}else if ( pid == 0 )
	{
		pid = fork();
		if ( pid < 0 )
		{
			printf( "Fork error\n" );
		}else if ( pid > 0 )
		{
			exit( 0 );
		}
		printf( "Grandson pid %d, ppid %d, count %d\n", getpid(), getppid(), ++count );
		exit( 0 );
	}else{
		printf( "Main pid %d, ppid %d, count %d\n", getpid(), getppid(), ++count );
		exit( 0 );
	}
	if ( waitpid( pid, NULL, 0 ) != pid )
		printf( "Wait error\n" );
	return(0);
}