#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <regex.h>
#include <stdlib.h>
#include <locale.h>



void handle_game(char lines[] , char name[] ){
    clear();
    int len_line = strlen(lines);
    char filename[200] ;
    attroff(COLOR_PAIR(1));
    FILE *ptr = fopen(filename , "a");
    int c;
    int x = 11;
    int y =11;
    int rows;
    int cols;
    keypad(stdscr , 1);
    noecho();
    getmaxyx(stdscr , rows ,cols);
    c=getch();
    do{

        if(c== KEY_UP) y--;
        if(c== KEY_DOWN) y++;
        if(c== KEY_LEFT) x--;
        if(c== KEY_RIGHT) x++;
        if(c== 'q'){
            x--;
            y--;
        }
        if(c== 'w'){
            x++;
            y--;
        }
        if(c=='a'){
            y++;
            x--;
        }
        if(c=='s'){
            x++;
            y++;
        }
        mvprintw(y , x ,"@");
    
    }
    while ((c = getch()) != 27);
    refresh();
    getch();


}

typedef struct {
    char name[50];
    int score;
} Player;

int compare(const void *a, const void *b) {
    Player *playerA = (Player *)a;
    Player *playerB = (Player *)b;
    return playerB->score - playerA->score;
}

int dificulty = 0;
void draw_table(int start_row, int start_col, int rows, int cols, int *col_widths, int cell_height) {
    int i, j, current_row, current_col;

   
    for (i = 0; i <= rows; i++) {
        current_col = start_col;
        for (j = 0; j < cols; j++) {
            for (int k = 0; k < col_widths[j]; k++) {
                mvaddch(start_row + i * cell_height, current_col + k, '_');
            }
            current_col += col_widths[j];
        }
    }

    
    for (j = 0; j <= cols; j++) {
        current_row = start_row;
        current_col = start_col;
        for (int col = 0; col < j; col++) {
            current_col += col_widths[col];
        }
        for (i = 0; i < rows; i++) {
            for (int k = 0; k < cell_height; k++) {
                mvaddch(current_row + k, current_col, '|');
            }
            current_row += cell_height;
        }
        mvaddch(current_row, current_col, '|'); 
    }
}
void handle_settings(char *choices[], int i) {
    if (strcmp(choices[i], "Dificulty") == 0) {
        clear();
        const char *dificulties[] = {"Hard", "Medium", "Easy"};
        int choice;
        int n_choice = sizeof(dificulties) / sizeof(dificulties[0]);
        int counter = 0;

        while (1) {
            clear();
            for (int j = 0; j < n_choice; j++) {
                if (j == counter) {
                    attron(A_REVERSE);
                    mvprintw(j, 0, "%s", dificulties[j]);
                    attroff(A_REVERSE);
                } else {
                    mvprintw(j, 0, "%s", dificulties[j]);
                }
            }
            refresh();

            choice = getch();
            if (choice == 27) // دکمه ESC
                return; // برگرد به منوی قبل
            switch (choice) {
                case KEY_UP:
                    counter = (counter == 0) ? n_choice - 1 : counter - 1;
                    break;
                case KEY_DOWN:
                    counter = (counter == n_choice - 1) ? 0 : counter + 1;
                    break;
                case 10:
                    dificulty = counter;
                    return;
            }
        }
    }
}

char* my_strdup(const char* str) {
    size_t length = strlen(str) + 1;
    char* copy = (char*)malloc(length);
    if (copy) {
        memcpy(copy, str, length);
    }
    return copy;
}

char* make_login(const char* name) {
    static char filename[400];
    snprintf(filename, sizeof(filename), "%s_games.txt", name);
    return filename;
}

char* convertor(const char* name, const char* suffix, const char* game) {
    static char filename[400];
    snprintf(filename, sizeof(filename), "%s_%s_%s.txt", name, suffix, game);
    return filename;
}

