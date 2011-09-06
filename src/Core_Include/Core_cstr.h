#ifndef _CORE_CSTR_H_
#define _CORE_CSTR_H_

class cstr
{
public:
	static int strcasecmp(const char* string0, const char* string1)
	{
		int pos = 0;
		while (1)
		{
			char c0 = string0[pos];
			char c1 = string1[pos];

			if (c0 >= 'a' && c0 <= 'z')
			{
				c0 -= 0x20;
			}
			if (c1 >= 'a' && c1 <= 'z')
			{
				c1 -= 0x20;
			}
			if (c0 == 0)
			{
				return ((c1 ==0) ? 0 : -1);
			}
			if (c1 == '\0')
			{
				return 1;
			}
			if (c0 < c1)
			{
				return -1;
			}
			if (c0 > c1)
			{
				return 1;
			}
			pos += 1;
		}
	}
};

#endif