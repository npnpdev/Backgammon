#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#define BOARD_ROWS 2
#define BOARD_COLS 14
#define TOTAL_PAWNS 15
#define CORNER_SPACE 2
#define SPACE_FROM_EDGE 5
#define BAR_SPACE 6

struct Coordinates{
    int x;
    int y;
};

struct Pawn{
    int player;
    Coordinates position;
};

struct Board{
    Pawn pawns[TOTAL_PAWNS * 2]; // Dwukrotność, bo są dwaj gracze
};

struct Column{
    struct Coordinates position;
    int pawns;
};

void menuFrame(){
    attron(A_REVERSE);
    mvvline(CORNER_SPACE, CORNER_SPACE, ' ', LINES);
    mvvline(CORNER_SPACE, COLS-1, ' ', LINES);
    mvhline(CORNER_SPACE, CORNER_SPACE, ' ',COLS);
    mvhline(LINES-1, CORNER_SPACE, ' ',COLS);
    attroff(A_REVERSE);
}

void menuOptions(int highlight){
    const char choices[][5] = {"PLAY","INFO","EXIT"};

    for (int i = 0; i < 3; i++) {
        if (highlight == i + 1) {
            attron(A_REVERSE);
        }
        mvprintw((LINES/2-1) + i, COLS/2-(CORNER_SPACE*2), "%s", choices[i]);
        attroff(A_REVERSE);
    }

    refresh();
}

void columnsNumbers(int i, int spacing){
    //Pierwsze 6  kolumn
    mvprintw(LINES - BAR_SPACE + 1, COLS - BAR_SPACE - i * (2 + spacing) - 1, "%d", i+1);

    //Kolumny 7-12
    mvprintw(LINES - BAR_SPACE + 1, BAR_SPACE + i * (2 + spacing) + 1, "%d", 12-i);

    //Kolumny 13-18
    mvprintw(BAR_SPACE-1, BAR_SPACE + i * (2 + spacing)+1, "%d", 13+i);

    //Kolumn 19-24
    mvprintw(BAR_SPACE - 1, COLS - BAR_SPACE - i * (2 + spacing) - 1, "%d", 24-i);

}

void columnsPosition(Column *columns, int i, int spacing){

    //Pierwsze 6 kolumn
    columns[i].position.x = LINES - BAR_SPACE + 1;
    columns[i].position.y = COLS - BAR_SPACE - i * (2 + spacing) - 1;

    //Kolumny 7-12
    columns[11-i].position.x = LINES - BAR_SPACE + 1;
    columns[11-i].position.y = BAR_SPACE + i * (2 + spacing) + 1;
    

    //Kolumny 13-18
    columns[i+12].position.x = BAR_SPACE-1;
    columns[i+12].position.y = BAR_SPACE + i * (2 + spacing)+1;
    
    
    //Kolumny 19-24
    columns[23-i].position.x = BAR_SPACE-1;
    columns[23-i].position.y = COLS - BAR_SPACE - i * (2 + spacing) - 1;
}

void Columns(Column *columns) {
    char character;

    int spacing = ((COLS/2) / 6) - 1; // Obliczenie odstępu między kolumnami

    for (int i = 0; i < 6; ++i) {
        if(i % 2 == 0){
            character = '[';
        } 
        else{
            character = '(';
        }
        for (int j = 0; j < LINES / 4; ++j){
            for (int k = 0; k < 3; ++k) {
                mvaddch(j + BAR_SPACE, BAR_SPACE + i * (2 + spacing) + k, character);
                mvaddch(LINES - j - BAR_SPACE, BAR_SPACE + i * (2 + spacing) + k, character);
                mvaddch(LINES - j - BAR_SPACE, COLS - BAR_SPACE - i * (2 + spacing) - k, character);
                mvaddch(j + BAR_SPACE, COLS - BAR_SPACE - i * (2 + spacing) - k, character);
            }
        }
        columnsPosition(columns, i, spacing);
        columnsNumbers(i, spacing);
    }

    refresh();
}

void gameBarsPosition(Column *columns){
    //RED BAR
    columns[24].position.x = LINES-LINES/4;
    columns[24].position.y = COLS/2;

    //WHITE BAR
    columns[25].position.x = LINES/4;
    columns[25].position.y = COLS/2;

    //RED BACKYARD
    columns[26].position.x = LINES/4;
    columns[26].position.y = COLS-3;

    //WHITE BACKYARD
    columns[27].position.x = LINES-LINES/4;
    columns[27].position.y = COLS-3;
}

void drawGameBars(){
    int middleX = COLS / 2; 
    // BAR
    mvvline(BAR_SPACE, middleX - 1, '[', LINES-(BAR_SPACE*2)+1); // Lewa linia
    mvvline(BAR_SPACE, middleX, 'B', LINES-(BAR_SPACE*2)+1);     // Srodkowa linia
    mvvline(BAR_SPACE, middleX + 1, ']', LINES-(BAR_SPACE*2)+1); // Prawa linia
    // HOME BAR
    mvvline(BAR_SPACE, COLS-(BAR_SPACE/2), 'H', LINES-(BAR_SPACE*2)+1);
}

void initializeBoard(Board *board, Column *columns){
    for (int i = 0; i < TOTAL_PAWNS; i++) {
        board->pawns[i].player = 1;
        board->pawns[TOTAL_PAWNS+i].player = 2; 
    }
    for(int i=0;i<TOTAL_PAWNS*2;i++){
        board->pawns[i].position.x = 99;
        board->pawns[i].position.y = 99;
    }
    for(int i=0;i<(BOARD_ROWS*BOARD_COLS);i++){
        columns[i].pawns = 0;
    }
}

int isPawnPresent(Board *myBoard, int x, int y) {
    for (int i = 0; i < TOTAL_PAWNS * 2; ++i) {
        if (myBoard->pawns[i].position.x == x && myBoard->pawns[i].position.y == y) {
            return 1; // Pionek jest już na tych współrzędnych
        }
    }
    return 0; // Na tych współrzędnych nie ma pionka
}

