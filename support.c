#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Insert new user data into file and user database
void insert_user_data_into_file(struct user_database *user_db)
{
    // Update user count from file
    user_db->userscount = calculate_number_of_users(); 

    FILE *fptr;
    char name[50], password[50];
    long long int phonenumber;
    int valid;

    // Input and validate name
    do
    {
        printf("\n 👤 Name: ");
        scanf(" %[^\n]", name);  // %[^\n] reads until newline, allows spaces
        valid = validate_name(name, user_db); 
        if(valid == 1)
            printf(" Invalid name! Only letters, spaces, '.', '-' allowed.\n");
        else if(valid == 2)
            printf(" Sorry! That name is already taken, choose another one...\n");
    } while(valid != 0);

    // Input and validate phone number
    do
    {
        printf(" 📞 Phone Number: ");
        scanf(" %lld", &phonenumber);  // %lld for long long int
        valid = validate_phonenumber(phonenumber, user_db);
        if(valid == 1)
         printf(" Phone number must be 10 digits.\n");
        else if(valid == 2)
         printf(" Phone number must contain digits only.\n");
        else if(valid == 3) 
         printf(" Phone number already exists.\n");
    } while(valid != 0);

    // Input and validate password
    do
    {
        printf(" 🔏 Password: ");
        scanf(" %[^\n]", password);  // allows spaces in password
        valid = validate_password(password);
        if(valid) 
         printf(" Password must be at least 4 characters.\n");
    } while(valid);

    // Store user data in memory
    int i = user_db->userscount;				
    strcpy(user_db->user[i].name, name);
    strcpy(user_db->user[i].password, password);
    user_db->user[i].phonenumber = phonenumber;

    // Append user data to CSV file
    fptr = fopen("user_database.csv", "a"); // 'a' mode appends data
    if(fptr == NULL)
    {
        printf(" Error opening file!\n");
        return;
    }
    fprintf(fptr, "%s,%lld,%s\n", name, phonenumber, password);
    fclose(fptr);

    user_db->userscount++;
    printf("\n ✅ Your account has been created!\n"); 
}

// Check user login credentials
int check_user_data_from_file(struct user_database *user_db)
{
    char name[50], password[50];
    long long int phonenumber;

    printf("\n 👤 Name: ");
    scanf(" %[^\n]", name);
    printf(" 📞 Phone Number: ");
    scanf(" %lld", &phonenumber);
    printf(" 🔏 Password: ");
    scanf(" %[^\n]", password);

    int login_success = 0;

    // Loop through all users in memory
    for(int i = 0; i < user_db->userscount; i++)
    {
        if(strcmp(name, user_db->user[i].name) == 0) // compare names
        {
            if(phonenumber == user_db->user[i].phonenumber)
            {
                if(strcmp(password, user_db->user[i].password) == 0)
                {
                    printf("\n Checking user details");
                    fflush(stdout);

                     // Step 2: Add dots with delay
                     for (int i = 0; i < 5; i++)
                     {
                       printf(".");
                       fflush(stdout);
                       usleep(500000); // 500ms delay
                     }

                   // Step 3: Replace line with login success message
                   printf("\r"); // Move to beginning of the line
                   printf("✓ Login Successful!                    \n"); // Extra spaces to clear dots
                    login_success = 1;

                    // Show user habit details
                    int temp = user_habit_details(user_db, i);
                    if(temp != 0)
                        return 0;

                    break;
                }
                else
                {
                    printf("\n ❌ Wrong password. Please try again.... ");
                    login_success = -1;
                    break;
                }
            }
            else
            {
                printf("\n ❌ Invalid phone number. Please try again.... ");
                login_success = -1;
                break;
            }
        }
    }

    if(login_success == 0)
        printf("\n ❌ User not found. Please check your details and try again... ");

    return 1;
}

// Load all users from CSV file into memory
void load_users_from_file(struct user_database *user_db)
{
    FILE *fptr = fopen("user_database.csv", "r"); // open in read mode
    if(fptr == NULL)
    {
        user_db->userscount = 0; // No users found
        return;
    }

    char line[200];
    int i = 0;

    while(fgets(line, sizeof(line), fptr) != NULL && i < 100)
    {
        line[strcspn(line, "\n")] = 0; // Remove newline character

        char *token;

        // Extract name
        token = strtok(line, ",");  // strtok splits string by ',' delimiter
        if(token != NULL)
            strcpy(user_db->user[i].name, token);

        // Extract phone number
        token = strtok(NULL, ","); // strtok remembers previous position
        if(token != NULL)
            user_db->user[i].phonenumber = atoll(token); // convert string to long long int

        // Extract password
        token = strtok(NULL, ",");
        if(token != NULL)
            strcpy(user_db->user[i].password, token);

        i++;
    }

    user_db->userscount = i; // update count in memory
    fclose(fptr);
}

// Count number of users from CSV file
int calculate_number_of_users(void)
{
    FILE *fptr = fopen("user_database.csv", "r");
    if(fptr == NULL)
        return 0;

    int count = 0;
    char line[200];

    while(fgets(line, sizeof(line), fptr) != NULL) // read each line
        count++;

    fclose(fptr);
    return count;
}
