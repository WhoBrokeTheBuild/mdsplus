
#include <mdsconfig.h>
#include <mdsdescrip.h>
#include <mdsshr.h>

#include <linux/limits.h>

#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Connection
{
    int id;
    SQLHENV env;
    SQLHDBC dbc;
};

#define MAX_CONNECTIONS 64
int _NEXT_CONNECTION_ID = 0;
struct Connection _CONNECTION_LIST[MAX_CONNECTIONS];

void extract_error(
    char *fn,
    SQLHANDLE handle,
    SQLSMALLINT type)
{
    SQLINTEGER   i = 0;
    SQLINTEGER   native;
    SQLCHAR      state[ 7 ];
    SQLCHAR      text[256];
    SQLSMALLINT  len;
    SQLRETURN    ret;

    fprintf(stderr,
            "\n"
            "The driver reported the following diagnostics whilst running "
            "%s\n\n",
            fn);

    do
    {
        ret = SQLGetDiagRec(type, handle, ++i, state, &native, text,
                            sizeof(text), &len );
        if (SQL_SUCCEEDED(ret))
            printf("%s:%d:%d:%s\n", state, i, native, text);
    }
    while( ret == SQL_SUCCESS );
}

EXPORT int list()
{
    SQLHENV env;
    unsigned char dsn[256];
    unsigned char desc[256];
    SQLSMALLINT dsn_ret;
    SQLSMALLINT desc_ret;
    SQLUSMALLINT direction;
    SQLRETURN ret;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

    direction = SQL_FETCH_FIRST;

    ret = SQLDataSources(env, direction, dsn, sizeof(dsn), &dsn_ret, desc, sizeof(desc), &desc_ret);
    while (SQL_SUCCEEDED(ret)) {
        direction = SQL_FETCH_NEXT;
        printf("%s - %s\n", dsn, desc);
        if (ret == SQL_SUCCESS_WITH_INFO) {
            printf("\tdata truncation\n");
        }

        ret = SQLDataSources(env, direction, dsn, sizeof(dsn), &dsn_ret, desc, sizeof(desc), &desc_ret);
    }

    SQLFreeHandle(SQL_HANDLE_ENV, env);

    return 1;
}

EXPORT int connect(const char * name)
{
    struct Connection conn = {
        .env = NULL,
        .dbc = NULL,
    };

    SQLRETURN ret;
    SQLCHAR outstr[1024];
    SQLSMALLINT outstrlen;

    char filename[PATH_MAX]; // TODO: Improve
    snprintf(filename, sizeof(filename), "%s/%s.sybase_login", getenv("HOME"), name);
    FILE * sybase_file = fopen(filename, "rt");

    // TODO: Completely rewrite this trash

    size_t length;
    unsigned char * mdshost = NULL;
    unsigned char * server = NULL;
    unsigned char * username = NULL;
    unsigned char * password = NULL;
    unsigned char * database = NULL;

    ssize_t bytes_read = 0;

    bytes_read = getline((char **)&mdshost, &length, sybase_file);

    length = 0;
    bytes_read = getline((char **)&server, &length, sybase_file);
    server[bytes_read - 1] = '\0';

    length = 0;
    bytes_read = getline((char **)&database, &length, sybase_file);
    database[bytes_read - 1] = '\0';

    length = 0;
    bytes_read = getline((char **)&username, &length, sybase_file);
    username[bytes_read - 1] = '\0';

    length = 0;
    bytes_read = getline((char **)&password, &length, sybase_file);
    password[bytes_read - 1] = '\0';

    // printf("%s\n%s\n%s\n%s\n", server, database, username, password);

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &conn.env);
    SQLSetEnvAttr(conn.env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, conn.env, &conn.dbc);

    unsigned char connstr[1024];
    snprintf((char *)connstr, sizeof(connstr), "DRIVER={ODBC Driver 18 for SQL Server};SERVER=%s;UID=%s;PWD=%s;DATABASE=%s;TrustServerCertificate=yes;", server, username, password, database);

    // ret = SQLConnect(dbc, server, SQL_NTS, username, SQL_NTS, password, SQL_NTS);

    ret = SQLDriverConnect(
        conn.dbc, NULL,
        connstr, // (unsigned char *)"DSN=logbook;",
        SQL_NTS, // NULL-Terminated String
        outstr, sizeof(outstr),
        NULL, SQL_DRIVER_NOPROMPT
    );

    if (SQL_SUCCEEDED(ret)) {
        if (ret == SQL_SUCCESS_WITH_INFO) {
            extract_error("SQLDriverConnect", conn.dbc, SQL_HANDLE_DBC);
        }
    }
    else {
        printf("Failed to connect\n");
        extract_error("SQLDriverConnect", conn.dbc, SQL_HANDLE_DBC);

        goto error;
    }

    if (_NEXT_CONNECTION_ID == 0) {
        struct Connection empty = { -1, NULL, NULL };
        for (int i = 0; i < MAX_CONNECTIONS; ++i) {
            memcpy(_CONNECTION_LIST + i, &empty, sizeof(empty));
        }
    }

    conn.id = _NEXT_CONNECTION_ID;
    ++_NEXT_CONNECTION_ID;

    bool found = false;
    for (int i = 0; i < MAX_CONNECTIONS; ++i) {
        if (_CONNECTION_LIST[i].id == -1) {
            found = true;
            memcpy(_CONNECTION_LIST + i, &conn, sizeof(conn));
            break;
        }
    }

    if (!found) {
        printf("No empty connection found\n");
        goto error;
    }

    return conn.id;

