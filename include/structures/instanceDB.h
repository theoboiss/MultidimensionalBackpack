#define INSTANCEdb_H

#ifndef INSTANCE_H
#include "instance.h"
#endif

#include <stdio.h>
#include <stdlib.h>


typedef struct
{
	int nbInstance;
	Instance **listInstance;
} InstanceDB;


int InstanceDB_init(InstanceDB *instanceDB);

InstanceDB* InstanceDB_new(void);

void InstanceDB_del(InstanceDB *instanceDB);


InstanceDB* InstanceDB_generate(FILE *file);

