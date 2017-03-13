#ifndef HASHSET_IS_INCLUDED
#define HASHSET_IS_INCLUDED
/* { */

#include <vector>

#include "hashbase.h"

template <class KeyType>
class HashSet : public HashBase <KeyType>, public HashCommon
{
protected:
	typedef unsigned long long int CodeType;

private:
	class Entry
	{
	public:
		KeyType key;
		CodeType code;
	};
	std::vector <std::vector <Entry> > table;

public:
	HashSet()
	{
		SetSize(MINIMUM_TABLE_SIZE);
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

	EnumHandle First(void) const
	{
		EnumHandle hd;
		hd.row=-1;
		hd.column=-1;
		for(int r=0; r<table.size(); ++r)
		{
			if(0<table[r].size())
			{
				hd.row=r;
				hd.column=0;
				break;
			}
		}
		return hd;
	}
    EnumHandle Next(EnumHandle hd) const
    {
		if(true!=IsNotNull(hd))
		{
			return First();
		}
		else
		{
			++hd.column;
			if(hd.column<table[hd.row].size())
			{
				return hd;
			}
			else
			{
				++hd.row;
				hd.column=0;
				while(hd.row<table.size() && 0==table[hd.row].size())
				{
					++hd.row;
				}
				if(table.size()<=hd.row)
				{
					hd.row=-1;
					hd.column=-1;
				}
				return hd;
			}
		}
	}
    KeyType *operator[](EnumHandle hd)
    {
		if(true==IsNotNull(hd))
		{
			return &table[hd.row][hd.column].key;
		}
		return nullptr;
	}
    const KeyType *operator[](EnumHandle hd) const
    {
		if(true==IsNotNull(hd))
		{
			return &table[hd.row][hd.column].key;
		}
		return nullptr;
	}
};

/* } */
#endif
