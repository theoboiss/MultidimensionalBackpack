#include "../../include/structures/instanceDB.h"


int InstanceDB_init(InstanceDB *instanceDB)
{
	if (!instanceDB) return 1;
	instanceDB->nbInstance = 0;
	instanceDB->listInstance = NULL;
	return 0;
}

InstanceDB* InstanceDB_new(void)
{
	InstanceDB *instanceDB = (InstanceDB*) malloc(sizeof(InstanceDB));
	if (instanceDB) 
	{
		if (InstanceDB_init(instanceDB))
		{
			free(instanceDB);
			instanceDB = NULL;
		}
	}
	return instanceDB;
}

void InstanceDB_del(InstanceDB *instanceDB)
{
	for (int i = 0; i < instanceDB->nbInstance; i++)
	{
		Instance_del(instanceDB->listInstance[i]);
	}
	free(instanceDB->listInstance);
  	free(instanceDB);
}

///////////////////////////////////////////////////////////

/**
 * Return InstanceDB with nbInstance and listInstance
 *
 * Create a new InstanceDB, read nbInstance in the data file and allocates listInstance
 */
InstanceDB* InstanceDB_generate(FILE *file)
{
	InstanceDB *DB = InstanceDB_new();
	fscanf(file, "%d", &DB->nbInstance);
	DB->listInstance = (Instance**) malloc((size_t) (DB->nbInstance)*sizeof(Instance*));
	return DB;
}

