#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "header.h"

int validate_name(char *name, struct user_database *user_db)
{
    // Check if empty
    if(strlen(name) == 0)
        return 1; // empty name

    // Check for valid characters
    for(int i = 0; name[i]; i++)
    {
        if(!isalpha(name[i]) && name[i] != ' ' && name[i] != '.' && name[i] != '-')
            return 1; // invalid character
    }

    // Check for duplicate name
    for(int i = 0; i < user_db->userscount; i++)
    {
        if(strcmp(name, user_db->user[i].name) == 0)
            return 2; // duplicate
    }

    return 0; // valid
}


// Validate phone number: returns 0 if valid, 1=length, 2=non-digit, 3=duplicate
int validate_phonenumber(long long int phonenumber, struct user_database *user_db)
{
    char temp[20];
    sprintf(temp, "%lld", phonenumber);

    // length check
    if(strlen(temp) != 10)
        return 1;

    // all digits check
    for(int i = 0; i < 10; i++)
    {
        if(!isdigit(temp[i]))
            return 2;
    }

    // duplicate check
    for(int i = 0; i < user_db->userscount; i++)
    {
        if(phonenumber == user_db->user[i].phonenumber)
            return 3;
    }

    return 0; // valid
}

// Validate password: returns 1 if invalid, 0 if valid
int validate_password(char *password)
{
    if(strlen(password) < 4)
        return 1; // too short
    return 0; // valid
}

