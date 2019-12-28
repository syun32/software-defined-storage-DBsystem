#include <mysql.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <string.h>
#include <io.h>
#include "header_declare.h"

MYSQL *connection = NULL, conn;
MYSQL_RES   *result_of_sql;
MYSQL_ROW   row_in_sql;
MYSQL_FIELD *fields;

#define HOST_ADDRESS "127.0.0.1"
#define PORT_USE 3306


int CONNECTOR() {
	char user_id[100], user_pw[100], DB_FILE_NAME[100];

	LOGGING(user_id, user_pw, DB_FILE_NAME);
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, HOST_ADDRESS, user_id, user_pw, DB_FILE_NAME, PORT_USE, NULL, 0);

	if (connection == NULL) {
		fprintf(stderr, "Connection fail : %s", mysql_error(&conn));
		return 1;
	}
}

void CLOSER() {
	printf("Thanks. Goobye.");
	mysql_close(&conn);
}
void LOGGING(char id[], char pw[], char db_file_name[])
{
	printf("Enter a User ID : ");
	scanf("%s", id);
	printf("Enter a User PW: ");
	scanf("%s", pw);
	printf("Enter a User DB_FILE_NAME : ");
	scanf("%s", db_file_name);
}
void INTERFAACE_DISPLAY() {
	int query_stat, i, num_fields;
	query_stat = mysql_query(connection, "SELECT LRG_SML.L_S, SUM(IF(S.H_C = 'COLD' AND METADATA.ID = LRG_SML.ID, 1, 0)) AS COLD, SUM(IF(S.H_C = 'HOT' AND METADATA.ID = LRG_SML.ID, 1, 0)) AS HOT FROM METADATA, LRG_SML, SML_H_C S WHERE LRG_SML.ID = S.ID UNION SELECT LRG_SML.L_S, SUM(IF(L.H_C = 'COLD' AND METADATA.ID = LRG_SML.ID, 1, 0)) AS LARGE_COLD, SUM(IF(L.H_C = 'HOT' AND METADATA.ID = LRG_SML.ID, 1, 0)) AS LARGE_HOT FROM METADATA, LRG_SML, LRG_H_C L WHERE LRG_SML.ID = L.ID");
	result_of_sql = mysql_store_result(connection);
	fields = mysql_fetch_fields(result_of_sql);
	num_fields = mysql_num_fields(result_of_sql);
	fields = mysql_fetch_fields(result_of_sql);
	for (i = 0; i < num_fields; i++)
	{
		printf("|  %+1s\t", fields[i].name);
	}
	printf("|\n");
	while ((row_in_sql = mysql_fetch_row(result_of_sql)) != NULL)
	{
		printf("| %+3s\t| %+3s\t| %+3s\t|\n", row_in_sql[0], row_in_sql[1], row_in_sql[2]);

	}
}
void UPDATE() {
	int query_stat;
	char szQuery[100] = "\0";
	char size[10];
	query_stat = mysql_query(connection, "SELECT COUNT(ID) AS NUM FROM METADATA");
	result_of_sql = mysql_store_result(connection);
	row_in_sql = mysql_fetch_row(result_of_sql);
	strcpy(szQuery, "UPDATE METADATA SET SIZE=CEIL(RAND()*2000), U_DATE=NOW() WHERE ID = CEIL(RAND()*(");
	strcpy(size, row_in_sql[0]);
	strcat(size, "))");
	strcat(szQuery, size);
	query_stat = mysql_query(connection, szQuery);
	printf("update complete\n");
}
void METADATA_DISPLAY() {
	int query_stat, i, num_fields;
	query_stat = mysql_query(connection, "SELECT M.NAME,M.SIZE, S.H_C, M.U_DATE FROM METADATA M INNER JOIN SML_H_C S WHERE M.ID = S.ID");
	result_of_sql = mysql_store_result(connection);
	fields = mysql_fetch_fields(result_of_sql);
	num_fields = mysql_num_fields(result_of_sql);
	printf("\nDATABASE\n");
	for (i = 0; i < num_fields; i++)
	{
		printf("|%+10s\t ", fields[i].name);
	}
	printf("\t|\n");
	while ((row_in_sql = mysql_fetch_row(result_of_sql)) != NULL)
	{
		printf("|%+10s\t |%+10s\t |%+10s\t |%+10s\t|\n", row_in_sql[0], row_in_sql[1], row_in_sql[2], row_in_sql[3]);

	}
	query_stat = mysql_query(connection, "SELECT M.NAME, M.SIZE, L.H_C, M.U_DATE FROM METADATA M INNER JOIN LRG_H_C L WHERE M.ID = L.ID");
	result_of_sql = mysql_store_result(connection);
	fields = mysql_fetch_fields(result_of_sql);
	num_fields = mysql_num_fields(result_of_sql);
	printf("\nFILE SYSTEM\n");
	for (i = 0; i < num_fields; i++)
	{
		printf("|%+10s\t ", fields[i].name);
	}
	printf("\t|\n");
	while ((row_in_sql = mysql_fetch_row(result_of_sql)) != NULL)
	{
		printf("|%+10s\t |%+10s\t |%+10s\t |%+10s\t|\n", row_in_sql[0], row_in_sql[1], row_in_sql[2], row_in_sql[3]);

	}
}
void DATABASE_DISPLAY() {

	int query_stat, i, num_fields;
	query_stat = mysql_query(connection, "SELECT M.ID, M.NAME, D.DATA FROM METADATA M INNER JOIN DATA D WHERE M.ID=D.ID");
	result_of_sql = mysql_store_result(connection);
	fields = mysql_fetch_fields(result_of_sql);
	num_fields = mysql_num_fields(result_of_sql);
	printf("FILE IN DATABASE\n");
	for (i = 0; i < num_fields; i++)
	{
		printf("|%+10s\t", fields[i].name);
	}
	printf("|\n");
	while ((row_in_sql = mysql_fetch_row(result_of_sql)) != NULL)
	{
		printf("|%+10s\t |%+10s\t |%+10s\t|\n", row_in_sql[0], row_in_sql[1], row_in_sql[2]);

	}
}
void FILESYSTEM_DISPLAY() {
	int query_stat, i, num_fields;
	query_stat = mysql_query(connection, "SELECT M.NAME, M.SIZE, L.H_C, M.U_DATE FROM METADATA M INNER JOIN LRG_H_C L WHERE M.ID=L.ID");
	result_of_sql = mysql_store_result(connection);
	fields = mysql_fetch_fields(result_of_sql);
	num_fields = mysql_num_fields(result_of_sql);
	printf("FILE IN FILESYSTEM\n");
	for (i = 0; i < num_fields; i++)
	{
		printf("|%+10s\t", fields[i].name);
	}
	printf("|\n");
	while ((row_in_sql = mysql_fetch_row(result_of_sql)) != NULL)
	{
		printf("|%+10s\t |%+10s\t |%+10s\t |%+10s\t|\n", row_in_sql[0], row_in_sql[1], row_in_sql[2], row_in_sql[3]);

	}
}
int Repetitional_function()
{
	int Option;
	printf("\n\nOPTION\n");
	printf("1.UPDATE  2.UPLOAD  3.D_DATABASE  4.D_FILESYSTEM  5.D_METADATA  6.D_INTERFACE  7.QUIT\n");
	scanf("%d", &Option);
	printf("\n\n");
	switch (Option)
	{
	case 1: UPDATE(); return 0;
	case 2: UPLOAD(); return 0;
	case 3: DATABASE_DISPLAY(); return 0;
	case 4: FILESYSTEM_DISPLAY(); return 0;
	case 5: METADATA_DISPLAY(); return 0;
	case 6: INTERFAACE_DISPLAY(); return 0;
	case 7: return 1;
	}
	return 0;
}
void UPLOAD() {

	FILE *fp;
	struct _finddata_t fd;
	intptr_t handle;
	int result = 1;
	int i, j;
	int count = 0;
	char dir_path[100], f_dir[100][100];

	printf("Enter a Path of File Directory\n Directory Path is :");
	scanf("%s", &dir_path);

	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 100; j++)
		{
			f_dir[i][j] = dir_path[j];
		}
	}

	strcat(dir_path, "\\*.txt");

	for (i = 0; i < 100; i++)
	{
		strcat(f_dir[i], "\\");
	}

	handle = _findfirst(dir_path, &fd);
	if (handle == -1)
	{
		printf("There is no file. Check the directiory");
	}
	else {
		do {

			if (fd.size <= (1204 * 1000))
			{
				strcat(f_dir[count], fd.name);
				fp = fopen(f_dir[count], "rb");
				fseek(fp, 0, SEEK_END);
				long flen = ftell(fp);
				fseek(fp, 0, SEEK_SET);

				char* buffer = (char*)malloc(sizeof(char)*(flen + 1));

				fread(buffer, flen, 1, fp);

				int r = fclose(fp);
				INPUT_QUERY(buffer, flen, fd);
				free(buffer);
				count++;
			}

		} while (_findnext(handle, &fd) == 0);

		_findclose(handle);
	}
}


