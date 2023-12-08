
#ifndef LABEL_H
#define LABEL_H

#include <stdio.h>
#include <string.h>

#define MAX_LABEL 100

int label_count = 0;

int label_create();
const char *label_name(int label);

#endif
