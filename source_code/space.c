#include "space.h"

char *my_space(char *userID, char *temp)
{
    MYSQL *sql = mysql_init(NULL);
    strcpy(temp, "0");
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return temp;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return temp;
    }
    char *query = (char *)malloc(255 * sizeof(char));

    sprintf(query, "SELECT SpaceName, SpaceDescription, SpaceID FROM network_space where UserID = \"%s\" ", userID);
    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return temp;
    }
    else
    {
        MYSQL_RES *result;
        result = mysql_store_result(sql);
        if (result->row_count == 0)
        {
            // fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return temp;
        }
        else
        {
            MYSQL_ROW row;
            long k = result->row_count;
            printf("row count = %ld", k);
            if (k == 0)
            {
                mysql_free_result(result);
                mysql_close(sql);
                return temp;
            }
            else
            {
                printf("-------- Tra ve my space----------\n");
                strcpy(temp, userID);
                char s[10];
                strcat(temp, "*");
                sprintf(s, "%ld", k);
                strcat(temp, s);
                while ((row = mysql_fetch_row(result)) != NULL)
                {
                    int i = 0;
                    strcat(temp, "*");
                    strcat(temp, row[i]);
                    strcat(temp, "*");
                    strcat(temp, row[i + 1]);
                    strcat(temp, "*");
                    strcat(temp, row[i + 2]);
                    // printf("temp : %s\n", temp);
                }
            }
        }
        mysql_free_result(result);
    }

    mysql_close(sql);
    return temp;
}

char *shareSpace(char *userID, char *temp)
{
    MYSQL *sql = mysql_init(NULL);
    strcpy(temp, "0");
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return temp;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return temp;
    }
    char *query = (char *)malloc(200 * sizeof(char));
    char *query_check = (char *)malloc(200 * sizeof(char));
    sprintf(query, "SELECT SpaceID FROM share_space where UserID = \"%s\" and SpaceID not in (SELECT SpaceID FROM network_space where UserID = \"%s\" );", userID, userID);

    if (mysql_query(sql, query))
    {
        fprintf(stderr, "1: %s \n", mysql_error(sql));
        mysql_close(sql);
        return temp;
    }
    else
    {
        MYSQL_RES *result;
        MYSQL_RES *result1;
        result = mysql_store_result(sql);
        if (result == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return temp;
        }
        else
        {
            MYSQL_ROW row;
            MYSQL_ROW row1;
            long k = result->row_count;
            printf("row count = %ld\n", k);
            if (k == 0)
            {
                return temp;
            }
            else
            {
                printf("------- Tra ve list My Share Space --------\n");
                // strcpy(temp,user)
                char s[3];
                strcpy(temp, userID);
                strcat(temp, "*");
                sprintf(s, "%ld", k);
                strcat(temp, s);
                while ((row = mysql_fetch_row(result)) != NULL)
                {
                    int i = 0;
                    printf("[%d] : %s\n", i, row[i]);
                    sprintf(query_check, "SELECT SpaceID, SpaceName FROM network_space WHERE SpaceID = \"%s\"", row[i]);
                    mysql_query(sql, query_check);
                    result1 = mysql_store_result(sql);
                    while ((row1 = mysql_fetch_row(result1)) != NULL)
                    {
                        int j = 0;
                        // printf("SpaceID[%s]: %s\n",row1[j],row[j+1]);
                        printf("SpaceID: %s \t Space Name %s\n", row1[j], row1[j + 1]);
                        strcat(temp, "*");
                        strcat(temp, row1[j]);
                        strcat(temp, "*");
                        strcat(temp, row1[j + 1]);
                        printf("string : %s\n", temp);
                    }
                    k++;
                }
                return temp;
            }
        }
        mysql_free_result(result);
        mysql_free_result(result1);
    }

    mysql_close(sql);
    return temp;
}
int delete_space(char *spaceID)
{
    MYSQL *sql = mysql_init(NULL);
    int state = 0;
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return state;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {

        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    char *foldername = get_foldername_via_gid(sql, spaceID);
    char cmd[255];
    sprintf(cmd, "rm -rf %s", foldername);
    int k = system(cmd);
    if (k == 1)
    {
        return state;
    }
    else
    {
        char *query = (char *)malloc(255 * sizeof(char));
        sprintf(query, "DELETE FROM network_space WHERE SpaceID = \"%s\" ", spaceID);
        if (mysql_query(sql, query) == 1)
        {
            fprintf(stderr, "0 : %s\n", mysql_error(sql));
            mysql_close(sql);
            return state;
        }
        else
        {
            state = 1; 
        }
        free(query);
        mysql_close(sql);
        return state;
    }
}
int out_space(char *userID, char *spaceID)
{
    MYSQL *sql = mysql_init(NULL);
    int state = 0;
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return state;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {

        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    char *query = (char *)malloc(255 * sizeof(char));

    sprintf(query, "DELETE FROM share_space WHERE SpaceID = \"%s\" AND UserID = \"%s\"", spaceID, userID);
    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "0 : %s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    else
    {
        state = 1;
    }
    mysql_close(sql);
    return state;
}
int create_space(char *userID, char *spaceName, char *spaceDescrp)
{
    int state = 0;
    MYSQL *sql = mysql_init(NULL);
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return state;
    }
    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    char *query = (char *)malloc(255 * sizeof(char));
    char *query1 = (char *)malloc(255 * sizeof(char));
    char *query2 = (char *)malloc(255 * sizeof(char));
    char *query3 = (char *)malloc(255 * sizeof(char));

    sprintf(query, "INSERT INTO network_space(UserID, SpaceName, SpaceDescription) VALUES (\"%s\",\"%s\",\"%s\")", userID, spaceName, spaceDescrp);
    sprintf(query3, "SELECT * FROM network_space WHERE UserID = \"%s\" AND SpaceName = \"%s\" ", userID, spaceName);
    sprintf(query1, "SELECT * FROM network_space WHERE UserID = \"%s\" AND SpaceName = \"%s\" AND SpaceDescription = \"%s\"", userID, spaceName, spaceDescrp);

    if (mysql_query(sql, query3))
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        state = 0;
    }
    else
    {
        MYSQL_RES *result;
        result = mysql_store_result(sql);
        if (result->row_count != 0)
        {
            // printf("%s",result);
            state = 2;
            return state;
        }
        else
        {
            if (mysql_query(sql, query))
            {
                fprintf(stderr, "%s\n", mysql_error(sql));
                mysql_close(sql);
                return state;
            }
            else
            {
                mysql_query(sql, query1);
                MYSQL_RES *result;
                result = mysql_store_result(sql);
                if (result == NULL)
                {
                    fprintf(stderr, "%s\n", mysql_error(sql));
                    mysql_close(sql);
                    return state;
                }
                else
                {
                    MYSQL_ROW row;
                    int i = 0;
                    char spaceID[10];
                    while ((row = mysql_fetch_row(result)) != NULL)
                    {
                        printf("-------- tra ve create space----------\n");
                        printf("spaceID : %s\n", row[i]);
                        strcpy(spaceID, row[i]);
                        printf("spaceID : %s\n", spaceID);
                    }
                    sprintf(query2, "INSERT INTO share_space(UserID, SpaceID) VALUES (\"%s\",\"%s\")", userID, spaceID);
                    mysql_query(sql, query2);

                    char cmd[255];
                    strcpy(cmd, spaceName);
                    // create folder with spaceName
                    sprintf(cmd, "mkdir \"%s\"", spaceName);
                    int check = system(cmd);
                    if (!check)
                    {
                        char *query4 = (char *)malloc(255 * sizeof(char));
                        sprintf(query4, "INSERT INTO share_folder(FolderName,UserID,SpaceID) values(\"%s\",\"%s\",\"%s\")", spaceName, userID, spaceID);
                        mysql_query(sql, query4);
                        printf("add folder %s\n", spaceName);
                    }
                    else
                    {
                        printf("cannot create %s\n", spaceName);
                    }

                    state = 1;
                    return state;
                }
            }
        }
    }

    mysql_close(sql);
    return state;
}