int findColumn(int x, int y){

    if(x==15 && y==0){
        return 26;
    }
    else if(x==15 && y==1){
        return 27;
    }
    
    if(x==13 && y==1){
        return 24;
    }
    else if(x==12 && y==0){
        return 25;
    }

    if(y==0){
        return 12+x;
    }
    else if(y==1){
        return 12-x;
    }
    else{
        return -1;
    }

    return -1;
}

void inscription(int *score1, int *score2){
    mvprintw(CORNER_SPACE*2-1, COLS/2-SPACE_FROM_EDGE, "BACKGAMMON");
    mvprintw(LINES-CORNER_SPACE*2, SPACE_FROM_EDGE, "Player 1 - RED");
    mvprintw(LINES-(CORNER_SPACE*2)+1, SPACE_FROM_EDGE, "Player 2 - WHITE");
    mvprintw(0, COLS-(SPACE_FROM_EDGE*5), "SCORE:    P1: %d    P2: %d", *score1, *score2);
    mvprintw(CORNER_SPACE*2, COLS-(COLS/4), "RED HOME BOARD");
    mvprintw(LINES-(CORNER_SPACE*2), COLS-(COLS/4), "WHITE HOME BOARD");
}

void board(Column *columns, int *score1, int *score2){    
    mvvline(SPACE_FROM_EDGE, SPACE_FROM_EDGE, '*', LINES-SPACE_FROM_EDGE-CORNER_SPACE);
    mvvline(SPACE_FROM_EDGE, COLS-SPACE_FROM_EDGE, '*', LINES-SPACE_FROM_EDGE*2);
    mvhline(SPACE_FROM_EDGE, SPACE_FROM_EDGE, '*',COLS-SPACE_FROM_EDGE*2);
    mvhline(LINES-SPACE_FROM_EDGE, SPACE_FROM_EDGE, '*',COLS-SPACE_FROM_EDGE*2);

    drawGameBars();
    gameBarsPosition(columns);
    Columns(columns);
    inscription(score1, score2);

    attron(A_REVERSE);
    mvprintw(LINES-CORNER_SPACE*2, COLS/2-CORNER_SPACE, "ROLL");
    attroff(A_REVERSE);

    refresh();
}

void clearPart(){
    for(int i=0;i<COLS/2;i++){
        mvaddch(0,i,' ');
        mvaddch(LINES-3,(i+COLS/2)-2,' ');
    }
    for(int i=5; i<20; i++){
        mvaddch(LINES-4,COLS-i,' ');
    }
}

void roll(int *dice1, int *dice2) {
    *dice1 = rand() % 6 + 1;
    *dice2 = rand() % 6 + 1;

    mvprintw(LINES - (CORNER_SPACE*2), (COLS - (CORNER_SPACE*2)) / 2 + 5, "%d", *dice1);
    mvprintw(LINES - (CORNER_SPACE*2), (COLS - (CORNER_SPACE*2)) / 2 + 6, ",");
    mvprintw(LINES - (CORNER_SPACE*2), (COLS - (CORNER_SPACE*2)) / 2 + 7, "%d", *dice2);
}

int firstRoll(){
    int roll1, roll2;
    int player = 1;

    srand(time(NULL));

    roll1 = rand() % 6 + 1;
    roll2 = rand() % 6 + 1;

    clearPart();
    mvprintw(0, 0, "Player1 rolled a: %d (Press ENTER to continue...)", roll1);

    if(getch()==10){
        clearPart();
        mvprintw(0, 0, "Player2 rolled a: %d (Press ENTER to continue...)", roll2);

        if(getch()==10){
            if(roll1>roll2){
                clearPart();
                player = 1;
            }
            else if (roll2>roll1){
                clearPart();
                player = 2;
            }
            else{
                firstRoll();
            }
        }
    }
    return player;
}

int findPawnsIndexes(Board *gameBoard, int x, int y, int indexes[TOTAL_PAWNS]) {
    int foundCount = 0;

    for (int i = 0; i < TOTAL_PAWNS * 2; ++i) {
        if (gameBoard->pawns[i].position.x == x && gameBoard->pawns[i].position.y == y) {
            if (foundCount < TOTAL_PAWNS) {
                indexes[foundCount++] = i;
            }
        }
    }
    if(foundCount == 0){
        return -1;
    }

    return foundCount; // Zwraca liczbę znalezionych pionków
}

int findPawnIndex(Board *gameBoard, int x, int y) {
    for (int i = 0; i < TOTAL_PAWNS * 2; ++i) {
        if (gameBoard->pawns[i].position.x == x && gameBoard->pawns[i].position.y == y) {
            return i;
        }
    }
    return -1; // Nie znaleziono pionka
}

int pawnsOption(){
    int key;
    int option = 1;

    while(1){
        attron(A_REVERSE);
        mvprintw(LINES - 3, (COLS - 4) / 2 + 6, "%d", option);
        attroff(A_REVERSE);

        key = getch();

        if(key == 10){
            break; 
        } 
        else if(key == KEY_UP){
            option = (option % 4) + 1; // Przesunięcie opcji
        }
    }

    return option;
}

void clearNumberField(){
    mvprintw(LINES - 3, (COLS - 4)/2+6, "  ");
}

int fieldNumber() {
    int num = 0;

    echo();
    clearNumberField();

    while(1){
        mvprintw(LINES - 3, (COLS - 4)/2, "FIELD:");
        scanw("%d", &num);

        if(num==99){
            endwin();
            exit(0);
        }
        if(num >= 0 && num <= 25){
            noecho();
            return num;
        } 
        else{
            clearNumberField();
        }
    }
    return 0;
}

int oneMove(){
    int num = fieldNumber();

    return num;
}

int chooseField(int *field1, int *field2, int *field3, int *field4){
    int fields[4] = {*field1, *field2, *field3, *field4};

    mvprintw(LINES-3, (COLS-4)/2, "PAWNS: ");
    int playerChoice = pawnsOption();

    for (int i = 0; i < playerChoice; i++) {
        fields[i] = oneMove();
    }

    *field1 = fields[0];
    *field2 = fields[1];
    *field3 = fields[2];
    *field4 = fields[3];

    return playerChoice;
}

