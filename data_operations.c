#include "header.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Displays or manages user habits for a specific user (index i)
int user_habit_details(struct user_database *user_db, int i)
{
    // Load habits from file into memory
    load_user_habits_from_file(user_db, i);

    // Check if the user has any saved habits
    if(strlen(user_db->habits[i].habit1) == 0)
    {
        printf("\n📂 No habits saved yet.\n");
        printf("   [1] Create a new habit\n   [2] Exit\n");

        char ch;
        printf("\n🔹 Select an option: ");
        scanf(" %c", &ch);

        switch(ch)
        {
            case '1':
                create_user_habits(user_db, i); // Create new habits
                manage_user_habits(user_db, i);
                return 0;

            case '2':
                printf("\n👋 Leaving dashboard...\n");
                return 1;

            default:
                printf("❌ Invalid choice. Please try again.\n");
                return 1;
        }
    }
    else
    {
        // User has habits, manage them
        manage_user_habits(user_db, i);
    }

    return 0;
}
// Manage user habits: display, edit, or mark progress
int manage_user_habits(struct user_database *user_db, int i)
{
    // Display existing habits
    printf("\n📋 Your Saved Habits:\n");
    printf("   ✪  %s\n", user_db->habits[i].habit1);
    printf("   ✪  %s\n", user_db->habits[i].habit2);
    printf("   ✪  %s\n", user_db->habits[i].habit3);
    printf("   ✪  %s\n", user_db->habits[i].habit4);
    printf("   ✪  %s\n", user_db->habits[i].habit5);

    // Prompt user to edit habits or mark today's progress
    printf("\n⚡ What would you like to do?\n");
    printf("   [1] Edit Habits\n");
    printf("   [2] Mark Today’s Progress\n");

    char ch;
    printf("\n🔹 Select an option: ");
    scanf(" %c", &ch);

    switch(ch)
    {
        case '1':
            create_user_habits(user_db, i); // Edit habits
            return 0;

        case '2':
            mark_today_user_progress(user_db, i); // Record today's progress
            return 0;

        default:
            printf("❌ Invalid choice. Please try again.\n");
            return 1;
    }
}


// Prompt user to enter 5 habits and save them
void create_user_habits(struct user_database *user_db, int i)
{
    printf("\n🚀 Add your 5 habits and start your journey:\n");

    printf(" 📌 Habit1: "); 
    scanf(" %[^\n]", user_db->habits[i].habit1);
    printf(" 📌 Habit2: "); 
    scanf(" %[^\n]", user_db->habits[i].habit2);
    printf(" 📌 Habit3: ");
    scanf(" %[^\n]", user_db->habits[i].habit3);
    printf(" 📌 Habit4: "); 
    scanf(" %[^\n]", user_db->habits[i].habit4);
    printf(" 📌 Habit5: ");
    scanf(" %[^\n]", user_db->habits[i].habit5);

    update_user_habits_in_file(user_db, i); // Save habits to file
}


// Update a user's habits in the CSV file
void update_user_habits_in_file(struct user_database *user_db, int i)
{
    FILE *fptr = fopen("user_database.csv", "r"); // Open original file for reading
    FILE *temp = fopen("temp.csv", "w");          // Temporary file for updated data
    if (!fptr || !temp)
    {
        printf("Error opening file!\n");
        return;
    }

    char line[1024], user_name[50];
    strcpy(user_name, user_db->user[i].name);    // Store the current user's name
    int found = 0;

    // Read each line from the original file
    while (fgets(line, sizeof(line), fptr))
    {
        char name_in_file[50];
        sscanf(line, "%49[^,]", name_in_file);  // Extract name from line

        if (strcmp(name_in_file, user_name) == 0)
        {
            // Write updated user data with habits
            fprintf(temp, "%s,%llu,%s,%s,%s,%s,%s,%s\n",
                    user_db->user[i].name,
                    user_db->user[i].phonenumber,
                    user_db->user[i].password,
                    user_db->habits[i].habit1,
                    user_db->habits[i].habit2,
                    user_db->habits[i].habit3,
                    user_db->habits[i].habit4,
                    user_db->habits[i].habit5);
            found = 1;
        }
        else
        {
            fputs(line, temp); // Copy line as-is if not the current user
        }
    }

    // If user not found in file, append at the end
    if (!found)
    {
        fprintf(temp, "%s,%llu,%s,%s,%s,%s,%s,%s\n",
                user_db->user[i].name,
                user_db->user[i].phonenumber,
                user_db->user[i].password,
                user_db->habits[i].habit1,
                user_db->habits[i].habit2,
                user_db->habits[i].habit3,
                user_db->habits[i].habit4,
                user_db->habits[i].habit5);
    }

    fclose(fptr);
    fclose(temp);

    remove("user_database.csv");      // Delete original file
    rename("temp.csv", "user_database.csv"); // Rename temp file

    printf("✔️ User habits have been successfully updated.\n");
}

