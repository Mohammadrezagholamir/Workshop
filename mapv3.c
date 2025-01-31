#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

/*
خیلی مهمه که برای ران شدن کد حتما صفحتون بزرگ باشه
need to add :
1 . login as guest
2 . change hero color
3 . savein map nd other things
4 . add button f for moving.

*/
#define MAP_WIDTH 105
#define MAP_HEIGHT 35
#define MIN_ROOM_SIZE 7
#define MAX_ROOM_SIZE 12
#define MAX_ROOMS 8

typedef struct {
    int x, y;
    bool exists;
} Stair;

// Floor containers seen
bool seen[MAP_WIDTH][MAP_HEIGHT] = {{false}};
bool seen2[MAP_WIDTH][MAP_HEIGHT] = {{false}};
bool seen3[MAP_WIDTH][MAP_HEIGHT] = {{false}};
bool seen4[MAP_WIDTH][MAP_HEIGHT] = {{false}};

// Floor containers
char container[MAP_WIDTH][MAP_HEIGHT];
char container2[MAP_WIDTH][MAP_HEIGHT];
char container3[MAP_WIDTH][MAP_HEIGHT];
char container4[MAP_WIDTH][MAP_HEIGHT];
typedef struct {
    int x , y;
    int type;
    bool used;
    
} Gun;
int guns =0;
int macecounter = 1;
int daggercounter = 0;
int mwandcounter = 0;
int narrowcounter = 0;
int swordcounter =0 ;
int scounter = 0;
int dcounter = 0;
int hcounter =0;
typedef struct {
    int x ,y;
    int type;

} Food;
typedef struct {
    int x , y;
    int value;
} Gold;


Gold goldcontainer[50];
Gun guncontainer[50];

typedef struct {
    int x , y ;
} BGold;

BGold bgoldcontainer[50];

int floorcount = 0;

void map_container(WINDOW* win, char container[MAP_WIDTH][MAP_HEIGHT]) {
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            chtype ch = mvwinch(win, j, i) & A_CHARTEXT;
            container[i][j] = (char)ch;
        }
    }
}

typedef struct {
    int x , y;
    bool alwaysclose;
    bool open;
    int password;
} Pdoor;
typedef struct {
    int x;
    int y;
    bool active;
} Trap;

Trap trapcounter[30];

typedef struct{
    int x ,y;
    int type;
} Poison;

Poison poisoncontainer[50];

typedef struct {
    int x, y;
    char HeroColor[30];
    int heart;
    int food;
    int guncount;
    int typefood;
    Food foods[30];
    Gun guns[30];
    BGold bgolds[30];
    Poison poisons[50];
    int poisoncount;
    int bgoldcounter;
    int goldcount;
    int gold;
    int move;
    int typeofInitialGun;



    int score;

    
} Hero;
typedef struct {
    int x , y;
    int health ; 
    bool active;
    int type;
    int damage;
    int moves;
    char nmd;
    bool die;
    bool stop;
} Monster;
int enemycount = 0;

Monster monstercontainer[50];
int enemy = 0 ;


typedef struct {
    int x, y;
    int width, height;
    bool haspillar;
    // has 3 types: Regular , treasure , poison :
    int typeroom;
    //Gun and Gold
    int guncount;
    Gun guns[30];
    Gold golds[30];
    BGold bgolds[50];
    int bgoldcounter;
    int goldcount;
    //traps
    
    
    //Foods
    int foodCount;
    Food foods[20];
    int trapcount;
    Trap traps[30];
    Poison poisons[50];
    int poisoncounter;

    int monstercount;
    Monster monsters[50];

} Room;
void revealRoom(Room room, bool seen[MAP_WIDTH][MAP_HEIGHT]) {
    for (int i = 0; i < room.height; i++) {
        for (int j = 0; j < room.width; j++) {
            int nx = room.x + j;
            int ny = room.y + i;
            if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT) {
                seen[nx][ny] = true;
            }
        }
    }
}


void updateVisibility(WINDOW* win, Hero hero, int radius, bool seen[MAP_WIDTH][MAP_HEIGHT] , Room* rooms , int roomcount) {
    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            int nx = hero.x + dx;
            int ny = hero.y + dy;

            if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT) {
                seen[nx][ny] = true;  // Mark as seen
            }
        }
    }
    for (int i = 0; i < roomcount; i++) {
    Room room = rooms[i];
    if (hero.x >= room.x && hero.x < room.x + room.width &&
            hero.y >= room.y && hero.y < room.y + room.height) {
            // اگر وارد اتاق شد، اتاق را نشان بده
            revealRoom(room, seen);
            break;
        }
    }
}

void drawSeenMap(WINDOW* win, char container[MAP_WIDTH][MAP_HEIGHT], bool seen[MAP_WIDTH][MAP_HEIGHT] , Pdoor* pdoor , Room* rooms , int roomcount) {

    
    

    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            int roomnumber = inside(x , y ,rooms ,roomcount );
            if(roomnumber != -1 && rooms[roomnumber].typeroom == 3){
                if (seen[x][y]) {
                    
                    

                    chtype ch = container[x][y];
                    if((char)ch == '.' || (char)ch == '^'){
                        wattron(win , COLOR_PAIR(2));
                        mvwaddch(win, y, x, ch); 
                        wattroff(win , COLOR_PAIR(2));

                    }
                    else if((char)ch == '|' || (char)ch == '_' || (char)ch == 'O' || (char)ch == '&' || (char)ch == '+'){
                        wattron(win , COLOR_PAIR(4));
                        mvwaddch(win, y, x, ch); 
                        wattroff(win , COLOR_PAIR(4));
                    }
                    else if((char)ch == 'A' || (char)ch == 'M' || (char)ch == 'G' || (char)ch == 'K' || (char)ch == 'X' ){
                        wattron(win , COLOR_PAIR(5));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(5));
                    }
                    else if((char)ch =='#' || (char)ch == '>' || (char)ch == 'N' || (char)ch == 'E' || (char)ch == 'T' || (char)ch == 'U' || (char)ch == 'F'){
                        wattron(win , COLOR_PAIR(6));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(6)); 

                    }
                    else if( (char)ch == '!' || (char)ch == '1' || (char)ch == '4' || (char)ch == 'I' || (char)ch == '/'){
                        wattron(win , COLOR_PAIR(1));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(1));
                    }
                    else if((char)ch =='$'){
                        wattron(win ,COLOR_PAIR(4));
                        mvwaddch(win , y , x ,ch);
                        wattroff(win , COLOR_PAIR(4));
                    }
                    else if((char)ch =='@' && pdoor->open == true){
                        wattron(win , COLOR_PAIR(5));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(5));

                    }
                    else if((char)ch == '@' && pdoor->open == false){
                        wattron(win , COLOR_PAIR(1));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(1)); 
                    }
                    else if((char)ch == 'D' || (char)ch == 'S'|| (char)ch == 'C' ){
                        wattron(win , COLOR_PAIR(7));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(7));
                    }
    // Display the actual character
                } else {
                    mvwaddch(win, y, x, ' ');  // Display empty space
                }
            }
            else if(roomnumber != -1 && rooms[roomnumber].typeroom == 2){
                
                if (seen[x][y]) {
                    
                    

                    chtype ch = container[x][y];
                    if((char)ch == '.' || (char)ch == '^'){
                        wattron(win , COLOR_PAIR(2));
                        mvwaddch(win, y, x, ch); 
                        wattroff(win , COLOR_PAIR(2));

                    }
                    else if((char)ch == '|' || (char)ch == '_' || (char)ch == 'O' || (char)ch == '&' || (char)ch == '+'){
                        wattron(win , COLOR_PAIR(2));
                        mvwaddch(win, y, x, ch); 
                        wattroff(win , COLOR_PAIR(2));
                    }
                    else if((char)ch == 'A' || (char)ch == 'M' || (char)ch == 'G' || (char)ch == 'K' ||(char)ch == 'X'){
                        wattron(win , COLOR_PAIR(5));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(5));
                    }
                    else if((char)ch =='#' || (char)ch == '>' || (char)ch == 'N' || (char)ch == 'E' || (char)ch == 'T' || (char)ch == 'U' || (char)ch == 'F'){
                        wattron(win , COLOR_PAIR(6));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(6)); 

                    }
                    else if( (char)ch == '!' || (char)ch == '1' || (char)ch == '4' || (char)ch == 'I' || (char)ch == '/'){
                        wattron(win , COLOR_PAIR(1));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(1));
                    }
                    else if((char)ch =='$'){
                        wattron(win ,COLOR_PAIR(4));
                        mvwaddch(win , y , x ,ch);
                        wattroff(win , COLOR_PAIR(4));
                    }
                    else if((char)ch =='@' && pdoor->open == true){
                        wattron(win , COLOR_PAIR(5));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(5));

                    }
                    else if((char)ch == '@' && pdoor->open == false){
                        wattron(win , COLOR_PAIR(1));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(1)); 
                    }
                    else if((char)ch == 'D' || (char)ch == 'S'|| (char)ch == 'C' ){
                        wattron(win , COLOR_PAIR(7));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(7));
                    }
    // Display the actual character
                } else {
                    mvwaddch(win, y, x, ' ');  // Display empty space
                }





            }
            else{
                if (seen[x][y]) {
                    
                    

                    chtype ch = container[x][y];
                    if((char)ch == '.' || (char)ch == '^'){
                        wattron(win , COLOR_PAIR(2));
                        mvwaddch(win, y, x, ch); 
                        wattroff(win , COLOR_PAIR(2));

                    }
                    else if((char)ch == '|' || (char)ch == '_' || (char)ch == 'O' || (char)ch == '&' || (char)ch == '+'){
                        wattron(win , COLOR_PAIR(3));
                        mvwaddch(win, y, x, ch); 
                        wattroff(win , COLOR_PAIR(3));
                    }
                    else if((char)ch == 'A' || (char)ch == 'M' || (char)ch == 'G' || (char)ch == 'K' || (char)ch == 'X' ){
                        wattron(win , COLOR_PAIR(5));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(5));
                    }
                    else if((char)ch =='#' || (char)ch == '>' || (char)ch == 'N' || (char)ch == 'E' || (char)ch == 'T' || (char)ch == 'U' || (char)ch == 'F'){
                        wattron(win , COLOR_PAIR(6));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(6)); 

                    }
                    else if( (char)ch == '!' || (char)ch == '1' || (char)ch == '4' || (char)ch == 'I' || (char)ch == '/'){
                        wattron(win , COLOR_PAIR(1));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(1));
                    }
                    else if((char)ch =='$'){
                        wattron(win ,COLOR_PAIR(4));
                        mvwaddch(win , y , x ,ch);
                        wattroff(win , COLOR_PAIR(4));
                    }
                    else if((char)ch =='@' && pdoor->open == true){
                        wattron(win , COLOR_PAIR(5));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(5));

                    }
                    else if((char)ch == '@' && pdoor->open == false){
                        wattron(win , COLOR_PAIR(1));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(1)); 
                    }
                    else if((char)ch == 'D' || (char)ch == 'S'|| (char)ch == 'C' ){
                        wattron(win , COLOR_PAIR(7));
                        mvwaddch(win , y ,x , ch);
                        wattroff(win , COLOR_PAIR(7));
                    }
    // Display the actual character
                } else {
                    mvwaddch(win, y, x, ' ');  // Display empty space
                }
            }
        }
        
    }
}
void markseen(int x , int y , bool seen[MAP_WIDTH][MAP_HEIGHT]){
    seen[x][y] = true;
}
bool isitwallorO(WINDOW* win, int x, int y ) {
    chtype ch = mvwinch(win, y, x);
    if ((char)ch != '|' && (char)ch != '_' && (char)ch != 'O' && (char)ch != ' ') {
        return true;
    }

    return false;
}

