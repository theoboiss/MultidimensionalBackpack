#define PARSER_H

#ifndef FILESYSTEM_H
#include "filesystem.h"
#endif

#include "structures/allDataBase.h"

#include <stdio.h>
#include <stdlib.h>


InstanceDB* parse();

void Parser_giveObjectsToInstance(FILE *file, Instance *instance);