// Mark today's habit completion for a specific user
void mark_today_user_progress(struct user_database *user_db, int i)
{
    char *username = user_db->user[i].name;

    //Display yesterday's pending habits
    show_yesterday_pending_habits(username);

    //Input today's completion (Y/N) for each habit
    char ch[5];           // Store Y/N for 5 habits
    int count_yes = 0, count_no = 0;

    printf("\n🎯 Track your progress for today (Y/N):\n");

    char *habits[5] = {
       user_db->habits[i].habit1,
       user_db->habits[i].habit2,
       user_db->habits[i].habit3,
       user_db->habits[i].habit4,
       user_db->habits[i].habit5
   };

    for(int j = 0; j < 5; j++)
    {
       printf("   ➔ %s (Y/N): ", habits[j]);
       scanf(" %c", &ch[j]);
   }


    //Calculate today's Yes/No counts
    calculate_yes_or_no_score(ch[0], ch[1], ch[2], ch[3], ch[4], &count_yes, &count_no);

    //Display today's progress
    display_today_progress(count_yes, count_no);

    //Save today's progress to files
    save_progress_into_yes_file(user_db, i, ch[0], ch[1], ch[2], ch[3], ch[4]);
    save_progress_into_no_file(user_db, i, ch[0], ch[1], ch[2], ch[3], ch[4]);

    //Show today's percentage
    float today_yes_percent = (count_yes / 5.0) * 100;
    float today_no_percent  = (count_no / 5.0) * 100;
    printf("\n📈 Today's Progress: Yes: %.2f%% | No: %.2f%%\n", today_yes_percent, today_no_percent);


    // Show overall percentage including today
    show_overall_percentage(username);
}

// Display today's habit completion summary
void display_today_progress(int count1, int count2)
{
    if (count1 == 5)
        printf("\n✅ You completed all your works today! Great job!\n");
    else if (count1 > 0)
        printf("\n⚡ You completed %d works today, %d is/are still pending. Keep going!\n", count1, 5 - count1);
    else
        printf("\n❌ You haven’t completed any works today. Let’s get started!\n");
}


// Calculate the number of Yes/No responses for today's habits
void calculate_yes_or_no_score(char ch1, char ch2, char ch3, char ch4, char ch5, int *count1, int *count2)
{
    char arr[5] = {ch1, ch2, ch3, ch4, ch5}; // Store responses in an array
    *count1 = 0; // Initialize Yes count
    *count2 = 0; // Initialize No count

    // Loop through each response
    for (int i = 0; i < 5; i++)
    {
        if (arr[i] == 'Y' || arr[i] == 'y')       // Count Yes
            (*count1)++;
        else if (arr[i] == 'N' || arr[i] == 'n')  // Count No
            (*count2)++;
    }
}

// Display yesterday's pending habits for a specific user
// Only considers the last matching entry in the file
void show_yesterday_pending_habits(const char *username)
{
    FILE *file = fopen("no_database.csv", "r"); // Open pending habits file
    if (!file)
    {
        printf(" → No pending habits file found.\n");
        return;
    }

    char line[512];
    char last_pending[512] = ""; // Store last matching line for the user

    while (fgets(line, sizeof(line), file))
    {
        // Check if line starts with username
        if (strncmp(line, username, strlen(username)) == 0)
        {
            char *ptr = line + strlen(username); // Move past username

            // Skip comma and spaces if present
            while (*ptr == ',' || *ptr == ' ') ptr++;

            // Remove trailing newline
            ptr[strcspn(ptr, "\n")] = 0;

            // Update last_pending (even if empty)
            strcpy(last_pending, ptr);
        }
    }

    fclose(file);

    if (strlen(last_pending) == 0)
        printf("\n 🚀 %s, you don`t have any pending habits/work from yesterday. Keep it up!\n", username);
    else
        printf("\n ⏰ These habits/works were left undone yesterday: \"%s\".\n", last_pending);
}



// Display overall habit completion percentage for a specific user
// Sums up all habits across multiple entries in Yes and No files
void show_overall_percentage(const char *username)
{
    FILE *file_yes = fopen("yes_database.csv", "r"); // Completed habits
    FILE *file_no  = fopen("no_database.csv", "r");  // Pending habits

    if (!file_yes && !file_no)
    {
        printf(" → No database files found!\n");
        return;
    }

    int total_yes = 0, total_no = 0;
    char line[512];

    // Count all 'Yes' habits for the user across multiple lines
    if (file_yes)
    {
        while (fgets(line, sizeof(line), file_yes))
        {
            if (strncmp(line, username, strlen(username)) == 0)
            {
                char *token = strtok(line, ","); // Skip username
                while ((token = strtok(NULL, ",")) != NULL)
                {
                    while (*token == ' ') token++; // Trim leading spaces
                    if (*token != '\0') total_yes++;
                }
            }
        }
        fclose(file_yes);
    }

    // Count all 'No' habits for the user across multiple lines
    if (file_no)
    {
        while (fgets(line, sizeof(line), file_no))
        {
            if (strncmp(line, username, strlen(username)) == 0)
            {
                char *token = strtok(line, ","); // Skip username
                while ((token = strtok(NULL, ",")) != NULL)
                {
                    while (*token == ' ') token++; // Trim leading spaces
                    if (*token != '\0') total_no++;
                }
            }
        }
        fclose(file_no);
    }

    int total = total_yes + total_no;
    if (total == 0) total = 1; // Prevent division by zero

    float overall_yes = (total_yes / (float)total) * 100;
    float overall_no  = (total_no / (float)total) * 100;

    printf("\n📊 Overall Progress: Yes: %.2f%% | No: %.2f%%\n", overall_yes, overall_no);
}