bool isInsideRoom(int x, int y, Room* rooms, int roomCount);

int Herox() {
    return rand() % (MAP_WIDTH - 2);
}

int Heroy() {
    return rand() % (MAP_HEIGHT - 2);
}

void HeroPlace(WINDOW* win, Room* rooms, int roomCount, int* savedX, int* savedY) {
    Room room = rooms[2];
    int xrnd = room.x + 1 + rand() % (room.width - 2);
    int yrnd = room.y + 1 + rand() % (room.height - 2);
    *savedX = xrnd;
    *savedY = yrnd;
    mvwaddch(win, yrnd, xrnd, 'H');
}

int oneOrTwo() {
    return rand() % 2 + 1;
}

bool haspillar(Room* room) {
    return oneOrTwo() == 2;
}

void placeTraps(Room* rooms , Trap* trapcounter , int roomcount) {
    int num = 0 ;
    for(int i=0 ; i<roomcount ; i++){
        if(rooms[i].typeroom == 3){
            rooms[i].trapcount = 3;
        }
        else{
            rooms[i].trapcount = rand() % 2;
        }
        int counter=rooms[i].trapcount;

        Room room = rooms[i];
        for(int j=0 ; j<counter ; j++){
            int xrnd = room.x + 1 + rand() % (room.width - 2);
            int yrnd = room.y + 1 + rand() % (room.height - 2);
            rooms[i].traps[j].x = xrnd;
            rooms[i].traps[j].y =yrnd;
            rooms[i].traps[j].active = false;
            trapcounter[num].x = xrnd;
            trapcounter[num].y = yrnd;
            trapcounter[num].active = false;
            num++;
        }

    }
}

void isontrap(WINDOW* win,WINDOW* messagewin , Room* rooms, int roomCount, Hero* hero  ,Trap* trapcounter , char container[MAP_WIDTH][MAP_HEIGHT]) {
    for(int i=0 ; i<50 ; i++){
        if(hero->x == trapcounter[i].x  && hero->y == trapcounter[i].y){
            trapcounter[i].active = true;
            hero->heart -= 2;
            wclear(messagewin);
            mvwprintw(messagewin , 0 ,0 , "You heart!");
            mvwprintw(messagewin ,1 ,0 ,"%d" , hero->heart);
            mvwaddch(win , trapcounter[i].y , trapcounter[i].x , '^');
            container[trapcounter[i].x][trapcounter[i].y] = '^';

            wrefresh(messagewin);
            sleep(2);


        }
    }
}

void placestair(WINDOW* win, Room* rooms, int roomcount, Stair* stair) {
    Room room = rooms[3];
    int xrnd = room.x + 1 + rand() % (room.width - 2);
    int yrnd = room.y + 1 + rand() % (room.height - 2);
    stair->x = xrnd;
    stair->y = yrnd;
    stair->exists = true;
    mvwaddch(win, yrnd, xrnd, '>');
}

void drawpillar(WINDOW* win, Room room) {
    if (room.haspillar) {
        int xrnd = room.x + 1 + rand() % (room.width - 2);
        int yrnd = room.y + 1 + rand() % (room.height - 2);
        mvwaddch(win, yrnd, xrnd, 'O');
    }
}

bool doesOverlap(Room* rooms, int roomCount, Room newRoom) {
    for (int i = 0; i < roomCount; i++) {
        Room r = rooms[i];

        if (newRoom.x < r.x + r.width &&
            newRoom.x + newRoom.width > r.x &&
            newRoom.y < r.y + r.height &&
            newRoom.y + newRoom.height > r.y) {
            return true;
        }

        int horizontalDist = (newRoom.x > r.x + r.width) ? newRoom.x - (r.x + r.width)
                          : (r.x > newRoom.x + newRoom.width) ? r.x - (newRoom.x + newRoom.width)
                          : 0;

        int verticalDist = (newRoom.y > r.y + r.height) ? newRoom.y - (r.y + r.height)
                        : (r.y > newRoom.y + newRoom.height) ? r.y - (newRoom.y + newRoom.height)
                        : 0;

        if (horizontalDist < 10 && verticalDist < 10) {
            return true;
        }
    }
    return false;
}

