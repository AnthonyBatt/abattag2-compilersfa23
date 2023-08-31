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

#include "encoder.h"


int main(int argc, char *argv[])
{
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
	unsigned char *stringBUF = NULL;
	// the string we decode, it is 256 for 255 chars plus the null terminator
	unsigned char ds[256];
	// the string that holds our attempt at re encoding
	unsigned char es[BUFSIZ];

// READ THE FIRST LINE OF THE FILE
// ======================================================

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
	
	// now that we know the size of the encoded string calloc buff
	stringBUF = calloc(s.st_size + 1, sizeof(unsigned char));

	// read the file and check if the system call failed
	if (read(fd, stringBUF, s.st_size * sizeof(unsigned char)) != s.st_size * sizeof(unsigned char))
	{
		fprintf(stderr, "read(%s): %s\n", argv[2], strerror(errno));
		ret = 1;
		goto failure;
	}

	// for some reason whenever the string is read from the file it keeps putting in a newline before the null
	// this is just overwriting that random newline with a null
	stringBUF[s.st_size-1] = 0;

// ======================================================
// END READING THE FIRST LINE OF THE FILE

	// decode and if it fails print an appropiate error message
	if (!(string_decode(stringBUF, ds)))
	{
		fprintf(stderr, "decode(%s): the string was unable to be decoded\n", stringBUF);
		ret = 1;
		goto failure;
	}

	// Note that i'm using >< as delimeters for the string
	fprintf(stdout, "\nFor the purpose of being able to see tabs, spaces, etc. printed I'm delimitng the start and end of the printed strings with ><\n\n");

	//print out the decoded string
	fprintf(stdout, "decoded string: >%s<\n", ds);

	// if decoding succeeded then re encode it and check if it fails (even thought it cant fail)
	if (!(string_encode(ds, es)))
	{
		fprintf(stderr, "re encoding the decoded string failed\n");
		ret = 1;
		goto failure;
	}

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
