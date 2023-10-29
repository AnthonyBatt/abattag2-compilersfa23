struct type *type_create(type_t kind, struct type *subtype, struct param_list *params, int length)
{
	struct type *t = malloc(sizeof(struct type));

	t->kind = kind;
	t->subtype = subtype;
	t->params = params;
	t->length = length;
	
	return t;
}
