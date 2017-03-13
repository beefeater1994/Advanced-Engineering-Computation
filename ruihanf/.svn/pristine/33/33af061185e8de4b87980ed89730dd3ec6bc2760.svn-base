#ifndef HASHBASE_IS_INCLUDED
#define HASHBASE_IS_INCLUDED
/* { */

template <class KeyType>
class HashBase
{
public:
	// This function must be specialized for a key type.
	unsigned long long int HashCode(const KeyType &key) const {
		SimpleBitmap test;
		test = key;
		long long int sum = 0;
		const unsigned char * bits = test.GetBitmapPointer();
		for (int i = 0; i < test.GetTotalNumComponent(40,40); i++) {
			if (i % 5 == 0) {
				sum += 2 * bits[i];
			}
			else if (i % 5 == 1) {
				sum += 3 * bits[i];
			}
			else if (i % 5 == 2) {
				sum += 5 * bits[i];
			}
			else if (i % 5 == 3) {
				sum += 7 * bits[i];
			}
			else if (i % 5 == 4) {
				sum += 11 * bits[i];
			}
		}
		return sum;
	}
};

class HashCommon
{
public:
	typedef unsigned long long CodeType;

	class Handle
	{
	public:
		long long int row,column;
	};

	enum 
	{
		MINIMUM_TABLE_SIZE=7
	};

protected:
	long long int nElem;

public:
	HashCommon()
	{
		nElem=0;
	}
	Handle First(void) const
	{
		Handle h;
		if(0<nElem)
		{
			h.row=0;
			h.column=0;
		}
		else
		{
			h.row=-1;
			h.column=-1;
		}
		return h;
	}
	bool IsNotNull(Handle hd) const
	{
		if(0<=hd.row && 0<=hd.column)
		{
			return true;
		}
		return false;
	}
};

/* } */
#endif
