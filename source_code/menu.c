#include "menu.h"
int validateLogin(char *email, char *password)
{
    char emailcpy[200], passwordcpy[200];
    strcpy(emailcpy, email);
    strcpy(passwordcpy, password);
    if (strstr(emailcpy, "@gmail.com") != NULL)
    {
        if (strlen(strstr(emailcpy, "@gmail.com")) == 10)
        {
            if (strlen(passwordcpy) >= 6)
            {
                return 1;
            }
        }
    }
    return 0;
}

void loginPage()
{
    printf(COLOR_BLUE "\n_______You need to sign in to use, if you don't have an account, please sign up_______\n" COLOR_RESET);
    printf("\n");
    printf("\t\t\t\t1. Sign in.\n");
    printf("\t\t\t\t2. Sign up.\n");
    printf("\t\t\t\t3. Exit.\n");
    printf(COLOR_BLUE "_______________________________________________________________________________________\n" COLOR_RESET);
}

void homePage()
{
    printf(COLOR_BLUE "\n_____________________________________HOME_____________________________________\n" COLOR_RESET);
    printf("\n");
    printf("\t\t\t1. Your profile\n");
    printf("\t\t\t2. User list\n");
    printf("\t\t\t3. My space\n");
    printf("\t\t\t4. My share space\n");
    printf("\t\t\t5. Create a space\n");
    printf("\t\t\t6. Logout\n");
    printf(COLOR_BLUE "_________________________________________________________________________________\n" COLOR_RESET);
}

void shareSpacePage()
{
    printf(COLOR_BLUE "\n___________________________________SPACE SHARE PAGE_____________________________\n\n" COLOR_RESET);
    printf("\t\t\t1. Member info.\n");
    printf("\t\t\t2. List folder.\n");
    printf("\t\t\t3. List file.\n");
    printf("\t\t\t4. Upload file.\n");
    printf("\t\t\t5. Download file.\n");
    printf("\t\t\t6. Remove file.\n");
    printf("\t\t\t7. Create Folder. \n");
    printf("\t\t\t8. Out space.\n");
    printf("\t\t\t9. Back .\n");
    printf(COLOR_BLUE "________________________________________________________________________________________\n" COLOR_RESET);
}

void adminPage()
{
    printf(COLOR_BLUE "\n__________________________________MY SPACE PAGE________________________________\n" COLOR_RESET);
    printf("\n");
    printf("\t\t\t1. Member info.\n");
    printf("\t\t\t2. List folder.\n");
    printf("\t\t\t3. List file.\n");
    printf("\t\t\t4. Upload file.\n");
    printf("\t\t\t5. Download file.\n");
    printf("\t\t\t6. Remove file.\n");
    printf("\t\t\t7. Create Folder \n");
    printf("\t\t\t8. Delele Folder \n");
    printf("\t\t\t9. Add member\n");
    printf("\t\t\t10. Delete memeber\n");
    printf("\t\t\t11. Delete space\n");
    printf("\t\t\t12. Back\n");
    printf(COLOR_BLUE "________________________________________________________________________________________\n" COLOR_RESET);
}
