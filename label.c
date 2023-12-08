#include "label.h"

int label_create()
{
	return label_count++;
}

const char *label_name(int label)
{
	char str[MAX_LABEL];
	sprintf(str, ".L%d", label);

	return strdup(str);
}