bool isInsideRoom(int x, int y, Room* rooms, int roomCount) {
    for (int i = 0; i < roomCount; i++) {
        Room r = rooms[i];
        if (x >= r.x && x < r.x + r.width && y >= r.y && y < r.y + r.height) {
            return true;
        }
    }
    return false;
}
int inside(int x , int y ,Room* rooms , int roomcount){
    for (int i = 0; i < roomcount; i++) {
        Room r = rooms[i];
        if (x >= r.x && x <= r.x + r.width && y >= r.y && y <= r.y + r.height) {
            return i;
        }
    }
    return -1;

}
void gunsinroom(WINDOW* win , Room* rooms , int roomcount , char container[MAP_WIDTH][MAP_HEIGHT] , Gun* guncontainer){
    int count = 0;
    for(int i=0 ; i<roomcount ; i++){
    
        int guncounter = rand() %2+1;
        rooms[i].guncount = guncounter;
        for(int j=0 ; j<guncounter ; j++){
            int xrnd = rooms[i].x + 1 + rand() % (rooms[i].width - 2);
            int yrnd = rooms[i].y + 1 + rand() % (rooms[i].height - 2);
            int typeg = rand() % 5 + 1;
            switch(typeg){
                case 1:
                    mvwaddch(win , yrnd , xrnd ,'4' );
                    container[xrnd][yrnd]= '4';
                    wrefresh(win);
                    break;
                case 2 :
                    mvwaddch(win , yrnd , xrnd ,'!' );
                    container[xrnd][yrnd]= '!';
                    wrefresh(win); 
                    break; 
                case 3:
                    mvwaddch(win , yrnd , xrnd ,'I' );
                    container[xrnd][yrnd]= 'I';
                    wrefresh(win);
                    break; 
                case 4 :
                    mvwaddch(win , yrnd , xrnd ,'/' );
                    container[xrnd][yrnd]= '/';
                    wrefresh(win);
                    break;
                case 5:
                    mvwaddch(win , yrnd , xrnd ,'1' );
                    container[xrnd][yrnd]= '1';
                    wrefresh(win);
                    break;
            }
            rooms[i].guns[j].x = xrnd;
            rooms[i].guns[j].y = yrnd;
            rooms[i].guns[j].type = typeg;
            rooms[i].guns[j].used = 1;
            guncontainer[count].x =xrnd;
            guncontainer[count].y = yrnd;
            guncontainer[count].type =typeg;
            guncontainer[count].used = 1;
            count++;
        }

    }
    guns = count;
}
int isitongun(WINDOW* win , WINDOW* messagewin , int x , int y ){
    chtype ch = mvwinch(win , y , x);
    if((char)ch == '4' || (char)ch == '!' || (char)ch == '1' || (char)ch == '/' || (char)ch =='I'){
        return 1;
    }
    return 0;
    
}
void addtoinventory(WINDOW* win , WINDOW* messagewin , Hero* hero , int x , int y ,Gun* guncontainer ){
    wclear(messagewin);
    for(int i=0 ; i<50 ; i++){
        if(guncontainer[i].x == x && guncontainer[i].y == y){
            hero->guns[hero->guncount].x = x; 
            hero->guns[hero->guncount].y = y;
            hero->guns[hero->guncount].type = guncontainer[i].type;
            if(guncontainer[i].type == 1){
                macecounter++;
            }
            else if(guncontainer[i].type == 2){
                if(guncontainer[i].used == 1){
                    daggercounter+= 10;
                }
                else {
                    daggercounter++;
                }
            }
            else if(guncontainer[i].type == 3){
                if(guncontainer[i].used ==1){
                    mwandcounter+= 15;
                }
                else {
                    mwandcounter++;
                }
            }
            else if(guncontainer[i].type == 4){

                if(guncontainer[i].used == 1){

                
                    narrowcounter+=12;
                }
                else{
                    narrowcounter++;
                }

            }
            else if(guncontainer[i].type == 5){
                swordcounter++;
            }
            hero->guncount++;
            
            mvwprintw(messagewin , 0 , 0 , "You add gun type : %d" , guncontainer[i].type);
            wrefresh(messagewin);
            sleep(2);
            container[x][y] = '.';
            break;
            
        }
    }
}
void changegun(WINDOW* win , WINDOW*  messagewin , Hero* hero  ){
    wclear(messagewin);
    mvwprintw(messagewin , 0 , 0  , "YOUR GUNS : ");
    mvwprintw(messagewin , 1 , 0 , "Mace : %d" , macecounter);
    mvwprintw(messagewin , 2 ,0 , "Dagger : %d" , daggercounter);
    mvwprintw(messagewin , 3 , 0 , "Magic Wand : %d" , mwandcounter);
    mvwprintw(messagewin ,4, 0, "Normal Arrow : %d" , narrowcounter);
    mvwprintw(messagewin , 5 , 0 , "Sword : %d" , swordcounter);


    int choice;
    echo();
    mvwprintw(messagewin , 6 , 0 , "Enter a  number : ");
    wscanw(messagewin , "%d" , &choice);
    noecho();
    if(choice > 5){
        return;
    }
    if(choice == 1 && macecounter !=0){
        hero->typeofInitialGun = 1;
    }
    else if(choice == 2 && daggercounter != 0){
        hero->typeofInitialGun = 2;
    }
    else if(choice == 3 && mwandcounter != 0){
        hero->typeofInitialGun = 3;

    }
    else if(choice == 4 && narrowcounter != 0 ){
        hero->typeofInitialGun = 4;
    }
    else if(choice == 5 && swordcounter != 0){
        hero->typeofInitialGun = 5;
    }


}
void foodsinroom(WINDOW* win , Room* rooms , int roomcount , char container[MAP_WIDTH][MAP_HEIGHT]){
    
    for(int i =0 ; i<roomcount ; i++){
        int foodcounter = rand() % 2+1;
        rooms[i].foodCount = foodcounter;
        for(int j=0 ; j<foodcounter ; j++){
            int xrnd = rooms[i].x + 1 + rand() % (rooms[i].width - 2);
            int yrnd = rooms[i].y + 1 + rand() % (rooms[i].height - 2);
            
            int typef = rand() % 4+1;

            switch(typef){
                case 1 :
                    mvwaddch(win , yrnd , xrnd , 'A');
                    container[xrnd][yrnd] = 'A';
                    wrefresh(win);
                    break;
                case 2 :
                    mvwaddch(win , yrnd , xrnd , 'G');
                    container[xrnd][yrnd] = 'G';
                    wrefresh(win);
                    break;
                case 3 :
                    mvwaddch(win , yrnd , xrnd , 'M');
                    container[xrnd][yrnd] = 'M';
                    wrefresh(win);
                    break;
                case 4 :
                    mvwaddch(win , yrnd , xrnd , 'K');
                    container[xrnd][yrnd] = 'K';
                    wrefresh(win);
                    break;
            }
            rooms[i].foods[j].x = xrnd;
            rooms[i].foods[j].y =yrnd;
            rooms[i].foods[j].type = typef;


        }
    }

}
void poisonsinroom(WINDOW* win , Room* rooms , int roomcount , char container[MAP_WIDTH][MAP_HEIGHT] , Poison* poisoncontainer){
    int count = 0;
    for(int i=0 ; i<roomcount ; i++){
    
        int guncounter;
        if(rooms[i].typeroom ==2){
            guncounter = 3;
        }
        else {
            guncounter = rand() % 2;
        }
        rooms[i].poisoncounter = guncounter;
        for(int j=0 ; j<guncounter ; j++){
            int xrnd = rooms[i].x + 1 + rand() % (rooms[i].width - 2);
            int yrnd = rooms[i].y + 1 + rand() % (rooms[i].height - 2);
            int typeg = rand() % 3 + 1;
            switch(typeg){
                case 1:
                    mvwaddch(win , yrnd , xrnd ,'S' );
                    container[xrnd][yrnd]= 'S';
                    wrefresh(win);
                    break;
                case 2 :
                    mvwaddch(win , yrnd , xrnd ,'D' );
                    container[xrnd][yrnd]= 'D';
                    wrefresh(win); 
                    break; 
                case 3:
                    mvwaddch(win , yrnd , xrnd ,'C' );
                    container[xrnd][yrnd]= 'C';
                    wrefresh(win);
                    break; 

            }
            rooms[i].poisons[j].x = xrnd;
            rooms[i].poisons[j].y = yrnd;
            rooms[i].poisons[j].type = typeg;
            poisoncontainer[count].x =xrnd;
            poisoncontainer[count].y = yrnd;
            poisoncontainer[count].type =typeg;
            count++;
        }

    }
}
int isitonpoison(WINDOW* win , WINDOW* messagewin , int x , int y ){
    chtype ch = mvwinch(win , y , x);
    if((char)ch == 'D' || (char)ch == 'C' || (char)ch == 'S' ){
        return 1;
    }
    return 0;
    
}

void addtoinventoryp(WINDOW* win , WINDOW* messagewin , Hero* hero , int x , int y ,Poison* poisoncontainer ){
    wclear(messagewin);
    for(int i=0 ; i<50 ; i++){
        if(poisoncontainer[i].x == x && poisoncontainer[i].y == y){
            hero->poisons[hero->poisoncount].x = x; 
            hero->poisons[hero->poisoncount].y = y;
            hero->poisons[hero->poisoncount].type = poisoncontainer[i].type;
            if(poisoncontainer[i].type == 1){
                scounter++;
            }
            else if(poisoncontainer[i].type == 2){
                dcounter++;
            }
            else if(poisoncontainer[i].type == 3){
                hcounter++;
            }

            hero->poisoncount++;
            
            mvwprintw(messagewin , 0 , 0 , "You add poison type : %d" , poisoncontainer[i].type);
            wrefresh(messagewin);
            sleep(2);
            container[x][y] = '.';
            break;
            
        }
    }
}
void usepoison(WINDOW* win , WINDOW*  messagewin , Hero* hero  ){
    wclear(messagewin);
    mvwprintw(messagewin , 0 , 0  , "YOUR POISONS : ");
    mvwprintw(messagewin , 1 , 0 , "Speed : %d" , scounter);
    mvwprintw(messagewin , 2 ,0 , "Damage : %d" , dcounter);
    mvwprintw(messagewin , 3 , 0 , "Health : %d" , hcounter);
    echo();
    char yorn;
    mvwprintw(messagewin , 4 , 0 ,"Use poisons? : ");
    wscanw(messagewin , "%c" , &yorn);
    noecho();
    if(yorn == 'n'){
        return;
    }


    int choice;
    
    mvwprintw(messagewin , 5 , 0 , "Enter a  number : ");
    wscanw(messagewin , "%d" , &choice);
    noecho();
    if(choice > 3){
        return;
    }
    if(choice == 1 && scounter !=0){
        return;
    }
    else if(choice == 2 && dcounter != 0){
        return;
    }
    else if(choice == 3 &&hcounter != 0){
        return;

    }



}
void bgoldsinroom(WINDOW* win , Room* rooms , int roomcount , char container[MAP_WIDTH][MAP_HEIGHT] , BGold* bgoldcontainer){
    int counter = 0;
    for(int i= 0 ; i< roomcount ; i++){
        int bgoldcount;
        if(rooms[i].typeroom == 3){
            bgoldcount = 2;
        }
        else {
            bgoldcount = rand() % 2;
        }
        rooms[i].bgoldcounter = bgoldcount;
        for(int j =0 ; j<bgoldcount ; j++){
            int xrnd = rooms[i].x + 1 + rand() % (rooms[i].width - 2);
            int yrnd = rooms[i].y + 1 + rand() % (rooms[i].height - 2);
            
            rooms[i].bgolds[j].x = xrnd ;
            rooms[i].bgolds[j].y = yrnd;
            
            bgoldcontainer[counter].x =xrnd;
            bgoldcontainer[counter].y = yrnd;
            
            counter ++;
            mvwaddch(win, yrnd , xrnd , 'X');
            container[xrnd][yrnd] = 'X';
            wrefresh(win);            
        }
    }
}
void isitinbgold(WINDOW* win,WINDOW* messagewin ,int x , int y , Hero* hero , BGold* bgoldcontainer){
    chtype ch = mvwinch(win , y ,x);
    if((char)ch == 'X'){
        wclear(messagewin);
        for(int i= 0 ; i<50 ; i++){
            if( x == bgoldcontainer[i].x && y== bgoldcontainer[i].y){
                mvwprintw(messagewin , 0 , 0 , "you earned blck gold!");
                hero->goldcount += 4;
                container[x][y]='.';
            }
        }
        
        
    }
    
}

void goldsinroom(WINDOW* win  , Room* rooms , int roomcount , char container[MAP_WIDTH][MAP_HEIGHT] , Gold* goldcontainer){
    
    


    int counter = 0;
    for(int i= 0 ; i< roomcount ; i++){
        int goldcount;
        if(rooms[i].typeroom == 3){
            goldcount = 4;
        }
        else {
            goldcount = rand() % 2 + 1;
        }
        rooms[i].goldcount = goldcount;
        for(int j =0 ; j<goldcount ; j++){
            int xrnd = rooms[i].x + 1 + rand() % (rooms[i].width - 2);
            int yrnd = rooms[i].y + 1 + rand() % (rooms[i].height - 2);
            int what = rand() % 3 + 1;
            rooms[i].golds[j].x = xrnd ;
            rooms[i].golds[j].y = yrnd;
            rooms[i].golds[j].value = what;
            goldcontainer[counter].x =xrnd;
            goldcontainer[counter].y = yrnd;
            goldcontainer[counter].value = what;
            counter ++;
            mvwaddch(win, yrnd , xrnd , '$');
            container[xrnd][yrnd] = '$';
            wrefresh(win);            
        }
    }
}

