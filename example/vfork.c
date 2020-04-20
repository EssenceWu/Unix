
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( void )
{
	int	count	= 0;
	pid_t	pid	= vfork();
	if ( pid < 0 )
	{
		printf( "Fork error\n" );
	}else if ( pid == 0 )
	{
		printf( "Child pid %d, ppid %d, count %d\n", getpid(), getppid(), ++count );
		exit( 0 );
	}else{
		printf( "Main pid %d, ppid %d, count %d\n", getpid(), getppid(), ++count );
		exit( 0 );
	}
	return(0);
}