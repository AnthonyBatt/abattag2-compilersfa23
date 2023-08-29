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
 * 			1 basic hello world
 * 			2 have a string of only numbers
 * 			3 have a string that mixes the nums and lets
 *				4 have an empty string
 *				5 have a string that is 255 chars long (not including the null)
 *				6 have a string with every printable char in it from 32 to 126
 *				7 have a string with all the non reserved escape chars
 *				8 have one with the reserved escape chars
 *				9 have one that proeperly does the byte thing
 *				10 write a couple sentences and use some numbers and escape characters
 * 			
 * 		bad:
 * 			1 no starting "
 * 			2 no ending "
 * 			3 doesn't terminate
 * 			4 char less than 32
 * 			5 char greater than 126
 *				6 invaid escape character
 *				7 string is more than 255 chars long
 *				8 chars before the starting "
 *				9 chars after the ending quote
 *				10 not escaping " as needed
 *				11 not escaping \ as needed
 *				12 improper use of the byte thing
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

int string_decode(const char *es, char *s);
int string_encode(const char *s, char *es);

int main(int argc, char *argv[])
{

	/*
	 *
	 * TODO maybe make all the file checking stuff its own function to unclutter main 
	 *
	 */

	// set the default to success
	int ret = 0;

	// if the call isn't to perform encoding let the user know nothing else is supported right now
	if ((strcmp(argv[1], "--encode")))
	{
		fprintf(stderr, "Currently no functionality other than --encode is supported\n");
		ret = 1;
		return EXIT_FAILURE;
	}

	// if the call is trying to encode and there was no file specified
	if (!(strcmp(argv[1], "--encode")) && (argc < 3))
	{
		fprintf(stderr, "Not enough command line arguments given, a file name must follow --encode\n");
		ret = 1;
		return EXIT_FAILURE;
	}

	// declare the buffer string will be placed into
	uint8_t *stringBUF = NULL;
	// the string we decode, it is 256 for 255 chars plus the null terminator
	char s[256];
	// the string that holds our attempt at re encoding
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
	
	// now that we know the size of the encoded string let's malloc buff and make es
	buffer = malloc(sizeof(char) * s.st_size);
	char es[s.st_size];

	// read the file and check if the system call failed
	if (read(fd, stringBUF, s.st_size * sizeof(char)) != s.st_size * sizeof(char));
	{
		fprintf(stderr, "read(%s): %s\n", argv[2], strerror(errno));
		ret = 1;
		goto failure;
	}

	// decode and if it fails print an appropiate error message
	// 	inside decode check if it surpasses the 255 char length
	if (!(string_decode(stringBUF, s)))
	{
		fprintf(stderr, "decode(%s): the string was unable to be decoded\n", stringBUF);
		ret = 1;
		goto failure;
	}
	
	// if decoding succeeded then re encode it and check if it fails
	if (!(string_encode(s, es)))
	{
		fprintf(stderr, "re encoding the decoded string failed\n");
		ret = 1;
		goto failure;
	}

	// if re encoding didn't fail, check that it was re encoded properly
	if (strcmp(stringBUF, es))
	{
		fprintf(stderr, "The re encoded string \n%s\n does not match the original \n%s\n", es, stringBUF);
		ret = 1;
		goto failure;
	}

	// if they are the same string then print out the re encoded string
	fprintf(stdout, "%s\n", es);

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

	// if at any point something failed ret will exit as non zero which will indicate failure
	// otherwise it will exit as zero which will indicate success
	return ret;
	
}

int string_decode(const char *es, char *s)
{
	
}

int string_encode(const char *s, char *es)
{

}
