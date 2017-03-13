#ifndef YSSTRINGPARSER_IS_INCLUDED
#define YSSTRINGPARSER_IS_INCLUDED
/* { */

#include "ysstring.h"

template <class STRTYPE>
class YsStringParser
{
private:
	YsArray <STRTYPE> argv;
	YsArray <STRTYPE> separator;
	YsArray <STRTYPE> comma;
	YsArray <STRTYPE> takeAsWord;
	YsArray <STRTYPE> quote;
	YsArray <STRTYPE> inlineComment;
	YsArray <STRTYPE> escape;

	enum STATE
	{
		IN_BLANK,
		IN_QUOTE,
		IN_WORD,
	};

	static YSSIZE_T Match(const typename STRTYPE::CHAR_T strIn[],const STRTYPE &pattern);
	static YSSIZE_T Match(const typename STRTYPE::CHAR_T strIn[],const YsArrayMask <STRTYPE> &patternArray);

public:
	YsStringParser();
	YSRESULT Parse(const typename STRTYPE::CHAR_T strIn[]);
	const YsArray <STRTYPE> &Argv(void) const;

	void SetTakeAsWord(const YsConstArrayMask <STRTYPE> &takeAsWord);
	void SetInlineComment(const YsConstArrayMask <STRTYPE> &inlineComment);
};

template <class STRTYPE>
YSSIZE_T YsStringParser<STRTYPE>::Match(const typename STRTYPE::CHAR_T strIn[],const STRTYPE &pattern)
{
	int i;
	for(i=0; 0!=strIn[i] && i<pattern.Strlen(); ++i)
	{
		if(strIn[i]!=pattern[i])
		{
			break;
		}
	}
	if(i==pattern.Strlen())
	{
		return pattern.Strlen();
	}
	return 0;
}

template <class STRTYPE>
YSSIZE_T YsStringParser<STRTYPE>::Match(const typename STRTYPE::CHAR_T strIn[],const YsArrayMask <STRTYPE> &patternArray)
{
	for(auto &ptn : patternArray)
	{
		auto match=Match(strIn,ptn);
		if(0<match)
		{
			return match;
		}
	}
	return 0;
}

template <>
inline YsStringParser<YsString>::YsStringParser()
{
	separator.Add(" ");
	separator.Add("\t");

	comma.Add(",");
	comma.Add(";");

	takeAsWord.Add("{");
	takeAsWord.Add("}");
	takeAsWord.Add("(");
	takeAsWord.Add(")");
	takeAsWord.Add("[");
	takeAsWord.Add("]");

	quote.Add("\"");
	quote.Add("'");

	escape.Add("\\");
}

template <>
inline YsStringParser<YsWString>::YsStringParser()
{
	separator.Add(L" ");
	separator.Add(L"\t");

	comma.Add(L",");
	comma.Add(L";");

	takeAsWord.Add(L"{");
	takeAsWord.Add(L"}");
	takeAsWord.Add(L"(");
	takeAsWord.Add(L")");
	takeAsWord.Add(L"[");
	takeAsWord.Add(L"]");

	quote.Add(L"\"");
	quote.Add(L"'");

	escape.Add(L"\\");
}


template <class STRTYPE>
YSRESULT YsStringParser<STRTYPE>::Parse(const typename STRTYPE::CHAR_T strIn[])
{
	YSSIZE_T ptr=0;
	STATE state=IN_BLANK;
	STRTYPE closingQuote;
	YSBOOL escapeNext=YSFALSE;

	argv.CleanUp();
	while(strIn[ptr]!=0)
	{
		if(IN_BLANK==state)
		{
			YSSIZE_T sz;
			if(0<(sz=Match(strIn+ptr,separator)))
			{
				ptr+=sz;
			}
			else if(0<(sz=Match(strIn+ptr,comma)))
			{
				argv.Increment();
				argv.Last().CleanUp();
				state=IN_WORD;
				ptr+=sz;
			}
			else if(0<(sz=Match(strIn+ptr,takeAsWord)))
			{
				argv.Increment();
				argv.Last().Set(sz,strIn+ptr);
				ptr+=sz;
			}
			else if(0<(sz=Match(strIn+ptr,quote)))
			{
				argv.Increment();
				argv.Last().CleanUp();
				closingQuote.Set(sz,strIn+ptr);
				state=IN_QUOTE;
				ptr+=sz;
			}
			else if(0<(sz=Match(strIn+ptr,escape)))
			{
				escapeNext=YSTRUE;
				argv.Increment();
				argv.Last().CleanUp();
				state=IN_WORD;
				ptr+=sz;
			}
			else if(0<(sz=Match(strIn+ptr,inlineComment)))
			{
				return YSOK;
			}
			else
			{
				argv.Increment();
				argv.Last().CleanUp();
				argv.Last().Append(strIn[ptr]);
				state=IN_WORD;
				++ptr;
			}
		}
		else if(IN_WORD==state)
		{
			YSSIZE_T sz;
			if(YSTRUE==escapeNext)
			{
				argv.Last().Append(strIn[ptr]);
				++ptr;
				escapeNext=YSFALSE;
			}
			else if(0<(sz=Match(strIn+ptr,separator)))
			{
				state=IN_BLANK;
				ptr+=sz;
			}
			else if(0<(sz=Match(strIn+ptr,comma)))
			{
				argv.Increment();
				argv.Last().CleanUp();
				state=IN_WORD;
				ptr+=sz;
			}
			else if(0<(sz=Match(strIn+ptr,takeAsWord)))
			{
				argv.Increment();
				argv.Last().Set(sz,strIn+ptr);
				ptr+=sz;
				state=IN_BLANK;
			}
			else if(0<(sz=Match(strIn+ptr,quote)))
			{
				argv.Increment();
				argv.Last().CleanUp();
				closingQuote.Set(sz,strIn+ptr);
				state=IN_QUOTE;
			}
			else if(0<(sz=Match(strIn+ptr,escape)))
			{
				escapeNext=YSTRUE;
				ptr+=sz;
			}
			else if(0<(sz=Match(strIn+ptr,inlineComment)))
			{
				return YSOK;
			}
			else
			{
				argv.Last().Append(strIn[ptr]);
				++ptr;
			}
		}
		else if(IN_QUOTE==state)
		{
			YSSIZE_T sz;
			if(YSTRUE==escapeNext)
			{
				argv.Last().Append(strIn[ptr]);
				++ptr;
				escapeNext=YSFALSE;
			}
			else if(0<(sz=Match(strIn+ptr,closingQuote)))
			{
				state=IN_BLANK;
				ptr+=sz;
			}
			else if(0<(sz=Match(strIn+ptr,escape)))
			{
				escapeNext=YSTRUE;
				ptr+=sz;
			}
			else
			{
				argv.Last().Append(strIn[ptr]);
				++ptr;
			}
		}
	}

	return YSOK;
}

template <class STRTYPE>
const YsArray <STRTYPE> &YsStringParser<STRTYPE>::Argv(void) const
{
	return argv;
}

template <class STRTYPE>
void YsStringParser<STRTYPE>::SetTakeAsWord(const YsConstArrayMask <STRTYPE> &takeAsWord)
{
	this->takeAsWord.Set(takeAsWord.GetN(),takeAsWord);
}

template <class STRTYPE>
void YsStringParser<STRTYPE>::SetInlineComment(const YsConstArrayMask <STRTYPE> &inlineComment)
{
	this->inlineComment=inlineComment;
}

/* } */
#endif
