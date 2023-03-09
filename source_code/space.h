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
#include "user.h"
#include "define.h"

int access_space(char *spaceID, char *userID);
int access_share_space(char *spaceID, char *userID);
char *my_space(char *userID, char *temp);
char *shareSpace(char *userID, char *temp);
int create_space(char *userID, char *spaceName, char *spaceDescrp);
int delete_space(char *spaceID);
int out_space(char *userID, char *spaceID);