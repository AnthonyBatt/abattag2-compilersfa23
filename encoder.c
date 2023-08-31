/* encoder
 *
 * Anthony Battaglia
 * Compilers
 * August 27, 2023
 *
 * Outline:
 * int string_decode(const char *es, const char *s); true if decoding worked, false if not
 * int string_encode(const char *s, const char *es); true if encoding worked, false if not
 *
 *	strings/identifiers can be max 255 chars long decoded (not including null terminator)
 *	for strings and chars they must fall in between 32 and 126
 *
 * decode:
 * 	the return should end with NULL
 * 		ASCII - 0
 * 	for the input string make sure to chop off the starting " and last "
 * 		in positions 0 and -2
 * 	if a backslash is read keep reading till no longer needed
 * 		use backtracking as needed if overread
 *
 * encode:
 * 	the return should begin with " and end with " NULL
 * 		ASCII - begin 34; end 34 0
 * 	if something is read with a corresponding escape character put in \ char in its place
 * 		ASCII - 92 char
 * 	Escapable chars and their corresponding decimal vals:
 * 		found on github site
 *
 * main:
 * 	have the executable be named bminor
 * 	have it use an --encode flag to read in the line of a file and feed it to decode
 * 		if valid have it use encode to convert it back, print it, and exit with success
 * 		else if not valid print detailed error message and exit with failure
 * 			for error message have it specify which rule of b minor strings it broke
 *
 * testing:
 * 	make test/encode/good[0-9].bminor and bad[0-9].bminor
 * 		good:
 * 			1  basic hello world
 * 			2  have a string of only numbers
 * 			3  have a string that mixes the nums and lets
 *				4  have an empty string
 *				5  have a string that is 255 chars long (not including the null)
 *				6  have a string with every printable char in it from 32 to 126
 *				7  have a string with all the non reserved escape chars
 *				8  have one with the reserved escape chars
 *				9  have one that proeperly does the byte thing (multiple)
 *				10 write a couple sentences and use some numbers and escape characters
 * 			
 * 		bad:
 * 			1  no starting "
 * 			2  no ending "
 * 			3  char less than 32
 * 			4  char greater than 126
 *				5  invaid escape character
 *				6  string is more than 255 chars long
 *				7  chars before the starting "
 *				8  chars after the ending quote
 *				9  not escaping " as needed
 *				10 not escaping \ as needed
 *				11 improper use of the byte thing
 * 	use run_test.sh to ensure exit codes are all good
 */

// printing
#include <stdio.h>
// system calls
#include <stdlib.h>
// NULL
#include <unistd.h>
// error messages
#include <errno.h>
// strcmp and error messages
#include <string.h>

// for the the stat command
#include <sys/stat.h>
// for system calls as well
#include <fcntl.h>


int string_decode(const char *es, char *s);
int string_encode(const char *s, char *es);

int main(int argc, char *argv[])
{

	/*
	 *
	 * TODO maybe make all the file checking stuff its own function to unclutter main 
	 *
	 */

	if (argc < 2)
	{
		fprintf(stderr, "Not enough command line arguments\n");
		return EXIT_FAILURE;
	}

	// if the call isn't to perform encoding let the user know nothing else is supported right now
	if ((strcmp(argv[1], "--encode")))
	{
		fprintf(stderr, "Currently no functionality other than --encode is supported\n");
		return EXIT_FAILURE;
	}

	// if the call is trying to encode and there was no file specified
	if (!(strcmp(argv[1], "--encode")) && (argc < 3))
	{
		fprintf(stderr, "Not enough command line arguments given, a file name must follow --encode\n");
		return EXIT_FAILURE;
	}

	// set the default to success
	int ret = 0;

	// declare the buffer string will be placed into
	char *stringBUF = NULL;
	// the string we decode, it is 256 for 255 chars plus the null terminator
	char ds[256];
	// the string that holds our attempt at re encoding
	char es[BUFSIZ];
	struct stat s;
	int fd = -1;

	// open the file and check if it failed
	fd = open(argv[2], O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "open(%s): %s\n", argv[2], strerror(errno));
		ret = 1;
		goto failure;
	}

	// stat the file and check if it failed
	if (stat(argv[2], &s) < 0)
	{
		fprintf(stderr, "stat(%s): %s\n", argv[2], strerror(errno));
		ret = 1;
		goto failure;
	}
	
	// now that we know the size of the encoded string let's calloc buff and make es
	stringBUF = calloc(s.st_size + 1, sizeof(char));

	// read the file and check if the system call failed
	if (read(fd, stringBUF, s.st_size * sizeof(char)) != s.st_size * sizeof(char))
	{
		fprintf(stderr, "read(%s): %s\n", argv[2], strerror(errno));
		ret = 1;
		goto failure;
	}

	// TODO ask about this at OH
	// for some reason whenever I read in the string from the file it keeps putting in a newline before the null
	stringBUF[s.st_size-1] = 0;

	// decode and if it fails print an appropiate error message
	if (!(string_decode(stringBUF, ds)))
	{
		fprintf(stderr, "decode(%s): the string was unable to be decoded\n", stringBUF);
		ret = 1;
		goto failure;
	}
	
	//print out the decoded string
	fprintf(stdout, "decoded string: >%s<\n", ds);

	// if decoding succeeded then re encode it and check if it fails (even thought it cant fail)
	if (!(string_encode(ds, es)))
	{
		fprintf(stderr, "re encoding the decoded string failed\n");
		ret = 1;
		goto failure;
	}