void drawStatusMenu(int selectedSave, int option){
    attron(A_REVERSE);
    if(option == 1){
        mvprintw(LINES-(CORNER_SPACE*2), COLS-(BAR_SPACE*2), "SAVE: %d", selectedSave);
    }
    else{
        mvprintw(LINES-(CORNER_SPACE*2), COLS-(BAR_SPACE*2), "READ: %d", selectedSave);
    }
    attroff(A_REVERSE);
}

int statusMenu(int option){
    int selectedSave = 1;
    int choice;

    drawStatusMenu(selectedSave, option);

    while((choice = getch()) != 10){
        switch (choice){
            case KEY_UP:
                if (selectedSave < 5){
                    selectedSave++;
                }
                break;
            case KEY_DOWN:
                if (selectedSave > 1){
                    selectedSave--;
                }
                break;
        }
        drawStatusMenu(selectedSave,option);
    }

    return selectedSave;
}

void playerTurn(int player){
    mvprintw(1, COLS-(CORNER_SPACE*7), "Player%d turn!", player);
}

void stepSave(Board *board, int player, int pawnIndex, int x, int y){
    FILE *file = fopen("stepSave.txt", "a");
    if(file != NULL){
        player = player + 0x20;
        pawnIndex = pawnIndex + 0x20;
        x = x + 0x20;
        y = y + 0x20;

        fprintf(file, "%c%c%c%c%c", (char)player, (char)pawnIndex, (char)x, (char)y, 0xA);

        fclose(file);
    } 
    else{
        printf("CAN'T OPEN A FILE.");
    }
}

void getPawnPosition(int *x, int *y, int field){
    if(field==0){
        *x = 13;
        *y = 1;
    }
    else if(field==25){
        *x = 12;
        *y = 0;
    }

    if(field<13 && field>0){
        *x = 13-field;
        *y = 1;
    }
    else if(field>12 && field<25){
        *x = field-13;
        *y = 0;
    }
}

int checkPawnDetails(Board *gameBoard, Column *columns, int player, int newIndex, int newField, int allHome){
    if(newField>24 || newField<1){
        if(allHome==1){
            return 1;
        }
        else{
            mvprintw(1,0, "OUT OF RANGE");
            return -1;
        }
    }
    if(newIndex!=-1){
        if(columns[newField-1].pawns>0 && gameBoard->pawns[newIndex].player != player){
            if(columns[newField-1].pawns>1){
                mvprintw(1,0, "CHOOSE ANOTHER FIELD");
                return -1;
            }
            else{
                mvprintw(1,0, "Possible catching");
                return 1;
            }
        }
    }
    return 1;
}

int checkPawn(Board *gameBoard, Column *columns, int field, int player, int dice, int allHome){
    int x, y, x2, y2, field2, oldIndex, newIndex;

    getPawnPosition(&x, &y, field);

    oldIndex = findPawnIndex(gameBoard, x, y);

    if(oldIndex<TOTAL_PAWNS){
        //Gracz 1
        field2 = field+dice;
    }
    else{
        //Gracz 2
        field2 = field-dice;
    }

    getPawnPosition(&x2, &y2, field2);
    newIndex = findPawnIndex(gameBoard, x2, y2);

    if(gameBoard->pawns[oldIndex].player==player){
        if(checkPawnDetails(gameBoard, columns, player, newIndex, field2, allHome)==1){
            return oldIndex;
        }
        else{
            return -1;
        }
    }
    else{
        mvprintw(1,0, "NOT YOUR PAWN!");
        return -1;
    }
}

void clearColumn(int columnIndex, int columnX, int columnY){
    if(columnIndex>11){
        mvaddch(columnX+1, columnY, '[');
        mvprintw(columnX+2, columnY, "[");
    }
    else{
        mvaddch(columnX-1, columnY, '[');
        mvprintw(columnX-2, columnY, "[");
    }
}

void removePawnDraw(int columnIndex, int columnX, int columnY, int playerChar, int pawnsNumber){
    if(pawnsNumber==0){
        clearColumn(columnIndex, columnX, columnY);
    }
    else{
        attron(A_REVERSE);
        if(columnIndex>11){
            mvaddch(columnX+1, columnY, playerChar);
            mvprintw(columnX+2, columnY, "%d", pawnsNumber);
        }
        else{
            mvaddch(columnX-1, columnY, playerChar);
            mvprintw(columnX-2, columnY, "%d", pawnsNumber);
        }
        attroff(A_REVERSE);
    }
}

void getColumnPosition(Column *columns, int columnIndex, int *columnX, int *columnY){
    *columnX = columns[columnIndex].position.x;
    *columnY = columns[columnIndex].position.y;
}

void removePawn(Column *columns, int x, int y, char playerChar){
    int columnX, columnY;

    int columnIndex = findColumn(x,y);
    int pawnsNumber = columns[columnIndex].pawns-1;

    // Pobierz współrzędne x i y dla podanej kolumny
    getColumnPosition(columns, columnIndex, &columnX, &columnY);

    removePawnDraw(columnIndex, columnX, columnY, playerChar, pawnsNumber);

    columns[columnIndex].pawns = pawnsNumber;
}

void drawPawnOnBackyard(Column *columns, int columnIndex){
    if(columnIndex==26){
        columns[26].pawns++;
        mvprintw(6,COLS-3, "R");
        mvprintw(7,COLS-3, "%d", columns[26].pawns);
    }
    else{
        columns[27].pawns++;
        mvprintw(LINES-6,COLS-3, "W");
        mvprintw(LINES-7,COLS-3, "%d", columns[27].pawns);
    } 
}

void drawOnBar(Column *columns, char playerChar){
    if(playerChar=='W'){
        playerChar = 'R';
        mvaddch(columns[24].position.x+2, columns[24].position.y, playerChar);
        mvprintw(columns[24].position.x+1, columns[24].position.y, "%d", columns[24].pawns);
    }
    else{
        playerChar = 'W';
        mvaddch(columns[25].position.x+2, columns[25].position.y, playerChar);
        mvprintw(columns[25].position.x+1, columns[25].position.y, "%d", columns[25].pawns);
    }
}