void isitingold(WINDOW* win,WINDOW* messagewin ,int x , int y , Hero* hero , Gold* goldcontainer){
    chtype ch = mvwinch(win , y ,x);
    if((char)ch == '$'){
        wclear(messagewin);
        for(int i= 0 ; i<50 ; i++){
            if( x == goldcontainer[i].x && y== goldcontainer[i].y){
                mvwprintw(messagewin , 0 , 0 , "you earned %d golds!" , goldcontainer[i].value);
                hero->goldcount += goldcontainer[i].value;
                container[x][y]='.';
            }
        }
        
        
    }
    
}
int isitinfood(WINDOW* win , int x , int y){
    chtype ch  = mvwinch(win , y , x);
    if((char)ch == 'G' || (char)ch == 'M' || (char)ch == 'A' || (char)ch == 'K'){
        return 1;
    }
    return 0;
}
void addfoodhero(WINDOW* win , Hero* hero , char container[MAP_WIDTH][MAP_HEIGHT] , int x , int y ){
    if (hero->food > 5){
        return;
    }
    chtype ch = mvwinch(win , y , x);
    if((char)ch =='A'){
        hero->foods[hero->food].type = 1;
        hero->food++;
        mvwaddch(win , y , x , '.');
        container[x][y]= '.';
    }
    if((char)ch =='G'){
        hero->foods[hero->food].type = 2;
        hero->food++;
        mvwaddch(win , y , x , '.');
        container[x][y]= '.';
    }
    if((char)ch =='M'){
        hero->foods[hero->food].type = 3;
        hero->food++;
        mvwaddch(win , y , x , '.');
        container[x][y]= '.';
    }
    if((char)ch =='K'){
        hero->foods[hero->food].type = 4;
        hero->food++;
        mvwaddch(win , y , x , '.');
        container[x][y]= '.';
    }

    
}
void removeFood(Hero* hero, int index) {
    if (index >= hero->food || index < 0) return; // بررسی اینکه اندیس معتبر باشد
    
    for (int i = index; i < hero->food - 1; i++) {
        hero->foods[i] = hero->foods[i + 1];
    }
    hero->food -= hero->food; // کاهش تعداد غذاها
}

void showingfoods(WINDOW* win , WINDOW* messagewin , Hero* hero){
    int count =hero->food;
    if(count==0){
        mvwprintw(messagewin , 0 , 0 , "No food !");
        wrefresh(messagewin);
        return ;
    }
    
    int count_copy = 0;
    char* typecontainer[30];
    for(int i=0 ; i<count ; i++){
        int type = hero->foods[i].type;
        switch (type) {
            case 1:
                typecontainer[count_copy] = "Apple";
                count_copy++;
                break;
            case 2 :
                typecontainer[count_copy] = "Golden Apple";
                count_copy++;
                break;
            case 3:
                typecontainer[count_copy] = "Magic Apple";
                count_copy++;
                break;
            case 4:
                typecontainer[count_copy] = "Poisin Apple";
                count_copy++;
                break;
        }
    }
    wclear(messagewin);
    for(int i=0 ; i<count ; i++){
        mvwprintw(messagewin,i+1 , 0 , "%s\n" , typecontainer[i]);
    }
    mvwprintw(messagewin, count + 1, 0, "Choose a number to eat:");
    wrefresh(messagewin);
    
    echo();
    int choice;
    wscanw(messagewin, "%d", &choice);
    noecho();

    if (choice >= 0 && choice < count) {
        if(strcmp(typecontainer[choice] , "Apple") == 0){
            hero->heart += 2;

        }
        else if(strcmp(typecontainer[choice] , "Golden Apple") == 0){
            hero->heart +=4;
        }
        else if(strcmp(typecontainer[choice] , "Magic Apple") == 0){
            hero->heart +=6;

        }
        else if(strcmp(typecontainer[choice] , "Poison Apple") == 0){
            hero->heart -= 3;
        }
        mvwprintw(messagewin, count + 2, 0, "Eating food %d...", choice);
        wrefresh(messagewin);

        // حذف غذا از اینونتوری
        removeFood(hero, choice);
    } else {
        mvwprintw(messagewin, count + 2, 0, "Invalid choice!");
        wrefresh(messagewin);
        return 0;
    }
}
void monstersinroom(WINDOW* win , Room* rooms , int roomcount , Monster* monstercontainer  , char container[MAP_WIDTH][MAP_HEIGHT] ){
    int count =0 ;
    for(int i=0 ; i<roomcount; i++){
        int mon;
        if(rooms[i].typeroom == 3){
            mon = 3;
        }
        else{
            mon = rand() % 2;
        }
        rooms[i].monstercount = mon;
        for(int j=0 ; j<mon ; j++){
            int xrnd = rooms[i].x + 1 + rand() % (rooms[i].width - 2);
            int yrnd = rooms[i].y + 1 + rand() % (rooms[i].height - 2);
            int typeg = rand() % 5;

            switch(typeg){
                case 1:
                    mvwaddch(win , yrnd , xrnd , 'N');
                    container[xrnd][yrnd] = 'N';
                    wrefresh(win);
                    rooms[i].monsters[j].x = xrnd;
                    rooms[i].monsters[j].y = yrnd;
                    rooms[i].monsters[j].type = typeg;
                    rooms[i].monsters[j].health = 5;
                    rooms[i].monsters[j].damage = 1;
                    rooms[i].monsters[j].active = false;
                    rooms[i].monsters[j].moves = 5;
                    rooms[i].monsters[j].nmd = 'N';
                    rooms[i].monsters[j].die = false;
                    monstercontainer[count].die = false ;
                    monstercontainer[count].nmd = 'N';
                    monstercontainer[count].moves = 5;
                    monstercontainer[count].x = xrnd;
                    monstercontainer[count].y = yrnd;
                    monstercontainer[count].type = typeg;
                    monstercontainer[count].health = 5;
                    monstercontainer[count].damage = 1;
                    monstercontainer[count].active = false;
                    count++;
                    break;
                
                case 2:
                    mvwaddch(win , yrnd , xrnd , 'F');
                    container[xrnd][yrnd] = 'F';
                    wrefresh(win);
                    rooms[i].monsters[j].x = xrnd;
                    rooms[i].monsters[j].y = yrnd;
                    rooms[i].monsters[j].type = typeg;
                    rooms[i].monsters[j].health = 10;
                    rooms[i].monsters[j].damage = 2;
                    rooms[i].monsters[j].active = false;
                    rooms[i].monsters[j].moves = 7;
                    rooms[i].monsters[j].nmd = 'F';
                    rooms[i].monsters[j].die = false;
                    monstercontainer[count].die = false ;
                    monstercontainer[count].nmd = 'F';
                    monstercontainer[count].moves = 7;
                    monstercontainer[count].x = xrnd;
                    monstercontainer[count].y = yrnd;
                    monstercontainer[count].type = typeg;
                    monstercontainer[count].health = 10;
                    monstercontainer[count].damage = 2;
                    monstercontainer[count].active = false;
                    count++;
                    break;

                case 3:

                    mvwaddch(win , yrnd , xrnd , 'T');
                    container[xrnd][yrnd] = 'T';
                    wrefresh(win);
                    rooms[i].monsters[j].x = xrnd;
                    rooms[i].monsters[j].y = yrnd;
                    rooms[i].monsters[j].type = typeg;
                    rooms[i].monsters[j].health = 15;
                    rooms[i].monsters[j].damage = 3;
                    rooms[i].monsters[j].active = false;
                    rooms[i].monsters[j].moves = 8;
                    rooms[i].monsters[j].nmd = 'T';
                    rooms[i].monsters[j].die = false;
                    monstercontainer[count].die = false ;
                    monstercontainer[count].nmd = 'T';
                    monstercontainer[count].moves = 8;
                    monstercontainer[count].x = xrnd;
                    monstercontainer[count].y = yrnd;
                    monstercontainer[count].type = typeg;
                    monstercontainer[count].health = 15;
                    monstercontainer[count].damage = 3;
                    monstercontainer[count].active = false;
                    count++;
                    break;
                
                case 4 :
                    mvwaddch(win , yrnd , xrnd , 'E');
                    container[xrnd][yrnd] = 'E';
                    wrefresh(win);
                    rooms[i].monsters[j].x = xrnd;
                    rooms[i].monsters[j].y = yrnd;
                    rooms[i].monsters[j].type = typeg;
                    rooms[i].monsters[j].health = 20;
                    rooms[i].monsters[j].damage = 4;
                    rooms[i].monsters[j].active = false;
                    rooms[i].monsters[j].moves = 10;
                    rooms[i].monsters[j].nmd = 'E';
                    rooms[i].monsters[j].die = false;
                    monstercontainer[count].die = false ;
                    monstercontainer[count].nmd = 'E';
                    monstercontainer[count].moves = 10;
                    monstercontainer[count].x = xrnd;
                    monstercontainer[count].y = yrnd;
                    monstercontainer[count].type = typeg;
                    monstercontainer[count].health = 20;
                    monstercontainer[count].damage = 4;
                    monstercontainer[count].active = false;
                    count++;
                    break;

                case 5:
                    mvwaddch(win , yrnd , xrnd , 'U');
                    container[xrnd][yrnd] = 'U';
                    wrefresh(win);
                    rooms[i].monsters[j].x = xrnd;
                    rooms[i].monsters[j].y = yrnd;
                    rooms[i].monsters[j].type = typeg;
                    rooms[i].monsters[j].health = 30;
                    rooms[i].monsters[j].damage = 5;
                    rooms[i].monsters[j].active = false;
                    rooms[i].monsters[j].moves = 12;
                    rooms[i].monsters[j].nmd = 'U';
                    rooms[i].monsters[j].die = false;
                    monstercontainer[count].die = false ;
                    monstercontainer[count].nmd = 'U';
                    monstercontainer[count].moves = 12;
                    monstercontainer[count].x = xrnd;
                    monstercontainer[count].y = yrnd;
                    monstercontainer[count].type = typeg;
                    monstercontainer[count].health = 30;
                    monstercontainer[count].damage = 5;
                    monstercontainer[count].active = false;
                    count++;
                    break;

                
            }
        }
    }
    enemy = count;
}
int aroundhero(Hero* hero, Monster monster) {
    int dx = abs(hero->x - monster.x);
    int dy = abs(hero->y - monster.y);
    return (dx <= 1 && dy <= 1); // اگر دشمن در همسایگی قهرمان باشد
}


