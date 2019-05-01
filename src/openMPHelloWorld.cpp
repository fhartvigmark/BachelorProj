#include <stdio.h>
#include <openMPHelloWorld.h>

static int OpenMPHello()
{
#pragma omp parallel
	{
		printf("Hello World");
	}
	return 0;
}