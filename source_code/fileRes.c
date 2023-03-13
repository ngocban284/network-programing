#include "fileRes.h"
#include "user.h"

// DOWNLOAD
// DOWNLOAD
// DOWNLOAD
// DOWNLOAD


void download_file_res(int new_socket)
{
    MYSQL *con;
    con = mysql_init(NULL);

    // char status[SIZE] = {0};
    char token[SIZE] = {0};
    char temp[SIZE] = {0};
    if (read(new_socket, token, SIZE) == -1)
    {
        perror("Couldn't receive command");
    }
    else
    {
        printf("\nToken :%s \n", token);
        strcpy(temp, token);

        char *ID = strtok(temp, "*");
        char *foldername = strtok(NULL, "/");

        char *UserID = strtok(token, "*");
        char *file = strtok(NULL, "*");
        char filename[SIZE] ={0};
        char *GID = get_spaceid_via_foldername(con, foldername);
        strcpy(filename, file);
        printf("UserID : %s\nFile belong to Space : %s\nfoldername :%s \nFilename : %s\n", ID, GID, foldername, filename);

        printf(COLOR_BLUE "[+] Sending ....." COLOR_RESET);
        printf("%s", filename);
        FILE *fd = fopen(filename, "r");

        // checking if the file exist or not
        if (fd == NULL)
        {
            char conf[9] = "notfound";
            char buf[SIZE] = {0};
            strncpy(buf, conf, strlen(conf));
            if (send(new_socket, buf, SIZE, 0) == -1)
            {
                perror("Couldn't send file confirmation message.\n");
            }
        }
        else
        {
            char conf[6] = "found";
            char buf[SIZE] = {0};
            strncpy(buf, conf, strlen(conf));
            // send confirm
            if (send(new_socket, buf, SIZE, 0) == -1)
            {
                perror("Couldn't send file confirmation message.\n");
            }
            // check allow
            if (check_user_access(con, UserID, foldername) == 0)
            {
                send(new_socket, "notallow", SIZE, 0);
                // strcpy(status, "download failed");
            }
            else
            {
                send(new_socket, "allow", SIZE, 0);
                // getting size of file
                fseek(fd, 0L, SEEK_END);
                long int sz = ftell(fd);
                fclose(fd);

                // opening file
                int fp = open(filename, O_RDONLY);
                if (fp == -1)
                {
                    perror("Couldn't open file");
                }

                ssize_t n = 0;
                //ssize_t total = 0;
                // printf("size: %d", sz);
                // sending file size to client
                if (write(new_socket, &sz, sizeof(sz)) == -1)
                {
                    perror("write");
                }

                char buffer[SIZE] = {0};

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
                }
                // strcpy(status, "\ndownload successfully \n");
                //  send(new_socket, status, strlen(status), 0);
                close(fp);
            }
            // send status
            // if (send(new_socket, status, SIZE, 0) > 0)
            // {
            //     printf("status : %s\n",status);
            // }
        }
    }
}


// completed
void remove_res(int sockfd)
{
    MYSQL *con = mysql_init(NULL);
    char buff[SIZE] = {0};
    char temp[SIZE] = {0};
    char status[SIZE] = {0};
    if (recv(sockfd, buff, SIZE, 0) > 0)
    {
        strcpy(temp, buff);
        char *ID = strtok(temp, "*");
        printf("UserID : %s\n", ID);
        char *foldername = strtok(NULL, "/");
        printf("Foldername : %s\n", foldername);

        char *UserID = strtok(buff, "*");
        char *filepath = strtok(NULL, "*");
        printf("filepath : %s\n", filepath);
        char *filename = basename(filepath);
        printf("filename : %s\n", filename);
        char *GID = get_spaceid_via_foldername(con, foldername);
        printf("SpaceID : %s\n", GID);

        if (isSpaceAdmin(con, UserID, GID) == 0 && check_file_owner(con, UserID, filepath) == 0)
        {
            printf("[+] The file is not deleted\n");
            strcpy(status, "[+] The file is not deleted maybe you don't have rights to remove \n");
        }
        else
        {
            int del = remove(filepath);
            printf(COLOR_GREEN "[+] Remove file %s\n" COLOR_RESET, filename);
            if (!del)
            {
                con = mysql_init(NULL);
                if (con == NULL)
                {
                    fprintf(stderr, "mysql_init() failed\n");
                }

                if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
                {
                    finish_with_error(con);
                }
                char *que = malloc(SIZE * sizeof(que));
                sprintf(que, "DELETE FROM share_file WHERE FileName LIKE \"%s\"", filepath);
                if (mysql_query(con, que))
                {
                    finish_with_error(con);
                }

                printf(COLOR_BLUE "[+] The file is deleted successfully.\n" COLOR_RESET);
                strcpy(status, "[+] The file is deleted successfully\n");
            }
            else
            {
                printf(COLOR_RESET "[+] The file is not deleted.\n" COLOR_RESET);
                strcpy(status, "[+] The file is not deleted, check filename again\n");
            }
        }
    }
    send(sockfd, status, SIZE, 0);
}