void INPUT_QUERY(char *buffer, long size, struct _finddata_t fd) {
	int query_stat;
	const char *command = "INSERT INTO METADATA SET ID='%s', NAME = '%s', SIZE='%d', U_DATE=NOW()";
	const char *command_1 = "INSERT INTO METADATA SET ID='%s', NAME = '%s', SIZE='%d', U_DATE=NOW()";
	size_t st_len = strlen(command);
	char *query = (char*)malloc(sizeof(char)*(st_len + strlen(fd.name) + 4));
	size_t q_size = sizeof(query);
	query_stat = mysql_query(connection, "select max(id)+1 from metadata");
	result_of_sql = mysql_store_result(connection);
	row_in_sql = mysql_fetch_row(result_of_sql);
	int len = sprintf(query, command_1, "1", fd.name, fd.size);
	query_stat = mysql_query(connection, query);
	len = sprintf(query, command, row_in_sql[0], fd.name, fd.size);
	query_stat = mysql_query(connection, query);

	char *chunk = (char*)malloc(sizeof(char)*(2 * size + 1));
	mysql_real_escape_string(connection, chunk, buffer, size);
	const char *command_2 = "UPDATE DATA SET DATA = '%s' WHERE ID=%s";
	st_len = strlen(command_2);
	query_stat = mysql_query(connection, "select max(id) from metadata");
	result_of_sql = mysql_store_result(connection);
	row_in_sql = mysql_fetch_row(result_of_sql);
	query = (char*)realloc(query, (st_len + 2 * size + 1));
	len = sprintf(query, command_2, chunk, row_in_sql[0]);
	query_stat = mysql_query(connection, query);
	free(chunk);
	free(query);
}
