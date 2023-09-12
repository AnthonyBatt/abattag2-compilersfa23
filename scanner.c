/*
 * Anthony Battaglia
 * Compilers: Scanner
 * September 11, 2023
 *
 * Escape char:
 * 	\
 * 	anything that comes after this changes it meaning
 *
 * Whitespace:
 * 	tabs, spaces, newlines, and carriage returns (\r) are all white space and not significant
 *
 * 	regex: [ \t\n\r]
 * 	TOKEN_WHITESPACE
 *
 * Comments:
 * 	both C and C++ style comments are supported
 *
 * 	regex:
 * 	TOKEN_COMMENT
 * 
 * Identifiers:
 * 	must start with letter or underscore
 * 	may contain letters, numbers, or underscores
 *
 * 	regex: [a-z_][a-z0-9_]				TODO THERE IS A MAX LENGTH 255
 *		TOKEN_IDENT
 *
 * Keywords:
 * 	array
 * 	auto
 * 	boolean		(type)
 * 	char			(type)
 * 	else
 * 	false
 * 	float			(type)
 * 	for
 * 	function
 * 	if
 * 	integer		(type)
 * 	print
 * 	return
 * 	string		(type)
 * 	true
 * 	void
 * 	while
 *
 * 	regex: just the word
 * 	TOKEN_{word name}
 *
 * Integers:
 * 	signed 64-bit values
 * 	can have leading - and +
 * 		ex: +123 and -12 are both valid
 *
 * 	regex: (+|-)?[1-9][0-9]*			TODO THERE ARE BOUNDARIES
 * 	TOKEN_INTEGER_LITERAL
 *
 * Floats:
 * 	1 bit of sign
 * 	11 bits of exponent
 * 	53 bits of significand (52 explicitly stored)
 *		12.34 and 1.234E1 and 123.4e-1 are all the same
 *		.11 is valid, but 11. is not
 *		TODO ask if the +- thing is for this too
 *
 * 	regex: 
 * 	TOKEN_FLOAT_LITERAL
 *
 * Booleans:
 * 	can be either true or false
 *
 * 	regex: true | false
 * 	TOKEN_BOOL_LITERAL
 *
 * Chars:
 * 	8-bit ASCII characters
 * 	TODO ask if the empty char is allowed
 *
 * 	regex: '[ -~]' apostrophe 32 (space) through 126 (~) 		TODO make it ? if empty char allowed
 * 	TOKEN_CHARACTER_LITERAL
 *
 * Strings:
 * 	double-quoted constant strings that are null terminated and cannot be modified
 *
 * 	regex: "[ -~]?" then send to the encoder to test if it is good or not
 * 	TOKEN_STRING_LITERAL
 *
 * Arrays:
 * 	global arrays of a fixed size
 * 	local arrays of a variable size
 * 	if declared with no value, initialize to zero
 * 	the entire array can also be given specific values
 *
 * 	regex:
 * 	TOKEN_ARRAY
 *
 * Expressions:
 * 	() 	-	grouping							regex:
 * 	[] 	- 	array subscript				regex:
 * 	f() 	- 	function call					regex:
 * 	++		- 	postfix increment				regex:
 * 	-- 	- 	postifix decrement			regex:
 * 	-		- 	unary negation					regex:
 * 	!		- 	logical not						regex:
 * 	^		- 	exponentiation					regex:
 * 	*		- 	multiplication					regex:
 *		/ 		- 	division							regex:
 *		%		- 	modulus							regex:
 *		+		- 	addition							regex:
 *		-		- 	subtraction						regex:
 *		<		- 	less than						regex:
 *		<= 	-	less than or equal to		regex:
 *		>		-	greater than					regex:
 *		>=		- 	greater than or equal to	regex:
 *		==		- 	equal to							regex:
 *		!=		- 	not equal to					regex:
 *		&&		- 	logical and						regex:
 *		||		- 	logical or						regex:
 *		=		- 	assignment						regex:
 *		TOKEN_{expression name}
 *
 *	B-minor is strictly typed
 *		you may only assign a variable, pass it to a function, or compare if the types match exactly
 *
 *	Declarations and Statements:
 *		function definitions may not be nested
 *		statements end with a semicolon ;
 *		fucntions, ifs, and loops are contained within {}
 *		print is not a function, but rather a statement
 */
