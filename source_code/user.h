#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h> 
#include <errno.h>
#include "define.h"


//done
char *get_folderid_via_foldername(MYSQL *con, char *foldername);
char *get_spaceid_via_foldername(MYSQL *con,char *foldername); // tra ve SpaceId cua filename
int check_user_access(MYSQL *con, char *UserID, char *foldername); // kiem tra nguoi dung co thuoc nhom hay cua folder hay khong
char *get_foldername_via_gid(MYSQL *con, char *spaceid);
int check_file_owner(MYSQL *con, char *UserID, char *filename); // tra ve UserId cua folder
int check_folder_owner(MYSQL *con, char *UserID, char *foldername); // tra ve UserId cua file
int isSpaceAdmin(MYSQL *con, char *UserID, char *SpaceName); // tra ve 1 neu dung la admin , 0 neu ko phai


char *get_userid_via_email(MYSQL *con, char *email);
char *get_spacename_via_spaceID(MYSQL *con, char *spaceID);

char *remove_space(char *str);
int sign_up(char *username, char *email, char *password);
char *sign_in(char *email, char *password, char *temp);
int delete_member(char *username , char *spaceID);
int add_member(char *email , char *spaceID);
void finish_with_error();
void show_menu();
//

