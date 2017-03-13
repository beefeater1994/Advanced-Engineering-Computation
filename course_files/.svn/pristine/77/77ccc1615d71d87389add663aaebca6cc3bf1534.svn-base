#include <stdio.h>
#include <string>

#include "hashset.h"



template <>
typename HashSet<std::string>::CodeType HashSet<std::string>::HashCode(const std::string &key)
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
    HashSet <std::string> set;

    set.Add("Phantom");
    set.Add("Tiger");
    set.Add("Crusader");
    set.Add("Tomcat");
    set.Add("Eagle");
    set.Add("Falcon");
    set.Add("Hornet");
    set.Add("Raptor");

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

        std::string tst=str;
        if(true==set.IsIncluded(tst))
        {
            printf("Included\n");
        }
        else
        {
            printf("Not Included\n");
        }
    }

    return 0;
}
