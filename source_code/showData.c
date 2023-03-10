#include "showData.h"

char *checkPermission(char *userID, char *spaceID)
{
    printf("userID: %s\n", userID);
    printf("spaceID: %s\n", spaceID);
    char *query = (char *)malloc(255 * sizeof(char));
    MYSQL *sql = mysql_init(NULL);
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return "0";
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return "0";
    }
    sprintf(query, "SELECT UserID FROM share_space where SpaceID = \"%s\" ", spaceID);

    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return "0";
    }

    MYSQL_RES *result;
    result = mysql_store_result(sql);
    if (result == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return "0";
    }
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        int i = 0;
        if (strcmp(row[i], userID) == 0)
        {
            return "1";
        }
    }
    return "0";
}

char *showProfile(char *userID, char *temp)
{
    strcpy(temp, "0");
    MYSQL *sql = mysql_init(NULL);

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

    sprintf(query, "SELECT UserID, UserName, Email FROM network_user where UserID = \"%s\" ", userID);
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
        printf("check row count : %ld\n", result->row_count);
        if (result->row_count == 0)
        {
            fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return temp;
        }
        else
        {
            MYSQL_ROW row;
            int i = 0;
            while ((row = mysql_fetch_row(result)) != NULL)
            {
                printf("-------- tra ve profile----------\n");
                printf("UserID : %s\n", row[i]);
                printf("UserName : %s\n", row[i + 1]);
                printf("Email : %s\n", row[i + 2]);
                strcpy(temp, row[i]);
                strcat(temp, "*");
                strcat(temp, row[i + 1]);
                strcat(temp, "*");
                strcat(temp, row[i + 2]);
                printf("temp : %s\n", temp);
            }
        }
        mysql_free_result(result);
    }

    mysql_close(sql);
    return temp;
}

char *showAllUser(char *temp)
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
    strcpy(query, "SELECT UserID, UserName, Email FROM network_user");
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
        printf("check row_count : %ld\n", result->row_count);
        if (result->row_count == 0)
        {
            fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return temp;
        }
        else
        {
            MYSQL_ROW row;

            printf("-------- Tra ve list user----------\n");
            long k = result->row_count;
            strcat(temp, "*");
            printf("temp : %s\n", temp);
            char s[2];
            sprintf(s, "%ld", k);
            strcat(temp, s);
            printf("temp : %s\n", temp);
            while ((row = mysql_fetch_row(result)) != NULL)
            {
                int i = 0;
                printf("[%d] : %s\n", i, row[i]);
                strcat(temp, "*");
                strcat(temp, row[i]);
                strcat(temp, "*");
                strcat(temp, row[i + 1]);
                strcat(temp, "*");
                strcat(temp, row[i + 2]);
            }
        }
        mysql_free_result(result);
    }
    mysql_close(sql);
    return temp;
}

char *listMember(char *userID, char *spaceID, char *temp)
{
    char *permission = (char *)malloc(10 * sizeof(char));
    strcpy(permission, "0");
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
    char *query1 = (char *)malloc(255 * sizeof(char));

    sprintf(query, "SELECT UserID FROM share_space where SpaceID = \"%s\" ", spaceID);

    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
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

            printf("-------- Tra ve list member----------\n");
            strcpy(temp, spaceID);
            long k = result->row_count;
            printf("row count = %ld\n", k);
            strcat(temp, "*");
            printf("temp : %s\n", temp);
            char s[2];
            sprintf(s, "%ld", k);
            strcat(temp, s);
            printf("temp : %s\n", temp);
            while ((row = mysql_fetch_row(result)) != NULL)
            {
                int i = 0;
                printf("[%d] : %s\n", i, row[i]);
                if (strcmp(row[i], userID) == 0)
                {
                    strcpy(permission, "1");
                }
                sprintf(query1, "SELECT UserID, UserName, Email FROM network_user WHERE UserID = \" %s\"", row[i]);
                // sprintf(query1, "SELECT SpaceID FROM share_space  where UserID = 12 and SpaceID not in (SELECT SpaceID FROM network_space  where UserID = 12)" );
                mysql_query(sql, query1);
                result1 = mysql_store_result(sql);
                while ((row1 = mysql_fetch_row(result1)) != NULL)
                {
                    int j = 0;
                    // printf("user[%d] : %s \t %s\n", j, row1[j] , row1[j+1]);
                    strcat(temp, "*");
                    strcat(temp, row1[j]);
                    strcat(temp, "*");
                    strcat(temp, row1[j + 1]);
                    strcat(temp, "*");
                    strcat(temp, row1[j + 2]);
                    // printf("string : %s\n", temp);
                }
                k++;
            }
        }
        mysql_free_result(result);
        mysql_free_result(result1);
    }

    mysql_close(sql);
    if (strcmp(permission, "0") == 0)
    {
        return permission;
    }
    return temp;
}

