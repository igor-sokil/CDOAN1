/*
 * utilities.c
 * 
 * Version 1.0.0
 * 
 * March 2, 2021
 */

#include "header.h"
#include "scMrzs.h"

/*
 * Place an interger value into a DNP3 message for transmisison 
 */
void IntToChars(char *c, int16_t start, int16_t length, int32_t val)
{   int32_t temp;
	
	while (length-- > 0)
	{
		temp = val / 256;
		c[start++] = val - (temp * 256);
		val = temp;
	}
}
/*
 * Parse and integer from a characters array (LSB first)
 */
int32_t CharsToInt(char *c, int16_t start, int16_t length)
{   int32_t answer = 0;
	
	start += length;
	while (length-- > 0)
		answer = (answer * 256) + (int32_t) (c[--start] & 0xFF);
	return answer;
}
/*
 *  char ToLowerCase(char c)
 */
char ToLowerCase(char c)
{
	if ((c >= 'A') && (c <= 'Z'))
		return (c - 'A' + 'a');
	return (c);
}
int EncodeFloatAs32BitInt(float f)
{
//	int i;
	IntFloat_u fi;
	
	fi.f = f;
	return(fi.i);
}

