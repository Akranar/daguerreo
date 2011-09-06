#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

class Exception
{
protected:
	const char * reason;
public:
	Exception(const char * text)
		: reason(text)
	{};
	const char * GetReason() const;
	void PrintReason() const;
};

#define CONDITIONAL_EXCEPTION(exp, text) \
	if (exp) throw Exception(#text);

#endif