char *listFolder(char *userID, char *spaceID, char *temp)
{
    strcpy(temp, "0");
    MYSQL *sql = mysql_init(NULL);

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
    sprintf(query, "SELECT f.FolderName, u.UserName FROM share_folder as f, network_user as u where f.UserID = u.UserID AND SpaceID = \"%s\" ", spaceID);
    
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
        printf("check row_count : %ld\n", result->row_count);
        if (result->row_count == 0)
        {
            fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return temp;
        }
        else
        {
            MYSQL_ROW row;

            printf("-------- Tra ve list folder----------\n");
            strcpy(temp, spaceID);
            long k = result->row_count;
            strcat(temp, "*");
            printf("temp : %s\n", temp);
            char s[2];
            sprintf(s, "%ld", k);
            strcat(temp, s);
            printf("temp : %s\n", temp);
            while ((row = mysql_fetch_row(result)) != NULL)
            {
                int i = 0;
                printf("[%d] : %s\n", i, row[i]);
                strcat(temp, "*");
                strcat(temp, row[i]);
                strcat(temp, "*");
                strcat(temp, row[i + 1]);
            }
        }
        mysql_free_result(result);
    }

    mysql_close(sql);

    char *permission = checkPermission(userID, spaceID);
    if (strcmp(permission, "0") == 0)
    {
        return permission;
    }
    return temp;
}

char *listFile(char *userID, char *spaceID, char *temp)
{
    strcpy(temp, "0");
    char *permission = checkPermission(userID, spaceID);
    printf("permission: %s\n", permission);

    if (strcmp(permission, "0") == 0)
    {
        return permission;
    }

    MYSQL *sql = mysql_init(NULL);

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
    char *query1 = (char *)malloc(255 * sizeof(char));

    sprintf(query, "SELECT FolderName FROM share_folder where SpaceID = \"%s\" ", spaceID);

    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return temp;
    }
    else
    {
        MYSQL_RES *result;
        MYSQL_RES *result1;
        result = mysql_store_result(sql);
        printf("check result row_count : %ld\n", result->row_count);
        if (result->row_count == 0)
        {
            fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return temp;
        }
        else
        {
            MYSQL_ROW row;
            MYSQL_ROW row1;
            char buff[1024];
            printf("-------- Tra ve list file----------\n");
            while ((row = mysql_fetch_row(result)) != NULL)
            {
                int i = 0;

                strcpy(buff, row[i]);
            }
            strtok(buff, "/");
            printf("FolderName : %s\n", buff);
            char *folderid = get_folderid_via_foldername(sql, buff);
            sprintf(query1, "SELECT f.FileName, u.UserName FROM share_file as f, network_user as u where f.UserID = u.UserID AND FolderID LIKE \"%s\" ", folderid);
            if (mysql_query(sql, query1) == 1)
            {
                fprintf(stderr, "%s\n", mysql_error(sql));
                mysql_close(sql);
                return temp;
            }
            else
            {
                result1 = mysql_store_result(sql);
                strcpy(temp, spaceID);
                long k = result1->row_count;
                printf("check result1 -> row_count : %ld\n", result1->row_count);
                strcat(temp, "*");
                printf("temp : %s\n", temp);
                char s[2];
                sprintf(s, "%ld", k);
                strcat(temp, s);
                printf("temp : %s\n", temp);
                while ((row1 = mysql_fetch_row(result1)) != NULL)
                {
                    int j = 0;
                    strcat(temp, "*");
                    strcat(temp, row1[j]);
                    strcat(temp, "*");
                    strcat(temp, row1[j + 1]);
                    // printf("string : %s\n", temp);
                }
                printf("temp : %s\n", temp);
            }
        }
        mysql_free_result(result);
        mysql_free_result(result1);
    }

    mysql_close(sql);
    return temp;
}
