#include "encoder.h"

int string_decode(const unsigned char *es, unsigned char *s)
{
	int i = 0;
	int j = 0;
	unsigned char c = es[0];

	// go until you reach a null terminator or you exceed the char limit
	while(c && j < 256)
	{
		c = es[i];
		// if it is the null terminator
		if (c == 0)
		{
			s[j] = 0;
			j++;
		}
		// check for if there is a starting quote
		else if (i == 0 && c != 34)
		{
			fprintf(stderr, "Strings must begin with a \", this one began with %c\n", c);
			return 0;
		}
		// check for invalid quote (unescaped) 
		// 	if it has gotten this far it isnt at the start and this checks if it is at the end
		else if (c == 34 && (i != 0 && es[i+1] != 0))
		{
			fprintf(stderr, "An \" must be at the start or end of the string, if you meant to print one then please escape it: \\\"\n");
			return 0;
		}
		// if char is less than 32
		else if (c < 32)
		{
			fprintf(stderr, "The character with value %d is not an allowable character for printing (must be >= 32)\n", c);
			return 0;
		}
		// if char is greater than 126
		else if (c > 126)
		{
			fprintf(stderr, "The character with value %d is not an allowable character for printing (must be <= 126)\n", c);
			return 0;
		}
		// escape characters sub if chain
		else if (c == 92)
		{
			unsigned char c2 = es[i+1];
			// if chain for c2
			// \a
			if (c2 == 97)
			{
				s[j] = 7;
			}
			// \b
			else if (c2 == 98)
			{
				s[j] = 8;
			}
			// \e
			else if (c2 == 101)
			{
				s[j] = 27;
			}
			// \f
			else if (c2 == 102)
			{
				s[j] = 12;
			}
			// \n
			else if (c2 == 110)
			{
				s[j] = 10;
			}
			// \r
			else if (c2 == 114)
			{
				s[j] = 13;
			}
			// \t
			else if (c2 == 116)
			{
				s[j] = 9;
			}
			// \v
			else if (c2 == 118)
			{
				s[j] = 11;
			}
			// /* \\ */
			else if (c2 == 92)
			{
				s[j] = 92;
			}
			// \'
			else if (c2 == 39)
			{
				s[j] = 39;
			}
			// \"
			else if (c2 == 34)
			{
				s[j] = 34;
			}
			// \0
			else if (c2 == 48)
			{
				// this could be hex digit stuff
				// \0x
				if (es[i+2] != 120)
				{
					fprintf(stderr, "invalid attempt at using hexadecimal replacement, please format as such:\n\t\\0xHH where HH is a valid hexdigit combo (0-9a-fA-F)\n");
					return 0;
				}
				// valid hexdigits are 48-57, 65-70, and 97-102
				unsigned char digit1 = es[i+3];
				unsigned char digit2 = es[i+4];
				if (	 ((digit1 >= 48 && digit1 <= 57) || (digit1 >= 65 && digit1 <= 70) || (digit1 >= 97 && digit1 <= 102))
					 && ((digit2 >= 48 && digit2 <= 57) || (digit2 >= 65 && digit2 <= 70) || (digit2 >= 97 && digit2 <= 102))	 )
				{
					// convert to decimal, these can be outside the printable range
					unsigned char hex[3] = {es[i+3], es[i+4], 0};
					int dec = strtol((char *)hex, NULL, 16) & 0x000000ff;
					s[j] = dec;
				}
				else
				{
					fprintf(stderr, "invalid hexdigits given, valid hexdigits are 0-9, a-f, and A-F, given digits were: %c and %c\n", digit1, digit2);
					return 0;
				}
				
				// end with adding three extra to i for the two hex digits and x
				i += 3;
			}
			// invalid escape char 
			else
			{
				fprintf(stderr, "invalid escape character: %c, if you meant to just print \\ type \\\\\n", c2);
				return 0;
			}
			// end with moving forward in string positions
			i++;
			j++;
		}
		// it is a valid non escaped char
		else
		{
			if (c == 34) ;
			else
			{
				s[j] = c;
				j++;
			}
		}

		i++;
	}


	// after decode checks
	
	// is it less than 255 (not including \0)
	if (strlen((char *)s) > 255)
	{
		fprintf(stderr, "Strings must not exceed 255 chars (not including the null terminator), this string was %ld chars\n", strlen((char *)s));
		return 0;
	}
	// does it end with " \0
	if (es[i-2] != 34 || es[i-1] != 0)
	{
		fprintf(stderr, "Strings must end with ASCII codes 34 0, this one ended with %d %d\n", es[i-2], es[i-1]);
		return 0;
	}

	// if it made it all the way down here then everything should be all good
	return 1;
}

int string_encode(const unsigned char *s, unsigned char *es)
{
	// now that everything that comes through is data we already produced, errors shouldn't be possible
	int i = 0;
	int j = 0;
	unsigned char c = s[0];

	es[j] = 34;
	j++;
	
	// edge case for the empty string: need to check this if before entering the loop	
	// 	if it is "" won't enter the while loop and a closing quote and null won't be added
	if (c == 0)
	{
		es[j] = 34;
		j++;
		es[j] = 0;
		j++;
	}

	while(c)
	{
		c = s[i];
		
		// the end of the string
		if (c == 0)
		{
			es[j] = 34;
			j++;
			es[j] = 0;
			j++;
		}
		// \\    //
		else if (c == 92)
		{
			es[j] = 92;
			j++;
			es[j] = 92;
		}
		// \"
		else if (c == 34)
		{
			es[j] = 92;
			j++;
			es[j] = 34;
		}
		// \'
		else if (c == 39)
		{
			es[j] = 92;
			j++;
			es[j] = 39;
		}
		// \a
		else if (c == 7)
		{
			es[j] = 92;
			j++;
			es[j] = 97;
		}
		// \b
		else if (c == 8)
		{
			es[j] = 92;
			j++;
			es[j] = 98;
		}
		// \e
		else if (c == 27)
		{
			es[j] = 92;
			j++;
			es[j] = 101;
		}
		// \f
		else if (c == 12)
		{
			es[j] = 92;
			j++;
			es[j] = 102;
		}
		// \n
		else if (c == 10)
		{
			es[j] = 92;
			j++;
			es[j] = 110;
		}
		// \r
		else if (c == 13)
		{
			es[j] = 92;
			j++;
			es[j] = 114;
		}
		// \t
		else if (c == 9)
		{
			es[j] = 92;
			j++;
			es[j] = 116;
		}
		// \v
		else if (c == 11)
		{
			es[j] = 92;
			j++;
			es[j] = 118;
		}
		// \0xHH
		else if (c > 126 || c < 32)
		{
			es[j] = 92;
			j++;
			es[j] = 48; // 0
			j++;
			es[j] = 120;// x
			j++;
			// we need to make two hexdigits out of the value 
			unsigned char hex[3];
			sprintf((char *)hex, "%.2x", c);
			//printf("%s\n", hex);
			es[j] = hex[0];
			j++;
			es[j] = hex[1];
		}
		// it is a printable non special character
		else es[j] = c;

		// increment place in the strings
		i++;
		j++;
	}

	return 1;
}