int handle_login(char* login_choices[], int i, char* name) {
    clear();
    if (strcmp(login_choices[i], "Create a new game") == 0) {
        FILE* game_list = fopen(make_login(name), "a");
        if (!game_list) {
            mvprintw(0, 0, "Error creating game list file.");
            refresh();
            getch();
            return;
        }

        char game_name[100];
        mvprintw(0, 0, "Please choose a name for your game: ");
        echo();
        scanw("%s", game_name);
        noecho();

        fprintf(game_list, "%s\n", game_name);
        fclose(game_list);

        FILE* game_file = fopen(convertor(name, "games", game_name), "a");
        if (!game_file) {
            mvprintw(1, 0, "Error creating game file.");
            refresh();
            getch();
            return;
        }
        fclose(game_file);

        mvprintw(2, 0, "Game created successfully! Press any key to continue.");
        refresh();
        getch();
    } else if (strcmp(login_choices[i], "Load") == 0) {
        FILE* game_list = fopen(make_login(name), "r");
        if (!game_list) {
            mvprintw(0, 0, "Error: Unable to open game list file.");
            refresh();
            getch();
            return;
        }

        char* lines[1024];
        int line_count = 0;

        while (!feof(game_list) && !ferror(game_list)) {
            char buffer[200];
            if (fgets(buffer, sizeof(buffer), game_list) != NULL) {
                buffer[strcspn(buffer, "\n")] = '\0';
                lines[line_count] = my_strdup(buffer);
                line_count++;
                if (line_count >= 1024) {
                    break;
                }
            }
        }
        fclose(game_list);

        int counter = 0;
        int choice;
        while (1) {
            clear();
            for (int j = 0; j < line_count; j++) {
                if (j == counter) {
                    attron(A_REVERSE);
                    mvprintw(j, 0, "%s", lines[j]);
                    attroff(A_REVERSE);
                } else {
                    mvprintw(j, 0, "%s", lines[j]);
                }
            }
            refresh();
            choice = getch();
            switch (choice) {
                case KEY_UP:
                    counter = (counter == 0) ? line_count - 1 : counter - 1;
                    break;
                case KEY_DOWN:
                    counter = (counter == line_count - 1) ? 0 : counter + 1;
                    break;
                case 10:
                    if (counter == line_count - 1) { // اگر دکمه "Back" انتخاب شود
                        return -1; // برگشت به منوی قبلی
                    }
                    handle_settings((char **)dificulty, counter);
                    return;
            }
        }

        for (int i = 0; i < line_count; i++) {
            free(lines[i]);
        }
    } else if (strcmp(login_choices[i], "Settings") == 0) {
        const char *dificulties[] = {"Dificulty", "Hero color", "Music", "Back"};
        int choice;
        int n_choice = sizeof(dificulties) / sizeof(dificulties[0]);
        int counter = 0;
        while (1) {
            clear();
            for (int j = 0; j < n_choice; j++) {
                if (j == counter) {
                    attron(A_REVERSE);
                    mvprintw(j, 0, "%s", dificulties[j]);
                    attroff(A_REVERSE);
                } else {
                    mvprintw(j, 0, "%s", dificulties[j]);
                }
            }
            refresh();
            choice = getch();
            switch (choice) {
                case KEY_UP:
                    counter = (counter == 0) ? n_choice - 1 : counter - 1;
                    break;
                case KEY_DOWN:
                    counter = (counter == n_choice - 1) ? 0 : counter + 1;
                    break;
                case 10:
                    handle_settings((char **)dificulties, counter);
                    return;
            }
        }
    }

    else if(strcmp(login_choices[i], "Back") ==0){
        return -1;
    }
    else if(strcmp(login_choices[i] , "Score schedule") == 0){
        attroff(COLOR_PAIR(1));
        curs_set(FALSE);
        clear();
        int rows = 6, cols = 7, cell_height = 3;
        int col_widths[] = {5, 10, 10, 10 , 10 ,10 ,10}; // عرض ستون‌ها

        draw_table(1, 0, rows, cols, col_widths, cell_height);
    int counter =1;
    int place = 2;
    int count = 5;
    char *menus[]={
        "Name",
        "Score",
        "Golds",
        "Exp",
        "Others",
        "Com Games"
    };
    mvprintw(0 , 0 , "|");
    int menu_counter = 7;
    for(int i=0 ; i<7 ; i++){
        mvprintw(0  ,count ,"|" );
        count +=10;
    }
    for(int i =0 ; i<6 ; i++){
        mvprintw(0 , menu_counter , "%s" , menus[i]);
        menu_counter+=10;
    }
    for(int i=0 ; i<6 ; i++){
        mvprintw(place, 2 , "%d",  counter);
        counter ++;
        place+=3;
    }
    start_color();
    init_pair(2 , COLOR_RED , COLOR_YELLOW);
    init_pair(3 , COLOR_GREEN , COLOR_BLUE);
    init_pair(4 , COLOR_YELLOW , COLOR_CYAN);
    init_pair(5 , COLOR_BLACK , COLOR_WHITE);
    FILE *file = fopen("scores.txt", "r");
    if (!file) {
        endwin();
        perror("Unable to open the file.");
        return 1;
    }

    Player players[100];
    int counters = 0;
    while (fscanf(file, "%[^:]: %d\n", players[counters].name, &players[counters].score) == 2) {
        counters++;
    }
    fclose(file);

    qsort(players, counters, sizeof(Player), compare);

    

    int place_name = 2, limit = counters < 6 ? counters : 6;

    for (int i = 0; i < limit; i++) {
        int color_pair = 2; // رنگ پیش‌فرض برای سایر افراد

        // تنظیم رنگ برای نفرات اول تا سوم
        if (i == 0)
            color_pair = 3; // رنگ نفر اول
        else if (i == 1)
            color_pair = 4; // رنگ نفر دوم
        else if (i == 2)
            color_pair = 5; // رنگ نفر سوم

        attron(COLOR_PAIR(color_pair)); // اعمال رنگ
        
        if (strcasecmp(players[i].name, name) == 0) {
            mvprintw(place_name, 3, ">"); // نمایش فلش
            attron(A_BOLD);                // متن پررنگ برای کاربر فعلی
        }

        mvprintw(place_name, 7, "%s", players[i].name);  // نام بازیکن
        mvprintw(place_name, 17, "%d", players[i].score); // امتیاز بازیکن

        // ریست استایل‌ها
        if (strcmp(players[i].name, name) == 0)
            attroff(A_BOLD); // غیرفعال‌سازی متن پررنگ

        attroff(COLOR_PAIR(color_pair)); 
        place_name += 3; 
        refresh();

    getch();
    attron(COLOR_PAIR(1));
    }
    return 0;
}
}

