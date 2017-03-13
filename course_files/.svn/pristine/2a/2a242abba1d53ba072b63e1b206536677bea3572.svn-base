#include <stdio.h>
#include <string>

#include "hashtable.h"



template <>
typename HashTable<std::string,std::string>::CodeType HashTable<std::string,std::string>::HashCode(const std::string &key)
{
	CodeType sum=0;
	for(auto c : key)
	{
		sum+=c;
	}
	return sum;
}



int main(void)
{
    HashTable <std::string,std::string> table;

    table.Update("F4","Phantom");
    table.Update("F5","Tiger");
    table.Update("F8","Crusader");
    table.Update("F14","Tomcat");
    table.Update("F15","Eagle");
    table.Update("F16","Falcon");
    table.Update("F18","Hornet");
    table.Update("F22","Raptor");


	for(auto hd=table.First(); true==table.IsNotNull(hd); hd=table.Next(hd))
	{
		printf("%s -> %s\n",table.GetKey(hd).c_str(),table[hd]->c_str());
	}


    for(;;)
    {
        printf("Enter String>");

        char str[256];
        fgets(str,255,stdin);
        for(int i=0; 0!=str[i]; ++i)
        {
            if(' '>str[i])
            {
                str[i]=0;
                break;
            }
        }

		std::string keyStr(str);
		auto foundPtr=table[keyStr];
		if(nullptr!=foundPtr)
		{
			printf("Found\n");
			printf("  %s\n",foundPtr->c_str());
		}
		else
		{
			printf("Not found.\n");
		}
    }

    return 0;
}
