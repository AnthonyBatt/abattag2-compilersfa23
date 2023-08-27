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
 *	strings/identifiers can be max 255 chars long (not including null terminator)
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