/* NOT NEEDED ANYMORE
	// if re encoding didn't fail, check that it was re encoded properly
	if (strcmp(stringBUF, es))
	{
		fprintf(stderr, "The re encoded string \n%s\n does not match the original \n%s\n", es, stringBUF);
		ret = 1;
		goto failure;
	}

	// if they are the same string then print out the re encoded string
	fprintf(stdout, "%s\n", es);
*/


	//print out the encoded string
	fprintf(stdout, "encoded string >%s<\n", es);

// goto label for failure so that files aren't left open
failure:
	// if a file was opened
	if (fd > -1)
	{
		// close the file and if that fails do the proper error procdeures
		if (close(fd) < 0)
		{
			fprintf(stderr, "close(%s): %s\n", argv[2], strerror(errno));
			ret = 1;
		}
	}

	if (stringBUF) free(stringBUF);

	// if at any point something failed ret will exit as non zero which will indicate failure
	// otherwise it will exit as zero which will indicate success
	return ret;
}

int string_decode(const char *es, char *s)
{
	int i = 0;
	int j = 0;
	char c = es[0];


	// go until you reach a null terminator or you exceed the char limit
	while(c && j < 256)
	{
		c = es[i];
		//printf("Now processing >%c< (%d)\n", c, c);

		// WOULDN'T HAPPEN, THE WHILE LOOP ENDS WHEN THIS HAPPENS
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
		// check for invalid apostrophe (unescaped) 
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
			char c2 = es[i+1];
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
				s[j] = 8;
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
			// TODO
			// whatever the hex thing is
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
				char digit1 = es[i+3];
				char digit2 = es[i+4];
				if (	 ((digit1 >= 48 && digit1 <= 57) || (digit1 >= 65 && digit1 <= 70) || (digit1 >= 97 && digit1 <= 102))
					 && ((digit2 >= 48 && digit2 <= 57) || (digit2 >= 65 && digit2 <= 70) || (digit2 >= 97 && digit2 <= 102))	 )
				{
					// convert to decimal, these can be outside the printable range
					char hex[3] = {es[i+3], es[i+4], 0};
					s[j] = strtol(hex, NULL, 16);
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
			// end with adding one extra to i
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

		//printf("%2d: %c (%d) || %2d: %c (%d)\n", i, c, c, j, s[j], s[j]);
		i++;
	}

	//printf("%d %d\nlen: %ld\n", es[i-2], es[i-1], strlen(s));

	// after decode checks
	// does it end with " \0
	if (es[i-2] != 34 || es[i-1] != 0)
	{
		fprintf(stderr, "Strings must end with ASCII codes 34 0, this one ended with %d %d\n", es[i-2], es[i-1]);
		return 0;
	}
	// is it less than 255 (not including \0)
	if (strlen(s) > 255)
	{
		fprintf(stderr, "Strings must not exceed 255 chars (not including the null terminator, this string was %ld chars\n", strlen(s));
		return 0;
	}

	return 1;
}

int string_encode(const char *s, char *es)
{
	return 0;
}