// Save today's pending habits (No) for a specific user into the file
void save_progress_into_no_file(struct user_database *user_db, int i,
                                char ch1, char ch2, char ch3, char ch4, char ch5)
{
    char *username = user_db->user[i].name;
    
    // Array of the user's habits
    char *habits[5] = {
        user_db->habits[i].habit1,
        user_db->habits[i].habit2,
        user_db->habits[i].habit3,
        user_db->habits[i].habit4,
        user_db->habits[i].habit5
    };
    
    // Store today's responses
    char responses[5] = {ch1, ch2, ch3, ch4, ch5};

    FILE *file = fopen("no_database.csv", "a"); // Open pending habits file in append mode
    if (!file)
    {
        printf(" → Error opening pending habits file.\n");
        return;
    }

    fprintf(file, "%s", username); // Write username first
    int first = 1;

    // Append habits marked as 'No'
    for (int j = 0; j < 5; j++)
    {
        if (responses[j] == 'N' || responses[j] == 'n')
        {
            if (first) { fprintf(file, ", "); first = 0; }
            else        { fprintf(file, ", "); }
            fprintf(file, "%s", habits[j]);
        }
    }
    fprintf(file, "\n"); // End line after all pending habits
    fclose(file);

    //printf("📌 %s, your pending habits have been recorded!\n", username);
}

// Save today's completed habits (Yes) for a specific user into the file
void save_progress_into_yes_file(struct user_database *user_db, int i,
                                 char ch1, char ch2, char ch3, char ch4, char ch5)
{
    char *username = user_db->user[i].name;
    
    // Array of the user's habits
    char *habits[5] = {
        user_db->habits[i].habit1,
        user_db->habits[i].habit2,
        user_db->habits[i].habit3,
        user_db->habits[i].habit4,
        user_db->habits[i].habit5
    };
    
    // Store today's responses
    char responses[5] = {ch1, ch2, ch3, ch4, ch5};

    FILE *file = fopen("yes_database.csv", "a"); // Open completed habits file in append mode
    if (!file)
    {
        printf(" → Error opening completed habits file.\n");
        return;
    }

    fprintf(file, "%s", username); // Write username first
    int first = 1;

    // Append habits marked as 'Yes'
    for (int j = 0; j < 5; j++)
    {
        if (responses[j] == 'Y' || responses[j] == 'y')
        {
            if (first) { fprintf(file, ", "); first = 0; }
            else        { fprintf(file, ", "); }
            fprintf(file, "%s", habits[j]);
        }
    }
    fprintf(file, "\n"); // End line after all completed habits
    fclose(file);

    //printf("📌 %s, your completed habits have been recorded!\n", username);
}


// Load habits and user details for a specific user from the CSV file
void load_user_habits_from_file(struct user_database *user_db, int i)
{
    FILE *fptr = fopen("user_database.csv", "r"); // Open user database for reading
    if (!fptr)
    {
        printf(" → Could not open user database file!\n");
        return;
    }

    char line[1024];

    // Read each line to find the matching user
    while (fgets(line, sizeof(line), fptr))
    {
        char name[50], password[50];
        unsigned long long phonenumber;
        char h1[50], h2[50], h3[50], h4[50], h5[50];

        // Parse line into username, phone, password, and 5 habits
        if (sscanf(line, "%49[^,],%llu,%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]",
                   name, &phonenumber, password, h1, h2, h3, h4, h5) == 8)
        {
            // If this line matches the requested user
            if (strcmp(user_db->user[i].name, name) == 0)
            {
                // Copy user details into database structure
                strcpy(user_db->user[i].password, password);
                user_db->user[i].phonenumber = phonenumber;

                strcpy(user_db->habits[i].habit1, h1);
                strcpy(user_db->habits[i].habit2, h2);
                strcpy(user_db->habits[i].habit3, h3);
                strcpy(user_db->habits[i].habit4, h4);
                strcpy(user_db->habits[i].habit5, h5);
                break; // Stop after finding the user
            }
        }
    }

    fclose(fptr); // Close the file
}