error:

    SQLDisconnect(conn.dbc);

    SQLFreeHandle(SQL_HANDLE_DBC, conn.dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, conn.env);

    return -1;
}

EXPORT int disconnect(int * pindex)
{
    int index = *pindex;
    if (index >= _NEXT_CONNECTION_ID) {
        printf("Invalid connection ID %d >= %d\n", index, _NEXT_CONNECTION_ID);
        return 0;
    }

    struct Connection * conn = NULL;
    for (int i = 0; i < MAX_CONNECTIONS; ++i) {
        if (_CONNECTION_LIST[i].id == index) {
            conn = &_CONNECTION_LIST[i];
            break;
        }
    }

    if (conn) {
        SQLDisconnect(conn->dbc);

        SQLFreeHandle(SQL_HANDLE_DBC, conn->dbc);
        conn->dbc = NULL;

        SQLFreeHandle(SQL_HANDLE_ENV, conn->env);
        conn->env = NULL;

        conn->id = -1;
    }

    return 1;
}

EXPORT mdsdsc_t * query(int * pindex, const char * sql)
{
    int index = *pindex;

    struct Connection * conn = NULL;
    for (int i = 0; i < MAX_CONNECTIONS; ++i) {
        if (_CONNECTION_LIST[i].id == index) {
            conn = &_CONNECTION_LIST[i];
            break;
        }
    }

    if (!conn) {
        printf("No open connection\n");
        return 0;
    }

    SQLHSTMT stmt = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, conn->dbc, &stmt);

    // "SELECT * FROM entries WHERE shot=1090909010"
    SQLRETURN ret = SQLExecDirect(stmt, (unsigned char *)sql, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Query failed\n");
        return 0;
    }

    SQLSMALLINT columns = 0;
    SQLNumResultCols(stmt, &columns);

    array_coeff dsc = {};
    dsc.length = 1024;
    dsc.class = CLASS_A;
    dsc.dtype = DTYPE_T;
    dsc.pointer = calloc(columns, dsc.length);
    dsc.arsize = columns * dsc.length;
    dsc.aflags.coeff = 0;
    dsc.aflags.column = 0;
    dsc.dimct = 1;
    dsc.m[0] = columns;

    // while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
    if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        SQLLEN length;

        for (int i = 0; i < columns; ++i) {
            char * buffer = &dsc.pointer[i * dsc.length];
            memset(buffer, ' ', dsc.length);

            ret = SQLGetData(stmt, i + 1, SQL_C_CHAR, buffer, dsc.length, &length);
            if (SQL_SUCCEEDED(ret)) {
                if (length == SQL_NULL_DATA) {
                    strcpy(buffer, "NULL");
                    length = 4;
                }

                memset(buffer + length, ' ', dsc.length - length);
            }
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    static mdsdsc_xd_t xd = MDSDSC_XD_INITIALIZER;
    MdsCopyDxXd((mdsdsc_t *)&dsc, &xd);

    free(dsc.pointer);

    return (mdsdsc_t *)&xd;
}