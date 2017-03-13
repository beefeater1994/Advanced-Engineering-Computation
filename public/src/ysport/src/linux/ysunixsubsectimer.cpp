/* ////////////////////////////////////////////////////////////

File Name: ysunixsubsectimer.cpp
Copyright (c) 2015 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#include <stdio.h>
#include <sys/time.h>
#include "../yssubsectimer.h"

double YsSubSecTimer::GetSecond(void)
{
	timeval tm;
	gettimeofday(&tm,NULL);
	return (double)tm.tv_sec+(double)tm.tv_usec/1000.0;;
}

void YsSubSecTimer::SleepMilliSecond(unsigned int ms)
{
	if(ms>0)
	{
		fd_set set;
		struct timeval wait;
		wait.tv_sec=ms/1000;
		wait.tv_usec=(ms%1000)*1000;
		FD_ZERO(&set);
		select(0,&set,NULL,NULL,&wait);
	}
}

void YsSubSecTimer::SleepSecond(const double sec)
{
	if(0.0<sec)
	{
		unsigned int ms=(unsigned int)(sec*1000.0);
		if(0==ms)
		{
			ms=1;
		}
		SleepMilliSecond(ms);
	}
}
