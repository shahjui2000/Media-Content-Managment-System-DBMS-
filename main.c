#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <string.h>

int main()
{
	PGconn *conn;
	PGresult *res;
	int rec_count;
	int row;
	int col;
	int flag = 1;
	// Change your user, password and dbname accordingly
	// If you are using a local server define a port
	// If a server hosted on a network change the host field
	conn = PQconnectdb("host=10.100.71.21 user=username dbname=db password=password");
	if (PQstatus(conn) == CONNECTION_BAD)
	{
		puts("We were unable to connect to the database");
		exit(0);
	}
	char *string = (char *)malloc(1000 * sizeof(char));
	do
	{
		res = PQexec(conn, "set search_path to freelance;");
		printf("1. Enter query to execute\n2. Exit (Please press 2 twice to exit)\n");
		int type;
		scanf("%d\n", &type);
		fflush(stdout);
		if (type == 1)
		{
			scanf("%[^\n]s", string);
			res = PQexec(conn, string);
			if (PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				printf("%s", PQresultErrorMessage(res));
				continue;
			}
			rec_count = PQntuples(res);
			printf("We received %d records.\n", rec_count);
			puts("==========================");
			int col_count = PQnfields(res);
			for (col = 0; col < col_count; col++)
			{
				printf("%s\t", PQfname(res, col));
			}
			puts("");
			for (row = 0; row < rec_count; row++)
			{
				for (col = 0; col < col_count; col++)
				{
					printf("%s\t", PQgetvalue(res, row, col));
				}
				puts("");
			}
			puts("==========================");
		}
		else if (type == 2)
		{
			printf("Exited\n");
			PQclear(res);
			PQfinish(conn);
			flag = 2;
			exit(0);
		}
	} while (flag != 2);
	return 0;
}
