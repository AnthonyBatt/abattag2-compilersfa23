#include "scope.h"
struct hash_table *ht_global = 0;
int CURRENT_SCOPE = 0;

void scope_enter()
{
	// entering the global scope
	if (!ht_global)
	{
		ht_global = hash_table_create(0, NULL);
	}
	else
	{
		struct hash_table *curr;
		for (curr = ht_global; curr; curr = curr->next)
		{
			if (!curr->next)
			{
				CURRENT_SCOPE++;
				//printf("scope level: %d\n", CURRENT_SCOPE);
				struct hash_table *new_ht = hash_table_create(0, NULL);
				//printf("New HT's level: %d\n", new_ht->level);
				curr->next = new_ht;
				//printf("CurrNext's level: %d\n", curr->next->level);
				//printf("Curr's level: %d\n", curr->level);
				break;
			}
		}
	}
}

void scope_exit()
{
	// exiting the global scope
	if (!CURRENT_SCOPE)
	{
		//hash_table_delete(ht_global);
		ht_global = NULL;
	}
	else
	{
		struct hash_table *curr;
		for (curr = ht_global; curr->next; curr = curr->next)
		{
			if (!curr->next->next)
			{
				CURRENT_SCOPE--;
				//hash_table_delete(curr->next);
				//printf("CurrNext's level: %d\n", curr->next->level);
				curr->next = NULL;
				//printf("Curr's level: %d\n", curr->level);
				break;
			}
		}
	}
}

int scope_level()
{
	return CURRENT_SCOPE;
}

void scope_bind(char *name, struct symbol *s)
{
	struct hash_table *curr;
	for (curr = ht_global; curr; curr = curr->next)
	{
		if (!curr->next)
		{
			s->which = curr->vars;
			hash_table_insert(curr, (const char *)name, s);
			curr->vars++;
			break;
		}
	}
}

struct symbol *scope_lookup(char *name)
{
	struct symbol *sym = NULL;
	struct hash_table *curr;
	for (curr = ht_global; curr; curr = curr->next)
	{
		// if it is in a higher precedence scope then set it to that
		if (hash_table_lookup(curr, (const char *)name))
		{
			sym = hash_table_lookup(curr, (const char *)name);
		}
	}

	return sym;
}

struct symbol *scope_lookup_curr(char *name)
{
	struct symbol *sym = NULL;
	struct hash_table *curr;
	for (curr = ht_global; curr; curr = curr->next)
	{
		if (!curr->next)
		{
			sym = hash_table_lookup(curr, (const char *)name);
			/*if (sym) 
			{
				printf("I found sym\n");
				return sym;
			}
			else
			{
				printf("I can't find sym\n");
				return 0;
			}*/
		}
	}

	return sym;
}