void activatemonsters(WINDOW* win, Room* rooms, int roomcount, int x, int y, Monster* monstercontainer, Hero* hero, WINDOW* messagewin) {
    static int monster_move_counter = 0; // شمارنده برای کنترل حرکت
    monster_move_counter++;


    int room = inside(x, y, rooms, roomcount);
    if (room == -1) {
        return;
    }

    for (int i = 0; i < rooms[room].monstercount ; i++) {
        if(!rooms[room].monsters[i].die){
            rooms[room].monsters[i].active = true;

            // ذخیره موقعیت قبلی
            int prevX = rooms[room].monsters[i].x;
            int prevY = rooms[room].monsters[i].y;

            wattron(messagewin , COLOR_PAIR(2));

            mvwaddch(win, prevY, prevX, container[prevX][prevY]);

            wattroff(messagewin , COLOR_PAIR(2));

            // محاسبه فاصله از قهرمان
            int distance = abs(hero->x - prevX) + abs(hero->y - prevY);

            // اگر خیلی نزدیک است، کندتر حرکت کند
            if (distance < 3 && monster_move_counter % 5 != 0) { 
                continue;
            }

            // حرکت به سمت قهرمان
            rooms[room].monsters[i].x += (hero->x > prevX) ? 1 : (hero->x < prevX) ? -1 : 0;
            rooms[room].monsters[i].y += (hero->y > prevY) ? 1 : (hero->y < prevY) ? -1 : 0;

            // بررسی برخورد با قهرمان
            if (aroundhero(hero, rooms[room].monsters[i])) {
                hero->heart -= rooms[room].monsters[i].damage;
                mvwprintw(messagewin, 0, 0, "Monster hit you! Health: %d", hero->heart);
                wrefresh(messagewin);
            }

            wattron(messagewin , COLOR_PAIR(6));
            mvwaddch(win, rooms[room].monsters[i].y, rooms[room].monsters[i].x, rooms[room].monsters[i].nmd);
            wattroff(messagewin , COLOR_PAIR(6));
            wrefresh(win);
            usleep(200000);
            } // تاخیر بیشتر برای حرکت کندتر
    }
}
void attackmonster(WINDOW* win , WINDOW* messagewin, Hero* hero, Room* rooms, int roomcount, Monster* monstercontainer){
    int room = inside(hero->x  ,hero->y , rooms , roomcount);
    if(room == -1){
        return;
    }
    for(int i=0 ; i<rooms[room].monstercount ; i++){
        int damage =0 ;
        int mx = rooms[room].monsters[i].x;
        int my = rooms[room].monsters[i].y;
        if(abs(hero->x - mx) <= 1 && abs(hero->y - my) <= 1 && hero->typeofInitialGun == 1){
            damage = 5; 
            rooms[room].monsters[i].health -= damage;
            mvwprintw(messagewin ,0 ,0 ,"You attacked with mace!");
            wrefresh(messagewin);
            sleep(1);
            if(rooms[room].monsters[i].health <= 0){
                rooms[room].monsters[i].die = true;
                mvwprintw(messagewin , 0  , 0 , "you killed enemy %c!" , rooms[room].monsters[i].nmd);
                wrefresh(messagewin);
                sleep(1);
            }
            return;
        }
        else if(abs(hero->x - mx) <= 1 && abs(hero->y - my) <= 1 && hero->typeofInitialGun == 5 ){
            damage = 10; 
            rooms[room].monsters[i].health -= damage;
            mvwprintw(messagewin ,0 ,0 ,"You attacked with sword!");
            wrefresh(messagewin);
            sleep(1);
            if(rooms[room].monsters[i].health <= 0){
                rooms[room].monsters[i].die = true;
                mvwprintw(messagewin , 0  , 0 , "you killed enemy %c!" , rooms[room].monsters[i].nmd);
                wrefresh(messagewin);
                sleep(1);
            }
            return;
        }
        else if(hero->typeofInitialGun == 2  && daggercounter > 0){
            damage = 12;
            int c = getch();
            if(c=='d'){
                for(int k=hero->x ; k<hero->x + 6 ; k++){
                    chtype ch = mvwinch(win , hero->y  ,k);
                    if(rooms[room].monsters[i].x == k && rooms[room].monsters[i].y == hero->y){
                        rooms[room].monsters[i].health -= damage;
                        mvwprintw(messagewin , 0 , 0, "You attacked with dagger!");
                        wrefresh(messagewin);
                        sleep(1);
                        if(rooms[room].monsters[i].health <= 0){
                            rooms[room].monsters[i].die = true;
                            mvwprintw(messagewin , 0  , 0 , "you killed enemy %c!" , rooms[room].monsters[i].nmd);
                            wrefresh(messagewin);
                            sleep(1);
                        }
                        daggercounter --;
                        return;
                    
                    }
                    else if((char)ch == '|'){
                        mvwaddch(win , hero->y , k-1 , '!');
                        container[k-1][hero->y] = '!';
                        guncontainer[guns].used = 0;
                        guncontainer[guns].x = k-1;
                        guncontainer[guns].y = hero->y;
                        guncontainer[guns].type = 2;
                        guns++;
                        daggercounter--;
                        wrefresh(win);
                        return;
                    }
                }
            }
            else if(c=='a'){
                for(int k=hero->x ; k>hero->x - 6 ; k--){
                    chtype ch = mvwinch(win , hero->y , k);
                    if(rooms[room].monsters[i].x == k && rooms[room].monsters[i].y == hero->y){
                        rooms[room].monsters[i].health -= damage;
                        mvwprintw(messagewin , 0 , 0, "You attacked with dagger!");
                        wrefresh(messagewin);
                        sleep(1);
                        if(rooms[room].monsters[i].health <= 0){
                            rooms[room].monsters[i].die = true;
                            mvwprintw(messagewin , 0  , 0 , "you killed enemy %c!" , rooms[room].monsters[i].nmd);
                            wrefresh(messagewin);
                            sleep(1);
                        }
                        daggercounter--;
                        return;
                    }
                    else if((char)ch == '|'){
                        mvwaddch(win , hero->y , k+1 , '!');
                        container[k+1][hero->y] = '!';
                        guncontainer[guns].used = 0;
                        guncontainer[guns].x = k+1;
                        guncontainer[guns].y = hero->y;
                        guncontainer[guns].type = 2;
                        guns++;
                        daggercounter--;
                        wrefresh(win);
                        return;
                    }
                    
                }

            }
            else if(c=='w'){
                for(int k=hero-> y; k>hero->y - 6 ; k--){
                    chtype ch = mvwinch(win , k , hero->x);
                    if(rooms[room].monsters[i].y == k && rooms[room].monsters[i].x == hero->x){
                        rooms[room].monsters[i].health -= damage;
                        mvwprintw(messagewin , 0 , 0, "You attacked with dagger!");
                        wrefresh(messagewin);
                        sleep(1);
                        if(rooms[room].monsters[i].health <= 0){
                            rooms[room].monsters[i].die = true;
                            mvwprintw(messagewin , 0  , 0 , "you killed enemy %c!" , rooms[room].monsters[i].nmd);
                            wrefresh(messagewin);
                            sleep(1);
                        }
                        daggercounter--;
                        return;
                    }
                    else if((char)ch == '_'){
                        mvwaddch(win ,k+1 ,hero->x, '!');
                        container[hero->x][k+1] = '!';
                        guncontainer[guns].used = 0;
                        guncontainer[guns].x = hero->x;
                        guncontainer[guns].y = k+1;
                        guncontainer[guns].type = 2;
                        guns++;
                        daggercounter--;
                        wrefresh(win);
                        return;
                    }
                }
            }
            else if(c=='s'){
                for(int k=hero->y ; k<hero->y + 6 ; k++){
                    chtype ch  = mvwinch(win , k , hero->x);
                    if(rooms[room].monsters[i].y == k && rooms[room].monsters[i].x == hero->x){
                        rooms[room].monsters[i].health -= damage;
                        mvwprintw(messagewin , 0 , 0, "You attacked with dagger!");
                        wrefresh(messagewin);
                        sleep(2);
                        if(rooms[room].monsters[i].health <= 0){
                            rooms[room].monsters[i].die = true;
                            mvwprintw(messagewin , 0  , 0 , "you killed enemy %c!" , rooms[room].monsters[i].nmd);
                            wrefresh(messagewin);
                            sleep(1);
                        }
                        daggercounter--;
                        return;
                    }

                    else if((char)ch == '_'){
                        mvwaddch(win , k-1 ,hero->x , '!');
                        container[hero->x][k-1] = '!';
                        guncontainer[guns].used = 0;
                        guncontainer[guns].x =hero->x;
                        guncontainer[guns].y = k-1;
                        guncontainer[guns].type = 2;
                        guns++;
                        daggercounter--;
                        wrefresh(win);
                        return;
                    }
                }
            }
        else if(hero->typeofInitialGun == 3  && mwandcounter > 0){
            damage = 15;
            int c = getch();
            if(c=='d'){
                for(int k=hero->x ; k<hero->x + 10 ; k++){
                    chtype ch = mvwinch(win , hero->y  ,k);
                    if(rooms[room].monsters[i].x == k && rooms[room].monsters[i].y == hero->y){
                        rooms[room].monsters[i].health -= damage;
                        mvwprintw(messagewin , 0 , 0, "You attacked with magicwand!");
                        
                        wrefresh(messagewin);
                        sleep(1);
                        if(rooms[room].monsters[i].health <= 0){
                            rooms[room].monsters[i].die = true;
                            mvwprintw(messagewin , 0 , 0 , "you killed enemy %c" , rooms[room].monsters[i].nmd);
                            sleep(1);
                        }
                        mwandcounter --;
                        rooms[room].monsters[i].stop = true;
                        return;
                    
                    }
                    else if((char)ch == '|'){
                        mvwaddch(win , hero->y , k-1 , 'I');
                        container[k-1][hero->y] = 'I';
                        guncontainer[guns].used = 0;
                        guncontainer[guns].x = k-1;
                        guncontainer[guns].y = hero->y;
                        guncontainer[guns].type = 3;
                        guns++;
                        mwandcounter--;
                        wrefresh(win);
                        return;
                    }
                }
            }
            else if(c=='a'){
                for(int k=hero->x ; k>hero->x - 10 ; k--){
                    chtype ch = mvwinch(win , hero->y , k);
                    if(rooms[room].monsters[i].x == k && rooms[room].monsters[i].y == hero->y){
                        rooms[room].monsters[i].health -= damage;
                        mvwprintw(messagewin , 0 , 0, "You attacked with magicwand!");
                        wrefresh(messagewin);
                        sleep(1);
                        if(rooms[room].monsters[i].health <= 0){
                            rooms[room].monsters[i].die = true;
                            mvwprintw(messagewin , 0 , 0 , "you killed enemy %c" , rooms[room].monsters[i].nmd);
                            wrefresh(messagewin);
                            sleep(1);
                        }
                        mwandcounter--;
                        rooms[room].monsters[i].stop = true;
                        return;
                    }
                    else if((char)ch == '|'){
                        mvwaddch(win , hero->y , k+1 , 'I');
                        container[k+1][hero->y] = 'I';
                        guncontainer[guns].used = 0;
                        guncontainer[guns].x = k+1;
                        guncontainer[guns].y = hero->y;
                        guncontainer[guns].type = 3;
                        guns++;
                        mwandcounter--;
                        wrefresh(win);
                        return;
                    }
                    
                }

            }
            else if(c=='w'){
                for(int k=hero-> y; k>hero->y - 10 ; k--){
                    chtype ch = mvwinch(win , k , hero->x);
                    if(rooms[room].monsters[i].y == k && rooms[room].monsters[i].x == hero->x){
                        rooms[room].monsters[i].health -= damage;
                        mvwprintw(messagewin , 0 , 0, "You attacked with magicwand!");
                        wrefresh(messagewin);
                        sleep(1);
                        if(rooms[room].monsters[i].health <= 0){
                            rooms[room].monsters[i].die = true;
                            mvwprintw(messagewin , 0 , 0 , "you killed enemy %c" , rooms[room].monsters[i].nmd);
                            wrefresh(messagewin);
                            sleep(1);
                        }
                        mwandcounter--;
                        rooms[room].monsters[i].stop = true;
                        return;
                    }
                    else if((char)ch == '_'){
                        mvwaddch(win ,k+1 ,hero->x, 'I');
                        container[hero->x][k+1] = 'I';
                        guncontainer[guns].used = 0;
                        guncontainer[guns].x = hero->x;
                        guncontainer[guns].y = k+1;
                        guncontainer[guns].type = 3;
                        guns++;
                        mwandcounter--;
                        wrefresh(win);
                        return;
                    }
                }
            }
            else if(c=='s'){
                for(int k=hero->y ; k<hero->y + 10 ; k++){
                    chtype ch  = mvwinch(win , k , hero->x);
                    if(rooms[room].monsters[i].y == k && rooms[room].monsters[i].x == hero->x){
                        rooms[room].monsters[i].health -= damage;
                        mvwprintw(messagewin , 0 , 0, "You attacked with magicwand!");
                        wrefresh(messagewin);
                        sleep(1);
                        if(rooms[room].monsters[i].health <= 0){
                            rooms[room].monsters[i].die = true;
                            mvwprintw(messagewin , 0 , 0 , "you killed enemy %c" , rooms[room].monsters[i].nmd);
                            wrefresh(messagewin);
                            sleep(1);
                        }
                        daggercounter--;
                        rooms[room].monsters[i].stop = true;
                        return;
                    }

                    else if((char)ch == '_'){
                        mvwaddch(win , k-1 ,hero->x , 'I');
                        container[hero->x][k-1] = 'I';
                        guncontainer[guns].used = 0;
                        guncontainer[guns].x = hero->x;
                        guncontainer[guns].y = k-1;
                        guncontainer[guns].type = 3;
                        guns++;
                        daggercounter--;
                        wrefresh(win);
                        return;
                    }
                }
            }
        
        }
    }
}
}

    