void moveOnBar(Board *gameBoard, Column *columns, int pawns, int oldPawnIndex, char playerChar){
    if(playerChar=='W'){
        gameBoard->pawns[oldPawnIndex].position.x = 13;
        gameBoard->pawns[oldPawnIndex].position.y = 1;
        columns[24].pawns++;
    }
    else{
        gameBoard->pawns[oldPawnIndex].position.x = 12;
        gameBoard->pawns[oldPawnIndex].position.y = 0;
        columns[25].pawns++;
    }
}

void columnToDrawPawn(int columnIndex, int columnX, int columnY, int pawns, char playerChar){
    if(columnIndex>11){
        mvaddch(columnX+1, columnY, playerChar);
        mvprintw(columnX+2, columnY, "%d", pawns);
    }
    else{
        mvaddch(columnX-1, columnY, playerChar);
        mvprintw(columnX-2, columnY, "%d", pawns);
    }
}

void drawPawn(Column *columns, int x, int y, char playerChar, int allHome){
    int columnX, columnY, columnIndex, pawns;    

    columnIndex = findColumn(x,y);
    pawns = columns[columnIndex].pawns+1;
    
    getColumnPosition(columns, columnIndex, &columnX, &columnY);

    attron(A_REVERSE);
    if(allHome==1 && x>12){
        if(playerChar=='R'){
            drawPawnOnBackyard(columns, 26);
        }
        else if(playerChar=='W'){
            drawPawnOnBackyard(columns, 27);
        }
    }
    else{
        columnToDrawPawn(columnIndex, columnX, columnY, pawns, playerChar);
    }

    columns[columnIndex].pawns = pawns;
    attroff(A_REVERSE); 
}

void catchPawn(Board *gameBoard, Column *columns, int x, int y, char playerChar, int oldPawnIndex){
    int columnIndex, pawns, columnX, columnY;
    
    columnIndex = findColumn(x,y);
    pawns = columns[columnIndex].pawns;

    getColumnPosition(columns, columnIndex, &columnX, &columnY);

    //Postawienie pionka
    attron(A_REVERSE);
    columnToDrawPawn(columnIndex, columnX, columnY, pawns, playerChar);

    //Przesuwanie zbitego pionka na bar
    moveOnBar(gameBoard, columns, pawns, oldPawnIndex, playerChar);
    drawOnBar(columns, playerChar);

    attroff(A_REVERSE);
}

int pawnMoveSystemToHome(Board *gameBoard, int pawnIndex, int x, int y, int dice, int allHome){
    if(pawnIndex<TOTAL_PAWNS){
        if(allHome==1){
            if(x+dice>=12){
               gameBoard->pawns[pawnIndex].position.x = 15;
               gameBoard->pawns[pawnIndex].position.y = 0;
               return 1;
            }
        }
    }
    else{
        if(allHome==1){
            if(x+dice>=13){
                gameBoard->pawns[pawnIndex].position.x = 15;
                gameBoard->pawns[pawnIndex].position.y = 1;
                return 1;
            }
        }
    }
    return 0;
}

void pawnMoveSystemPlayer1(Board *gameBoard, int pawnIndex, int x, int y, int dice, int allHome){
    if(pawnMoveSystemToHome(gameBoard, pawnIndex, x, y, dice, allHome)==1){
        return;
    }
    if(y==0){
        gameBoard->pawns[pawnIndex].position.x = x+(dice);
    }
    else{
        if((x-(dice))<1){
            gameBoard->pawns[pawnIndex].position.y = 0;
        }
        gameBoard->pawns[pawnIndex].position.x = abs(x-(dice));
    }
}

void pawnMoveSystemPlayer2(Board *gameBoard, int pawnIndex, int x, int y, int dice, int allHome){
    if(pawnMoveSystemToHome(gameBoard, pawnIndex, x, y, dice, allHome)==1){
        return;
    }
    if(y==0){
        if((x-(dice))<0){
            gameBoard->pawns[pawnIndex].position.y = 1;
        }
        gameBoard->pawns[pawnIndex].position.x = abs(x-(dice));
    }
    else{
        gameBoard->pawns[pawnIndex].position.x = x+(dice);
    }
}

void pawnMoveSystem(Board *gameBoard, Column *columns, int pawnIndex, int dice, int player, int allHome){
    int x = gameBoard->pawns[pawnIndex].position.x;
    int y = gameBoard->pawns[pawnIndex].position.y;

    if(pawnIndex<TOTAL_PAWNS){
        //Gracz 1
        pawnMoveSystemPlayer1(gameBoard, pawnIndex, x, y, dice, allHome);
    }
    else{
        //Gracz 2
        pawnMoveSystemPlayer2(gameBoard, pawnIndex, x, y, dice, allHome);
    }
}

int getNewPawnIndex(Board *gameBoard, Column *columns, int pawnIndex, int dice, int player){
    int x = gameBoard->pawns[pawnIndex].position.x;
    int y = gameBoard->pawns[pawnIndex].position.y;

    if(pawnIndex<TOTAL_PAWNS){
        //Gracz 1
        if(y==0){
            x= x+(dice);
        }
        else{
            if((x-(dice))<1){
                y = 0;
            }
            x = abs(x-(dice));
        }
    }
    else{
        //Gracz 2
        if(y==0){
            if((x-(dice))<0){
                y = 1;
            }
            x = abs(x-(dice));
        }
        else{
            x = x+(dice);
        }

    }

    if(x==12 && y==0){
        return 1; // jakis indeks pionka gracza 1
    }
    else if(x==13 && y==1){
        return 16; // jakis indeks pionka gracza 2
    }

    int newPawn = findPawnIndex(gameBoard, x, y);

    return newPawn;
}