int access_space(char *spaceID, char *userID)
{
    MYSQL *sql = mysql_init(NULL);
    int state = 0;
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return state;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {

        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    char *query = (char *)malloc(255 * sizeof(char));
    sprintf(query, "SELECT * FROM network_space where SpaceID = \"%s\" and UserID = \"%s\" ", spaceID, userID);
    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    else
    {
        MYSQL_RES *result;
        result = mysql_store_result(sql);
        if (result == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return state;
        }
        else
        {
            MYSQL_ROW row;
            char temp[255] = "";
            printf("-------- Tra ve access space----------\n");
            // strcpy(temp, userID);
            while ((row = mysql_fetch_row(result)) != NULL)
            {
                int i = 0;
                printf("Space : %s %s %s %s\n", row[i], row[i + 1], row[i + 2], row[i + 3]);
                strcpy(temp, row[i]);
            }
            printf("check temp : %s\n", temp);
            if (strcmp(temp, spaceID) == 0)
            {
                state = 1;
            }
        }
        mysql_free_result(result);
    }
    mysql_close(sql);
    return state;
}

int access_share_space(char *spaceID, char *userID)
{
    MYSQL *sql = mysql_init(NULL);
    int state = 0;
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return state;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {

        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    printf("check spaceid : %s\n", spaceID);
    printf("check userid : %s\n", userID);
    char *query = (char *)malloc(255 * sizeof(char));
    sprintf(query, "SELECT * FROM share_space where SpaceID = \"%s\" and UserID = \"%s\" ", spaceID, userID);
    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    else
    {
        MYSQL_RES *result;
        result = mysql_store_result(sql);
        if (result == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return state;
        }
        else
        {
            MYSQL_ROW row;
            if (result->row_count != 0)
            {
                printf("check row count : %ld\n", result->row_count);
            }
            char temp[255] = "";
            printf("-------- Tra ve access share space----------\n");
            // strcpy(temp, userID);
            while ((row = mysql_fetch_row(result)) != NULL)
            {
                int i = 0;
                printf("Space : %s %s\n", row[i], row[i + 1]);
                strcpy(temp, row[i + 1]);
            }
            printf("check temp : %s\n", temp);
            if (strcmp(temp, spaceID) == 0)
            {
                state = 1;
            }
        }
        mysql_free_result(result);
    }
    mysql_close(sql);
    printf("check state : %d\n", state);
    return state;
}