// printing
#include <stdio.h>
// strlen
#include <string.h>
// strtol
#include <stdlib.h>

// decodes the string
int string_decode(const unsigned char *es, unsigned char *s);
// re encodes the decoded string
int string_encode(const unsigned char *s, unsigned char *es);
// encodes chars
int char_decode(const unsigned char *es);
// decodes chars
int char_encode(int c, unsigned char *es);
