#include "user.h"

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int isSpaceAdmin(MYSQL *con, char *UserID, char *SpaceID)
{
    int permission = 0;
    con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    char *que = (char *)malloc(255 * sizeof(que));
    // check user in space or not
    sprintf(que, " SELECT * FROM network_space WHERE SpaceID LIKE \"%s\" AND UserID LIKE \"%s\" ", SpaceID, UserID);
    if (mysql_query(con, que))
    {
        finish_with_error(con);
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
    {
        finish_with_error(con);
    }
    MYSQL_ROW row;
    if (!(row = mysql_fetch_row(result)))
    {
        permission = 0;
    }
    else
    {
        permission = 1;
    }
    free(que);
    mysql_free_result(result);
    mysql_close(con);
    return permission;
}
char *get_userid_via_email(MYSQL *con, char *email)
{
    con = mysql_init(NULL);
    char *temp = (char *)malloc(255 * sizeof(temp));
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }
    char *ery = (char *)malloc(255 * sizeof(char));
    sprintf(ery, "SELECT UserID FROM network_user WHERE Email like \"%s\"", email);

    if (mysql_query(con, ery))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }
    MYSQL_ROW row;
    int i = 0;
    while ((row = mysql_fetch_row(result)))
    {
        strcpy(temp, row[i]);
    }
    mysql_close(con);
    free(ery);
    return temp;
}
char *get_spaceid_via_foldername(MYSQL *con, char *foldername)
{
    con = mysql_init(NULL);
    char *temp = (char *)malloc(255 * sizeof(temp));
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }
    char *ery = (char *)malloc(255 * sizeof(char));
    sprintf(ery, "SELECT SpaceID FROM share_folder WHERE FolderName like \"%s\"", foldername);

    if (mysql_query(con, ery))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }
    MYSQL_ROW row;
    int i = 0;
    while ((row = mysql_fetch_row(result)))
    {
        strcpy(temp, row[i]);
    }
    mysql_close(con);
    free(ery);
    return temp;
}
char *get_foldername_via_gid(MYSQL *con, char *spaceid)
{
    con = mysql_init(NULL);
    char *temp = (char *)malloc(255 * sizeof(temp));
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }
    char *ery = (char *)malloc(255 * sizeof(char));
    sprintf(ery, "SELECT SpaceName FROM network_space WHERE SpaceID like \"%s\"", spaceid);

    if (mysql_query(con, ery))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }
    MYSQL_ROW row;
    int i = 0;
    while ((row = mysql_fetch_row(result)))
    {
        strcpy(temp, row[i]);
    }
    free(ery);
    mysql_close(con);
    return temp;
}
char *get_folderid_via_foldername(MYSQL *con, char *foldername)
{
    con = mysql_init(NULL);
    char *temp = (char *)malloc(255 * sizeof(temp));
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }
    char *ery = (char *)malloc(255 * sizeof(char));
    sprintf(ery, "SELECT FolderID FROM share_folder WHERE FolderName like \"%s\"", foldername);

    if (mysql_query(con, ery))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }
    MYSQL_ROW row;
    int i = 0;
    while ((row = mysql_fetch_row(result)))
    {
        strcpy(temp, row[i]);
    }
    mysql_close(con);
    free(ery);
    return temp;
}
int check_user_access(MYSQL *con, char *UserID, char *foldername)
{
    int permission = 0;

    con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }
    char *GID = get_spaceid_via_foldername(con, foldername);
    char *que = (char *)malloc(SIZE * sizeof(que));
    // check user in space or not

    sprintf(que, "SELECT * FROM share_space WHERE UserID = \"%s\" and SpaceID = \"%s\"", UserID, GID);

    if (mysql_query(con, que))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }
    MYSQL_ROW row;
    if (!(row = mysql_fetch_row(result)))
    {
        permission = 0;
    }
    else
    {
        permission = 1;
    }
    free(que);
    mysql_free_result(result);
    mysql_close(con);
    return permission;
}
int check_folder_owner(MYSQL *con, char *UserID, char *foldername)
{
    int permission = 0;
    con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    char *que = (char *)malloc(255 * sizeof(que));
    // check user in space or not
    sprintf(que, "SELECT * FROM share_folder WHERE UserID LIKE \"%s\" FolderName like \"%s\")", UserID, foldername);
    if (mysql_query(con, que))
    {
        finish_with_error(con);
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
    {
        finish_with_error(con);
    }
    MYSQL_ROW row;
    if (!(row = mysql_fetch_row(result)))
    {
        permission = 0;
    }
    else
    {
        permission = 1;
    }
    free(que);
    mysql_free_result(result);
    mysql_close(con);
    return permission;
}
int check_file_owner(MYSQL *con, char *UserID, char *filename)
{
    int permission = 0;
    con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    char *que = (char *)malloc(255 * sizeof(que));
    // check user in space or not
    sprintf(que, "SELECT * FROM share_file WHERE UserID like \"%s\" AND FileName like \"%s\"", UserID, filename);

    if (mysql_query(con, que))
    {
        finish_with_error(con);
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
    {
        finish_with_error(con);
    }
    MYSQL_ROW row;
    if (!(row = mysql_fetch_row(result)))
    {
        permission = 0;
    }
    else
    {
        permission = 1;
    }
    free(que);
    mysql_free_result(result);
    mysql_close(con);
    return permission;
}
char *sign_in(char *email, char *password, char *temp)
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

    sprintf(query, "SELECT * FROM network_user where Email = \"%s\" AND Password = \"%s\" ", email, password);
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
        if (result == NULL)
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
                printf("----------tra ve sign in--------\n");
                printf("UserID : %s\n", row[i]);
                printf("UserName : %s\n", row[i + 1]);
                strcpy(temp, row[i]);
                strcat(temp, "*");
                strcat(temp, row[i + 1]);
            }
        }
        mysql_free_result(result);
    }
    free(query);
    mysql_close(sql);
    return temp;
}