void pawnMoveVisual(Board *gameBoard, Column *columns, int oldX, int oldY, int pawnIndex, int newPawnIndex, int player, int allHome, char playerChar){
    int newX = gameBoard->pawns[pawnIndex].position.x;
    int newY = gameBoard->pawns[pawnIndex].position.y;
    int columnIndex = findColumn(newX, newY);
    int pawns = columns[columnIndex].pawns;
    int newPlayer = gameBoard->pawns[newPawnIndex].player; 

    removePawn(columns, oldX, oldY, playerChar);

    if(pawns==1 && newPlayer!=player && newX<13){
        catchPawn(gameBoard, columns, newX, newY, playerChar, newPawnIndex);
    }
    else{
        drawPawn(columns, newX, newY, playerChar, allHome);
    }
}

void stepSaving(Board *gameBoard, int oldX, int oldY, int pawnIndex, int player){
    int newX, newY;
    
    newX = gameBoard->pawns[pawnIndex].position.x;
    newY = gameBoard->pawns[pawnIndex].position.y;

    stepSave(gameBoard, player, pawnIndex, oldX, oldY);
    stepSave(gameBoard, player, pawnIndex, newX, newY);
}

void pawnMove(Board *gameBoard, Column *columns, int pawnIndex, int dice, int player, int allHome){    
    int oldX = gameBoard->pawns[pawnIndex].position.x;
    int oldY = gameBoard->pawns[pawnIndex].position.y;
    char playerChar = 'R';

    if(player==2){
        playerChar = 'W';
    }

    int newPawnIndex = getNewPawnIndex(gameBoard, columns, pawnIndex, dice, player);

    pawnMoveSystem(gameBoard, columns, pawnIndex, dice, player, allHome); //Zmiana pozycji pionka

    pawnMoveVisual(gameBoard, columns, oldX, oldY, pawnIndex, newPawnIndex, player, allHome, playerChar);    
    
    gameBoard->pawns[pawnIndex].player = player;

    stepSaving(gameBoard, oldX, oldY, pawnIndex, player);
}

void stepSystem(Board *gameBoard, Column *columns, int player, int pawnIndex, int x, int y, int option, int moveOption){
    char playerChar;

    if(player==1){
        playerChar = 'R';
    }
    else{
        playerChar = 'W';
    }

    if((moveOption == 0 && option == 0) || (moveOption == 1 && option == 1)) {
        removePawn(columns, x, y, playerChar);
    } 
    else{
        drawPawn(columns, x, y, playerChar, 0);
        gameBoard->pawns[pawnIndex].position.x = x;
        gameBoard->pawns[pawnIndex].position.y = y;
    }
}

int countPositionInFile(int *moveNumber){
    int position = -5;

    if(*moveNumber!=0){
        position = position + (-10*(*moveNumber));
    }

    return position;
}

void convertFromFile(FILE *file, int position, int *player, int *pawnIndex, int *x, int *y){
    char player_char, pawnIndex_char, x_char, y_char;

    fseek(file, position, SEEK_END);

    fscanf(file, "%c%c%c%c\n", &player_char, &pawnIndex_char, &x_char, &y_char);

    *player = player_char - 0x20;
    *pawnIndex = pawnIndex_char - 0x20;
    *x = x_char - 0x20;
    *y = y_char - 0x20;
}

void stepMove(Board *gameBoard, Column *columns, FILE *file, int *moveNumber, int moveOption, int position){
    int player, pawnIndex, x, y;

    convertFromFile(file, position, &player, &pawnIndex, &x, &y);
    stepSystem(gameBoard, columns, player, pawnIndex, x, y, 0, moveOption);
    
    convertFromFile(file, position-5, &player, &pawnIndex, &x, &y);
    stepSystem(gameBoard, columns, player, pawnIndex, x, y, 1, moveOption);

    fclose(file);
}

void loadStepMove(Board *gameBoard, Column *columns, int *moveNumber, int moveOption){
    FILE *file = fopen("stepSave.txt", "r");
    if (file != NULL) {
        if(*moveNumber>-1){
            // Obliczenie początkowej pozycji w pliku dla moveNumber
            int position = countPositionInFile(moveNumber);
            
            stepMove(gameBoard, columns, file, moveNumber, moveOption, position);
            
        }        
    } 
}

void moveBackward(Board *gameBoard, Column *columns, int *moveNumber){
    int choice;

    while(choice!=10){
        if((*moveNumber)<0){
            return;
        }
        mvprintw(10,10, "%d", *moveNumber);
        loadStepMove(gameBoard, columns, moveNumber, 0);
        (*moveNumber)--;
        choice = getch();
        attroff(A_REVERSE);
        refresh();
    }
}

void moveForward(Board *gameBoard, Column *columns, int *moveNumber, int maxMoves, int byStep){
    int choice;

    while(choice!=10){
        if((*moveNumber)>=maxMoves){
            return;
        }
        (*moveNumber)++;
        loadStepMove(gameBoard, columns, moveNumber, 1);
        if(byStep==1){
            choice = getch();
        }   
        attroff(A_REVERSE);
        refresh();
    }
}

int isOnBar(Column *columns, int player){
    if(player==1 && columns[24].pawns>0){
        return 1;        
    }
    else if(player==2 && columns[25].pawns>0){
        return 1;
    }
    return 0;
}

int possibleMoveFromBar(Board *gameBoard, Column *columns, int player, int dice1, int dice2){
    int condition1, condition2;

    if(player==1){
        condition1 = checkPawn(gameBoard, columns, 0, player, dice1, 0); 
        condition2 = checkPawn(gameBoard, columns, 0, player, dice2, 0);
        if(condition1 == -1 && condition2 == -1){
            return -1;
        }
    }
    else{
        condition1 = checkPawn(gameBoard, columns, 25, player, dice1, 0); 
        condition2 = checkPawn(gameBoard, columns, 25, player, dice2, 0);
        if(condition1 == -1 && condition2 == -1){
            return -1;
        }
    }
    return 0;
}

