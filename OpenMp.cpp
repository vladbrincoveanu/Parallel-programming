#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <boost/multiprecision/cpp_int.hpp>
#include <time.h>
#include <windows.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

using boost::multiprecision::cpp_int;

struct Data {
	long long startInterval;
	long long endInterval;
	int increment;
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

int main()
{
	int id; 	
	char name[32][50];

	for (long long numThreads = 16; numThreads <= 16; numThreads++) {
		omp_set_num_threads(numThreads);
		long long** results = (long long**)malloc(8 * sizeof(long long*));
		long long values = 0;
		#pragma omp parallel private(id,values)
		{
			values = 0;
			clock_t start = clock();
			id = omp_get_thread_num(); 
			long long intervalLength = sizeof(long long);
			results[id] = (long long*)malloc(intervalLength);
			long long startInterval = id + 1;
			long long endInterval = 35000;
			int increment = numThreads;
			_itoa_s(id, name[id], 10);
			printf_s("\nSTARTING  Thread %s\n", name[id]);
			bool checkIfThreadFoundWilson = false;

			for (long long i = startInterval; i < endInterval; i = i + increment)
			{
				if (ReturnPrime(i))
				{
					if (CheckWilson(i))
					{
						checkIfThreadFoundWilson = true;
						if ((values + 1)*sizeof(long long) <= intervalLength) {
							results[id][values++] = i;
						}
						else {
							intervalLength +=sizeof(long long);
							results[id] = (long long*)realloc(results[id], intervalLength);
							results[id][values++] = i;
						}
					}
				}
			}
			clock_t stop = clock();
			float duration = (float)(stop - start) / CLOCKS_PER_SEC;
			if (checkIfThreadFoundWilson) {
				printf_s("\nWILSON PRIME THREAD %s -> ", name[id]);
				for (long long i = 0; i < intervalLength / sizeof(long long); i++) {
					printf_s(" %ld", results[id][i]);
				}
				printf_s("\nEXIT Thread %s in TIME %f\n", name[id], duration);
			}
		}

		printf("---------------------------");
		printf("Done with %d threads",numThreads);
		printf("---------------------------");
		printf("\n\n");
		free(results);
	}

	getchar();
	return 0;
}
