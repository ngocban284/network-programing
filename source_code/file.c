#include "file.h"
#include "user.h"

void errorMsg(char *str)
{
    printf(COLOR_RED "ERROR: %s\n" COLOR_RESET, str);
}

void upload_file(char *UserID, int new_socket)
{
    ssize_t total = 0;
    printf(COLOR_BLUE "\n[+] Uploading ....." COLOR_RESET);
    char *filename = malloc(SIZE * sizeof(char));
    printf(COLOR_GREEN "\n[+] Enter filename : " COLOR_RESET);
    scanf("%[^\n]", filename);
    getchar();
    // printf("file name: %s\n", filename);

    char *filepath = malloc(SIZE * sizeof(char));
    printf(COLOR_GREEN "\n[+] Enter destination : " COLOR_RESET);
    scanf("%[^\n]", filepath);
    getchar();
    // printf("Destination path : %s\n", filepath);

    char *name = basename(filename);
    char request[SIZE] = "";

    sprintf(request, "%s*%s/%s", UserID, filepath, name);
    // printf("token send: %s\n", request);

    FILE *fd = fopen(filename, "r");
    // checking if the file exist or not
    if (fd == NULL)
    {
        errorMsg("\n[-] File not found !!");
    }
    else
    {
        printf("\n[+] File found !!\n");

        if (send(new_socket, request, SIZE, 0) == -1)
        {
            errorMsg("\n[-] Error sending file path ");
        }
        char conf[25];
        if (read(new_socket, conf, SIZE) == -1)
        {
            errorMsg("Couldn't receive confirmation message");
        }
        else
        {
            // check have or not
            if (strcmp(conf, "notallow") == 0)
            {
                errorMsg("Not allow to upload \n");
            }
            else
            {

                if (strcmp(conf, "exist") == 0)
                {
                    errorMsg("File exist \n");
                }
                else
                {

                    fseek(fd, 0L, SEEK_END);
                    long int sz = ftell(fd);
                    fclose(fd);

                    // opening file
                    int fp = open(filename, O_RDONLY);
                    if (fp == -1)
                    {
                        perror("Couldn't open file");
                    }

                    ssize_t n;

                    printf("\n[+] File size: %ld", sz);
                    // sending file size to client
                    if (write(new_socket, &sz, sizeof(sz)) == -1)
                    {
                        perror("write");
                    }

                    char buffer[SIZE] = "";

                    // reading from file and sending to client
                    while (1)
                    {
                        n = read(fp, buffer, SIZE);
                        if (n == 0)
                            break;
                        if (n == -1)
                        {
                            perror("read");
                            break;
                        }
                        if (write(new_socket, buffer, n) == -1)
                        {
                            perror("write");
                            break;
                        }
                        total += n;
                        int percen = (total * 100) / sz;

                        printf(COLOR_GREEN "\r%d %% upload. total %ld bytes" COLOR_RESET, percen, total);
                    }
                    close(fp);
                    char respone[SIZE] = {0};
                    if (recv(new_socket, respone, SIZE, 0) > 0)
                    {
                        printf("\n%s", respone);
                    }
                }
            }
        }
    }
    free(filename);
    free(filepath);
}


void write_file(int sock)
{
    MYSQL *con;
    con = mysql_init(NULL);

    char status[SIZE] = "";
    char buffer[SIZE] = "";
    char token[SIZE] = "";
    if (recv(sock, token, SIZE, 0) == -1)
    {
        errorMsg("error\n");
    }
    else
    {

        printf("token received : %s \n", token);
        if (strlen(token) < 10)
        {
        }
        else
        {
            ssize_t total = 0;
            ssize_t n = 0;
            char temp[SIZE];
            strcpy(temp, token);

            char *ID = strtok(token, "*");
            char *foldername = strtok(NULL, "/");
            char *GID = get_spaceid_via_foldername(con, foldername);
            char *FolderID = get_folderid_via_foldername(con, foldername);
            char *UserID = strtok(temp, "*");
            char *filepath = strtok(NULL, "*");
            printf("666\n");
            printf("UserID : %s \nSpaceID : %s\nfoldername: %s \nFolerID : %s\nfilepath: %s \n", ID, GID, foldername, FolderID, filepath);
            printf("5555\n");
            printf("%d\n", SIZE);
            
            char buff[SIZE] = "";
            printf("%d\n", SIZE);

            printf("1111");
            strncpy(buff, filepath, strlen(filepath));
            printf("aaaa");
            // checking if the file exist or not
            int k = check_user_access(con, UserID, foldername);
            if (k == 0)
            {
                char conf2[9] = "notallow";
                char buf[SIZE] = {0};
                strncpy(buf, conf2, strlen(conf2));
                if (send(sock, buf, SIZE, 0) == -1)
                {
                    perror("Couldn't send file confirmation message.\n");
                }
            }
            else
            {
                printf("bbbb");
                FILE *fd = fopen(filepath, "r");
                if (fd != NULL)
                {
                    send(sock, "exist", strlen("exist"), 0);
                }
                else
                {
                    printf("cccc");
                    send(sock, "allow", strlen("allow"), 0);
                    // opening a file with needed filename
                    int fp = open(filepath, O_WRONLY | O_CREAT, 0777);
                    if (fp == -1)
                    {
                        strcpy(status, "Error, file exists\n");
                        perror("Can't open file");
                    }
                    else
                    {
                        long int sz;

                        // reading file size
                        if (read(sock, &sz, sizeof(sz)) == -1)
                        {
                            perror("read");
                        }

                        printf("size: %ld", sz);

                        // reading from socket and writing to file
                        // block by block till all bytes are read
                        do
                        {
                            n = read(sock, buffer, SIZE);

                            if (n == -1)
                            {
                                perror("read");
                                break;
                            }
                            if (write(fp, buffer, n) == -1)
                            {
                                perror("write");
                                break;
                            }
                            // percentage of file downloaded
                            total += n;
                            printf("sz: %d\n", sz);
                            int percen = (total * 100) / sz;

                            printf(COLOR_BLUE "\r%d %% received. total %ld bytes" COLOR_RESET, percen, total);

                        } while (n > 0 && total < sz);
                        close(fp);
                        printf("\n");
                        con = mysql_init(NULL);
                        if (con == NULL)
                        {
                            fprintf(stderr, "mysql_init() failed\n");
                        }
                        if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
                        {
                            finish_with_error(con);
                        }
                        char *check = malloc(SIZE * sizeof(check));
                        sprintf(check, "SELECT *from share_file where FileName LIKE \"%s\"", filepath);
                        if (mysql_query(con, check))
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
                            char *que = malloc(SIZE * sizeof(que));
                            sprintf(que, "INSERT INTO share_file(FileName,UserID,FolderID) values(\"%s\",\"%s\",\"%s\")", filepath, UserID, FolderID);
                            if (mysql_query(con, que))
                            {
                                finish_with_error(con);
                            }
                            free(que);
                        }
                        strcpy(status, "successfully \n");
                    }
                    if (send(sock, status, SIZE, 0) > 0)
                    {
                        printf("status : %s", status);
                    }
                }
            }
        }
    }
    // getting next argument
}