int possibleMoves(Board *gameBoard, Column *columns, int player, int dice, int allHome){
    int isPossible = 24;

    for(int i=1; i<=(BOARD_COLS*BOARD_ROWS)-4; i++){
        if(checkPawn(gameBoard, columns, i, player, dice, allHome) == -1){
            isPossible--;
        }
        else{
            return i;
        }
    }

    if(isPossible==0){
        if(allHome==1){
            return isPossible;
        }
        else{
            mvprintw(1,1, "NO MOVES!");
            return -1;
        }
    }

    return isPossible;
}

int farthestPawn(Board *gameBoard, Column *columns, int player){ 
    if(player==1){
        for(int i=0;i<6;i++){
            if(columns[18+i].pawns>0){
                return 19+i;
            }
        }
    }
    else{
        for(int i=0;i<6;i++){
            if(columns[5-i].pawns>0){
                return 6+i;
            }
        }
    }

    return 0;
}

void forceMoveFromBar(Column *columns, int player, int *field1, int dice1, int dice2){
    if(isOnBar(columns, player)==1){
        if(player==1){
            while(*field1!=0){
                mvprintw(0, 1, "Move pawn from BAR!");
                *field1 = oneMove();
            }
        }
        else{
            while(*field1!=25){
                mvprintw(0, 1, "Move pawn from BAR!");
                *field1 = oneMove();
            }
        }
        refresh();
    }  
}

int validatePawnMoves(Board *gameBoard, Column *columns, int *field1, int player, int dice, int dice1, int dice2, int allHome){
    int condition1 = checkPawn(gameBoard, columns, *field1, player, dice, allHome); 
    int condition2 = checkPawn(gameBoard, columns, *field1, player, dice1, allHome);
    int condition3 = checkPawn(gameBoard, columns, *field1, player, dice2, allHome);

    if(condition1 == -1 || (condition2 == -1) && ( condition3 == -1)){
        return -1;
    }

    return 0;
}

int otherPossibleMoves(Board *gameBoard, Column *columns, int player, int dice, int dice1, int dice2, int allHome){
    int possibleCondition = possibleMoves(gameBoard, columns, player, dice, allHome);

    int condition5 = checkPawn(gameBoard, columns, possibleCondition, player, dice1, allHome);
    int condition6 = checkPawn(gameBoard, columns, possibleCondition, player, dice2, allHome);

    if(isOnBar(columns, player)==1){
        if(possibleMoveFromBar(gameBoard, columns, player, dice1, dice2)==-1){
            return -1;
        }
    }

    if(condition5 == -1 && condition6 == -1){
        return -1;
    }
    return 0;
}

void updateConditions(Board *gameBoard, Column *columns, int *field1, int player, int dice, int dice1, int dice2, int allHome){
    validatePawnMoves(gameBoard, columns, field1, player, dice, dice1, dice2, allHome);    
    otherPossibleMoves(gameBoard, columns, player, dice, dice1, dice2, allHome);
}

int makePawnMove(Board *gameBoard, Column *columns, int *field1, int player, int dice, int dice1, int dice2, int allHome){
    forceMoveFromBar(columns, player, field1, dice1, dice2);

    while (validatePawnMoves(gameBoard, columns, field1, player, dice, dice1, dice2, allHome)==-1){
        if(possibleMoves(gameBoard, columns, player, dice, allHome)!= -1){
            if(otherPossibleMoves(gameBoard, columns, player, dice, dice1, dice2, allHome) == -1){
                return -1;
            }
        }
        else{
            return -1;
        }
        mvprintw(0, 1, "Choose another field!");
        *field1 = oneMove();
        updateConditions(gameBoard, columns, field1, player, dice, dice1, dice2, allHome);
    }

    return 1;
}

void onePawn(Board *gameBoard, Column *columns, int *field1, int player, int dice, int dice1, int dice2, int doublet, int allHome){
    int pawnIndex = 0;

    if(doublet==1){
        dice = dice*2;
    }

    if(makePawnMove(gameBoard, columns, field1, player, dice, dice1, dice2, allHome) == 1){
        pawnIndex = checkPawn(gameBoard, columns, *field1, player, dice, allHome);
    }
    else{
        return;
    }
    pawnMove(gameBoard, columns, pawnIndex, dice, player, allHome);
}

void twoPawns(Board *gameBoard, Column *columns, int *field1, int *field2, int player, int dice, int dice1, int dice2, int doublet, int allHome){
    if(doublet==1){
        dice1 = dice1*2;
        dice2 = dice2*2;
    }

    onePawn(gameBoard, columns, field1, player, dice1, dice1, dice1, 0, allHome);
    onePawn(gameBoard, columns, field2, player, dice2, dice2, dice2, 0, allHome);
}

void makeAgainPawnDecision(Board *gameBoard, Column *columns, int *field1, int *field2, int *field3, int *field4, int dice1, int dice2, int dice, int player, int allHome, int fieldsCount){
    while(fieldsCount>2){
        fieldsCount = chooseField(field1, field2, field3, field4);
    }
    
    if(fieldsCount==1){
        onePawn(gameBoard, columns, field1, player, dice, dice1, dice2, 0, allHome);
        return;
    }
    else{
        twoPawns(gameBoard, columns, field1, field2, player, dice, dice1, dice2, 0, allHome);
        return;
    }
}

void moveThree(Board *gameBoard, Column *columns, int *field1, int *field2, int *field3, int player, int dice1, int dice2, int dice3, int allHome){
    onePawn(gameBoard, columns, field1, player, dice1, dice1, dice1, 0, allHome);
    onePawn(gameBoard, columns, field2, player, dice2, dice2, dice2, 0, allHome);
    onePawn(gameBoard, columns, field3, player, dice3, dice3, dice3, 0, allHome);
}

void threePawns(Board *gameBoard, Column *columns, int *field1, int *field2, int *field3, int *field4, int player, int dice, int dice1, int dice2, int doublet, int allHome){
    int dice3 = 0;
    int fieldsCount = 3;
    
    if(doublet==1){
        dice3 = dice1*2;
        dice = dice1+dice2+dice3;
    }
    else{
        makeAgainPawnDecision(gameBoard, columns, field1, field2, field3, field4, dice1, dice2, dice, player, allHome, fieldsCount);
        return;
    }

    moveThree(gameBoard, columns, field1, field2, field3, player, dice1, dice2, dice3, allHome);
}

