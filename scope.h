#include "symbol.h"

// push hashtable onto stack (function call)
void scpe_enter(); 
// pop hashtable from stack (return statement or end of function)
void scope_exit();
// returns depth of stack (let's us know what scope we are in)
int scope_level();
// adds (name, pair) to to current scope level
void scope_bind(char *name, struct symbol *s);
// searches hashtables for exact match to name, if not found return null
struct symbol *scope_lookup(char *name);
// searches only the current scope level for name, if not found return null
struct symbol *scope_lookup_curr(char *name);
