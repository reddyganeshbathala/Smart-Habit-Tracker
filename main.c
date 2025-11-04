#include "header.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main()
{
    // Welcome message
    printf("\n                             💪   WELCOME TO SMART HABIT TRACKER APP   💪");
    printf("\n                       +--------------------------------------------------------+");


    char ch;
    struct user_database user_db;

    // Load existing users from file into user_db structure
    load_users_from_file(&user_db);

    // Main menu loop
    do 
    {
        printf("\n 1 ▸ Login");
        printf("\n 2 ▸ Sign In");
        printf("\n 3 ▸ Sign Out");
        printf("\n\n🔹 Select an option: ");

        scanf(" %c", &ch);

        switch(ch) 
        {
            case '1': 
                // Verify user credentials and login
                check_user_data_from_file(&user_db); 
                break;
            case '2':
                // Register new user
                insert_user_data_into_file(&user_db);
                break;
            case '3':
                // Exit application
                printf("\nSigning out");
                for(int i=0;i<4;i++)
                {
                  printf(".");
                  fflush(stdout);
                 usleep(500000);
                }
                printf("\n");
                break;
            default:
                // Invalid input
                printf("Invalid choice\n");
        }
    } while(ch != '3');  // Repeat until user chooses to sign out

    return 0;
}
