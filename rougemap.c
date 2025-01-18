#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define MAP_WIDTH 100
#define MAP_HEIGHT 30
#define MIN_ROOM_SIZE 4
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

int floorcount = 1;

void map_container(WINDOW* win, char container[MAP_WIDTH][MAP_HEIGHT]) {
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            chtype ch = mvwinch(win, j, i) & A_CHARTEXT;
            container[i][j] = (char)ch;
        }
    }
}

typedef struct {
    int x;
    int y;
    bool active;
} Trap;

typedef struct {
    char type[30];
} Poison;

typedef struct {
    int x, y;
    char HeroColor[30];
    int heart;
    int food;
    int gold;
} Hero;

typedef struct {
    int x, y;
    int width, height;
    bool haspillar;
    char typeofroom[30];
    bool visited;
    bool hastrap;
    int trapcount;
    Trap traps;
    bool secretdoor;
} Room;



void updateVisibility(WINDOW* win, Hero hero, int radius, bool seen[MAP_WIDTH][MAP_HEIGHT]) {
    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            int nx = hero.x + dx;
            int ny = hero.y + dy;

            if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT) {
                seen[nx][ny] = true;  // Mark as seen
            }
        }
    }
}

void drawSeenMap(WINDOW* win, char container[MAP_WIDTH][MAP_HEIGHT], bool seen[MAP_WIDTH][MAP_HEIGHT]) {
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            if (seen[x][y]) {
                chtype ch = container[x][y];
                mvwaddch(win, y, x, ch);  // Display the actual character
            } else {
                mvwaddch(win, y, x, ' ');  // Display empty space
            }
        }
    }
}
void markseen(int x , int y , bool seen[MAP_WIDTH][MAP_HEIGHT]){
    seen[x][y] = true;
}
bool isitwallorO(WINDOW* win, int x, int y) {
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

void placeTraps(Room* room) {
    if (oneOrTwo() == 2) { // 50% chance to place a trap
        room->hastrap = true;
        room->trapcount = 1; // Number of traps: one
        room->traps.x = room->x + 1 + rand() % (room->width - 2); // Random trap location
        room->traps.y = room->y + 1 + rand() % (room->height - 2);
        room->traps.active = false; // Trap is inactive until the hero steps on it
    } else {
        room->hastrap = false;
        room->trapcount = 0; // No traps
    }
}

void isontrap(WINDOW* win, Room* rooms, int roomCount, Hero* hero) {
    for (int i = 0; i < roomCount; i++) {
        Room* room = &rooms[i];
        if (room->hastrap && room->traps.active == false &&
            hero->x == room->traps.x && hero->y == room->traps.y) {
            // Display the trap at the hero's location
            mvwaddch(win, hero->y, hero->x, '^');
            room->traps.active = true; // Trap is activated
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

void typeroom(WINDOW* win, Room* rooms, int roomcount) {
    int array[4];
    for (int i = 0; i < 4; i++) {
        array[i] = rand() % roomcount;
    }
    for (int i = 0; i < 4; i++) {
        strcpy(rooms[array[i]].typeofroom, "Regular Room");
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

void passwordkey(WINDOW* mapWin , Room* rooms  , char container[MAP_WIDTH][MAP_HEIGHT]){
    Room room = rooms[4];
    int xrnd = room.x + 1 + rand() % (room.width - 2);
    int yrnd = room.y + 1 + rand() % (room.height - 2);
    container[xrnd][yrnd] = '&';
    mvwaddch(mapWin , yrnd , xrnd , '&');
}
void isonpasswordkey(WINDOW* win ,WINDOW* messagewin ,  int x , int y){
    chtype ch = mvwinch(win , y , x);
    if((char)ch == '&'){
        mvwprintw(messagewin ,0,0, "you find key!");
        wrefresh(messagewin);
    }
}
void generateFloor(WINDOW* mapWin, Room rooms[], int* roomCount, Hero* hero, Stair* stair, bool seen[MAP_WIDTH][MAP_HEIGHT], char container[MAP_WIDTH][MAP_HEIGHT]) {
    floorcount++;
    werase(mapWin);

    // بازنشانی متغیرها
    *roomCount = 0;
    memset(seen, 0, sizeof(seen)); // بازنشانی seen
    memset(container, '.', sizeof(container)); // بازنشانی container

    // تولید اتاق‌ها
    while (*roomCount < MAX_ROOMS) {
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
        placeTraps(&rooms[i]);
    }

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
    mvwaddch(mapWin , y_doors[3], x_doors[3] , '@');
    passwordkey(mapWin , rooms , container);
    // قرار دادن پله‌ها
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
    updateVisibility(mapWin, *hero, 1, seen);

    
    wrefresh(mapWin);
}

int main() {
    srand(time(0));
    initscr();
    noecho();
    curs_set(0);
    

    WINDOW* mapWin = newwin(MAP_HEIGHT + 2, MAP_WIDTH + 2, 0, 0);
    WINDOW* messagewin = newwin(7 , 14 , MAP_HEIGHT + 5 , MAP_HEIGHT + 5);
    
    refresh();

    Hero hero;
    Stair stair;
    Room rooms[MAX_ROOMS];
    int roomCount = 0;

    // تولید طبقه‌ی اول
    generateFloor(mapWin, rooms, &roomCount, &hero, &stair, seen, container);
   
    keypad(stdscr, TRUE);
    wrefresh(mapWin);
    refresh();

    int c;
    do {
        if(floorcount == 1){
            drawSeenMap(mapWin, container, seen);
        }
        if(floorcount == 2){
            drawSeenMap(mapWin, container, seen2);
        }
        if(floorcount == 3){
            drawSeenMap(mapWin, container, seen3);
        }
        if(floorcount == 4){
            drawSeenMap(mapWin, container, seen4);
        }
        c = getch();
        int prevX = hero.x;
        int prevY = hero.y;

        // حرکت قهرمان (همان کد قبلی شما)
        switch (c) {
            case 50: // پایین
                if (isitwallorO(mapWin, hero.x, hero.y + 1)) {
                    hero.y++;
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
                if(isitwallorO(mapWin , hero.x , hero.y - 1)){
                    hero.y--;
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
                if(isitwallorO(mapWin , hero.x + 1 , hero.y)){
                hero.x++;
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
                if(isitwallorO(mapWin , hero.x - 1 , hero.y)){
                    hero.x--;
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
                if(isitwallorO(mapWin , hero.x - 1 , hero.y - 1)){
                    hero.x--;
                    hero.y--;
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
                if(isitwallorO(mapWin , hero.x + 1 , hero.y - 1)){
                    hero.x ++;
                    hero.y --;
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
                if(isitwallorO(mapWin , hero.x - 1 , hero.y + 1)){
                    hero.x --;
                    hero.y++;
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
                if(isitwallorO(mapWin , hero.x + 1 , hero.y + 1)){
                    hero.x++;
                    hero.y++;
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
            
        }

        // بررسی تله‌ها
        isontrap(mapWin, rooms, roomCount, &hero);
        isonpasswordkey(mapWin , messagewin , hero.x , hero.y);
        // اگر قهرمان روی پله رفت
        if (stair.exists && hero.x == stair.x && hero.y == stair.y) {
            floorcount++;
            generateFloor(mapWin, rooms, &roomCount, &hero, &stair, seen, container);
        }

        // به‌روزرسانی نقشه و موقعیت قهرمان
        if(floorcount == 1){
            updateVisibility(mapWin, hero, 1, seen);
        }
        if(floorcount == 2){
            updateVisibility(mapWin, hero, 1, seen2);
        }
        if(floorcount == 3){
            updateVisibility(mapWin, hero, 1, seen3);
        }
        if(floorcount == 4){
            updateVisibility(mapWin, hero, 1, seen4);
        }
        
        mvwaddch(mapWin, hero.y, hero.x, 'H');

        wrefresh(mapWin);
    } while (c != 27); // خروج با دکمه ESC

    getch();
    delwin(mapWin);
    endwin();
    return 0;
}