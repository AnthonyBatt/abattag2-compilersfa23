#include "symbol.h"
#include "hash_table.h"

struct hashtable *ht_global;
int CURRENT_SCOPE = 0;


// push hashtable onto stack (function call) increase depth
void scope_enter(); 
// pop hashtable from stack (return statement or end of function) decrease depth
void scope_exit();
// returns depth of stack (let's us know what scope we are in) return depth
int scope_level();
// adds (name, pair) to current scope level 
void scope_bind(char *name, struct symbol *s);
// searches hashtables for exact match to name, if not found return null
struct symbol *scope_lookup(char *name);
// searches only the current scope level for name, if not found return null
struct symbol *scope_lookup_curr(char *name);
