#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hashset.h"


template <>
unsigned long long int HashBase<unsigned int>::HashCode(const unsigned int &key) const
{
	return key;
}


int main(void)
{
	srand((int)time(nullptr));

	std::vector <unsigned int> random;
	HashSet <unsigned int> used;
	used.SetSize(7);
	while(random.size()<20)
	{
		auto r=1+rand()%20;
		if(true!=used.IsIncluded(r))
		{
			random.push_back(r);
			used.Add(r);
		}
	}
	printf("NumElem=%lld\n",used.GetN());
	for(auto v : random)
	{
		printf("%d\n",v);
	}

	for(unsigned int i=1; i<=20; i+=2)
	{
		used.Delete(i);
	}
	printf("NumElem=%lld\n",used.GetN());

	return 0;
}