void drawRoom(WINDOW* win, Room room) {
    for (int i = 0; i < room.height; i++) {
        for (int j = 0; j < room.width; j++) {
            if (i == 0) {
                mvwaddch(win, room.y + i, room.x + j, '_');
            } else if (i == room.height - 1) {
                if (j == 0 || j == room.width - 1) {
                    mvwaddch(win, room.y + i, room.x + j, '|');
                } else {
                    mvwaddch(win, room.y + i, room.x + j, '_');
                }
            } else {
                if (j == 0 || j == room.width - 1) {
                    mvwaddch(win, room.y + i, room.x + j, '|');
                } else {
                    mvwaddch(win, room.y + i, room.x + j, '.');
                }
            }
        }
    }
    drawpillar(win, room);
}
void roomtype(WINDOW* win , Room* rooms , int roomcount){
    if(floorcount == 4){
        rooms[roomcount -1].typeroom = 3;
        int poison = roomcount / 3 ; 
        for(int i=0 ; i<poison ; i++){
            rooms[i].typeroom = 2;

        }
        rooms[roomcount - 2].typeroom = 4;


    }
    else {
        int poison = roomcount / 3;
        for(int i=0 ; i<poison ; i++){
            rooms[i].typeroom = 2;
        }
        rooms[roomcount - 1].typeroom = 4;

    }
}

void drawCorridor(WINDOW* win, int x1, int y1, int x2, int y2, Room* rooms, int roomCount) {
    int x = x1, y = y1;

    while (x != x2 || y != y2) {
        if (!isInsideRoom(x, y, rooms, roomCount)) {
            mvwaddch(win, y, x, '#');
        } else {
            chtype ch1 = mvwinch(win, y + 1, x) & A_CHARTEXT;
            chtype ch2 = mvwinch(win, y - 1, x) & A_CHARTEXT;
            chtype ch3 = mvwinch(win, y, x + 1) & A_CHARTEXT;
            chtype ch4 = mvwinch(win, y, x - 1) & A_CHARTEXT;

            if ((ch1 == '#' || ch2 == '#' || ch3 == '#' || ch4 == '#')) {
                mvwaddch(win, y, x, '+');
            }
        }

        if (x != x2 && y != y2) {
            x += (x < x2) ? 1 : -1;
            y += (y < y2) ? 1 : -1;
        } else if (x != x2) {
            x += (x < x2) ? 1 : -1;
        } else if (y != y2) {
            y += (y < y2) ? 1 : -1;
        }
    }
}

void makedoor(WINDOW* win, int x, int y, Room* rooms, int roomCount) {
    chtype ch1 = mvwinch(win, y + 1, x);
    chtype ch2 = mvwinch(win, y - 1, x);
    chtype ch3 = mvwinch(win, y, x + 1);
    chtype ch4 = mvwinch(win, y, x - 1);

    if (isInsideRoom(x, y, rooms, roomCount)) {
        if ((char)ch1 == '#' || (char)ch2 == '#' || (char)ch3 == '#' || (char)ch4 == '#') {
            mvwaddch(win, y, x, '+');
        }
    }
}
int timer(int seconds) {
    clock_t start_time = clock(); // زمان شروع
    while (1) {
        clock_t current_time = clock();
        double elapsed_time = (double)(current_time - start_time) / CLOCKS_PER_SEC;

        if (elapsed_time >= seconds) {
            return 0;
            
        }
    }
}
void passwordkey(WINDOW* mapWin , Room* rooms  , char container[MAP_WIDTH][MAP_HEIGHT]){
    Room room = rooms[4];
    int xrnd = room.x + 1 + rand() % (room.width - 2);
    int yrnd = room.y + 1 + rand() % (room.height - 2);
    container[xrnd][yrnd] = '&';
    mvwaddch(mapWin , yrnd , xrnd , '&');
    
}
int generatepassword(WINDOW* win , WINDOW* messagewin , Pdoor* pdoor){
    int password = 1000 + rand() % 9000;
    pdoor->password = password;
    
    pdoor->open = false;
    return password;
}
void isonpasswordkey(WINDOW* win, WINDOW* messagewin, int x, int y, Pdoor* pdoor) {
    wclear(messagewin);
    chtype ch = mvwinch(win, y, x) & A_CHARTEXT; // خواندن کاراکتر در موقعیت (x, y)
    if ((char)ch == '&') { // اگر کاراکتر '&' باشد
        mvwprintw(messagewin, 0, 0, "You found the key!"); // پیام پیدا کردن کلید
        wrefresh(messagewin); // به‌روزرسانی پنجره

        // تولید رمز و ذخیره‌سازی آن در ساختار pdoor
        int password = generatepassword(win, messagewin, pdoor);
        mvwprintw(messagewin , 1 , 0 , "%d" , password);        



        wrefresh(messagewin);
    }
}



bool isitpassdoor(WINDOW* win, WINDOW* messagewin, int x, int y, Pdoor* pdoor) {

    wclear(messagewin);
    chtype ch = mvwinch(win, y, x);
    if ((char)ch == '@' && pdoor->open == false ){
        if (pdoor->alwaysclose == true) {
            attron(COLOR_PAIR(1));
             mvwprintw(messagewin, 0, 0, "The door is permanently locked!");
             attroff(COLOR_PAIR(1));
            wrefresh(messagewin);
            sleep(2);
            return false;
        }
        else{

 
            int input = -1; // مقداردهی اولیه برای جلوگیری از داده ناخواسته
            int wrongs = 0;

            while (wrongs < 3) {
                mvwprintw(messagewin, 0, 0, "Enter password: ");
                wrefresh(messagewin);
                echo(); // فعال کردن نمایش ورودی کاربر
                mvwscanw(messagewin, 1, 0, "%4d", &input);
                noecho(); // غیرفعال کردن نمایش ورودی کاربر

                if (input == pdoor->password) {
                    pdoor->open = true;
                    wattron(win , COLOR_PAIR(5));
                    mvwaddch(win , y ,x , '@');
                    wattroff(win , COLOR_PAIR(5));
                    mvwprintw(messagewin, 2, 0, "Access granted!");
                    wrefresh(messagewin);
                    sleep(2);
                    return true; // باز کردن درب
                } else {
                    wrongs++;
                    mvwprintw(messagewin, 2, 0, "Wrong password! Attempts left: %d", 3 - wrongs);
                    wrefresh(messagewin);
                }
            }

            // اگر تعداد تلاش‌ها تمام شود:
            mvwprintw(messagewin, 3, 0, "The door is now permanently locked!");
            wrefresh(messagewin);
            sleep(2);
            pdoor->alwaysclose = true; // درب برای همیشه بسته شود
        
            return false;
        }
    }
    return true;
}
void Dheartmove(Hero* hero , WINDOW* messagewin){
    if((hero->move) % 50 == 0){
        hero->heart --;
        mvwprintw(messagewin ,0,0, "Hero heart : %d" , hero->heart);
        
        wrefresh(messagewin);
        sleep(2);
    }
}


