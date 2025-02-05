#include <ncursesw/curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include "makemap.h"
#include "interance.h"
#include "creat_user.h"
#include <unistd.h>
#include <wchar.h>


newpoint whole_rooms[40][184][4];

int save_dimension1[5];

int save_dimension2[5];
int save_dimension3[5];


void start_game(){
    makemap();
    play_the_game();
}



void resume_game(){
    load_game();
    play_the_game();
    return ;
}
void print_weapon(){
    mvprintw(1,3,"Here you can see the number of your weapons.");
    if (user.weapon=='0')mvprintw(2,3, "You have no weapon in hand currently");
    else mvprintw(2,3, "Your current weeapon is %c", user.weapon);
    mvprintw(4,3,"Press Q to put the weapon in the backpack");

    mvprintw(5,3, "shortrange weapons: ");
    mvprintw(5, 30, "longrange weapons:");
    
    mvprintw (6,30, "| Dagger (D): %d ", user.selah[0]);
    
    mvprintw (6,3, "| Mace (M): ");
    if (user.selah[1]>0) printw("Yes");
    else printw("No");
    mvprintw (7,30, "| Magic wand (W): %d", user.selah[2]);
    mvprintw (8,30, "| Normal Arrow (N): %d", user.selah[3]);
    mvprintw (7,3, "| Sword (S): ");
    if (user.selah[4]>0) printw("Yes");
    else printw("No");
}

void print_potion(){

    mvprintw(1,3,"Here you can see the number of your potions.");
    mvprintw(2,3, "Press Q to take a potion");
    mvprintw(5,3, "| Health potion(H): %d", user.potion[0]);
    mvprintw(6,3, "| Damage potion(C): %d", user.potion[1]);
    mvprintw(7,3, "| Speed potion(V): %d", user.potion[2]);

}
int compare_users(const void *a, const void *b) {
    User *userA = (User *)a;
    User *userB = (User *)b;
    return userB->credit - userA->credit; 
}

void scorboard(const char *file_path) {
    char name[50];
    strcpy(name, user.username);
    start_color();
    
    mvprintw(3, 5, "Top 20 players are:");
    mvprintw(5, 5, "ranking");
    mvprintw(5, 15, "username");
    mvprintw(5, 40, "total_gold");
    mvprintw(5, 55, "experience");
    mvprintw(5,75, "total credit");
    mvprintw(6, 5, "---------------------------------------------------------------------------");

    User users[250];
    int user_count = 0;
    FILE *file=fopen(file_path, "r");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        User user1;
        sscanf(line, "%49[^,],%49[^,],%99[^,],%d,%d,%d",
               user1.username, user1.password, user.email, 
               &user1.gold, &user1.finished_games,&user1.credit);
        users[user_count++] = user1;
    }
    fclose(file);

    qsort(users, user_count, sizeof(User), compare_users);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    for (int i = 0; i < user_count && i < 20; i++) {
        if (i==0){
            attron(COLOR_PAIR(1));attron(A_ITALIC);
            
        }
      if (strcmp(users[i].username, name)==0)attron(A_BLINK); 
        if (i==1){
            attron(COLOR_PAIR(2));attron(A_ITALIC);
        }
        if (i==2){
            attron(COLOR_PAIR(3));attron(A_ITALIC);
        }
        mvprintw(i + 7, 5, "%d", i + 1);
        
        mvprintw(i + 7, 15, "%s", users[i].username); 
        if (i==0 )printw(" (Legend🏅)");
    
        if (i==1 )printw(" (Runner_up I🏅)");
        if (i==2 )printw(" (Runner_up II🏅)");
        mvprintw(i + 7, 40, "%d", users[i].gold); 
        mvprintw(i + 7, 55, "%d", users[i].finished_games); 
        mvprintw(i+7, 75, "%d", users[i].credit);

   if ( strcmp(users[i].username, name)==0)attroff(A_BLINK);
        if (i==0){
            attroff(COLOR_PAIR(1));attroff(A_ITALIC);
        }
        if (i==1){
            attroff(COLOR_PAIR(2));attroff(A_ITALIC);
        }
        if (i==2){
            attroff(COLOR_PAIR(3));attroff(A_ITALIC);
        }
    }

    refresh();
    return;
}


void setting (){
    int val=1;
    while(val){
        mvprintw(3,3, "choose the difficulty level of the game");
        mvprintw(4,3, "1: Easy");
        mvprintw(5,3, "2: Normal");
        mvprintw(6,3, "3: Hard");
        mvprintw(7,3,"Enter the number:");
        int how;
        scanw("%d", &how);
        switch (how)
        {
        case 1:
            user.life=70;
            val=0;
            break;
        case 2: 
            user.life=50;
            val=0;
            break;
        case 3:
            user.life=30;
            val=0;
            break;

        default:
            mvprintw (9,3,"Invalid command. try again");
            getch();
            clear();
            refresh();
            break;
        }
    } 
    val=1;
    while(val){
    mvprintw(3,50, "choose the color of your character");
    mvprintw(4,50, "1: blue");
    mvprintw(5,50, "2: yellow");
    mvprintw(6,50, "3: green");
    mvprintw(7,50, "4:white");

    mvprintw(8,50,"Enter the number:");
    int how;
    scanw("%d", &how);
    switch (how)
    {
    case 1:
        user.color='b';
        val=0;
        break;
    case 2: 
        user.color='y';
        val=0;
        break;
    case 3:
        user.color='g';
        val=0;
        break;
    case 4:
        user.color='w';
        val=0;
        break;

    default:
        mvprintw (10,50,"Invalid command. try again");
        getch();
        clear();
        //refresh();
        break;
    }
    }


    return ;
}

