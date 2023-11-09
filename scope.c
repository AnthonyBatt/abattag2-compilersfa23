#include "scope.h"

void scope_enter()
{
	// entering the global scope
	if (!CURRENT_SCOPE)
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
				curr->next = hash_table_create(0, NULL);
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
		hash_table_clear(ht_global);
		hash_table_delete(ht_global);
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
				hash_table_clear(curr->next);
				hash_table_delete(curr->next);
				curr->next = NULL;
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
			return sym;
		}
	}

	return sym;
}
