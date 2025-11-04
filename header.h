#ifndef HEADER_H
#define HEADER_H

#include <time.h>

typedef struct details
{
    char name[50];
    unsigned long long int phonenumber;
    char password[50];
} user_details;

typedef struct habits
{
    char habit1[50];
    char habit2[50];
    char habit3[50];
    char habit4[50];
    char habit5[50];
} user_habits;

struct user_database
{
    user_details user[100];
    user_habits habits[100];
    int yes_count;
    int no_count;
    int userscount;
};

// Function declarations
void insert_user_data_into_file(struct user_database *user_db);
int check_user_data_from_file(struct user_database *user_db);
void load_users_from_file(struct user_database *user_db);
int calculate_number_of_users(void);

// Validation functions
int validate_name(char *name, struct user_database *user_db);
int validate_phonenumber(long long int phonenumber, struct user_database *user_db);
int validate_password(char *password);

// Habit-related functions
int user_habit_details(struct user_database *user_db, int i);
void create_user_habits(struct user_database *user_db, int i);
void update_user_habits_in_file(struct user_database *user_db, int i);
int manage_user_habits(struct user_database *user_db, int i);
void mark_today_user_progress(struct user_database *user_db, int i);
void save_progress_into_yes_file(struct user_database *user_db, int i,
                             char ch1, char ch2, char ch3, char ch4, char ch5);

void save_progress_into_no_file(struct user_database *user_db, int i,
                             char ch1, char ch2, char ch3, char ch4, char ch5);
void show_overall_percentage(const char *username);
void display_today_progress(int count1, int count2);
void calculate_yes_or_no_score(char ch1, char ch2, char ch3, char ch4, char ch5, int *count1, int *count2);
void show_yesterday_pending_habits(const char *username);
void load_user_habits_from_file(struct user_database *user_db, int i);

#endif