int is_username_taken_before(const char *username, const char *file_path) {
    FILE *file = fopen(file_path, "r");

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        char saved_username[50];
        sscanf(line, "%49[^,],", saved_username);
        if (strcmp(username, saved_username) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int tatabogh( const char *file_path) {
    FILE *file = fopen(file_path, "r");
    
    char line[256];
    char saved_name[50], saved_pass[50], saved_email[100];
    int gold, age, score; 

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,],%49[^,],%99[^,],%d,%d,%d", saved_name, saved_pass, saved_email, &gold, &age, &score);

        saved_name[strcspn(saved_name, "\n")] = '\0';
        saved_pass[strcspn(saved_pass, "\n")] = '\0';
        saved_email[strcspn(saved_email, "\n")] = '\0';

        if (strcmp(user.username, saved_name) == 0 && strcmp(user.password, saved_pass) == 0) {
          
            strcpy(user.email, saved_email);
            user.finished_games=age;
            user.credit=score;
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
void findpass( const char *file_path) {
    FILE *file = fopen(file_path, "r");
    
    char line[256];
    char saved_name[50], saved_pass[50], saved_email[100];
    int gold, age, score; 

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,],%49[^,],%99[^,],%d,%d,%d", saved_name, saved_pass, saved_email, &gold, &age, &score);

        saved_name[strcspn(saved_name, "\n")] = '\0';
        saved_pass[strcspn(saved_pass, "\n")] = '\0';
        saved_email[strcspn(saved_email, "\n")] = '\0';

        if (strcmp(user.username, saved_name) == 0) {
          
            strcpy(user.email, saved_email);
            mvprintw(5,5,"Your password is: %s                                 ", saved_pass);
            fclose(file);
            return;
        }
    }

    fclose(file);
    return ;
}


int is_email_valid(char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return at && dot && at < dot && dot - at > 1 && strlen(dot + 1) > 1;
}

int is_password_valid(const char *password) {
    if (strlen(password) < 7) return 0;

    int digit = 0, upper = 0, lower = 0;
    for (int i = 0; password[i]; i++) {
        if (isdigit(password[i])) digit = 1;
        if (isupper(password[i])) upper = 1;
        if (islower(password[i])) lower = 1;
    }
    return digit && upper && lower;
}

int is_username_taken(const char *username, const char *file_path) {
    FILE *file = fopen(file_path, "r");

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        char saved_username[50];
        sscanf(line, "%49[^,],", saved_username);
        if (strcmp(username, saved_username) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void save_user(const User *user, const char *file_path) {
    FILE *file = fopen(file_path, "a");
    if (file) {
        fprintf(file, "%s,%s,%s,%d,%d,%d\n", user->username, user->password, user->email, user->gold, user->finished_games, user->credit);
        fclose(file);
    }
}

void monsfollowh(int nowposx, int nowposy){
    if (whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname!='0' && whole_rooms[nowposx-5-1][nowposy-1][user.floor].type!='|' && (whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname=='s' || whole_rooms[nowposx-5-1][nowposy][user.floor].monster.movement<5)){       
        whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster=whole_rooms[nowposx-5-1][nowposy][user.floor].monster;
        whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname='0';
        whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname!='0' && whole_rooms[nowposx-5][nowposy-1-1][user.floor].type!='|' && (whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname=='s' || whole_rooms[nowposx-5][nowposy-1][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5][nowposy-1-1][user.floor].monster=whole_rooms[nowposx-5][nowposy-1][user.floor].monster;
        whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname='0';
        whole_rooms[nowposx-5][nowposy-1-1][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname!='0' &&  whole_rooms[nowposx-5+1][nowposy-1][user.floor].type!='|' && (whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname=='s' || whole_rooms[nowposx-5+1][nowposy][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster=whole_rooms[nowposx-5+1][nowposy][user.floor].monster;
        whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname='0';
        whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname!='0' && whole_rooms[nowposx-5][nowposy][user.floor].type!='|' &&(whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname=='s' || whole_rooms[nowposx-5][nowposy+1][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5][nowposy][user.floor].monster=whole_rooms[nowposx-5][nowposy+1][user.floor].monster;
        whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname='0';
        whole_rooms[nowposx-5][nowposy][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    print_map(whole_rooms, user.floor);
    
}
void monsfollowl(int nowposx, int nowposy){
    if (whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname!='0' && whole_rooms[nowposx-5-1][nowposy+1][user.floor].type!='|' && (whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname=='s' || whole_rooms[nowposx-5-1][nowposy][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster=whole_rooms[nowposx-5-1][nowposy][user.floor].monster;
        whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname='0';
        whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname!='0' &&  whole_rooms[nowposx-5][nowposy-1+1][user.floor].type!='|' && (whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname=='s' || whole_rooms[nowposx-5][nowposy-1][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5][nowposy-1+1][user.floor].monster=whole_rooms[nowposx-5][nowposy-1][user.floor].monster;
        whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname='0';
        whole_rooms[nowposx-5][nowposy-1+1][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname!='0' && whole_rooms[nowposx-5+1][nowposy+1][user.floor].type!='|' && (whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname=='s' || whole_rooms[nowposx-5+1][nowposy][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster=whole_rooms[nowposx-5+1][nowposy][user.floor].monster;
        whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname='0';
        whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname!='0' && whole_rooms[nowposx-5][nowposy+1+1][user.floor].type!='|' && (whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname=='s' || whole_rooms[nowposx-5][nowposy+1][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5][nowposy+1+1][user.floor].monster=whole_rooms[nowposx-5][nowposy+1][user.floor].monster;
        whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname='0';
        whole_rooms[nowposx-5][nowposy+1+1][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    print_map(whole_rooms, user.floor);
    
}
void monsfollowj(int nowposx, int nowposy){
    if (whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname!='0' && whole_rooms[nowposx-5-1-1][nowposy][user.floor].type!='_'  &&(whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname=='s' || whole_rooms[nowposx-5-1][nowposy][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5-1-1][nowposy][user.floor].monster=whole_rooms[nowposx-5-1][nowposy][user.floor].monster;
        whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname='0';
        whole_rooms[nowposx-5-1-1][nowposy][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname!='0' && whole_rooms[nowposx-5-1][nowposy-1][user.floor].type!='_' && (whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname=='s' || whole_rooms[nowposx-5][nowposy-1][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster=whole_rooms[nowposx-5][nowposy-1][user.floor].monster;
        whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname='0';
        whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname!='0' && whole_rooms[nowposx-5][nowposy][user.floor].type!='_' && (whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname=='s' || whole_rooms[nowposx-5+1][nowposy][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5][nowposy][user.floor].monster=whole_rooms[nowposx-5+1][nowposy][user.floor].monster;
        whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname='0';
        whole_rooms[nowposx-5][nowposy][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname!='0' && whole_rooms[nowposx-5-1][nowposy+1][user.floor].type!='_' && (whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname=='s' || whole_rooms[nowposx-5][nowposy+1][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster=whole_rooms[nowposx-5][nowposy+1][user.floor].monster;
        whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname='0';
        whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    print_map(whole_rooms, user.floor);
    
}
void monsfollowk(int nowposx, int nowposy){
    if (whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname!='0' && whole_rooms[nowposx-5][nowposy][user.floor].type!='_' && (whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname=='s' || whole_rooms[nowposx-5-1][nowposy][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5][nowposy][user.floor].monster=whole_rooms[nowposx-5-1][nowposy][user.floor].monster;
        whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname='0';
        whole_rooms[nowposx-5][nowposy][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname!='0' && whole_rooms[nowposx-5+1][nowposy-1][user.floor].type!='_' && (whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname=='s' || whole_rooms[nowposx-5][nowposy-1][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster=whole_rooms[nowposx-5][nowposy-1][user.floor].monster;
        whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname='0';
        whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname!='0' && whole_rooms[nowposx-5+2][nowposy][user.floor].type!='_' && (whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname=='s' || whole_rooms[nowposx-5+1][nowposy][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5+2][nowposy][user.floor].monster=whole_rooms[nowposx-5+1][nowposy][user.floor].monster;
        whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname='0';
        whole_rooms[nowposx-5+2][nowposy][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    if (whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname!='0' && whole_rooms[nowposx-5+1][nowposy+1][user.floor].type!='_' && (whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname=='s' || whole_rooms[nowposx-5][nowposy+1][user.floor].monster.movement<5)){
        whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster=whole_rooms[nowposx-5][nowposy+1][user.floor].monster;
        whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname='0';
        whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.movement++;
        user.life--;
        mvprintw(1,3, "You were hurt by the enemy!");
    }
    print_map(whole_rooms, user.floor);
    
}
void creat_user() {
    char file_path[] = "users.txt";
    char error_message[100] = "";
    int valid = 0;

    initscr();
    cbreak();
    noecho();

    while (!valid) {
        clear();
        mvprintw(1, 5, "Let's get started!");

        mvprintw(3, 5, "username: ");
        echo();
        getnstr(user.username, 49);
        noecho();
        

        if (is_username_taken(user.username, file_path)) {
            strcpy(error_message, "Username already taken!");
            mvprintw(6, 5, "%s", error_message);
            refresh();
            getch();
            continue;
        }

        mvprintw(4, 5, "password: ");
        echo();
        getnstr(user.password, sizeof(user.password) - 1);
        noecho();

        if (!is_password_valid(user.password)) {
            strcpy(error_message, "week password!");
            mvprintw(6, 5, "%s", error_message);
            refresh();
            getch();
            continue;
        }

        mvprintw(5, 5, "email: ");
        echo();
        getnstr(user.email, sizeof(user.email) - 1);
        noecho();

        if (!is_email_valid(user.email)) {
            strcpy(error_message, " invalid email!");
            mvprintw(6, 5, "%s", error_message);
            refresh();
            getch();
            continue;
        }
        
        valid = 1;
    }   
    user.gold=0;
        user.finished_games=0;
        user.food=0;
        user.life=50;
        user.weapon='M';
        user.floor=0;
        user.credit=0;

        

    
    save_user(&user, file_path);
    mvprintw(7, 5, "you are registered!");
    //mvprintw(8,3,"%d", user.life);
    refresh();
    getch();
    //pregamemenu();
    endwin();

}
int max (int a, int b){
    if (a>b){
        return a;
    }
    else{
        return b;
    }
}
int min (int a, int b){
    if (a<b){
        return a;
    }
    else {
        return b;
    }
}
void print_map(newpoint whole_rooms[40][184][4],int s){
     start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK); 
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4,COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
    init_pair(6,COLOR_BLACK, COLOR_YELLOW);
    for (int i=0; i<40; i++){
        for (int j=0; j<184; j++){
            if (whole_rooms[i][j][s].being==1){
                if (whole_rooms[i][j][s].model=='T')attron(COLOR_PAIR(1));
                if (whole_rooms[i][j][s].model=='E')attron(COLOR_PAIR(2));
                if (whole_rooms[i][j][s].type=='R')attron(COLOR_PAIR(6));
                if (whole_rooms[i][j][s].type=='r')attron(COLOR_PAIR(3));
                if (whole_rooms[i][j][s].type=='@' && user.color=='b')attron(COLOR_PAIR(4));
                if (whole_rooms[i][j][s].type=='@' && user.color=='y')attron(COLOR_PAIR(3));
                if (whole_rooms[i][j][s].type=='@' && user.color=='w')attron(COLOR_PAIR(5));
                if (whole_rooms[i][j][s].type=='@' && user.color=='g')attron(COLOR_PAIR(2));
                if (whole_rooms[i][j][s].type=='D' || whole_rooms[i][j][s].type=='M'||whole_rooms[i][j][s].type=='W'||whole_rooms[i][j][s].type=='N'||whole_rooms[i][j][s].type=='S'){attron(COLOR_PAIR(4));attron(A_BOLD);}
                if (whole_rooms[i][j][s].type=='H' || whole_rooms[i][j][s].type=='C'||whole_rooms[i][j][s].type=='V'){attron(COLOR_PAIR(1));attron(A_BOLD);}
                if (whole_rooms[i][j][s].type=='Y' || whole_rooms[i][j][s].type=='X'||whole_rooms[i][j][s].type=='F')attron(COLOR_PAIR(2));

                mvprintw(i+5, j, "%c", whole_rooms[i][j][s].type);
                if (whole_rooms[i][j][s].monster.mname!='0'){
                mvprintw(i+5, j,"%c", whole_rooms[i][j][s].monster.mname);
                }
                if (whole_rooms[i][j][s].model=='T')attroff(COLOR_PAIR(1));
                if (whole_rooms[i][j][s].model=='E')attroff(COLOR_PAIR(2));
                if (whole_rooms[i][j][s].type=='R')attroff(COLOR_PAIR(6));
                if (whole_rooms[i][j][s].type=='r')attroff(COLOR_PAIR(3));
                if (whole_rooms[i][j][s].type=='@' && user.color=='b')attroff(COLOR_PAIR(4));
                if (whole_rooms[i][j][s].type=='@' && user.color=='y')attroff(COLOR_PAIR(3));
                if (whole_rooms[i][j][s].type=='@' && user.color=='w')attroff(COLOR_PAIR(5));
                if (whole_rooms[i][j][s].type=='@' && user.color=='g')attroff(COLOR_PAIR(2));
                if (whole_rooms[i][j][s].type=='D' || whole_rooms[i][j][s].type=='M'||whole_rooms[i][j][s].type=='W'||whole_rooms[i][j][s].type=='N'||whole_rooms[i][j][s].type=='S'){attroff(COLOR_PAIR(4));attroff(A_BOLD);}
                
                if (whole_rooms[i][j][s].type=='H' || whole_rooms[i][j][s].type=='C'||whole_rooms[i][j][s].type=='V'){attroff(COLOR_PAIR(1));attroff(A_BOLD);}
                if (whole_rooms[i][j][s].type=='Y' || whole_rooms[i][j][s].type=='X'||whole_rooms[i][j][s].type=='F')attroff(COLOR_PAIR(2));

            }
            

        }
    }

}
void makemap(){   
    initscr();

    srand(time(NULL));

    /*for (int i=0; i<40; i++){
        for (int j=0; j<184; j++){
            whole_rooms[i][j][4].type=' ';
            whole_rooms[i][j][4].being=1;
            whole_rooms[i][j][4].model='T';
            whole_rooms[i][j][4].trap=0;
            whole_rooms[i][j][4].monster.mname='0';
        }
    }
    for (int i=0; i<40; i++){
        whole_rooms[10][i+50][4].type='_';
        whole_rooms[30][i+50][4].type='_';
    }
    for (int i=0; i<20; i++){
        whole_rooms[11+i][49][4].type='|';
        whole_rooms[11+i][91][4].type='|';
    }
    for (int i=11; i<30; i++){
        for (int j=51; j<90; j++){
            whole_rooms[i][j][4].type='.';
        }
    }

    for (int i=0; i<100; i++){
        whole_rooms[rand()%16+12][rand()%36+53][4].trap=1;
    }
    for(int i=0; i<2; i++){
        whole_rooms[rand()%16+12][rand()%36+53][4].monster.mname='d';
        whole_rooms[rand()%16+12][rand()%36+53][4].monster.mname='u';
        whole_rooms[rand()%16+12][rand()%36+53][4].monster.mname='s';
        whole_rooms[rand()%16+12][rand()%36+53][4].monster.mname='f';
        whole_rooms[rand()%16+12][rand()%36+53][4].monster.mname='g';
    }
    for (int i=11; i<30; i++){
        for (int j=51; j<90; j++){
            if (whole_rooms[i][j][4].monster.mname!='0'){

                whole_rooms[i][j][4].monster.x=i;
                whole_rooms[i][j][4].monster.y=j;
                whole_rooms[i][j][4].monster.lifee=5;
                whole_rooms[i][j][4].monster.movement=0;
            }
        }
    }*/

    for (int s=0; s<4; s++){
        for (int i=0; i<40; i++){
            for (int j=0; j<184; j++){
                whole_rooms[i][j][s].trap=0;
                whole_rooms[i][j][s].being=0;
                whole_rooms[i][j][s].type=' ';
                whole_rooms[i][j][s].monster.mname='0';
            }
        }
        //mvprintw(40,1 ,"ah" );
        int addx[6] = {0, 0, 0, 20, 20, 20};
        int addy[6] = {0, 50, 100, 0, 50, 100};
        room allrooms[6];
        
        int x=1;
        for (int h = 0; h < 6; h++) {
            room room1;
            room1.room_number=x;
            room1.type='R';
            room1.y = rand() % 18+2;
            room1.x = rand() % 4+2;
            room1.arz = rand() % 8 + 5;
            while (room1.arz + room1.x >= 17) {
                room1.arz = rand() % 8 + 5;
            }
            room1.tool = rand() % 30 + 5;
            while (room1.tool + room1.y >= 55) {
                room1.tool = rand() % 20 + 5;

            }
            if (s==0 && room1.room_number==6 ){
                save_dimension1[0]=room1.y;
                save_dimension1[1]=room1.x;
                save_dimension1[2]=room1.arz;
                save_dimension1[3]=room1.tool;
            }
            if (s==1 && room1.room_number==4 ){
                save_dimension2[0]=room1.y;
                save_dimension2[1]=room1.x;
                save_dimension2[2]=room1.arz;
                save_dimension2[3]=room1.tool;
            }
            if (s==2 && room1.room_number==3 ){
                save_dimension3[0]=room1.y;
                save_dimension3[1]=room1.x;
                save_dimension3[2]=room1.arz;
                save_dimension3[3]=room1.tool;

            }
            if ( (s==1 && room1.room_number==6) ){
                room1.y=save_dimension1[0];
                room1.x=save_dimension1[1];
                room1.arz=save_dimension1[2];
                room1.tool=save_dimension1[3];
            }
            if ( (s==2 && room1.room_number==4) ){
                room1.y=save_dimension2[0];
                room1.x=save_dimension2[1];
                room1.arz=save_dimension2[2];
                room1.tool=save_dimension2[3];
            }
            if ( (s==3 && room1.room_number==3) ){
                room1.y=save_dimension3[0];
                room1.x=save_dimension3[1];
                room1.arz=save_dimension3[2];
                room1.tool=save_dimension3[3];
            }

            room1.x += addx[h];
            room1.y += addy[h];
            

            
            room1.doorx = rand() % (room1.tool-1) +1;
            room1.doory = rand() % (room1.arz-1) +1;

            
           
            allrooms[h] = room1;

            char room1arr[room1.arz+1 ][room1.tool +1];
            room1arr[0][0] = ' ';
            room1arr[0][room1.tool] = ' ';
            for (int i = 1; i <= room1.arz; i++) {
                room1arr[i][0] = '|';
                room1arr[i][room1.tool] = '|';
            }
            for (int i = 1; i < room1.tool; i++) {
                room1arr[0][i] = '_';
                room1arr[room1.arz][i] = '_';
            }
            int coin=rand()%2;

            if (room1.room_number==1){
                room1arr[room1.arz][room1.doorx]='+';
                room1arr[room1.doory][room1.tool]='+';
            }
            else if (room1.room_number==2){
                room1arr[room1.doory][0]='+';
                room1arr[room1.arz][room1.doorx]='+';
                room1arr[room1.doory][room1.tool]='+';
            }
            else if (room1.room_number==3){
                room1arr[room1.arz][room1.doorx]='+';
                room1arr[room1.doory][0]='+';
            }
            else if (room1.room_number==4){
                room1arr[0][room1.doorx]='+';
                room1arr[room1.doory][room1.tool]='+';

            }
            else if (room1.room_number==5){
                room1arr[0][room1.doorx]='+';
                room1arr[room1.doory][0]='+';
                room1arr[room1.doory][room1.tool]='+';
            }
            else if (room1.room_number==6){
                room1arr[0][room1.doorx]='+';
                room1arr[room1.doory][0]='+';
            }

            for (int i = 1; i < room1.arz; i++) {
                for (int j = 1; j < room1.tool; j++) {
                    room1arr[i][j] = '.';
                }
            }
            if (coin==1){
                for (int i=0; i<(rand()%2)+1; i++){
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='F';
                room1arr[room1.arz/3][room1.tool/3]='O';
                }
            }
            
            
            coin=(rand()%3)+1;
            if (room1.room_number%3+1==coin){
                room1.type='E';
            }
            
            
            if (room1.room_number==5 && s==3){
                room1.type='T';
            }
            

           /* coin=rand()%3;
            if (coin==1){
                for (int i=0; i<(rand()%2)+1; i++){
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='R';}//darkgold
            }*/

            coin=rand()%4;
            if (coin==1){
                for (int i=0; i<(rand()%2)+1; i++){
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='R';
                
                }//blackgold
            }
            coin=rand()%3;
            if (coin==1){
                for (int i=0; i<(rand()%2)+1; i++){
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='D';
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='H';
                }//dagger & healthpotion
            }
            coin=rand()%3;
            if (coin==1){
                for (int i=0; i<(rand()%2)+1; i++){
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='W';
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='V';}//magic wand & speedpotion
            }
            coin=rand()%3;
            if (coin==1){
                for (int i=0; i<(rand()%2)+1; i++){
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='N';
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='C';}//normal arrow & damagepotion
            }
            coin=rand()%3;
            if (coin==1){
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='S';//sword
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='X';//magicfood
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='Y';//perfectfood
            }


            coin=rand()%2;
            if (coin==1){
                coin=rand()%3+1;
                if (room1.type=='T')coin+=3;
                for (int i=0; i<coin; i++){
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='T';
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='r';}//trap & gold
            }
            if (room1.type!='E'){
                coin=rand()%2;
                if (coin==1){
                    room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='d';//deamon
                }
                coin=rand()%2;
                if (coin==1){
                    room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='u';//undeed
                }
                coin=rand()%3;
                if (coin==1){
                    room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='s';//snake
                }
                coin=rand()%2;
                if (coin==1){
                    room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='f';//firemonster
                }
                coin=rand()%2;
                if (coin==1){
                    room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='g';//giant
                }
            }
            if (room1.type=='T'){
                room1arr[rand()%(room1.arz-2)+1][rand()%(room1.tool-2)+1]='$';
            }
            

            if (s==0 && room1.room_number==6 || s==1 && room1.room_number==4 ||s==2 && room1.room_number==3){
                room1arr[room1.arz/2][room1.tool/2]='=';
            }

            
            for (int i = 0; i <= room1.arz; i++) {
                for (int j = 0; j <= room1.tool; j++) {
                    whole_rooms[room1.x+i][room1.y+j][s].type=room1arr[i][j];  
                    if (room1.type=='E'){
                        whole_rooms[room1.x+i][room1.y+j][s].model='E'; 
                    }
                    if (room1.type=='T'){
                        whole_rooms[room1.x+i][room1.y+j][s].model='T'; 
                    }
                    if (room1.type=='R'){
                        whole_rooms[room1.x+i][room1.y+j][s].model='R'; 
                    }
                    //mvprintw(room1.x + i, room1.y + j, "%c", room1arr[i][j]);
                    if (room1arr[i][j]=='d'){
                        whole_rooms[room1.x+i][room1.y+j][s].monster.mname='d';
                        whole_rooms[room1.x+i][room1.y+j][s].monster.x=room1.x+i;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.y=room1.y+j;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.lifee=5;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.movement=0;
                        whole_rooms[room1.x+i][room1.y+j][s].type='.';}
                    if (room1arr[i][j]=='f'){
                        whole_rooms[room1.x+i][room1.y+j][s].monster.mname='f';
                        whole_rooms[room1.x+i][room1.y+j][s].monster.x=room1.x+i;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.y=room1.y+j;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.lifee=10;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.movement=0;

                        whole_rooms[room1.x+i][room1.y+j][s].type='.';}
                    if (room1arr[i][j]=='g'){
                        whole_rooms[room1.x+i][room1.y+j][s].monster.mname='g';
                        whole_rooms[room1.x+i][room1.y+j][s].monster.x=room1.x+i;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.y=room1.y+j;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.lifee=15;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.movement=0;
                        whole_rooms[room1.x+i][room1.y+j][s].type='.';}
                    if (room1arr[i][j]=='s'){
                        whole_rooms[room1.x+i][room1.y+j][s].monster.mname='s';
                        whole_rooms[room1.x+i][room1.y+j][s].monster.x=room1.x+i;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.y=room1.y+j;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.lifee=20;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.movement=0;
                        whole_rooms[room1.x+i][room1.y+j][s].type='.';}
                    if (room1arr[i][j]=='u'){
                        whole_rooms[room1.x+i][room1.y+j][s].monster.mname='u';
                        whole_rooms[room1.x+i][room1.y+j][s].monster.x=room1.x+i;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.y=room1.y+j;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.lifee=30;
                        whole_rooms[room1.x+i][room1.y+j][s].monster.movement=0;
                        whole_rooms[room1.x+i][room1.y+j][s].type='.';}
                    
                    //کپی اتاقا
                    if (room1arr[i][j]=='T'){
                        whole_rooms[room1.x+i][room1.y+j][s].trap=1; 
                        whole_rooms[room1.x+i][room1.y+j][s].type='.';}
                    
                    if (room1.room_number==1 && s==0){
                        whole_rooms[room1.x+i][room1.y+j][s].being=1;
                        whole_rooms[room1.x+1][room1.y+1][s].type='@';
                    }
                    
                }
            }
            x++;
        }
        
        
        x=0;
        for (int j=0; j<3; j++){
            int tooly= max(allrooms[x+3].y-allrooms[x].y+allrooms[x+3].doorx-allrooms[x].doorx, -(allrooms[x+3].y-allrooms[x].y+allrooms[x+3].doorx-allrooms[x].doorx));
            point coriddor_1 [ allrooms[x+3].x -allrooms[x].x -allrooms[x].arz + tooly+1];
            int midpoint_x = (allrooms[x+3].x + allrooms[x].x + allrooms[x].arz) / 2;
            int wr = 0;

            for (int i = 0; i < (allrooms[x+3].x - allrooms[x].x - allrooms[x].arz) / 2 +1; i++) {
                coriddor_1[wr].x = i + allrooms[x].x + allrooms[x].arz;
                coriddor_1[wr].y = allrooms[x].y + allrooms[x].doorx;
                wr++;
            }

            int door_difference = abs(allrooms[x+3].doorx + allrooms[x+3].y - allrooms[x].doorx - allrooms[x].y);
            for (int i = 0; i < door_difference; i++) {
                coriddor_1[wr].x = midpoint_x;
                coriddor_1[wr].y = min(allrooms[x+3].y + allrooms[x+3].doorx, allrooms[x].y + allrooms[x].doorx) + i;
                wr++;
            }

            for (int i = 0; i < (allrooms[x+3].x - allrooms[x].x - allrooms[x].arz)- (allrooms[x+3].x - allrooms[x].x - allrooms[x].arz) / 2; i++) {
                coriddor_1[wr].x = i + midpoint_x;
                coriddor_1[wr].y = allrooms[x+3].y + allrooms[x+3].doorx;
                wr++;
            }

            for (int i = 1; i < wr; i++) {
                //mvprintw(coriddor_1[i].x, coriddor_1[i].y, "#");
                whole_rooms[coriddor_1[i].x][coriddor_1[i].y][s].type='#';//کپی راهروها
            }
            x++;
        }

        x = 0;
        for (int j = 0; j < 4; j++) {
            int tooly = abs(allrooms[x + 1].x - allrooms[x].x + allrooms[x + 1].doory - allrooms[x].doory);
            point coriddor_1[allrooms[x + 1].y - allrooms[x].y - allrooms[x].tool + tooly];
            int midpoint_y = (allrooms[x + 1].y + allrooms[x].y + allrooms[x].tool) / 2;
            int wr = 0;

            for (int i = 0; i < (allrooms[x + 1].y - allrooms[x].y - allrooms[x].tool) / 2+1; i++) {
                coriddor_1[wr].y = i + allrooms[x].y + allrooms[x].tool;
                coriddor_1[wr].x = allrooms[x].x + allrooms[x].doory;
                wr++;
            }

            int door_difference = abs(allrooms[x + 1].doory + allrooms[x + 1].x - allrooms[x].doory - allrooms[x].x);
            for (int i = 0; i < door_difference; i++) {
                coriddor_1[wr].y = midpoint_y;
                coriddor_1[wr].x = min(allrooms[x + 1].x + allrooms[x + 1].doory, allrooms[x].x + allrooms[x].doory) + i;
                wr++;
            }
        
            for (int i = 0; i < (allrooms[x + 1].y - allrooms[x].y - allrooms[x].tool) - (allrooms[x + 1].y - allrooms[x].y - allrooms[x].tool) / 2 ; i++) {
                coriddor_1[wr].y = i + midpoint_y;
                coriddor_1[wr].x = allrooms[x + 1].x + allrooms[x + 1].doory;
                wr++;
            }

            for (int i = 1; i < wr; i++) {
                //mvprintw(coriddor_1[i].x, coriddor_1[i].y, "#");

                whole_rooms[coriddor_1[i].x][coriddor_1[i].y][s].type='#';//کپی راهروها

            }
            x++;
            if (x == 2) {
                x++;
            }
        }
    }
    
    print_map(whole_rooms, user.floor);


}
void apearroom(int nowposx, int nowposy, int s) {
    //mvprintw(1,1, "%d    %d   %d", nowposx, nowposy, s);
    int m = nowposx-5, n = nowposy;
    int left = n, right = n, top = m, bottom = m;

    while (whole_rooms[top][n][s].type != ' ' && whole_rooms[top][n][s].type != '#') top--;
    while (whole_rooms[bottom][n][s].type != ' ' && whole_rooms[bottom][n][s].type != '#') bottom++;
    while (whole_rooms[m][left][s].type != ' ' && whole_rooms[m][left][s].type != '#') left--;
    while (whole_rooms[m][right][s].type != ' ' && whole_rooms[m][right][s].type != '#') right++;
    
    for (int i = top + 1; i < bottom; i++) {
        for (int j = left + 1; j < right; j++) {
            whole_rooms[i][j][s].being = 1;
        }
    }
    print_map(whole_rooms, s);
    return;
}


void updateuser() {
    FILE *file = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    char line[300];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char temp[300];
        strcpy(temp, line); 

        char *token = strtok(temp, ","); 

        if (token && strcmp(token, user.username) == 0) {
            fprintf(tempFile, "%s,%s,%s,%d,%d,%d\n",
                    user.username, user.password, user.email, user.gold, user.finished_games, user.credit);
            found = 1;
        } else {
            fprintf(tempFile, "%s", line); 
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("users.txt");             
    rename("temp.txt", "users.txt");   
}

void losethegame(){

    char name[50];
    strcpy(name, user.username);
    strcat(name, ".txt");
    FILE *file = fopen(name, "w");
    fprintf(file, "endofgame");
    fclose(file);
    clear();
    user.finished_games++;
    user.credit+=user.gold;
    mvprintw(3,3, "You lost the game! your life was finished");
    mvprintw( 5,3, "Your score: %d", user.credit);
    mvprintw (7, 3, "Press S to go back to the Start Menu");
    updateuser();
    char back;
    while (1){
        back=getch();
        if (back=='S'){
            pregamemenu();
        }
    }

}

void winthegame(){

    char name[50];
    strcpy(name, user.username);
    strcat(name, ".txt");
    FILE *file = fopen(name, "w");
    fprintf(file, "endofgame");
    fclose(file);
    clear();
    user.finished_games++;
    user.credit+=user.gold+500;
    mvprintw(3,3, "You won the game! Your performance was commendable");
    mvprintw( 5,3, "Your score: %d", user.credit);
    mvprintw (7, 3, "Press S to go back to the Start Menu");
    updateuser();
    char back;
    while (1){
        back=getch();
        if (back=='S'){
            pregamemenu();
        }
    }

}

int countdemon(int nowposx, int nowposy, int s){
    int m = nowposx-5, n = nowposy;
    int left = n, right = n, top = m, bottom = m;

    while (whole_rooms[top][n][s].type != ' ' && whole_rooms[top][n][s].type != '#') top--;
    while (whole_rooms[bottom][n][s].type != ' ' && whole_rooms[bottom][n][s].type != '#') bottom++;
    while (whole_rooms[m][left][s].type != ' ' && whole_rooms[m][left][s].type != '#') left--;
    while (whole_rooms[m][right][s].type != ' ' && whole_rooms[m][right][s].type != '#') right++;
    int counter=0;
    for (int i = top + 1; i < bottom; i++) {
        for (int j = left + 1; j < right; j++) {
            if (whole_rooms[i][j][s].monster.mname != '0')counter++;
        }
    }
    
    print_map(whole_rooms, s);
    if (counter==0)return 1;
    return 0;
}


void play_the_game() {

    int nowposx, nowposy;
    for (int i=0; i<40; i++){
        for (int j=0; j<184; j++){
            if (whole_rooms[i][j][user.floor].type=='@'){
                nowposx= i+5;
                nowposy= j;
                break;
            }
        }
    }
    noecho();


    
    user.selah[1]=1;
    user.selah[0]=0;user.selah[2]=0;user.selah[3]=0;user.selah[4]=0;
    int conti=1;
    char how_move;
    int hungercounter=400;
    int catch;
    int lifeadder=0;
    int potioncounter=0;
    int speedpot=0;
    while (conti){

    if(user.potionn!='0') potioncounter++;
    if (potioncounter==20){
        user.potionn='0';
        potioncounter=0;
    }
    if (user.potionn=='V')speedpot++;
    
    how_move= getch();
    mvprintw(1, 100 ,"you have %d life remained", user.life);
    mvprintw(2,100, "Health = %d  ", hungercounter);
    mvprintw(3,100, "Score = %d",user.credit );
    mvprintw(4,100, "Potion = %c", user.potionn);

     mvprintw(1,1, "                                                                                 ");
     mvprintw(2,1, "                                                                                 ");
    if (hungercounter>0){
        if (user.potionn=='V' && speedpot==2){
            speedpot=0;
             hungercounter--;
        }
        else if (user.potionn!='V') hungercounter--;
    }
    
    if (user.life==0){
        losethegame();
    }
    if (hungercounter==0){
        user.life--;
    }
    if (hungercounter>200){
        lifeadder++;
        if (lifeadder==3 && user.potionn=='H'){
            lifeadder=0;
            if (user.life<50)user.life++;
        }
        else if (lifeadder==5){
            lifeadder=0;
            if (user.life<50)user.life++;
        }
    }

    switch(how_move){
        case 'Z':
            FILE* file;
            char name[50];
            
            strcpy(name, user.username);
            strcat(name, ".txt");
            file =fopen (name, "w");
            fprintf (file, "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
            user.username, user.food, user.gold, user.life, user.selah[0],user.selah[1],user.selah[2],user.selah[3],user.selah[4],user.credit,
            user.potion[0], user.potion[1], user.potion[2],user.floor, user.finished_games );
            for (int s=0; s<4; s++){
                for (int i = 0; i < 40; i++) {
                    for (int j = 0; j < 184; j++) {
                        fprintf(file, "%d,%c,%d,%c,%d,%d,%d,%d,%c\n", 
                        whole_rooms[i][j][s].being, whole_rooms[i][j][s].type, whole_rooms[i][j][s].trap, whole_rooms[i][j][s].monster.mname
                        , whole_rooms[i][j][s].monster.movement, whole_rooms[i][j][s].monster.lifee,
                         whole_rooms[i][j][s].monster.x, whole_rooms[i][j][s].monster.y, whole_rooms[i][j][s].model);
                    }
                }
            }
            
            //char fullPath[100];
            //getcwd(fullPath, sizeof(fullPath)); // دریافت مسیر فعلی برنامه
            mvprintw(1,1,"File saved in: %s\n", name);
            //mvprintw(1,1, "%s", user.username);
            refresh();

            fclose(file);
             conti=0;
             break;

        case 'E' : 
            clear();
            
            mvprintw(1,1,"Please press the name of considered food");
            mvprintw(2,1,"Health:");
            int koochik=0;
            for (int i=0; i<hungercounter; i++){
                koochik++;
                if (koochik==5){
                koochik=0;
                mvprintw(2, i/5+11, "+");
                }
            }
            mvprintw(3,1, "you have %d foods", user.food);
            mvprintw(5,1, "Normal Food(F): %d", user.foods[0]);
            mvprintw(6,1, "Magic Food(X): %d", user.foods[1]);
            mvprintw(7,1, "Excellent Food(Y): %d", user.foods[2]);
            char type;
            if (user.food>0){
                type=getch();
                if (type=='X' && user.foods[1]>0){user.potionn='V'; user.foods[1]--; }
                else if (type=='Y' && user.foods[2]>0){user.potionn='C'; user.foods[2]--; }
                else if (type=='F' && user.foods[0]>0){user.foods[0]--; }
               user.food--; 
               hungercounter+=100;
               if (hungercounter>250)hungercounter=250;
            }
            refresh();  
            getchar();
            clear();
            refresh();
            print_map(whole_rooms, user.floor);
            break;

        case 'I' : 
            clear();
            print_weapon();
        
            refresh(); 
            char putornot= getch();
            if (putornot=='Q'){
                mvprintw(1,3, "Enter the considered weapon                                              ");
                if (user.weapon=='D')user.selah[0]++;
                else if (user.weapon=='N')user.selah[3]++;
                else if (user.weapon== 'W')user.selah[2]++;
                user.weapon='0';
                putornot=getch();
                if (putornot=='D' && user.selah[0]>0)
                {user.weapon='D';
                user.selah[0]--;}
                else if (putornot=='M')user.weapon='M';
                else if (putornot=='W' && user.selah[2]>0){
                    user.weapon='W';
                    user.selah[2]--;
                }
                else if (putornot=='N' && user.selah[3]>0)
                {user.weapon='N';
                user.selah[3]--;
                }
                else if (putornot=='S' && user.selah[4]>0)user.weapon='S';
            }
            getchar();

            clear();
            refresh();
            print_map(whole_rooms, user.floor);
            break;
        
        case 'Y':
            clear();
            print_potion();
            refresh();
            char takeornot=getch();
            if (takeornot=='Q'){
                mvprintw(1,3,"Enter the considered potion                          ");
                takeornot=getch();
                if (takeornot=='H' && user.potion[0]>0){
                    user.potionn='H';
                    user.potion[0]--;
                }
                else if (takeornot=='C' && user.potion[1]>0){
                    user.potionn='C';
                    user.potion[1]--;
                }
                else if (takeornot=='V' && user.potion[2]>0){
                    user.potionn='V';
                    user.potion[2]--;
                }
            }
            getchar();
            clear();
            refresh();
            print_map(whole_rooms, user.floor);
            break;

        
        case ' ':
            if (user.weapon== 'M' || user.weapon=='S') {
                if (whole_rooms[nowposx+1-5][nowposy][user.floor].monster.mname!='0'){
                    if (user.weapon=='M'){
                        if (user.potionn=='C')whole_rooms[nowposx-5+1][nowposy][user.floor].monster.lifee-=10;
                        else whole_rooms[nowposx-5+1][nowposy][user.floor].monster.lifee-=5;
                        }
                    else {
                        if (user.potionn=='C')whole_rooms[nowposx-5+1][nowposy][user.floor].monster.lifee-=20;
                        else whole_rooms[nowposx-5+1][nowposy][user.floor].monster.lifee-=10;
                        }
                    mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname, whole_rooms[nowposx-5+1][nowposy][user.floor].monster.lifee);
                    if (whole_rooms[nowposx-5+1][nowposy][user.floor].monster.lifee<=0){
                        mvprintw(1,3 ,"You killed the %c monster                          ", whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname);
                        whole_rooms[nowposx-5+1][nowposy][user.floor].monster.mname='0';
                }}
                if (whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.mname!='0'){
                    if (user.weapon=='M'){
                        if (user.potionn=='C')whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.lifee-=10;
                        else whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.lifee-=5;
                        }
                    else {
                        if (user.potionn=='C') whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.lifee-=20;
                        else whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.lifee-=10;}
                        mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.mname, whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.lifee);
                    if (whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.lifee<=0){
                        mvprintw(1,3 ,"You killed the %c monster                              ", whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.mname);
                        whole_rooms[nowposx-5+1][nowposy+1][user.floor].monster.mname='0';
                }}
                if (whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.mname!='0'){
                    if (user.weapon=='M'){
                        if (user.potionn=='C')whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.lifee-=10;
                        else whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.lifee-=5;}
                    else {
                        if (user.potionn=='C')whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.lifee-=20;
                        else whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.lifee-=10;}
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.mname, whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.lifee);
                    if (whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.lifee<=0){
                        mvprintw(1,3 ,"You killed the %c monster                            ", whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.mname);
                        whole_rooms[nowposx-5+1][nowposy-1][user.floor].monster.mname='0';
                }}
                if (whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.mname!='0'){
                    if (user.weapon=='M'){
                        if (user.potionn=='C')whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.lifee-=10;
                        else whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.lifee-=5;}
                    else {
                        if (user.potionn=='C')whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.lifee-=20;
                        else whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.lifee-=10;}
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.mname, whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.lifee);
                    if (whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.lifee<=0){
                        mvprintw(1,3 ,"You killed the %c monster                            ", whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.mname);
                        whole_rooms[nowposx-5-1][nowposy-1][user.floor].monster.mname='0';
                }}
                if (whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster.mname!='0'){
                    if (user.weapon=='M'){
                        if (user.potionn=='C')whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster.lifee-=10;
                        else whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster.lifee-=5;}
                    else {
                        if (user.potionn=='C')whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster.lifee-=20;
                        else whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster.lifee-=10;}

                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname, whole_rooms[nowposx-5-1][nowposy][user.floor].monster.lifee);
                    if (whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster.lifee<=0){
                        mvprintw(1,3 ,"You killed the %c monster                          ", whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster.mname);
                        whole_rooms[nowposx-5-1][nowposy+1][user.floor].monster.mname='0';
                }}
                if (whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname!='0'){
                    if (user.weapon=='M'){
                        if (user.potionn=='C')whole_rooms[nowposx-5-1][nowposy][user.floor].monster.lifee-=10;
                        else whole_rooms[nowposx-5-1][nowposy][user.floor].monster.lifee-=5;}
                    else {
                        if (user.potionn=='C')whole_rooms[nowposx-5-1][nowposy][user.floor].monster.lifee-=20;
                        whole_rooms[nowposx-5-1][nowposy][user.floor].monster.lifee-=10;}

                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname, whole_rooms[nowposx-5-1][nowposy][user.floor].monster.lifee);
                    if (whole_rooms[nowposx-5-1][nowposy][user.floor].monster.lifee<=0){
                        mvprintw(1,3 ,"You killed the %c monster                          ", whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname);
                        whole_rooms[nowposx-5-1][nowposy][user.floor].monster.mname='0';
                }}
                if (whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname!='0'){
                    if (user.weapon=='M'){
                       if (user.potionn=='C') whole_rooms[nowposx-5][nowposy-1][user.floor].monster.lifee-=10;
                        else whole_rooms[nowposx-5][nowposy-1][user.floor].monster.lifee-=5;}
                    else {
                        if (user.potionn=='C') whole_rooms[nowposx-5][nowposy-1][user.floor].monster.lifee-=20;
                        else whole_rooms[nowposx-5][nowposy-1][user.floor].monster.lifee-=10;}
                     mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname, whole_rooms[nowposx-5][nowposy-1][user.floor].monster.lifee);
                    if (whole_rooms[nowposx-5][nowposy-1][user.floor].monster.lifee<=0){
                        mvprintw(1,3 ,"You killed the %c monster                       ", whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname);
                        whole_rooms[nowposx-5][nowposy-1][user.floor].monster.mname='0';
                }}
                if (whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname!='0'){
                    if (user.weapon=='M'){
                        if (user.potionn=='C') whole_rooms[nowposx-5][nowposy+1][user.floor].monster.lifee-=10;
                        else whole_rooms[nowposx-5][nowposy+1][user.floor].monster.lifee-=5;}
                    else {
                        if (user.potionn=='C')whole_rooms[nowposx-5][nowposy+1][user.floor].monster.lifee-=20;
                        else whole_rooms[nowposx-5][nowposy+1][user.floor].monster.lifee-=10;}

                     mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname, whole_rooms[nowposx-5][nowposy+1][user.floor].monster.lifee);
                    if (whole_rooms[nowposx-5][nowposy+1][user.floor].monster.lifee<=0){
                        mvprintw(1,3 ,"You killed the %c monster                  ", whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname);
                        whole_rooms[nowposx-5][nowposy+1][user.floor].monster.mname='0';
                }}
            }
                else if (user.weapon=='D'){

                    mvprintw(1,3, "Enter the direction");
                    char direction=getch();
                    if (direction=='l'){
                        for (int i=1; i<6; i++){
                         if (whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname!='0'){

                                if (user.potionn=='C') whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee-=24;
                                else whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee-=12;
                                user.weapon='0';
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname, whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster                     ", whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname);
                                whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=5; 1<i; i--){
                                if (whole_rooms[nowposx-5][nowposy+i][user.floor].type=='.'){
                                        whole_rooms[nowposx-5][nowposy+i][user.floor].type='D';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                    else if (direction=='h'){
                        for (int i=1; i<6; i++){
                         if (whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname!='0'){
                                if (user.potionn=='C')whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee-=24;
                                else whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee-=12;
                                user.weapon='0';
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname, whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster                   ", whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname);
                                whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=5; 1<i; i--){
                                if (whole_rooms[nowposx-5][nowposy-i][user.floor].type=='.'){
                                        whole_rooms[nowposx-5][nowposy-i][user.floor].type='D';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                    else if (direction=='k'){
                        for (int i=1; i<6; i++){
                         if (whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname!='0'){
                                if (user.potionn=='C') whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee-=24;
                                else whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee-=12;
                                user.weapon='0';
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname, whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster                  ", whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname);
                                whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=5; 1<i; i--){
                                if (whole_rooms[nowposx-5+i][nowposy][user.floor].type=='.'){
                                        whole_rooms[nowposx-5+i][nowposy][user.floor].type='D';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                    else if (direction=='j'){
                        for (int i=1; i<6; i++){
                         if (whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname!='0'){

                                if (user.potionn=='C')whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee-=24;
                                else whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee-=12;
                                user.weapon='0';
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname, whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster                       ", whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname);
                                whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=5; 1<i; i--){
                                if (whole_rooms[nowposx-5-i][nowposy][user.floor].type=='.'){
                                        whole_rooms[nowposx-5-i][nowposy][user.floor].type='D';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                }

                else if (user.weapon=='W'){

                    mvprintw(1,3, "Enter the direction");
                    char direction=getch();
                    if (direction=='l'){
                        for (int i=1; i<11; i++){
                         if (whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname!='0'){

                                if (user.potionn=='C')whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee-=30;
                                else whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee-=15;
                                user.weapon='0';
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname, whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster                        ", whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname);
                                whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=10; 1<i; i--){
                                if (whole_rooms[nowposx-5][nowposy+i][user.floor].type=='.'){
                                        whole_rooms[nowposx-5][nowposy+i][user.floor].type='W';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                    if (direction=='h'){
                        for (int i=1; i<11; i++){//mvprintw(2,nowposy-i, "v");
                         if (whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname!='0'){

                                if (user.potionn=='C')whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee-=30;
                                else whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee-=15;
                                user.weapon='0';
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname, whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster               ", whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname);
                                whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=10; 1<i; i--){
                                if (whole_rooms[nowposx-5][nowposy-i][user.floor].type=='.'){//mvprintw(3,1,"kkkk");
                                        whole_rooms[nowposx-5][nowposy-i][user.floor].type='W';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                    if (direction=='k'){
                        for (int i=1; i<11; i++){
                         if (whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname!='0'){
                                if (user.potionn=='C') whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee-=30;
                                else whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee-=15;
                                user.weapon='0';
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname, whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster                        ", whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname);
                                whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=10; 1<i; i--){
                                if (whole_rooms[nowposx-5+i][nowposy][user.floor].type=='.'){
                                        whole_rooms[nowposx-5+i][nowposy][user.floor].type='W';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                    if (direction=='j'){
                        for (int i=1; i<11; i++){
                         if (whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname!='0'){
                                if (user.potionn=='C') whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee-=30;
                                else whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee-=15;
                                user.weapon='0';

                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname, whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster                  ", whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname);
                                whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=10; 1<i; i--){
                                if (whole_rooms[nowposx-5-i][nowposy][user.floor].type=='.'){
                                        whole_rooms[nowposx-5-i][nowposy][user.floor].type='W';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                }

                else if (user.weapon=='N'){
                    mvprintw(1,3, "Enter the direction");
                    char direction=getch();
                    if (direction=='l'){
                        for (int i=1; i<6; i++){
                         if (whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname!='0'){

                                if (user.potionn=='C')whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee-=10;
                                else whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee-=5;
                                user.weapon='0';

                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname, whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5][nowposy+i][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster                 ", whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname);
                                whole_rooms[nowposx-5][nowposy+i][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=5; 1<i; i--){
                                if (whole_rooms[nowposx-5][nowposy+i][user.floor].type=='.'){
                                        whole_rooms[nowposx-5][nowposy+i][user.floor].type='N';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                    if (direction=='h'){
                        for (int i=1; i<6; i++){
                         if (whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname!='0'){
                                if (user.potionn=='C')whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee-=10;
                                else whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee-=5;
                                user.weapon='0';
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname, whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5][nowposy-i][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster                     ", whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname);
                                whole_rooms[nowposx-5][nowposy-i][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=5; 1<i; i--){
                                if (whole_rooms[nowposx-5][nowposy-i][user.floor].type=='.'){
                                        whole_rooms[nowposx-5][nowposy-i][user.floor].type='N';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                    if (direction=='k'){
                        for (int i=1; i<6; i++){
                         if (whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname!='0'){
                               if (user.potionn=='C') whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee-=10;
                                else whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee-=5;
                                user.weapon='0';
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname, whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5+i][nowposy][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster                       ", whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname);
                                whole_rooms[nowposx-5+i][nowposy][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=5; 1<i; i--){
                                if (whole_rooms[nowposx-5+i][nowposy][user.floor].type=='.'){
                                        whole_rooms[nowposx-5+i][nowposy][user.floor].type='N';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                    if (direction=='j'){
                        for (int i=1; i<6; i++){
                         if (whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname!='0'){

                                if (user.potionn=='C')whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee-=10;
                                else whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee-=5;
                                user.weapon='0';
                                mvprintw(1,3, "You hit the %c! Its life is %d", whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname, whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee);
                            if (whole_rooms[nowposx-5-i][nowposy][user.floor].monster.lifee<=0){
                                mvprintw(1,3 ,"You killed the %c monster                      ", whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname);
                                whole_rooms[nowposx-5-i][nowposy][user.floor].monster.mname='0';
                            }
                            break;
                          }
                        }
                          if (user.weapon!='0'){
                            for (int i=5; 1<i; i--){
                                if (whole_rooms[nowposx-5-i][nowposy][user.floor].type=='.'){
                                        whole_rooms[nowposx-5-i][nowposy][user.floor].type='N';
                                        user.weapon='0';
                                        
                                break;
                                }
                            }
                          }             
                    }
                }            


            break;
        case 'M' :
            clear();
            for (int i=0; i<40; i++){
                for (int j=0; j<184; j++){
                        mvprintw(i+5, j, "%c", whole_rooms[i][j][user.floor].type);
                }
            } 
            how_move=getch();
            if (how_move=='M'){
                clear();
                print_map(whole_rooms, user.floor);
            }refresh();
        case 'j':
            monsfollowj(nowposx, nowposy);
            print_map(whole_rooms, user.floor);
            //mvprintw(1,1, "                                                                                 ");
            if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='.'){
                if (whole_rooms[nowposx-1-5][nowposy][user.floor].trap==1){
                    mvprintw(1,3, "Trap! you have lost one life");
                    //clearmess();
                    user.life--;mvprintw(2,3,  "life = %d", user.life);
                }
                nowposx--;
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                print_map(whole_rooms, user.floor);
            }
            else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='r' || whole_rooms[nowposx-1-5][nowposy][user.floor].type=='R'){
                if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='r'){
                    user.gold+=10;
                    user.credit+=10;
                    mvprintw(1,1, "You are getting richer!");
                    mvprintw(2,1, "Gold = %d", user.gold);
                }
                else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='R'){
                    user.gold+=40;
                    user.credit+=40;
                    mvprintw(1,1, "You are getting richer!");
                    mvprintw(2,1, "Gold = %d", user.gold);
                }
                nowposx--;
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx+1, nowposy, '.');
                mvaddch(nowposx, nowposy, '@');
                
            }
            else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='F' || whole_rooms[nowposx-1-5][nowposy][user.floor].type=='X' || whole_rooms[nowposx-1-5][nowposy][user.floor].type=='Y'){
                if (user.food<5){
                    user.food++;
                    if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='F')user.foods[0]++;
                    if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='X')user.foods[1]++;
                    if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='Y')user.foods[2]++;
                }
                nowposx--;
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx+1, nowposy, '.');
                mvaddch(nowposx, nowposy, '@');
                
            }
            else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='$' && countdemon(nowposx, nowposy, user.floor)){
                winthegame();
            }
            else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='='){
                clear();
                user.floor++;
                mvprintw(1,3,"You entered the %d floor", user.floor+1);
                apearroom(nowposx, nowposy, user.floor);
                nowposx--;
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                //mvaddch(nowposx, nowposy+1, '.');
                mvaddch(nowposx, nowposy, '@');
                print_map(whole_rooms, user.floor);
            }
            else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='D' || whole_rooms[nowposx-1-5][nowposy][user.floor].type=='M' || whole_rooms[nowposx-1-5][nowposy][user.floor].type=='W' || whole_rooms[nowposx-1-5][nowposy][user.floor].type=='N' || whole_rooms[nowposx-1-5][nowposy][user.floor].type=='S'){
                mvprintw(1,3, "press 1 to take the weapon");
                catch=getch()-'0';
                if (catch==1){
                    if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='D')user.selah[0]+=10;
                    else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='M')user.selah[1]++;
                    else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='W')user.selah[2]+=8;
                    else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='N')user.selah[3]+=20;
                    else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='S')user.selah[4]++;
                }
                nowposx--;
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx+1, nowposy, '.');
                mvaddch(nowposx, nowposy, '@');

            }
            else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='H' || whole_rooms[nowposx-1-5][nowposy][user.floor].type=='V' || whole_rooms[nowposx-1-5][nowposy][user.floor].type=='C'){
                mvprintw(1,3, "press 1 to take the potion");
                catch=getch()-'0';
                if (catch==1){
                    if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='H')user.potion[0]++;
                    else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='C')user.potion[1]++;
                    else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='V')user.potion[2]++;
                }
                nowposx--;
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx+1, nowposy, '.');
                mvaddch(nowposx, nowposy, '@');

            }
            else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='+'){//mvprintw(1,1,"k");
                if (whole_rooms[nowposx-1-1-5][nowposy][user.floor].type=='#'){
                    user.inroom=0;
                nowposx--;
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx+1, nowposy, '.');
                mvaddch(nowposx, nowposy, '@');
                getchar();
                nowposx--;
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='+';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx+1, nowposy, '+');
                mvaddch(nowposx, nowposy, '@');}
                else {
                    user.inroom=1;
                nowposx--;
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='#';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx+1, nowposy, '#');
                mvaddch(nowposx, nowposy, '@'); 
                getchar();
                nowposx--;
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='+';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx+1, nowposy, '+');
                mvaddch(nowposx, nowposy, '@');
                apearroom(nowposx, nowposy, user.floor);//*********************************************** */
                print_map(whole_rooms, user.floor);
                }

            }
            else if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='#'){
                for (int h=-3; h<5; h++){
                    for (int i=-3; i<5; i++){
                        if (whole_rooms[nowposx+h-1-5][nowposy+i][user.floor].type=='#'){
                        whole_rooms[nowposx+h-1-5][nowposy+i][user.floor].being=1;}
                    }
                    
                }
                print_map(whole_rooms, user.floor);
                nowposx--;
                if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='+'){
                    whole_rooms[nowposx+1-5][nowposy][user.floor].type='+';
                    whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                    mvaddch(nowposx+1, nowposy, '+');
                    mvaddch(nowposx, nowposy, '@');

                }
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='#';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx+1, nowposy, '#');
                mvaddch(nowposx, nowposy, '@');

            }
            if (whole_rooms[nowposx+1-5][nowposy][user.floor].trap==1){
                whole_rooms[nowposx+1-5][nowposy][user.floor].type='^';
                mvaddch(nowposx+1, nowposy,'^');
            }
            break;
        case 'k':
        monsfollowk(nowposx, nowposy);
        print_map(whole_rooms, user.floor);
            if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='.'){
                if (whole_rooms[nowposx+1-5][nowposy][user.floor].trap==1){
                    mvprintw(1,3, "Trap! you have lost one life");
                    //clearmess();
                    user.life--;mvprintw(2,3,  "life = %d", user.life);
                }
                nowposx++;
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                print_map(whole_rooms, user.floor);
            }
            else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='='){
                clear();
                user.floor++;

                mvprintw(1,3,"You entered the %d floor", user.floor+1);
                apearroom(nowposx, nowposy, user.floor);
                nowposx++;
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                //mvaddch(nowposx, nowposy+1, '.');
                mvaddch(nowposx, nowposy, '@');
                print_map(whole_rooms, user.floor);
            }
            else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='$' && countdemon(nowposx, nowposy, user.floor)){
                winthegame();
            }
            else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='r' || whole_rooms[nowposx+1-5][nowposy][user.floor].type=='R'){
                if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='r'){
                    user.gold+=10;
                    user.credit+=10;
                    mvprintw(1,1, "You are getting richer!");
                    mvprintw(2,1, "Gold = %d", user.gold);
                }
                else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='R'){
                    user.gold+=40;
                    user.credit+=40;

                    mvprintw(1,1, "You are getting richer!");
                    mvprintw(2,1, "Gold = %d", user.gold);
                }
                nowposx++;
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx-1, nowposy, '.');
                mvaddch(nowposx, nowposy, '@');
                
            }
            else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='F' || whole_rooms[nowposx+1-5][nowposy][user.floor].type=='X'|| whole_rooms[nowposx+1-5][nowposy][user.floor].type=='Y'){
                 if (user.food<5){
                    user.food++;
                    if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='F' )user.foods[0]++;
                    if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='X' )user.foods[1]++;
                    if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='Y' )user.foods[2]++;

                 }
                nowposx++;
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx-1, nowposy, '.');
                mvaddch(nowposx, nowposy, '@');
                
            }
            else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='D' || whole_rooms[nowposx+1-5][nowposy][user.floor].type=='M' || whole_rooms[nowposx+1-5][nowposy][user.floor].type=='W' || whole_rooms[nowposx+1-5][nowposy][user.floor].type=='N' || whole_rooms[nowposx+1-5][nowposy][user.floor].type=='S'){
                mvprintw(1,3, "press 1 to take the weapon");
                catch=getch()-'0';//mvprintw(1,1,"%d",catch);
                if (catch==1){
                    if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='D')user.selah[0]+=10;
                    else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='M')user.selah[1]++;
                    else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='W')user.selah[2]+=8;
                    else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='N')user.selah[3]+=20;
                    else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='S')user.selah[4]++;
                }
                nowposx++;
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx-1, nowposy, '.');
                mvaddch(nowposx, nowposy, '@');

            }
            else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='H' || whole_rooms[nowposx+1-5][nowposy][user.floor].type=='C' || whole_rooms[nowposx+1-5][nowposy][user.floor].type=='V' ){
                mvprintw(1,3, "press 1 to take the potion");
                catch=getch()-'0';//mvprintw(1,1,"%d",catch);
                if (catch==1){
                    if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='H')user.potion[0]++;
                    else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='C')user.potion[1]++;
                    else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='V')user.potion[2]++;
                }
                nowposx++;
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx-1, nowposy, '.');
                mvaddch(nowposx, nowposy, '@');

            }
            else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='+'){//mvprintw(1,1,"k");
                if (whole_rooms[nowposx+1+1-5][nowposy][user.floor].type=='#'){
                
                nowposx++;
                user.inroom=0;
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx-1, nowposy, '.');
                mvaddch(nowposx, nowposy, '@');
                getchar();
                nowposx++;
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='+';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx-1, nowposy, '+');
                mvaddch(nowposx, nowposy, '@');}
                else {
                user.inroom=1;
                nowposx++;
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='#';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx-1, nowposy, '#');
                mvaddch(nowposx, nowposy, '@'); 
                getchar();
                nowposx++;
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='+';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx-1, nowposy, '+');
                mvaddch(nowposx, nowposy, '@');
                apearroom(nowposx, nowposy, user.floor);
                print_map(whole_rooms, user.floor);
                }

            }
            else if (whole_rooms[nowposx+1-5][nowposy][user.floor].type=='#'){
                for (int h=-3; h<5; h++){
                    for (int i=-3; i<5; i++){
                        if (whole_rooms[nowposx+h+1-5][nowposy+i][user.floor].type=='#'){
                        whole_rooms[nowposx+h+1-5][nowposy+i][user.floor].being=1;}
                    }
                    
                }
                print_map(whole_rooms, user.floor);
                nowposx++;
                if (whole_rooms[nowposx-1-5][nowposy][user.floor].type=='+'){
                    whole_rooms[nowposx-1-5][nowposy][user.floor].type='+';
                    whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                    mvaddch(nowposx-1, nowposy, '+');
                    mvaddch(nowposx, nowposy, '@');

                }
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='#';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx-1, nowposy, '#');
                mvaddch(nowposx, nowposy, '@');

            }
            if (whole_rooms[nowposx-1-5][nowposy][user.floor].trap==1){
                whole_rooms[nowposx-1-5][nowposy][user.floor].type='^';
                mvaddch(nowposx-1, nowposy,'^');
            }
            break;
        case 'h':
        monsfollowh(nowposx, nowposy);
        print_map(whole_rooms, user.floor);
        if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='.' ){
            if (whole_rooms[nowposx-5][nowposy-1][user.floor].trap==1){
                    mvprintw(1,3, "Trap! you have lost one life");
                    //clearmess();
                    user.life--;mvprintw(2,3,  "life = %d", user.life);
                }
                nowposy--;
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                print_map(whole_rooms, user.floor);

            }
            else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='='){
                clear();
                user.floor++;
                mvprintw(1,3,"You entered the %d floor", user.floor+1);
                apearroom(nowposx, nowposy, user.floor);
                nowposy--;
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                //mvaddch(nowposx, nowposy+1, '.');
                mvaddch(nowposx, nowposy, '@');
                print_map(whole_rooms, user.floor);
            }
            else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='r' || whole_rooms[nowposx-5][nowposy-1][user.floor].type=='R'){
                if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='r'){
                    user.gold+=10;
                    user.credit+=10;
                    mvprintw(1,1, "You are getting richer!");
                    mvprintw(2,1, "Gold = %d", user.gold);
                }
                else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='R'){
                    user.gold+=40;
                    user.credit+=40;
                    mvprintw(1,1, "You are getting richer!");
                    mvprintw(2,1, "Gold = %d", user.gold);
                }
                nowposy--;
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy+1, '.');
                mvaddch(nowposx, nowposy, '@');
                
            }
            else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='$' && countdemon(nowposx, nowposy, user.floor)){
                winthegame();
            }
            
            else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='F' ||whole_rooms[nowposx-5][nowposy-1][user.floor].type=='X'||whole_rooms[nowposx-5][nowposy-1][user.floor].type=='Y' ){
               if (user.food<5){
                user.food++;
                if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='F')user.foods[0]++;
                if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='X')user.foods[1]++;
                if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='Y')user.foods[2]++;
               }
                nowposy--;
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy+1, '.');
                mvaddch(nowposx, nowposy, '@');
                
            }
            else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='D' || whole_rooms[nowposx-5][nowposy-1][user.floor].type=='M' || whole_rooms[nowposx-5][nowposy-1][user.floor].type=='W' || whole_rooms[nowposx-5][nowposy-1][user.floor].type=='N' || whole_rooms[nowposx-5][nowposy-1][user.floor].type=='S'){
                mvprintw(1,3, "press 1 to take the weapon");
                catch=getch()-'0';
                if (catch==1){
                    if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='D')user.selah[0]+=10;
                    else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='M')user.selah[1]++;
                    else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='W')user.selah[2]+=8;
                    else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='N')user.selah[3]+=20;
                    else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='S')user.selah[4]++;
                }
                nowposy--;
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy+1, '.');
                mvaddch(nowposx, nowposy, '@');

            }
            else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='H' || whole_rooms[nowposx-5][nowposy-1][user.floor].type=='C' || whole_rooms[nowposx-5][nowposy-1][user.floor].type=='V'){
                mvprintw(1,3, "press 1 to take the potion");
                catch=getch()-'0';
                if (catch==1){
                    if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='H')user.potion[0]++;
                    else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='C')user.potion[1]++;
                    else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='V')user.potion[2]++;
                }
                nowposy--;
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy+1, '.');
                mvaddch(nowposx, nowposy, '@');

            }
        else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='+'){
                if (whole_rooms[nowposx-5][nowposy-1-1][user.floor].type=='#'){
                    user.inroom=0;
                nowposy--;
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy+1, '.');
                mvaddch(nowposx, nowposy, '@');
                getchar();
                nowposy--;
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='+';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy+1, '+');
                mvaddch(nowposx, nowposy, '@');}
                            
            else /*if (whole_rooms[nowposx-5][nowposy+1+1]->type=='.')*/{
                user.inroom=1;
                nowposy--;
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='#';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy+1, '#');
                mvaddch(nowposx, nowposy, '@'); 
                getchar();
                nowposy--;
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='+';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy+1, '+');
                mvaddch(nowposx, nowposy, '@');
                apearroom(nowposx, nowposy, user.floor);
                print_map(whole_rooms, user.floor);
            }

            }
            else if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='#'){
                for (int h=-3; h<5; h++){
                    for (int i=-3; i<5; i++){
                        if (whole_rooms[nowposx+h-5][nowposy-1+i][user.floor].type=='#'){
                        whole_rooms[nowposx+h-5][nowposy-1+i][user.floor].being=1;}
                    }
                    
                }
                print_map(whole_rooms, user.floor);
                nowposy--;
                if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='+'){
                    whole_rooms[nowposx-5][nowposy+1][user.floor].type='+';
                    whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                    mvaddch(nowposx, nowposy+1, '+');
                    mvaddch(nowposx, nowposy, '@');

                }
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='#';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy+1, '#');
                mvaddch(nowposx, nowposy, '@');

            }
            if (whole_rooms[nowposx-5][nowposy+1][user.floor].trap==1){
                whole_rooms[nowposx-5][nowposy+1][user.floor].type='^';
                mvaddch(nowposx, nowposy+1,'^');
            }
            break;
        case 'l':
        
        monsfollowl(nowposx, nowposy);
        print_map(whole_rooms,user.floor);
        if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='.'){
            if (whole_rooms[nowposx-5][nowposy+1][user.floor].trap==1){
                    mvprintw(1,3, "Trap! you have lost one life");
                    
                   // clearmess();
                    user.life--;mvprintw(2,3,  "life = %d", user.life);
                }
            nowposy++;
            whole_rooms[nowposx-5][nowposy-1][user.floor].type='.';
            whole_rooms[nowposx-5][nowposy][user.floor].type='@';
            print_map(whole_rooms, user.floor);
        }
        else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='$' && countdemon(nowposx, nowposy, user.floor)){
                winthegame();
            }
        else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='r' || whole_rooms[nowposx-5][nowposy+1][user.floor].type=='R'){
                if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='r'){
                    user.gold+=10;
                    user.credit+=10;
                    mvprintw(1,1, "You are getting richer!");
                    mvprintw(2,1, "Gold = %d", user.gold);
                }
                else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='R'){
                    user.gold+=40;
                    user.credit+=40;
                    mvprintw(1,1, "You are getting richer!");
                    mvprintw(2,1, "Gold = %d", user.gold);
                }
                nowposy++;
                whole_rooms[nowposx-5][nowposy-1][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy-1, '.');
                mvaddch(nowposx, nowposy, '@');
                
            }

        else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='='){
                clear();
                user.floor++;
                mvprintw(1,3,"You entered the %d floor", user.floor+1);
                apearroom(nowposx, nowposy, user.floor);
                nowposy++;
                whole_rooms[nowposx-5][nowposy-1][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                //mvaddch(nowposx, nowposy+1, '.');
                mvaddch(nowposx, nowposy, '@');
                print_map(whole_rooms, user.floor);
            }

        else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='F' ||whole_rooms[nowposx-5][nowposy+1][user.floor].type=='X' ||whole_rooms[nowposx-5][nowposy+1][user.floor].type=='Y'){
            if (user.food<5){
                user.food++;
                if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='F')user.foods[0]++;
                if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='X')user.foods[1]++;
                if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='Y')user.foods[2]++;
            }
            nowposy++;
            whole_rooms[nowposx-5][nowposy-1][user.floor].type='.';
            whole_rooms[nowposx-5][nowposy][user.floor].type='@';
            mvaddch(nowposx, nowposy-1, '.');
            mvaddch(nowposx, nowposy, '@');
            
        }
        else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='D' || whole_rooms[nowposx-5][nowposy+1][user.floor].type=='M' || whole_rooms[nowposx-5][nowposy+1][user.floor].type=='W' || whole_rooms[nowposx-5][nowposy+1][user.floor].type=='N' || whole_rooms[nowposx-5][nowposy+1][user.floor].type=='S'){
                mvprintw(1,3, "press 1 to take the weapon");
                catch=getch()-'0'; 
                if (catch==1){
                    if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='D')user.selah[0]+=10;
                    else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='M')user.selah[1]++;
                    else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='W')user.selah[2]+=8;
                    else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='N')user.selah[3]+=20;
                    else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='S')user.selah[4]++;
                }
                nowposy++;
                whole_rooms[nowposx-5][nowposy-1][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy-1, '.');
                mvaddch(nowposx, nowposy, '@');

            }
        
        else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='H' || whole_rooms[nowposx-5][nowposy+1][user.floor].type=='C' || whole_rooms[nowposx-5][nowposy+1][user.floor].type=='V'){
                mvprintw(1,3, "press 1 to take the potion");
                catch=getch()-'0'; 
                if (catch==1){
                    if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='H')user.potion[0]++;
                    else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='C')user.potion[1]++;
                    else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='V')user.potion[2]++;
                }
                nowposy++;
                whole_rooms[nowposx-5][nowposy-1][user.floor].type='.';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy-1, '.');
                mvaddch(nowposx, nowposy, '@');

            }
        else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='.'){
            nowposy++;
            whole_rooms[nowposx-5][nowposy-1][user.floor].type='.';
            whole_rooms[nowposx-5][nowposy][user.floor].type='@';
            mvaddch(nowposx, nowposy-1, '.');
            mvaddch(nowposx, nowposy, '@');
          /// if (user.food<5)user.food++;
        }
        else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='+'){
            if (whole_rooms[nowposx-5][nowposy+1+1][user.floor].type=='#'){
            nowposy++;
            user.inroom=0;
            whole_rooms[nowposx-5][nowposy-1][user.floor].type='.';
            whole_rooms[nowposx-5][nowposy][user.floor].type='@';
            mvaddch(nowposx, nowposy-1, '.');
            mvaddch(nowposx, nowposy, '@'); 
            getchar();
            nowposy++;
            whole_rooms[nowposx-5][nowposy-1][user.floor].type='+';
            whole_rooms[nowposx-5][nowposy][user.floor].type='@';
            mvaddch(nowposx, nowposy-1, '+');
            mvaddch(nowposx, nowposy, '@');//; 
            }
            else /*if (whole_rooms[nowposx-5][nowposy+1+1]->type=='.')*/{
                user.inroom=1;
                 nowposy++;
                whole_rooms[nowposx-5][nowposy-1][user.floor].type='#';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy-1, '#');
                mvaddch(nowposx, nowposy, '@'); 
                getchar();
                nowposy++;
                whole_rooms[nowposx-5][nowposy-1][user.floor].type='+';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy-1, '+');
                mvaddch(nowposx, nowposy, '@');
                apearroom(nowposx, nowposy, user.floor);
                print_map(whole_rooms, user.floor);
            }

        }
        else if (whole_rooms[nowposx-5][nowposy+1][user.floor].type=='#'){
                for (int h=-3; h<4; h++){
                    for (int i=-3; i<4; i++){
                        if (whole_rooms[nowposx+h-5][nowposy+i][user.floor].type=='#'){
                        whole_rooms[nowposx+h-5][nowposy+i][user.floor].being=1;
                        }
                    }
                    
                }
                print_map(whole_rooms , user.floor);
                nowposy++;
                if (whole_rooms[nowposx-5][nowposy-1][user.floor].type=='+'){
                    whole_rooms[nowposx-5][nowposy-1][user.floor].type='+';
                    whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                    mvaddch(nowposx, nowposy-1, '+');
                    mvaddch(nowposx, nowposy, '@');

                }
                whole_rooms[nowposx-5][nowposy-1][user.floor].type='#';
                whole_rooms[nowposx-5][nowposy][user.floor].type='@';
                mvaddch(nowposx, nowposy-1, '#');
                mvaddch(nowposx, nowposy, '@');

            }
            if (whole_rooms[nowposx-5][nowposy-1][user.floor].trap==1){
                whole_rooms[nowposx-5][nowposy-1][user.floor].type='^';
                mvaddch(nowposx, nowposy-1,'^');
            }
            break;
            



    }
    }


    getch();
    endwin();

    //return 0;
}
void profile(){
    mvprintw (3,3, "Profile Menu");
    mvprintw(5,3,"Name: %s", user.username );
    mvprintw(6,3, "Email: %s", user.email);
    mvprintw (7,3, "Password: %s", user.password);
    char color[10];
     if (user.color=='b')strcpy(color, "blue");
    else if (user.color=='g')strcpy(color, "green");
    else if (user.color=='y')strcpy(color, "yellow");
    else if (user.color=='w')strcpy(color, "white");
    mvprintw( 8,3, "Color: %s", color);
    mvprintw(9,3, "Total_credit: %d", user.credit);
}
void load_game(){
    FILE* file;
    char name[50];
    
    strcpy(name, user.username);
    strcat(name, ".txt");
    file =fopen (name, "r");
    char line[300];
    char trash[50];
    fgets(line , 100, file);
    int selah1, selah2, selah3, selah4, selah0;
    sscanf(line,"%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", trash, &user.food, &user.gold, &user.life, &selah0, &selah1, &selah2, &selah3, &selah4, &user.credit,
            &user.potion[0], &user.potion[1], &user.potion[2], &user.floor, &user.finished_games);

    user.selah[0]=selah0; user.selah[1]=selah1; user.selah[2]=selah2; user.selah[3]=selah3; user.selah[4]=selah4;
    for (int s=0; s<4; s++){
        for (int i = 0; i < 40; i++) {
            for (int j = 0; j < 184; j++) {
                fgets(line , 100, file);
                sscanf(line, "%d,%c,%d,%c,%d,%d,%d,%d,%c", 
                &whole_rooms[i][j][s].being, &whole_rooms[i][j][s].type, &whole_rooms[i][j][s].trap, &whole_rooms[i][j][s].monster.mname
                , &whole_rooms[i][j][s].monster.movement, &whole_rooms[i][j][s].monster.lifee,
                    &whole_rooms[i][j][s].monster.x, &whole_rooms[i][j][s].monster.y, &whole_rooms[i][j][s].model);
            }
        }
    }
    
    
    return;
}
void pregamemenu(){
    
    initscr();
    clear();
    int valid=1;
    int how_continue;
    char file_path[] = "users.txt";

    while (valid){
        mvprintw(5, 5, "Choose what you want to do");
        mvprintw(6, 5, "1: start new game");
        mvprintw(7, 5, "2: resume last game");
        mvprintw(8, 5, "3: visit scorboard");
        mvprintw(9, 5, "4: setting");
        mvprintw(10,5, "5: profile menu");
        mvprintw(11, 5, "Choose an option:");
        refresh();

        //flushinp();
        scanw("%d", &how_continue);

        clear();

        switch (how_continue) {
            case 1:
                start_game(); 
                getch(); 
                //makemap();
                valid=0;
                break;
            case 2:
                resume_game();

                getch();
                valid=0;
                break;
            case 3:
                scorboard(file_path); 
                getch(); 
                clear();
                //valid=0;
                break;
            case 4:
                setting();  
                getch();
                clear();
                //valid=0;
                break;
            case 5: 
                profile();
                getch();
                clear();
                break;

         
            default:
                mvprintw(5, 5, "Invalid option. Please try again.");
                refresh();
                getch();
                clear();
                break;
        }
    
    }
    getch();
    endwin();

}
void interance(){

    //User user;
    int valid=0; 
    char file_path[] = "users.txt";

    initscr();
    cbreak();
    clear();

    while (!valid){
        
        mvprintw(0, 5, "Enter (Guest) for username And password to login as a guest");
        mvprintw(1, 5, "Username:");
        getnstr(user.username, 49);
        if (strcmp(user.username, "Guest")==0){
            mvprintw (3, 5, "password:");
            getnstr(user.password, 49);
            if (strcmp(user.password, "Guest")==0){
                mvprintw(5,5 ,"Welcome Guest!");
                getch();
                break;
            }
            else{
            mvprintw ( 5, 5, "Wrong password");
            getch();
            clear();
            refresh();
            continue;
            }
        }
        else if (!(is_username_taken_before(user.username, file_path))){
            mvprintw( 3, 5, "Username doesn't exist!");
            getch();
            clear();
            refresh();
            continue;
        }
        mvprintw(3, 5, "password:");
        getnstr (user.password, 49);
        if (tatabogh(file_path)){
            mvprintw( 5, 5, "Welcome, %s", user.username);
            getch();
            refresh();
            
        }
        else {
            mvprintw ( 5, 5, "Wrong password");
            mvprintw(6,5, "Have you lost your password? Enter Y to find it");

            char pass =getch();
            if (pass=='Y')findpass(file_path);
            getch();
            clear();
            refresh();
            continue;
        }
        valid=1;

        
    }
    user.gold=0;
        //user.finished_games=0;
        user.food=0;
        user.life=50;
        user.weapon='M';
        user.floor=0;

    getch();
    endwin();
    pregamemenu();
    
}
int main() {
    setlocale(LC_ALL, "");
    int how_enter = 0; 
    initscr();
    cbreak(); 
    clear();
    int x=1; 

    while (x){
        mvprintw(5, 5, "Hello, Welcome to Rogue. Choose how to enter");
        mvprintw(6, 5, "1: Login");
        mvprintw(7, 5, "2: Register");
        mvprintw(8, 5, "Choose an option:");
        refresh();

        scanw("%d", &how_enter);
        clear();

        switch (how_enter) {
            case 1:
                interance(); 
                getch(); 
                x=0;
                pregamemenu();
                break;
            case 2:
                creat_user();  
                getch();
                x=0;
                pregamemenu();
                break;

         
            default:
                mvprintw(5, 5, "Invalid option. Please try again.");
                refresh();
                getch();
                clear();
                break;
        }
    
    }
    endwin();
    return 0;
}