void moveFour(Board *gameBoard, Column *columns, int *field1, int *field2, int *field3, int *field4, int player, int dice1, int dice2, int dice3, int dice4, int allHome){
    onePawn(gameBoard, columns, field1, player, dice1, dice1, dice1, 0, allHome);
    onePawn(gameBoard, columns, field2, player, dice2, dice2, dice2, 0, allHome);
    onePawn(gameBoard, columns, field3, player, dice3, dice3, dice3, 0, allHome);
    onePawn(gameBoard, columns, field4, player, dice4, dice4, dice4, 0, allHome);
}

void fourPawns(Board *gameBoard, Column *columns, int *field1, int *field2, int *field3, int *field4, int player, int dice, int dice1, int dice2, int doublet, int allHome){
    int dice3 = 0;
    int dice4 = 0;
    int fieldsCount = 4;
    
    if(doublet==1){
        dice3 = dice1;
        dice4 = dice1;
        dice = dice1+dice2+dice3+dice4;
    }
    else{
        makeAgainPawnDecision(gameBoard, columns, field1, field2, field3, field4, dice1, dice2, dice, player, allHome, fieldsCount);
        return;
    }
    moveFour(gameBoard, columns, field1, field2, field3, field4, player, dice1, dice2, dice3, dice4, allHome);
}

void normalMove(Board *gameBoard, Column *columns, int *field1, int *field2, int player, int *dice, int *dice1, int *dice2, int doublet, int fieldsCount, int allHome){
    switch(fieldsCount){
        case 1:
            onePawn(gameBoard, columns, field1, player, *dice, *dice1, *dice2, doublet, allHome);
            break;
        case 2:
            twoPawns(gameBoard, columns, field1, field2, player, *dice, *dice1, *dice2, doublet, allHome);
            break;
    }
}

void doubletVariant(Board *gameBoard, Column *columns, int *field1, int *field2, int *field3, int *field4, int player, int *dice, int *dice1, int *dice2, int fieldsCount, int doublet, int allHome){
    switch(fieldsCount){
        case 3:
            threePawns(gameBoard, columns, field1, field2, field3, field4, player, *dice, *dice1, *dice2, doublet, allHome);
            break;
        case 4:
            fourPawns(gameBoard, columns, field1, field2, field3, field4, player, *dice, *dice1, *dice2, doublet, allHome);
            break;
    }
}

void moveOptions(Board *gameBoard, Column *columns, int player, int *dice1, int *dice2, int *dice, int *field1, int *field2, int *field3, int *field4, int fieldsCount, int doublet, int allHome, int arePawnsOnBar){
    *dice = *dice1+*dice2;
    
    if(fieldsCount<3){
        normalMove(gameBoard, columns, field1, field2, player, dice, dice1, dice2, doublet, fieldsCount, allHome);
        return;
    }
    else{
        doubletVariant(gameBoard, columns, field1, field2, field3, field4, player, dice, dice1, dice2, fieldsCount, doublet, allHome);
        return;
    }
}

int areAllPawnsAtHome(Board *gameBoard, Column *columns, int player){
    int count = 0;

    if(player==1){
        for(int i = 0; i < 15; i++){
            if(gameBoard->pawns[i].position.x >5 && gameBoard->pawns[i].position.y == 0){
                count++;
            }
        }
    }
    else{
        for(int i = 15; i < 30; i++){
            if(gameBoard->pawns[i].position.x >6 && gameBoard->pawns[i].position.y == 1){
                count++;
            }
        }
    }

    return count;
}

int areAllPawnsAtBackyard(Board *gameBoard, int player){
    if(player==1){
        for(int i=0;i<TOTAL_PAWNS;i++){
            if(gameBoard->pawns[i].position.x!=15){
                return 0;
            }
        }
    }
    else{
        for(int i=TOTAL_PAWNS;i<TOTAL_PAWNS*2;i++){
            if(gameBoard->pawns[i].position.x!=15){
                return 0;
            }
        }
    }
    return 1;
}

int allPawnsAtHome(Board *gameBoard, Column *columns, int player, int dice1, int dice2){
    if(areAllPawnsAtHome(gameBoard, columns, player)==15){
        if(dice2>farthestPawn(gameBoard, columns, player) && dice1>farthestPawn(gameBoard, columns, player)){
            mvprintw(10,10,"ALERT!!!!!");
        }
        return 1;
    }
    return 0;
}

void isDoublet(int *dice, int *doublet, int *dice1, int *dice2){
    if(*dice1==*dice2){
        *doublet = 1;
    }
    else{
        *dice = *dice1+*dice2;
    }
}

void makeMove(Board *gameBoard, Column *columns, int player, int *moveNumber){
    int dice, dice1, dice2, doublet, field1, field2, field3, field4;

    int arePawnsOnBar = isOnBar(columns, player);

    playerTurn(player);
    roll(&dice1, &dice2);
    isDoublet(&dice, &doublet, &dice1, &dice2);

    int allHome = allPawnsAtHome(gameBoard, columns, player, dice1, dice2);
    int fieldsCount = chooseField(&field1, &field2, &field3, &field4);

    moveOptions(gameBoard, columns, player, &dice1, &dice2, &dice, &field1, &field2, &field3, &field4, fieldsCount, doublet, allHome, arePawnsOnBar);

    *moveNumber = *moveNumber+fieldsCount;
}

void save(const char *filename, const Board *gameBoard) {
    FILE *file = fopen(filename, "w");
    if(file != NULL){
        for (int i = 0; i < TOTAL_PAWNS * 2; ++i) {
            fprintf(file, "%d %d %d \n", gameBoard->pawns[i].player, gameBoard->pawns[i].position.x, gameBoard->pawns[i].position.y);
        }
        fclose(file);
    }
    else{
        mvprintw(0,1,"CAN'T OPEN A FILE!");
    }
}