void generateFloor(WINDOW* mapWin, WINDOW* messagewin, Room rooms[], int* roomCount, Hero* hero, Stair* stair, bool seen[MAP_WIDTH][MAP_HEIGHT], char container[MAP_WIDTH][MAP_HEIGHT] , Pdoor* pdoor , Gold* goldcontainer) {
    floorcount++;
    werase(mapWin);
    mvwprintw(messagewin ,0,0, "You are in floor %d" , floorcount);
    wrefresh(messagewin);
    start_color();
    init_pair(1 , COLOR_RED , COLOR_BLACK);

    *roomCount = 0;
    memset(seen, 0, sizeof(seen)); // بازنشانی seen
    memset(container, '.', sizeof(container)); // بازنشانی container

    int tryes =0 ; 
    while (*roomCount < MAX_ROOMS && tryes<1000) {
        tryes++;
        Room newRoom;
        newRoom.width = rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE + 1) + MIN_ROOM_SIZE;
        newRoom.height = rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE + 1) + MIN_ROOM_SIZE;
        newRoom.x = rand() % (MAP_WIDTH - newRoom.width - 1) + 1;
        newRoom.y = rand() % (MAP_HEIGHT - newRoom.height - 1) + 1;
        newRoom.haspillar = haspillar(&newRoom);

        if (!doesOverlap(rooms, *roomCount, newRoom)) {
            rooms[*roomCount] = newRoom;
            (*roomCount)++;
        }
    }

    // رسم اتاق‌ها و راهروها
    for (int i = 0; i < *roomCount; i++) {
        drawRoom(mapWin, rooms[i]);

        if (i > 0) {
            int prevX = rooms[i - 1].x + rooms[i - 1].width / 2;
            int prevY = rooms[i - 1].y + rooms[i - 1].height / 2;
            int currX = rooms[i].x + rooms[i].width / 2;
            int currY = rooms[i].y + rooms[i].height / 2;
            drawCorridor(mapWin, prevX, prevY, currX, currY, rooms, *roomCount);
        }
        
    }

    roomtype(mapWin , rooms , *roomCount);
    placeTraps(rooms , trapcounter , *roomCount);
    foodsinroom(mapWin , rooms , *roomCount , container);
    bgoldsinroom( mapWin ,  rooms , *roomCount , container,bgoldcontainer);
    goldsinroom(mapWin , rooms , *roomCount , container , goldcontainer);
    poisonsinroom(mapWin , rooms , *roomCount , container , poisoncontainer);
    gunsinroom(mapWin , rooms ,*roomCount ,container , guncontainer);
    monstersinroom(mapWin, rooms , *roomCount , monstercontainer , container);
    // قرار دادن درها
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            makedoor(mapWin, i, j, rooms, *roomCount);
        }
    }
    char x_doors[60];
    char y_doors[60];
    int doorcount = 0;
    for(int i=0 ; i<MAP_WIDTH ; i++){
        for(int j=0; j<MAP_HEIGHT ; j++){
            chtype ch = mvwinch(mapWin , j , i);
            if((char)ch == '+'){
                x_doors[doorcount] = i;
                y_doors[doorcount] = j;
                doorcount++;
            }
        }
       
    }

    container[x_doors[3]][y_doors[3]] = '@';
    attron(COLOR_PAIR(1));
    mvwaddch(mapWin , y_doors[3], x_doors[3] , '@');
    attroff(COLOR_PAIR(1));
    wrefresh(mapWin);
    passwordkey(mapWin , rooms , container);
    pdoor->x = x_doors[3];
    pdoor->y =y_doors[3];
    
    pdoor->open = false;
    pdoor->alwaysclose = false;
    stair->exists = false;
    placestair(mapWin, rooms, *roomCount, stair);

    // قرار دادن قهرمان
    int heroX, heroY;
    HeroPlace(mapWin, rooms, *roomCount, &heroX, &heroY);
    hero->x = heroX;
    hero->y = heroY;
    mvwaddch(mapWin, hero->y, hero->x, '.');

    // به‌روزرسانی container و seen
    map_container(mapWin, container);
    updateVisibility(mapWin, *hero, 1, seen , rooms , roomCount);

    
    wrefresh(mapWin);
}
void save_game_binary(Hero* hero, char container[MAP_WIDTH][MAP_HEIGHT], int floorcount , WINDOW* messagewin) {
    FILE* file = fopen("save.bin", "wb");
    if (file == NULL) {
        perror("خطا در باز کردن فایل برای ذخیره");
        return;
    }

    // ذخیره اطلاعات قهرمان
    fwrite(hero, sizeof(Hero), 1, file);

    // ذخیره شماره طبقه
    fwrite(&floorcount, sizeof(int), 1, file);

    // ذخیره نقشه
    fwrite(container, sizeof(char), MAP_WIDTH * MAP_HEIGHT, file);

    fclose(file);
    wclear(messagewin);
    mvwprintw(messagewin, 0 , 0 , "game saved!");
    wrefresh(messagewin);
    sleep(1);
    
}
void save_explored_map(bool seen[MAP_WIDTH][MAP_HEIGHT], int floorcount , WINDOW* messagewin) {
    char filename[20];
    sprintf(filename, "save_floor_%d.bin", floorcount);  // ایجاد یک فایل برای هر طبقه

    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("خطا در ذخیره نقشه");
        return;
    }

    fwrite(&floorcount, sizeof(int), 1, file);  // ذخیره‌ی شماره‌ی طبقه
    fwrite(seen, sizeof(bool), MAP_WIDTH * MAP_HEIGHT, file);  // ذخیره وضعیت مناطق دیده‌شده

    fclose(file);
    mvwprintw(messagewin , 1 , 0 , "map saved! , please wait seconds!");
    wrefresh(messagewin);
    sleep(2);
}
void load_game_binary(Hero* hero, char container[MAP_WIDTH][MAP_HEIGHT], int* floorcount) {
    FILE* file = fopen("save.bin", "rb");
    if (file == NULL) {
        perror("فایل ذخیره‌شده‌ی باینری یافت نشد");
        return;
    }

    // خواندن اطلاعات قهرمان
    fread(hero, sizeof(Hero), 1, file);

    // خواندن شماره طبقه
    fread(floorcount, sizeof(int), 1, file);

    // خواندن نقشه
    fread(container, sizeof(char), MAP_WIDTH * MAP_HEIGHT, file);

    fclose(file);
    
}
void load_explored_map(bool seen[MAP_WIDTH][MAP_HEIGHT], int* floorcount) {
    char filename[20];
    sprintf(filename, "save_floor_%d.bin", *floorcount);

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("فایل ذخیره‌شده برای این طبقه یافت نشد، بازی جدیدی آغاز می‌شود.\n");
        memset(seen, false, sizeof(bool) * MAP_WIDTH * MAP_HEIGHT);  // مقداردهی پیش‌فرض
        return;
    }

    fread(floorcount, sizeof(int), 1, file);  // خواندن شماره‌ی طبقه از فایل
    fread(seen, sizeof(bool), MAP_WIDTH * MAP_HEIGHT, file);  // بارگذاری وضعیت دیده‌شدن

    fclose(file);
    printf("نقشه کشف‌شده طبقه %d بارگذاری شد.\n", *floorcount);
}



