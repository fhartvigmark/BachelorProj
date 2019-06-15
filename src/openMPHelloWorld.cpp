#include <stdio.h>
#include <openMPHelloWorld.h>

//Test OpenMP
static int OpenMPHello()
{
	#pragma omp parallel
	{
		printf("Hello World");
	}
	return 0;
}