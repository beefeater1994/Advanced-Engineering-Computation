#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>



class SimpleHashSet
{
private:
	std::vector <std::vector <unsigned int> > table;
	long long int nElem;

public:
	SimpleHashSet()
	{
		nElem=0;
	}
	void CleanUp(void)
	{
		table.clear();
		nElem=0;
	}
	unsigned long long GetN(void) const
	{
		return nElem;
	}
	void SetSize(int nRow)
	{
		CleanUp();
		table.resize(nRow);
	}
	void Resize(int nRow)
	{
		std::vector <unsigned int> backup;
		for(auto &r : table)
		{
			for(auto &c : r)
			{
				backup.push_back(c);
			}
		}

		SetSize(nRow);
		for(auto v : backup)
		{
			Add(v);
		}
	}
	bool IsIncluded(unsigned int key) const
	{
		if(0==table.size())
		{
			return false;
		}
		int r=key%table.size();
		for(auto v : table[r])
		{
			if(key==v)
			{
				return true;
			}
		}
		return false;
	}
	bool Add(unsigned int key)
	{
		if(true!=IsIncluded(key))
		{
			int r=key%table.size();
			table[r].push_back(key);
			++nElem;
			return true;
		}
		return false;
	}
	bool Delete(unsigned int key)
	{
		if(0<table.size())
		{
			int r=key%table.size();
			for(auto &v : table[r])
			{
				if(v==key)
				{
					v=table[r].back();
					table[r].pop_back();
					--nElem;
					return true;
				}
			}
		}
		return false;
	}
	void Print(void) const
	{
		printf("----\n");
		for(auto &r : table)
		{
			for(auto &c : r)
			{
				printf("%d\n",c);
			}
		}
		printf("----\n");
	}
};

int main(void)
{
	srand((int)time(nullptr));

	std::vector <unsigned int> random;
	SimpleHashSet used;
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

	for(unsigned int i=1; i<=20; i+=2)
	{
		used.Delete(i);
	}
	printf("NumElem=%lld\n",used.GetN());

	used.Print();

	used.Resize(10);

	used.Print();

	//for(auto r : random)
	//{
	//	printf("%d\n",r);
	//}

	return 0;
}