// DOWNLOAD

void download_file(char *UserID, int sock)
{
    // char respone[SIZE] = {0};
    char buffer[SIZE] = {0};
    char *filepath = malloc(100 * sizeof(char));
    printf(COLOR_GREEN "Enter file path to download : " COLOR_RESET);
    scanf("%[^\n]", filepath);
    getchar();

    long int sz;
    
    ssize_t n = 0;
    char request[SIZE] = {0};
    sprintf(request, "%s*%s", UserID, filepath);
    char token[SIZE] = {0};
    strncpy(token, request, strlen(request));
    // printf("UserID : %s \nfilepath : %s\n", UserID, filepath);
    // printf("Token : %s\n", token);

    // check file exist
    char *filename = basename(filepath);
    FILE *ff = fopen(filename, "r");
    if (ff != NULL)
    {
        errorMsg("[+] File exist \n");
    }
    else
    {
        if (send(sock, token, SIZE, 0) == -1)
        {
            perror("Couldn't send filename");
        }
        else
        {
            char conf[9] = {0};
            if (read(sock, conf, SIZE) == -1)
            {
                perror("Couldn't receive confirmation message");
            }
            else
            {
                // check have or not
                printf(COLOR_BLUE "Confirm: %s in server\n" COLOR_RESET, conf);
                if (strcmp(conf, "notfound") == 0)
                {
                    printf(COLOR_RED "No such file in server: %s\n" COLOR_RESET, filepath);
                }
                else
                {
                    char conf2[25] = "";
                    if (read(sock, conf2, SIZE) == -1)
                    {
                        perror("Couldn't receive confirmation message");
                    }
                    // check permission
                    if (strcmp(conf2, "notallow") == 0)
                    {
                        errorMsg("Not allow to download \n");
                    }
                    else
                    {
                        ssize_t total = 0;
                        // getting filename
                        char *filename = basename(filepath);
                        // opening a file with needed filename
                        int fp = open(filename, O_WRONLY | O_CREAT, 0777);
                        if (fp == -1)
                        {
                            perror("Can't open file");
                        }

                        // reading file size
                        if (read(sock, &sz, sizeof(sz)) == -1)
                        {
                            perror("read");
                        }

                        printf("size: %ld", sz);

                        // reading from socket and writing to file
                        // block by block till all bytes are read
                        do
                        {
                            n = read(sock, buffer, SIZE);

                            if (n == -1)
                            {
                                perror("read");
                                break;
                            }
                            if (write(fp, buffer, n) == -1)
                            {
                                perror("write");
                                break;
                            }
                            // percentage of file downloaded
                            
                            total += n;
                            int percen = (total * 100) / sz;

                            printf(COLOR_GREEN "\r%d %% downloaded , total %ld bytes" COLOR_RESET, percen, total);
                        } while (n > 0 && total < sz);
                        printf("\n");
                        close(fp);
                        if (total == sz)
                        {
                            printf(COLOR_GREEN "Download successfully \n" COLOR_RESET);
                        }
                        else
                            printf(COLOR_RED "Download failed \n" COLOR_RESET);
                    }
                }
                // get status
            }
        }
    }
    free(filepath);
    // getting next argument
}


// REMOVE

void remove_file(char *UserID, int sockfd)
{
    char buff[SIZE] = {0};
    char filename[SIZE] = {0};
    printf(COLOR_GREEN "\n[+]Remove............." COLOR_RESET);
    printf(COLOR_GREEN "\n[+]Enter filename to remove : " COLOR_RESET);
    scanf("%[^\n]", filename);
    getchar();
    // printf("%s\n", filename);
    char *sen = malloc(255 * sizeof(char));
    sprintf(sen, "%s*%s", UserID, filename);
    // printf("send token : %s", sen);
    if (send(sockfd, sen, SIZE, 0) == -1)
    {
        errorMsg("?");
    }

    if (recv(sockfd, buff, SIZE, 0) == -1)
    {
        errorMsg("?");
    }
    printf(COLOR_BLUE "\n%s" COLOR_RESET, buff);
    memset(buff, '\0', strlen(buff));
    memset(filename, '\0', strlen(filename));
    free(sen);
}
