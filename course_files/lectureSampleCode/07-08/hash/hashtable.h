#ifndef HASHTABLE_IS_INCLUDED
#define HASHTABLE_IS_INCLUDED
/* { */

#include <vector>

#include "hashbase.h"

template <class KeyType,class ValueType>
class HashTable : public HashBase <KeyType>, public HashCommon
{
protected:
	typedef unsigned long long int CodeType;
	using HashBase <KeyType>::HashCode;

private:
	class Entry
	{
	public:
		KeyType key;
		CodeType code;
		ValueType value;
	};
	std::vector <std::vector <Entry> > table;

public:
	HashTable()
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
		CleanUp();
		table.resize(nRow);
	}

private:
	bool Update(const KeyType &key,const ValueType &value,CodeType code)
	{
		auto r=code%table.size();

		for(auto &c : table[r])
		{
			if(c.key==key)
			{
				c.value=value;
				return true;
			}
		}

		Entry ent;
		ent.key=key;
		ent.code=code;
		ent.value=value;
		table[r].push_back((Entry &&)ent);
		++nElem;
		return true;
	}

public:
	void Resize(int nRow)
	{
		std::vector <Entry> backup;
		for(auto &r : table)
		{
			for(auto &c : r)
			{
				backup.push_back((Entry &&)c);
			}
		}
		SetSize(nRow);
		for(auto &b : backup)
		{
			Add(b.key,b.value,b.code);
		}
	}

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

	bool Update(const KeyType &key,const ValueType &value)
	{
		return Update(key,value,HashCode(key));
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

	ValueType *operator[](const KeyType &key)
	{
		if(0<table.size())
		{
			auto code=HashCode(key);
			auto r=code%table.size();
			for(auto &c : table[r])
			{
				if(code==c.code && key==c.key)
				{
					return &c.value;
				}
			}
		}
		return nullptr;
	}
	const ValueType *operator[](const KeyType &key) const
	{
		if(0<table.size())
		{
			auto code=HashCode(key);
			auto r=code%table.size();
			for(auto &c : table[r])
			{
				if(code==c.code && key==c.key)
				{
					return &c.value;
				}
			}
		}
		return nullptr;
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
    ValueType *operator[](EnumHandle hd)
    {
		if(true==IsNotNull(hd))
		{
			return &table[hd.row][hd.column].value;
		}
		return nullptr;
	}
    const ValueType *operator[](EnumHandle hd) const
    {
		if(true==IsNotNull(hd))
		{
			return &table[hd.row][hd.column].value;
		}
		return nullptr;
	}
    const KeyType &GetKey(EnumHandle hd) const
    {
		return table[hd.row][hd.column].key;
	}
};

/* } */
#endif