void make_file(char *name, char *password, char *email) {
    char filename[300];
    snprintf(filename, sizeof(filename), "%s_rouge.txt", name);
    FILE *ptr = fopen(filename, "w");
    fprintf(ptr, "name : %s\n", name);
    fprintf(ptr, "password : %s\n", password);
    fprintf(ptr, "email : %s\n", email);
    fclose(ptr);
}

int upper_lower_digit_password(const char *password) {
    int count_upper = 0;
    int count_lower = 0;
    int count_digit = 0;

    for (const char *p = password; *p; p++) {
        if ('A' <= *p && *p <= 'Z')
            count_upper++;
        else if ('a' <= *p && *p <= 'z')
            count_lower++;
        else if ('0' <= *p && *p <= '9')
            count_digit++;
    }
    return (count_digit > 0 && count_upper > 0 && count_lower > 0);
}

int validate_email(const char *email) {
    regex_t regex;
    const char *pattern = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";

    if (regcomp(&regex, pattern, REG_EXTENDED)) {
        return 0;
    }

    int result = (regexec(&regex, email, 0, NULL, 0) == 0);
    regfree(&regex);
    return result;
}

int correct_password(const char *password) {
    return (strlen(password) >= 7);
}

void clear_string(char *string) {
    memset(string, '\0', strlen(string));
}

