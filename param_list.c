struct param_list *param_list_create(char *name, struct type *type, struct param_list *next)
{
	struct param_list *p = malloc(sizeof(param_list));

	p->name = name;
	p->type = type;
	p->next = next;

	return p;
}