int main() {
    srand(time(0));
    setlocale(LC_ALL ,"");
    initscr();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1 , COLOR_RED , COLOR_BLACK);
    init_pair(2 , COLOR_MAGENTA , COLOR_BLACK);
    init_pair(3 , COLOR_CYAN , COLOR_BLACK);
    init_pair(4 , COLOR_YELLOW , COLOR_BLACK);
    init_pair(5 , COLOR_GREEN , COLOR_BLACK);
    init_pair(6, COLOR_BLUE , COLOR_BLACK);
    init_color(COLOR_CYAN , 500 , 300, 0);
    init_pair(7 , COLOR_CYAN , COLOR_BLACK);

    WINDOW* mapWin = newwin(MAP_HEIGHT + 2, MAP_WIDTH + 2, 0, 0);
    WINDOW* messagewin = newwin(MAP_HEIGHT + 2, 30, 0, MAP_WIDTH + 2);
     // مکان و ابعاد جدید
    box(messagewin, 0, 0); 
    


    
    refresh();

    Hero hero;
    hero.food = 0;
    hero.heart = 30;
    hero.move = 1;
    hero.typeofInitialGun = 1;
    Stair stair;
    Pdoor pdoor;
    Room rooms[MAX_ROOMS];
    int roomCount = 0;

    // تولید طبقه‌ی اول
    generateFloor(mapWin ,messagewin, rooms, &roomCount, &hero, &stair, seen, container , &pdoor , goldcontainer);
   
    keypad(stdscr, TRUE);
    wrefresh(mapWin);
    refresh();

    int c;
    do {
        if(floorcount == 1){
            drawSeenMap(mapWin, container, seen , &pdoor , rooms , roomCount);
        }
        if(floorcount == 2){
            drawSeenMap(mapWin, container, seen2 , &pdoor , rooms ,roomCount);
        }
        if(floorcount == 3){
            drawSeenMap(mapWin, container, seen3 , &pdoor , rooms , roomCount);
        }
        if(floorcount == 4){
            drawSeenMap(mapWin, container, seen4 , &pdoor ,rooms ,  roomCount);
        }
        c = getch();
        int prevX = hero.x;
        int prevY = hero.y;

        // حرکت قهرمان (همان کد قبلی شما)
        switch (c) {
            case 50: // پایین
                if (isitwallorO(mapWin, hero.x, hero.y + 1) && isitpassdoor(mapWin ,messagewin , hero.x , hero.y + 1 , &pdoor)) {
                    hero.y++;
                    hero.move ++;
                    Dheartmove(&hero , messagewin);
                    activatemonsters(mapWin, rooms , roomCount , hero.x , hero.y , monstercontainer , &hero , messagewin);
                    // اگر قهرمان از اتاق خارج شد، نقشه را بازسازی کن
 
                    

                    if(floorcount == 1){
                        markseen(hero.x , hero.y , seen);
                    }
                    if(floorcount == 2){
                        markseen(hero.x , hero.y , seen2);
                    }
                    if(floorcount == 3){
                        markseen(hero.x , hero.y , seen3);
                    }
                    if(floorcount == 4){
                        markseen(hero.x , hero.y , seen4);
                    }
                }
                break;
            case 56:
                if(isitwallorO(mapWin , hero.x , hero.y - 1 ) && isitpassdoor(mapWin ,messagewin , hero.x  , hero.y - 1, &pdoor)){
                    hero.y--;
                    hero.move++;
                    Dheartmove(&hero , messagewin);
                    activatemonsters(mapWin, rooms , roomCount , hero.x , hero.y , monstercontainer , &hero , messagewin);
 
                    
                    if(floorcount == 1){
                        markseen(hero.x , hero.y , seen);
                    }
                    if(floorcount == 2){
                        markseen(hero.x , hero.y , seen2);
                    }
                    if(floorcount == 3){
                        markseen(hero.x , hero.y , seen3);
                    }
                    if(floorcount == 4){
                        markseen(hero.x , hero.y , seen4);
                    }
                }
                break;
            case 54:
                if(isitwallorO(mapWin , hero.x + 1 , hero.y ) && isitpassdoor(mapWin ,messagewin , hero.x + 1 , hero.y , &pdoor)){
                    hero.x++;
                    hero.move++;
                    Dheartmove(&hero , messagewin);
                    activatemonsters(mapWin, rooms , roomCount , hero.x , hero.y , monstercontainer , &hero , messagewin);

                    
                    if(floorcount == 1){
                        markseen(hero.x , hero.y , seen);
                    }
                    if(floorcount == 2){
                        markseen(hero.x , hero.y , seen2);
                    }
                    if(floorcount == 3){
                        markseen(hero.x , hero.y , seen3);
                    }
                    if(floorcount == 4){
                        markseen(hero.x , hero.y , seen4);
                    }
                }
                break;
            case 52:
                if(isitwallorO(mapWin , hero.x - 1 , hero.y ) && isitpassdoor(mapWin ,messagewin , hero.x - 1 , hero.y , &pdoor)){
                    hero.x--;
                    hero.move++;
                    Dheartmove(&hero , messagewin);
                    activatemonsters(mapWin, rooms , roomCount , hero.x , hero.y , monstercontainer , &hero , messagewin);

                    
                     if(floorcount == 1){
                        markseen(hero.x , hero.y , seen);
                    }
                    if(floorcount == 2){
                        markseen(hero.x , hero.y , seen2);
                    }
                    if(floorcount == 3){
                        markseen(hero.x , hero.y , seen3);
                    }
                    if(floorcount == 4){
                        markseen(hero.x , hero.y , seen4);
                    }
                }
                break;
            case 55:
                if(isitwallorO(mapWin , hero.x - 1 , hero.y - 1 ) && isitpassdoor(mapWin ,messagewin , hero.x - 1 , hero.y - 1 , &pdoor)){
                    hero.x--;
                    hero.y--;
                    hero.move++;
                    Dheartmove(&hero , messagewin);
                    activatemonsters(mapWin, rooms , roomCount , hero.x , hero.y , monstercontainer , &hero , messagewin);

                    
                    if(floorcount == 1){
                        markseen(hero.x , hero.y , seen);
                    }
                    if(floorcount == 2){
                        markseen(hero.x , hero.y , seen2);
                    }
                    if(floorcount == 3){
                        markseen(hero.x , hero.y , seen3);
                    }
                    if(floorcount == 4){
                        markseen(hero.x , hero.y , seen4);
                    }
                }
                break;
            case 57:
                if(isitwallorO(mapWin , hero.x + 1 , hero.y - 1 ) && isitpassdoor(mapWin ,messagewin , hero.x + 1 , hero.y - 1 , &pdoor)){
                    hero.x ++;
                    hero.y --;
                    hero.move++;
                    Dheartmove(&hero , messagewin);
                    activatemonsters(mapWin, rooms , roomCount , hero.x , hero.y , monstercontainer , &hero , messagewin);

                    if(floorcount == 1){
                        markseen(hero.x , hero.y , seen);
                    }
                    if(floorcount == 2){
                        markseen(hero.x , hero.y , seen2);
                    }
                    if(floorcount == 3){
                        markseen(hero.x , hero.y , seen3);
                    }
                    if(floorcount == 4){
                        markseen(hero.x , hero.y , seen4);
                    }
                }
                break;
            case 49:
                if(isitwallorO(mapWin , hero.x - 1 , hero.y + 1 ) && isitpassdoor(mapWin ,messagewin , hero.x - 1 , hero.y + 1, &pdoor)){
                    hero.x --;
                    hero.y++;
                    hero.move++;
                    Dheartmove(&hero , messagewin);
                    activatemonsters(mapWin, rooms , roomCount , hero.x , hero.y , monstercontainer , &hero , messagewin);

                    if(floorcount == 1){
                        markseen(hero.x , hero.y , seen);
                    }
                    if(floorcount == 2){
                        markseen(hero.x , hero.y , seen2);
                    }
                    if(floorcount == 3){
                        markseen(hero.x , hero.y , seen3);
                    }
                    if(floorcount == 4){
                        markseen(hero.x , hero.y , seen4);
                    }
                }
                break;
            case 51:
                if(isitwallorO(mapWin , hero.x + 1 , hero.y + 1 ) && isitpassdoor(mapWin ,messagewin, hero.x + 1 , hero.y + 1, &pdoor)){
                    hero.x++;
                    hero.y++;
                    hero.move++;
                    Dheartmove(&hero , messagewin);
                    activatemonsters(mapWin, rooms , roomCount , hero.x , hero.y , monstercontainer , &hero , messagewin);

                    if(floorcount == 1){
                        markseen(hero.x , hero.y , seen);
                    }
                    if(floorcount == 2){
                        markseen(hero.x , hero.y , seen2);
                    }
                    if(floorcount == 3){
                        markseen(hero.x , hero.y , seen3);
                    }
                    if(floorcount == 4){
                        markseen(hero.x , hero.y , seen4);
                    }
                }
                break;
            case 101:
                if(isitinfood(mapWin , hero.x , hero.y)){
                    
                    showingfoods(mapWin , messagewin , &hero);
                }
                if(isitongun(mapWin ,messagewin , hero.x , hero.y)){
                    addtoinventory(mapWin , messagewin , &hero , hero.x , hero.y , guncontainer );
                }
                if(isitonpoison(mapWin , messagewin , hero.x , hero.y)){
                    addtoinventoryp(mapWin , messagewin , &hero , hero.x , hero.y , poisoncontainer);
                }
                break;
            case 117 :
                addfoodhero(mapWin , &hero ,container , hero.x , hero.y);
                break;
            case 105:
                changegun( mapWin ,  messagewin ,  &hero );
                break;
            case 112:
                usepoison(mapWin, messagewin , &hero);
                break;
            case 's':
                save_game_binary(&hero , container , floorcount , messagewin);
                if (floorcount == 1) {
                    save_explored_map(seen, floorcount , messagewin);
                } else if (floorcount == 2) {
                    save_explored_map(seen2, floorcount  ,messagewin);
                } else if (floorcount == 3) {
                    save_explored_map(seen3, floorcount , messagewin);
                } else if (floorcount == 4) {
                    save_explored_map(seen4, floorcount , messagewin);
                }
                break;
            case 32:
                attackmonster( mapWin ,  messagewin,  &hero, rooms, roomCount, monstercontainer);
                break;

            }
            

        chtype ch1  = mvwinch(mapWin , hero.y , hero.x);
        if((char)ch1 != 'D' && (char)ch1 != 'S' && (char)ch1 != 'C' && (char)ch1 != 'X'){
            isontrap(mapWin,messagewin , rooms, roomCount, &hero , trapcounter , container);
        }

        isonpasswordkey(mapWin , messagewin , hero.x , hero.y , &pdoor);
        isitingold( mapWin, messagewin ,hero.x , hero.y ,  &hero , goldcontainer);
        isitinbgold(mapWin, messagewin, hero.x , hero.y , &hero , bgoldcontainer);
        
        for(int i=0 ; i<enemy ; i++){
            if(monstercontainer[i].active == true){
                mvwaddch(mapWin, monstercontainer[i].y , monstercontainer[i].x , monstercontainer[i].nmd);
            }
        }

        wrefresh(messagewin);
        if (stair.exists && hero.x == stair.x && hero.y == stair.y) {
            
            generateFloor(mapWin, messagewin, rooms, &roomCount, &hero, &stair, seen, container , &pdoor , goldcontainer);
        }

        // به‌روزرسانی نقشه و موقعیت قهرمان
        if(floorcount == 1){
            updateVisibility(mapWin, hero, 1, seen , rooms , roomCount);
        }
        if(floorcount == 2){
            updateVisibility(mapWin, hero, 1, seen2 , rooms ,roomCount);
        }
        if(floorcount == 3){
            updateVisibility(mapWin, hero, 1, seen3 , rooms , roomCount);
        }
        if(floorcount == 4){
            updateVisibility(mapWin, hero, 1, seen4 , rooms , roomCount);
        }
        
        mvwaddch(mapWin, hero.y, hero.x, 'H');
        wrefresh(messagewin);
        wrefresh(mapWin);
        
    } while (c != 27); // خروج با دکمه ESC

    getch();
    delwin(mapWin);
    endwin();
    return 0;
}