int sign_up(char *username, char *email, char *password)
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
    char *query = (char *)malloc(200 * sizeof(char));
    char *query_check = (char *)malloc(200 * sizeof(char));
    sprintf(query, "INSERT INTO network_user(UserName, Email, Password) VALUES (\"%s\",\"%s\",\"%s\")", username, email, password);
    sprintf(query_check, "SELECT * FROM network_user where Email = \"%s\"", email);
    if (mysql_query(sql, query_check))
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
                state = 1;
                return state;
            }
        }
    }
    free(query);
    free(query_check);
    mysql_close(sql);
    return state;
}
char *get_spacename_via_spaceID(MYSQL *con, char *spaceID)
{

    con = mysql_init(NULL);
    char *temp = (char *)malloc(255 * sizeof(temp));
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }
    char *ery = (char *)malloc(255 * sizeof(char));
    sprintf(ery, "SELECT SpaceName FROM network_space WHERE SpaceID like \"%s\"", spaceID);

    if (mysql_query(con, ery))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }
    MYSQL_ROW row;
    int i = 0;
    while ((row = mysql_fetch_row(result)))
    {
        strcpy(temp, row[i]);
    }
    mysql_close(con);
    free(ery);
    return temp;
}
int delete_member(char *email, char *spaceID)
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
    char *UID = get_userid_via_email(sql, email);
    int check = isSpaceAdmin(sql, UID, spaceID);
    if (check == 1)
    {
        return state;
    }
    else
    {
        char *query = (char *)malloc(255 * sizeof(char));
        char *query1 = (char *)malloc(255 * sizeof(char));
        char *query2 = (char *)malloc(255 * sizeof(char));
        sprintf(query, "SELECT UserID FROM network_user where Email = \"%s\" ", email);
        if (mysql_query(sql, query) == 1)
        {
            fprintf(stderr, "0 : %s\n", mysql_error(sql));
            mysql_close(sql);
            return state;
        }
        else
        {
            MYSQL_RES *result;
            MYSQL_RES *result1;

            result = mysql_store_result(sql);
            long m1 = result->row_count;
            printf("check count result  : %ld\n", m1);
            // query dung nhung ko co data
            if (result->row_count == 0)
            {
                fprintf(stderr, "1 : %s\n", mysql_error(sql));
                mysql_close(sql);
                return state;
            }
            else
            {
                MYSQL_ROW row;
                int i = 0;
                while ((row = mysql_fetch_row(result)) != NULL)
                {
                    printf("----------tra ve member to delete--------\n");
                    printf("UserID : %s\n", row[i]);
                    printf("Space ID : %s\n", spaceID);
                    sprintf(query1, "SELECT * FROM share_space where SpaceID = \"%s\" AND UserID = \"%s\" ", spaceID, row[i]);
                    sprintf(query2, "DELETE FROM share_space WHERE UserID = \"%s\" AND SpaceID = \"%s\"", row[i], spaceID);
                    if (mysql_query(sql, query1))
                    {
                        fprintf(stderr, "2 : %s\n", mysql_error(sql));
                        mysql_close(sql);
                        mysql_free_result(result);
                        return state;
                    }
                    else
                    {
                        result1 = mysql_store_result(sql);
                        // query dung  co data
                        long m = result1->row_count;
                        printf("check count result 1 : %ld\n", m);
                        if (result1->row_count != 0)
                        {
                            printf("zo day 0 : \n");
                            if (mysql_query(sql, query2))
                            {
                                fprintf(stderr, "3 : %s\n", mysql_error(sql));
                                mysql_close(sql);
                                mysql_free_result(result);
                                mysql_free_result(result1);
                                return state;
                            }
                            else
                            {
                                state = 1;
                                // return state;
                            }
                        }
                        else
                        {
                            state = 2;
                        }

                        //return state;
                    }
                }
            }
            mysql_free_result(result);
            mysql_free_result(result1);
        }
        free(query1);
        free(query2);
        free(query);
    }
    mysql_close(sql);
    return state;
}

