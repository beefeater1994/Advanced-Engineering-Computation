#ifndef HASHSET_IS_INCLUDED
#define HASHSET_IS_INCLUDED
/* { */

#include <vector>



template <class KeyType>
class HashSet
{
protected:
	typedef unsigned long long int CodeType;
	static CodeType HashCode(const KeyType &key);

	enum
	{
		MINIMUM_TABLE_SIZE=7
	};

private:
	class Entry
	{
	public:
		KeyType key;
		CodeType code;
	};
	std::vector <std::vector <Entry> > table;
	unsigned long long int nElem;

public:
	HashSet()
	{
		nElem=0;
		SetSize(MINIMUM_TABLE_SIZE);
	}

	unsigned long long int GetN(void) const
	{
		return nElem;
	}

	void CleanUp(void)
	{
		table.clear();
		nElem=0;
	}
	void SetSize(int nRow)
	{
		table.resize(nRow);
	}
	void Resize(int nRow);

private:
	bool IsIncluded(const KeyType &key,CodeType code) const
	{
		if(0<table.size())
		{
			auto r=code%table.size();
			for(auto &c : table[r])
			{
				if(code==c.code && key==c.key)
				{
					return true;
				}
			}
		}
		return false;
	}

public:
	bool IsIncluded(const KeyType &key) const
	{
		auto code=HashCode(key);
		return IsIncluded(key,code);
	}
	bool Add(const KeyType &key)
	{
		auto code=HashCode(key);
		auto r=code%table.size();
		if(true!=IsIncluded(key,code))
		{
			Entry ent;
			ent.key=key;
			ent.code=code;
			table[r].push_back((Entry &&)ent);
			++nElem;
			return true;
		}
		return false;
	}
	bool Delete(const KeyType &key)
	{
		if(0<table.size())
		{
			auto code=HashCode(key);
			auto r=code%table.size();
			for(auto &c : table[r])
			{
				if(c.code==code && c.key==key)
				{
					std::swap(c,table[r].back());
					table[r].pop_back();
					--nElem;
					return true;
				}
			}
		}
		return false;
	}
};

/* } */
#endif
