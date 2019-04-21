#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>

using boost::multiprecision::cpp_int;

struct Data {
	long long startInterval;
	long long endInterval;
	int increment;
	long long** result;
	long long values;
	char name[50];
};

bool ReturnPrime(int number)
{
	if (number % 2 == 0 || number < 2)
	{
		return false;
	}

	for (int i = 3; i*i <= number; i = i + 2)
	{
		if (number % i == 0)
		{
			return false;
		}
	}

	return true;
}

cpp_int FactorialIterative(long long nb)
{
	cpp_int fac = 1;
	for (long long x = 2; x <= nb; ++x)
	{
		fac = fac * x;
	}
	return fac;
}

bool CheckWilson(long long i)
{
	return (FactorialIterative(i - 1) + 1) % (i*i) == 0;
}

DWORD WINAPI ThreadFunc(void* data) {
	clock_t start = clock();
	long long startInterval = ((struct Data*) data)->startInterval;
	long long endInterval = ((struct Data*) data)->endInterval;
	long long** results = ((struct Data*) data)->result;
	int increment = ((struct Data*) data)->increment;
	long long values = ((struct Data*) data)->values;
	char* name = ((struct Data*) data)->name;
	long long intervalLength = sizeof(long long);
	int id = atoi(name);
	results[id] = (long long*)malloc(intervalLength);
	printf_s("STARTING  Thread %s\n", name);
	bool checkIfThreadFoundWilson = false;
	for(long long i=startInterval; i<endInterval; i=i+increment)
	{
		if(ReturnPrime(i))
		{
			if(CheckWilson(i))
			{
				checkIfThreadFoundWilson = true;
				if ((values + 1) * sizeof(long long) <= intervalLength) {
					results[id][values++] = i;
				}
				else {
					intervalLength += sizeof(long long);
					results[id] = (long long*)realloc(results[id], intervalLength);
					results[id][values++] = i;
				}
				printf_s("WILSON PRIME %d THREAD %s \n", i, name);
			}
		}
	}
	clock_t stop = clock();
	float duration = (float)(stop - start) / CLOCKS_PER_SEC;
	if (checkIfThreadFoundWilson) {
		printf_s("\nWILSON PRIME THREAD %s -> ", name);
		for (long long i = 0; i < intervalLength / sizeof(long long); i++) {
			printf_s(" %ld", results[id][i]);
		}
		printf_s("\nEXIT Thread %s in TIME %f\n", name, duration);
	}
	return 0;
}

int main(int argc, char*argv[]) {
	HANDLE thread[32];
	struct Data data[32];

	for (long long numThreads = 1; numThreads <= 8; numThreads++) {
		long long** results = (long long**)malloc(8 * sizeof(long long*));
		long long values = 0;
		for (int i = 0; i < numThreads; ++i)
		{
			data[i].result = results;
			data[i].startInterval = i + 1;
			data[i].endInterval = 35000;
			data[i].increment = numThreads;
			data[i].values = values;
			_itoa_s(i, data[i].name, 10);
			thread[i] = CreateThread(NULL, 0, ThreadFunc, &data[i], 0, NULL);
		}

		for (int i = 0; i < numThreads; ++i)
		{
			WaitForSingleObject(thread[i], INFINITE);
		}

		printf("---------------------------");
		printf("Done with %d threads", numThreads);
		printf("---------------------------");
		printf("\n\n");
		free(results);
	}
	getchar();
	return 0;
}