int add_member(char *email, char *spaceID)
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
    char *query1 = (char *)malloc(255 * sizeof(char));
    char *query2 = (char *)malloc(255 * sizeof(char));
    sprintf(query, "SELECT UserID FROM network_user where Email = \"%s\" ", email);
    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "0 : %s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    else
    {
        MYSQL_RES *result;
        MYSQL_RES *result1;

        result = mysql_store_result(sql);
        // query dung nhung ko co data
        if (result->row_count == 0)
        {
            fprintf(stderr, "1 : %s\n", mysql_error(sql));
            mysql_close(sql);
            return state;
        }
        else
        {
            MYSQL_ROW row;
            int i = 0;
            while ((row = mysql_fetch_row(result)) != NULL)
            {
                printf("----------tra ve add member--------\n");
                printf("UserID : %s\n", row[i]);
                printf("Space ID : %s\n", spaceID);
                sprintf(query1, "SELECT * FROM share_space where SpaceID = \"%s\" AND UserID = \"%s\" ", spaceID, row[i]);
                sprintf(query2, "INSERT INTO share_space VALUES(\"%s\", \"%s\")", row[i], spaceID);
                if (mysql_query(sql, query1))
                {
                    fprintf(stderr, "2 : %s\n", mysql_error(sql));
                    mysql_close(sql);
                    return state;
                }
                else
                {
                    result1 = mysql_store_result(sql);
                    // query dung nhung ko co data
                    if (result1->row_count == 0)
                    {
                        if (mysql_query(sql, query2))
                        {
                            fprintf(stderr, "3 : %s\n", mysql_error(sql));
                            mysql_close(sql);
                            return state;
                        }
                        else
                        {
                            printf("co vao day khong \n");
                            state = 1;
                            return state;
                        }
                    }
                    state = 2;
                    //return state;
                }
            }
        }
        mysql_free_result(result);
        mysql_free_result(result1);
    }
    free(query1);
    free(query2);
    free(query);
    mysql_close(sql);
    return state;
}