void load(const char *filename, Board *gameBoard, Column *columns) {
    int pawnCount;
    int atHome = 0;
    char playerChar = 'R';

    FILE *file = fopen(filename, "r");

    if (file != NULL){
        Columns(columns); //Clear columns
        drawGameBars();
        for (int i = 0; i < TOTAL_PAWNS * 2; i++) {
            int player, x, y;
            pawnCount = 0;

            fscanf(file, "%d %d %d", &player, &x, &y);

            if(player==2){
                playerChar = 'W';
            }

            pawnCount = columns[findColumn(x,y)].pawns;
            pawnCount++;            

            if(x==15){
                atHome = 1;
            }

            drawPawn(columns, x, y, playerChar, atHome);
            gameBoard->pawns[i].player = player;
            gameBoard->pawns[i].position.x = x;
            gameBoard->pawns[i].position.y = y;

        }
        fclose(file);
    } 
}

void gameStatus (Board *gameBoard, int option, Column *columns){
    int saveNum = statusMenu(option);
    char filename[20];
    sprintf(filename, "save%d.txt", saveNum);

    if (option == 1) {
        save(filename, gameBoard);
        mvprintw(0, 1, "Game saved!");
    }
    else{
        initializeBoard(gameBoard, columns);
        load(filename, gameBoard, columns);
        mvprintw(0, 1, "Game loaded!");
    }
}

void moveToStart(Board *gameBoard, Column *columns, int *moveNumber){
    initializeBoard(gameBoard, columns);
    load("start.txt", gameBoard, columns);
    *moveNumber = -1;
}

void moveToEnd(Board *gameBoard, Column *columns, int *moveNumber, int maxMoves){
    for(int i = 0; i<maxMoves; i++){
        moveForward(gameBoard, columns, moveNumber, maxMoves, 0);
    } 
}

int pointsAfterWin(Column *columns, int player){
    if(player==1){
        if(columns[27].pawns==0){
            if(columns[25].pawns>0){
                return 3;
            }
            return 2;
        }
    }
    else{
        if(columns[26].pawns==0){
            if(columns[24].pawns>0){
                return 3;
            }
            return 2;
        }
    }
    return 1;
}

int endGame(Board *gameBoard, Column *columns, int player, int *score1, int *score2){
    clear();
    if(player==1){
        player = 2;
        *score2 = pointsAfterWin(columns, player);
        mvprintw(10,10, "Player%d won with %d points", player, *score2);
    }
    else{
        player = 1;
        *score1 = pointsAfterWin(columns, player);
        mvprintw(10,10, "Player%d won with %d points", player, *score1);
    }

    stepSave(gameBoard, player, 33, 33, 33);
    mvprintw(20,10, "PRESS ENTER");

    getch();
    clear();
    return 1;
}

void inGameOptions(Board *gameBoard, Column *columns, int *moveNumber, int *maxMoves){
    int choice, option;

    do{
        choice = getch();

        if (choice == 's') {
            option = 1;
            gameStatus(gameBoard, option, columns);
        } 
        else if (choice == 'l') {
            option = 2;
            gameStatus(gameBoard, option, columns);
        } 
        else if (choice == KEY_LEFT) {
            moveBackward(gameBoard, columns, moveNumber);
        } 
        else if (choice == KEY_RIGHT) {
            moveForward(gameBoard, columns, moveNumber, *maxMoves, 1);
        }
        else if(choice == 'p'){
            moveToStart(gameBoard, columns, moveNumber);
        }
        else if(choice == 'k'){
            moveToEnd(gameBoard, columns, moveNumber, *maxMoves);
        }
    } 
    while (choice != 10);
}

void nextPlayer(int *player){
    if(*player==1){
        *player = 2;
    }
    else{
        *player = 1;
    }
}

void prepareGame(Board *gameBoard, Column *columns, int *score1, int *score2){
    clearPart();
    menuFrame();
    board(columns, score1, score2);
    initializeBoard(gameBoard, columns);
    load("start.txt", gameBoard, columns);
    refresh();
}

void game(Board *gameBoard, int *player, Column *columns, int *score1, int *score2){
    int indexes[TOTAL_PAWNS]; // Na potrzeby find pawns indexes
    int end = 0;
    int moveNumber = -1;
    int maxMoves = -1;

    prepareGame(gameBoard, columns, score1, score2);

    while (end==0){ 
        clearPart();
        maxMoves = moveNumber;
        
        inGameOptions(gameBoard, columns, &moveNumber, &maxMoves);
        makeMove(gameBoard, columns, *player, &moveNumber);
        end = areAllPawnsAtBackyard(gameBoard, *player);
        
        nextPlayer(player);
        refresh();
    }
    if(endGame(gameBoard, columns, *player, score1, score2)==1){
        game(gameBoard, player, columns, score1, score2);
    }
}

void play(){
    clear();
    int score1 = 0;
    int score2 = 0;
    Board gameBoard;
    struct Column columns[28];
    printw("Press ENTER to start the game\n");
    menuFrame();
    board(columns, &score1, &score2);
    refresh();
    
    int choice = getch();

    if(choice==10){
        int player = firstRoll();
        game(&gameBoard, &player,columns, &score1, &score2);   
    }
}

void optionsSystem(){
    int highlight = 1;
    int choice;

    while(highlight > 0){
        menuOptions(highlight);

        choice = getch();

        switch (choice) {
            case KEY_UP:
                if (highlight > 1) {
                    --highlight;
                }
                break;
            case KEY_DOWN:
                if (highlight < 3) {
                    ++highlight;
                }
                break;
            default:
                break;
        }

        if (choice == 10) {
            if (highlight == 1) {
                
                play();
                highlight = 0;
            
            } else if (highlight == 2) {
                mvprintw(LINES - 2, 0, "Wybrano INFO");
                refresh();
                getch();
            } else if (highlight == 3) {
                break;
            }
        }
    }
}

int main(){
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    
    mvprintw(LINES-CORNER_SPACE,COLS-(SPACE_FROM_EDGE*3),"Autor: s194103");
    menuFrame();
    optionsSystem();

    endwin();
    return 0;
}