int repeate_name(const char *filename, const char *name) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, name) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void handle_menu(char *choices[], int i) {
    clear();
    start_color();
    init_pair(1, COLOR_RED, COLOR_WHITE);
    attron(COLOR_PAIR(1));

    if (strcmp(choices[i], "Sign in") == 0) {
        char new_name[400];
        mvprintw(0, 0, "Enter your name: ");
        echo();
        scanw("%399s", new_name);
        noecho();

        while (repeate_name("accountsname.txt", new_name)) {
            mvprintw(0, 0, "This name already exists, please choose a different name: ");
            clear_string(new_name);
            scanw("%399s", new_name);
        }

        char password[400];
        mvprintw(1, 0, "Enter your password: ");
        echo();
        scanw("%s", password);
        
        while (!correct_password(password) || !upper_lower_digit_password(password)) {
            mvprintw(2, 0, "Please choose a stronger password: ");
            clear_string(password);
            scanw("%s", password);
        }

        char email[400];
        mvprintw(3, 0, "Enter your email address: ");
        echo();
        scanw("%s", email);
        
        while (!validate_email(email)) {
            mvprintw(3, 0, "Please choose a correct email: ");
            clear_string(email);
            scanw("%s", email);
        }

        make_file(new_name, password, email);

        mvprintw(5, 0, "Account created successfully! Press any key to continue.");
        refresh();
        getch();
    } else if (strcmp(choices[i], "Login") == 0) {
        char username[400], password_user[400];
        mvprintw(0, 0, "Enter your name for login: ");
        echo();
        scanw("%s", username);
        

        char file_name[400];
        snprintf(file_name, sizeof(file_name), "%s_rouge.txt", username);
        FILE *userfile = fopen(file_name, "r");

        while (userfile == NULL) {
            mvprintw(2, 0, "Your name is not correct, please enter another name: ");
            clear_string(username);
            scanw("%s", username);
            snprintf(file_name, sizeof(file_name), "%s_rouge.txt", username);
            userfile = fopen(file_name, "r");
        }

        char line[400], stored_password[400];
        while (fgets(line, sizeof(line), userfile)) {
            if (strncmp(line, "password :", 9) == 0) {
                sscanf(line + 11, "%s", stored_password);
                break;
            }
        }
        fclose(userfile);

        mvprintw(1, 0, "Enter your password: ");
        echo();
        scanw("%s", password_user);
        
        while (strcmp(password_user, stored_password) != 0) {
            mvprintw(3, 0, "This password is incorrect, please try again: ");
            clear_string(password_user);
            scanw("%s", password_user);
        }

        mvprintw(5, 0, "Login was successful! Press any key to continue.");
        refresh();
        getch();

        char *login_choices[] = {
            "Create a new game",
            "Load",
            "Score schedule",
            "Settings",
            "Back"
        };

        int login = sizeof(login_choices) / sizeof(login_choices[0]);
        int highlight = 0;
        int choice;

        while (1) {
            clear();
            for (int j = 0; j < login; j++) {
                if (j == highlight) {
                    attron(A_REVERSE);
                    mvprintw(j, 0, "%s", login_choices[j]);
                    attroff(A_REVERSE);
                } else {
                    mvprintw(j, 0, "%s", login_choices[j]);
                }
            }
            refresh();

            choice = getch();
            switch (choice) {
                case KEY_UP:
                    highlight = (highlight == 0) ? login - 1 : highlight - 1;
                    break;
                case KEY_DOWN:
                    highlight = (highlight == login - 1) ? 0 : highlight + 1;
                    break;
                case 10:
                    int result = handle_login(login_choices, highlight, username);
                    if(result == -1){
                        return;
                    }
                    break;
            }
        }
    }
}

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    char *choices[] = {
        "Sign in",
        "Login",
        "Login as a guest",
        "Exit"
    };

    int n_choices = sizeof(choices) / sizeof(choices[0]);
    int highlight = 0;

    while (1) {
        clear();
        for (int i = 0; i < n_choices; i++) {
            if (i == highlight) {
                attron(A_REVERSE);
                mvprintw(i, 0, "%s", choices[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(i, 0, "%s", choices[i]);
            }
        }
        refresh();

        int choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight == 0) ? n_choices - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == n_choices - 1) ? 0 : highlight + 1;
                break;
            case 10: // Enter key
                if (strcmp(choices[highlight], "Exit") == 0) {
                    endwin();
                    return 0;
                } else {
                    handle_menu(choices, highlight);
                }
                break;
        }
    }

    endwin(); // خروج امن از ncurses
    return 0;
}


