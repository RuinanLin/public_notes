#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
	int id;
	int set;
	char *name;
	char *email;
};

struct Database {
	int num_rows;
	int data_size;
	struct Address *rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	Database_close(conn);
	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n",
			addr->id, addr->name, addr->email);
}

void Database_alloc(struct Connection *conn, int num_rows, int data_size) {
	if (!conn || !conn->db) die("conn->db should be allocated before calling Database_alloc", conn);

	conn->db->num_rows = num_rows;
	conn->db->data_size = data_size;
	conn->db->rows = malloc(num_rows * sizeof(struct Address));
	if (!conn->db->rows) die("Memory error", conn);

	for (int i = 0; i < num_rows; i++) {
		conn->db->rows[i].name = malloc(data_size * sizeof(char));
		if (!conn->db->rows[i].name) die("Memory error", conn);

		conn->db->rows[i].email = malloc(data_size * sizeof(char));
		if (!conn->db->rows[i].email) die("Memory error", conn);
	}
}

void Database_load(struct Connection *conn)
{
	int num_rows = 0;
	int rc = fread(&num_rows, sizeof(int), 1, conn->file);
	if (rc != 1) die("Failed to load database num_rows.", conn);
	if (num_rows < 0) die("Invalid database format: negative num_rows.", conn);
	
	int data_size = 0;
	rc = fread(&data_size, sizeof(int), 1, conn->file);
	if (rc != 1) die("Failed to load database data_size.", conn);
	if (data_size < 0) die("Invalid database format: negative data_size.", conn);

	Database_alloc(conn, num_rows, data_size);

	for (int i = 0; i < conn->db->num_rows; i++) {
		rc = fread(&conn->db->rows[i].id, sizeof(int), 1, conn->file); if (rc != 1) die("Failed to load row id.", conn);
		rc = fread(&conn->db->rows[i].set, sizeof(int), 1, conn->file); if (rc != 1) die("Failed to load row set.", conn);
		rc = fread(conn->db->rows[i].name, conn->db->data_size * sizeof(char), 1, conn->file); if (rc != 1) die("Failed to load row name.", conn);
		rc = fread(conn->db->rows[i].email, conn->db->data_size * sizeof(char), 1, conn->file); if (rc != 1) die("Failed to load row email.", conn);
	}
}

struct Connection *Database_open(const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die("Memory error", conn);

	conn->db = malloc(sizeof(struct Database));
	if(!conn->db) die("Memory error", conn);

	if(mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");

		if(conn->file) {
				Database_load(conn);
		}
	}

	if(!conn->file) die("Failed to open the file", conn);

	return conn;
}

void Database_close(struct Connection *conn)
{
	if(conn) {
		if(conn->file) fclose(conn->file);

		if (conn->db) {
			if (conn->db->rows) {
				for (int i = 0; i < conn->db->num_rows; i++) {
					if (conn->db->rows[i].name) free(conn->db->rows[i].name);
					if (conn->db->rows[i].email) free(conn->db->rows[i].email);
				}
				free(conn->db->rows);
			}
			free(conn->db);
		}

		free(conn);
	}
}

void Database_write(struct Connection *conn)
{
	rewind(conn->file);

	int rc = fwrite(&conn->db->num_rows, sizeof(int), 1, conn->file);
	if (rc != 1) die("Failed to write num_rows.", conn);

	rc = fwrite(&conn->db->data_size, sizeof(int), 1, conn->file);
	if (rc != 1) die("Failed to write data_size.", conn);

	for (int i = 0; i < conn->db->num_rows; i++) {
		rc = fwrite(&conn->db->rows[i].id, sizeof(int), 1, conn->file);
		if (rc != 1) die("Failed to write id.", conn);

		rc = fwrite(&conn->db->rows[i].set, sizeof(int), 1, conn->file);
		if (rc != 1) die("Failed to write set.", conn);

		rc = fwrite(conn->db->rows[i].name, conn->db->data_size * sizeof(char), 1, conn->file);
		if (rc != 1) die("Failed to write name.", conn);

		rc = fwrite(conn->db->rows[i].email, conn->db->data_size * sizeof(char), 1, conn->file);
		if (rc != 1) die("Failed to write email.", conn);
	}
	
	rc = fflush(conn->file);
	if(rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn, int num_rows, int data_size)
{
	Database_alloc(conn, num_rows, data_size);
	for (int i = 0; i < num_rows; i++) {
		conn->db->rows[i].id = i;
		conn->db->rows[i].set = 0;
		memset(conn->db->rows[i].name, 0, data_size * sizeof(char));
		memset(conn->db->rows[i].email, 0, data_size * sizeof(char));
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die("Already set, delete it first", conn);

	addr->set = 1;
	char *res = strncpy(addr->name, name, conn->db->data_size);
	addr->name[conn->db->data_size-1] = '\0';
	if(!res) die("Name copy failed", conn);

	res = strncpy(addr->email, email, conn->db->data_size);
	addr->email[conn->db->data_size-1] = '\0';
	if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];

	if(addr->set) {
		Address_print(addr);
	} else {
		die("ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id)
{
	conn->db->rows[id].id = id;
	conn->db->rows[id].set = 0;
	memset(conn->db->rows[id].name, 0, conn->db->data_size * sizeof(char));
	memset(conn->db->rows[id].email, 0, conn->db->data_size * sizeof(char));
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;

	for (i = 0; i < db->num_rows; i++) {
		struct Address *cur = &db->rows[i];

		if (cur->set) {
			Address_print(cur);
		}
	}
}

void Database_find(struct Connection *conn, char *name) {
	int found = 0;

	for (int i = 0; i < conn->db->num_rows; i++) {
		struct Address *addr = &conn->db->rows[i];
		if (addr->set && strcmp(addr->name, name) == 0) {
			Address_print(addr);
			found = 1;
		}
	}

	if (!found) {
		printf("No record found for %s\n", name);
	}
}

int main(int argc, char *argv[])
{
	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);

	if (action == 'c') {
		if (argc != 5) die("Need num_rows, data_size to create", conn);
		int num_rows = atoi(argv[3]);
		int data_size = atoi(argv[4]);

		Database_create(conn, num_rows, data_size);
		Database_write(conn);
	} else if (action == 'f') {
		if (argc != 4) die("Need name string for searching", conn);
		char *name = argv[3];

		Database_find(conn, name);
	} else {
		int id = 0;

		if(argc > 3) id = atoi(argv[3]);
		if(id >= conn->db->num_rows) die("There's not that many records.", conn);

		switch(action) {
			case 'g':
				if(argc != 4) die("Need an id to get", conn);

				Database_get(conn, id);
				break;

			case 's':
				if(argc != 6) die("Need id, name, email to set", conn);

				Database_set(conn, id, argv[4], argv[5]);
				Database_write(conn);
				break;

			case 'd':
				if(argc != 4) die("Need id to delete", conn);

				Database_delete(conn, id);
				Database_write(conn);
				break;

			case 'l':
				Database_list(conn);
				break;
			default:
				die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
		}
	}

	Database_close(conn);

	return 0;
}
