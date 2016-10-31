

//#include<eku\io\concol.h>
#include <C:\Users\Kenny\Dropbox\RPG\RPG122\RPG\concol.h>
#include <iostream>
#include <conio.h>
#include <ctype.h>
#include <cstdlib>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <mmsystem.h>
#include <sstream>
#include <math.h>



using namespace std;
struct Player
{
    int HP;
    int maxHP;
    int mana;
    int maxMana;
    int stamina;
    int maxStamina;
    int level;
    int experience;
    int x;
    int y;
    string name;
    int keys;
};
struct Monster
{
    string type;
    string element;
    int HP;
    int maxHP;
    int level;
};

struct Items
{
    int healthpots;
    int manapots;
    int staminapots;
};
struct Cursor
{
    int x;
    int y;
};

struct Spell
{
    string name;
    string element;
    int damage;
    float accuracy;
    int manaUsed;
};



int getKeyPlayer(); //move player
void clearScreen(); //empty screen
void drawScreen();  //draw GUI, map, npcs, player
void writeStats();  //write stats in gui
void intro();       //rood scherm
void drawBorder(int, int, int, int); //vierkant tekenen. x, y (linkerbovenhoek), breedte, hoogte
void showInventory(); //inventory
void showMap();       //kaart
void initWorld();   //maps initialiseren, npcs initializeren
void initSpells();
void initMelee();
void initMonsters();
void levelUp();     //level up
void hideCursor();  //duw de cursor in de rechter benedenhoek
bool checkCollide(int, int);    //check voor collision met muren etc (werkt nog niet?)
void drawBattle(Monster);
void battle(Monster);      //battle
int showSpells();
int attack(Spell, int, Monster);
void moveEnemies();
void dead(Monster);
bool checkPlayerCollide(int, int);
void win();
void showCheats();

Player speler {100, 100, 100, 100, 100, 100, 1, 100, 38, 12, "???", 0};
char worldmap[11][11][77][19]= {3}; //4D array, worldx, worldy, x, y.
char npcmap[11][11][77][19]; //npc map
int worldx=5, worldy=5;
char focus='O';
char visitedMaps [11][11];

Spell spells[10];
Spell melee[10];
Monster monsters[50];
Items inventory {3,3,3};

Cursor curs= {0,0};
COORD coord= {0,0};

string battlestr1, battlestr2;
bool jumpback=false;

void gotoxy(int x,int y)
{
    coord.X=x;
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
int main()
{
    PlaySound(TEXT("music/opening.wav"), NULL, SND_LOOP | SND_ASYNC);
    //win();

    SetConsoleTitle("ARMY OF DARKNESS");
    bool startmsg=false;
    initWorld();
    initSpells();
    initMelee();
    initMonsters();
    intro(); //play credits and intro
    bool game=true;
    drawScreen();
    while(game==true) //game loop
    {

        if(speler.HP<=0) //dead
            game=false;
        if(startmsg==false)
        {
            setcolor(yellow,dark_red);
            PlaySound(TEXT("music/dungeon.wav"), NULL, SND_LOOP | SND_ASYNC);
            drawBorder(10,3,60,15); //TEST
            gotoxy(12,4);
            setcolor(dark_aqua,dark_red);
            cout << "Mysterious Voice:";
            setcolor(yellow,dark_red);
            gotoxy(12,6);
            cout << "Welcome, Hero, to this world! Your arrival has been";
            gotoxy(12,7);
            cout << "foretold! Our world was overrun by evil thousands";
            gotoxy(12,8);
            cout << "of years ago. Millions have died, there are only a few";
            gotoxy(12,9);
            cout << "living humans left now. It was said that in humanity's";
            gotoxy(12,10);
            cout << "last moments, a hero would appear to lead us to";
            gotoxy(12,11);
            cout << "salvation! This hero is no ordinary warrior, he wields";
            gotoxy(12,12);
            cout << "both sword and Magicka! I beg of you, savior, to find and";
            gotoxy(12,13);
            cout << "destroy the evil lord Jelle, and free us from his grip!";
            gotoxy(12,14);
            cout << "Oh but where are my manners? I haven't even asked";
            gotoxy(12,15);
            cout << "your name.";
            gotoxy(12,16);
            cout << "Name: ";
            cin >> speler.name;

            drawBorder(10,3,60,15); //screen2
            gotoxy(12,4);
            cout << "Now, your quest is to destroy the evil lord Jelle.";
            gotoxy(12,5);
            cout << "He is hiding somewhere in these lands! But he is";
            gotoxy(12,6);
            cout << "protected behind a magical door that requires 3 keys,";
            gotoxy(12,7);
            cout << "which were given to his three most powerful servants.";
            gotoxy(12,8);
            cout << "You will need to fight all three of them, before you";
            gotoxy(12,9);
            cout << "can challenge Lord Jelle!";
            gotoxy(12,11);
            cout << "I bid you good luck, " << speler.name << "!";
            gotoxy(12,13);
            cout << "I wish I could help you out more... Oh I know!";
            gotoxy(12,14);
            cout << "Take these potions, drink them when you are low on";
            gotoxy(12,15);
            cout << "health, out of breath or magic.";


            hideCursor();
            int button=0;
            button=getch();

            startmsg=true;
            drawScreen();

        }
        //writeStats(); //update stats gui

        InvalidateRect(HWND_TOP, NULL, false); //fix weird bug
        getKeyPlayer();
        moveEnemies();
        if(npcmap[worldx][worldy][speler.x-1][speler.y-1]!=' ') //battle
        {
            srand(rand());
            int dx = 5 - worldx;
            int dy = 5 - worldy;
            int distance=sqrt(dx * dx + dy * dy);
            distance *=2;

            if(distance<0)
            {
                distance+=(distance * 2);
            }
            if(distance>10)
            {
                distance=10;
            }

            int rnd=rand()%(distance+1);

            Monster testmonster = monsters[rnd];
            PlaySound(TEXT("music/battle.wav"), NULL, SND_LOOP | SND_ASYNC);
            battle(testmonster);
            PlaySound(TEXT("music/dungeon.wav"), NULL, SND_LOOP | SND_ASYNC);
        }
        if(worldmap[worldx][worldy][speler.x-1][speler.y-1]=char(2))
        {
            worldmap[worldx][worldy][speler.x-1][speler.y-1]=char(176);
        }
        if(worldmap[worldx][worldy][speler.x-1][speler.y-1]=char(1))
        {
            worldmap[worldx][worldy][speler.x-1][speler.y-1]=char(176);
        }

    }


}
void intro()
{
    clearScreen();
    setcolor(yellow,dark_red);

    if(1==1)
    {
        drawBorder(20,4,40,15);
        gotoxy(35,5);
        cout << "!!OPGELET!!";
        gotoxy(22,7);
        cout << "Dit spel bevindt zich in een BETA-";
        gotoxy(22,8);
        cout << "fase vanwege de omvang van het";
        gotoxy(22,9);
        cout << "project en tijdsgebrek. (examens)";
        gotoxy(22,10);
        cout << "Verscheidene geplande features zijn";
        gotoxy(22,11);
        cout << "(nog) niet bug-vrij of zelfs";
        gotoxy(22,12);
        cout << "aanwezig. Gelieve hiermee rekening";
        gotoxy(22,13);
        cout << "te houden. Veel plezier! -Kenny";
        gotoxy(22,14);
        cout << "PS: gelieve het spel te herstarten";
        gotoxy(22,15);
        cout << "wanneer je op de eerste maps geen";
        gotoxy(22,16);
        cout << "rotsen ziet: ";
        setcolor(gray,dark_green);
        cout << "*";
        setcolor(yellow,dark_red);
        cout << " (probleem met random)";


        int button=0;
        button=getch();
    }


    drawBorder(0,0,79,24);
    gotoxy(0,1);
    cout <<char(186) <<"             _.'          ."<<endl
         <<char(186)<<"            |P`            ?\\"<<endl
         <<char(186)<<"           .'h              'B"<<endl
         <<char(186)<<"           (''h             'P"<<endl
         <<char(186)<<"           ?''',          .''P"<<endl
         <<char(186)<<"           {'``''oo____oo''''P"<<endl
         <<char(186)<<"            '''888888888888,;"<<endl
         <<char(186)<<"             `?88P^\\,?88^\\,Y"<<endl
         <<char(186)<<"               88?\\__d88\\_/'"<<endl
         <<char(186)<<"               `8o8888/\\88P"<<endl
         <<char(186)<<"                ,?oo88oo8P"<<endl
         <<char(186)<<"      ___  __===~88~\\\\\\\\\\|~====__ __       ___"<<endl
         <<char(186)<<"  .-==ooo~odoooob ?8/////'oooood88888ooo?P88888?ooo?888PooooOoooooooo=~--"<<endl
         <<char(186)<<" d,d8888obo8,oo8b,``~~~,o?8oo,8888**8P88?8`oo,o888?898o8P888P~8b"<<endl
         <<char(186)<<" 8o88888oP?.,,ooood8b,.oo.,oo88?o8888P^^^?8888P^^^^==+=~~~?8bo?P"<<endl
         <<char(186)<<" ?8.=~=.8do.,oo88888ooo,o,oo88888o,;                      `6"<<endl
         <<char(186)<<" .?*o88ob`8.,o88888888oo,o,o88888o,'                       `\\"<<endl
         <<char(186) <<" |*o8888o`8,oo88888888oo,o,o?8888o'"<<endl
         <<char(186)<<" `?o8888P`88,,oo88888oo,,.,oo88oo;"<<endl
         <<char(186)<<" .8`o888'**888oo,,,,oood88oo,,d8'"<<endl
         <<char(186) <<" doo,,oP  ?888o,,o8o,o8o,,,o8oo'"<<endl
         <<char(186)<<" 8od8bo'  `?88o,,o8o,o88o,.,o,o'"<<endl;


    drawBorder(31,3,18,3);
    gotoxy(32,4);
    cout << "ARMY OF DARKNESS";
    drawBorder(29,12,22,6);
    gotoxy(35,13);
    cout << "A GAME BY:";
    gotoxy(31,14);
    cout << "Kenny Van de Maele";
    gotoxy(38,15);
    cout << "AND";
    gotoxy(30,16);
    cout << "Jelle Vandenmeersche";

    drawBorder(26,19,27,3);
    gotoxy(27,20);
    cout << "Press any Button to start";

    //gotoxy(79,24);
    //printf(" ");
    int button=0;
    button=getch();
    clearScreen();
    drawBorder(10,3,60,15);
    gotoxy(34,4);
    cout << "Controls";
    gotoxy(12,6);
    cout << "ZQSD - Move";
    gotoxy(12,8);
    cout << "Return - Select";
    gotoxy(12,10);
    cout << "i - Inventory";
    gotoxy(12,12);
    cout << "m - Map";

    hideCursor();
    button=getch();
}

void clearScreen()
{
    system("cls");
}

void hideCursor()
{
    gotoxy(79,24); //put waiting cursor in corner
    printf("");
}
void drawScreen()
{
    clearScreen();
    setcolor(yellow,dark_red);
    int maxX, maxY;
    maxX= 79;
    maxY= 24;
    char spelerplaats[maxX][maxY];
    for(int y=0; y<maxY; y++)
    {
        for(int x=0; x<maxX; x++)
        {
            spelerplaats[x][y]=' ';
            if(y==0)
                spelerplaats[x][y]=205;
            if(y==maxY-1)
                spelerplaats[x][y]=205;
            if(y==maxY-4)
                spelerplaats[x][y]=205;
            if(x==0)
                spelerplaats[x][y]=186;
            if(x==maxX-1)
                spelerplaats[x][y]=186;
            if(x==0 && y==0)
                spelerplaats[x][y]=201;
            if(x==0 && y==maxY-1)
                spelerplaats[x][y]=200;
            if(x==maxX-1 && y==0)
                spelerplaats[x][y]=187;
            if(x==maxX-1 && y==maxY-1)
                spelerplaats[x][y]=188;
            if(x==0 && y==20)
                spelerplaats[x][y]=204;
            if(x==maxX-1 && y==20)
                spelerplaats[x][y]=185;
            if(x==37 && (y==21 || y==22))
                spelerplaats[x][y]=186;
            if(x==37 && y==20)
                spelerplaats[x][y]=203;
            if(x==37 && y==23)
                spelerplaats[x][y]=202;

        }
    }
    if(focus=='O')
        setcolor(black,dark_green);
    spelerplaats[speler.x][speler.y]=char(1);

    setcolor(yellow,dark_red);
    for(int y=0; y<maxY; y++) //draw borders
    {
        for(int x=0; x<maxX; x++)
        {
            cout << spelerplaats[x][y];
        }
        cout << endl;
    }
    setcolor(dark_green,black);
    for(int y=0; y<19; y++) //draw map
    {
        for(int x=0; x<77; x++)
        {
            gotoxy(x+1,y+1);
            switch(worldmap[worldx][worldy][x][y])
            {
            case char(176): //grass
                setcolor(dark_green,black);
                break;
            case char(178): //water
                setcolor(dark_blue,black);
                break;
            case char(89): //tree
                setcolor(green,dark_green);
                break;
            case char(42): //rock
                setcolor(gray,dark_green);
                break;
            case char(201): //houseparts
            case char(205):
            case char(187):
            case char(186):
            case char(200):
            case char(188):
                setcolor(black,black);
                break;
            case '#': //map edges
                setcolor(black,dark_green);
                break;
            case char(2):
            case char(1):
                setcolor(dark_red, dark_green);
                break;

            case 'X': //treasure
                setcolor(dark_red,dark_green);
            }
            cout << worldmap[worldx][worldy][x][y];
        }
        cout << endl;
    }
    for(int y=0; y<19; y++) //draw npc
    {
        for(int x=0; x<77; x++)
        {
            gotoxy(x+1,y+1);
            if(npcmap[worldx][worldy][x][y]!=' ')
            {
                setcolor(black,dark_green);
                cout << npcmap[worldx][worldy][x][y];
            }

        }
        cout << endl;
    }
    gotoxy(speler.x, speler.y);
    if(focus=='O')
        setcolor(black,dark_green);
    cout << char(1);
    writeStats();
}
int getKeyPlayer()
{
    if(speler.mana<speler.maxMana)
    {
        srand(rand());
        int rnd=rand()%10;
        if(rnd==5)
        {
            speler.mana+=1;
        }

    }

    if(speler.stamina<speler.maxStamina)
    {
        srand(rand());
        int rnd=rand()%10;
        if(rnd==5)
        {
            speler.stamina+=1;
        }
    }

    writeStats();
    hideCursor();
    int button=0;
    button=_getch();
    gotoxy(0,0);
    //cout << "Map:" << worldx << ", " << worldy << " x: " << speler.x << " y: " << speler.y << char(205) << button << char(205);
    Sleep(100);
    gotoxy(speler.x, speler.y); //remove player icon
    setcolor(dark_green,black);
    cout << worldmap[worldx][worldy][speler.x-1][speler.y-1]; //replace with world
    if(npcmap[worldx][worldy][speler.x-1][speler.y-1]!=' ')
    {
        gotoxy(speler.x, speler.y);
        //cout << npcmap[worldx][worldy][speler.x-1][speler.y-1];
    }

    if(button==56 || button == 122) //up or Z
    {
        //cout << "up" << endl;
        if(speler.y>1)
        {
            if(checkPlayerCollide(speler.x-1,speler.y-2)==true)
            {
                speler.y -= 1;
            }

        }
        else
        {
            if(worldy > 0)
            {
                worldy-=1;
                speler.y=19;
                visitedMaps[worldx][worldy] = 'x';
                drawScreen();
            }
        }
    }
    if(button==50 || button==115) //button up or S
    {
        //cout << "down" << endl;
        if(speler.y<19)
        {
            if(checkPlayerCollide(speler.x-1,speler.y)==true)
            {
                speler.y+=1;
            }
        }
        else
        {
            if(worldy<10)
            {
                worldy+=1;
                speler.y=1;
                visitedMaps[worldx][worldy] = 'x';
                drawScreen();
            }
        }
    }
    if(button==54 || button == 100) //right or D
    {
        //cout << "right" << endl;
        if(speler.x<77)
        {
            if(checkPlayerCollide(speler.x,speler.y-1)==true)
            {
                speler.x+=1;
            }
        }
        else
        {
            if(worldx <10)
            {
                worldx+=1;
                speler.x=1;
                visitedMaps[worldx][worldy] = 'x';
                drawScreen();
            }
        }
    }
    if(button==52 || button==113) //button left or Q
    {
        //cout << "left" << endl;
        if(speler.x>1)
        {
            if(checkPlayerCollide(speler.x-2,speler.y-1)==true)
            {
                speler.x-=1;
            }
        }
        else
        {
            if(worldx >0)
            {
                worldx-=1;
                speler.x=77;
                visitedMaps[worldx][worldy] = 'x';
                drawScreen();
            }
        }
    }
    if(button==105) //i
    {
        showInventory();

    }
    if(button==99)
    {
        focus='C';
        showCheats();
    }

    if(button==109) //m
    {
        showMap();

    }

    if(button==27)//esc
    {
        focus='O';
        drawScreen();
        return 0;
    }
    if(button==53) //5
    {
        //levelUp();
    }

    gotoxy(speler.x, speler.y);
    if(focus=='O')
        setcolor(black,dark_green);
    cout << char(1);

    return 0;
}

void writeStats()
{
    setcolor(yellow,dark_red);
    gotoxy(2,21);
    cout << "                                   ";
    gotoxy(2,22);
    cout << "                                   ";
    gotoxy(2,21);
    cout << "Name: " << speler.name;
    gotoxy(2,22);
    cout << char(3) << ": " << speler.HP << "/" << speler.maxHP;
    gotoxy(18,22);
    cout << "Mana:    " << speler.mana << "/" << speler.maxMana;
    gotoxy(18,21);
    cout << "Stamina: " << speler.stamina << "/" << speler.maxStamina;
    gotoxy(39,21);
    cout << "Level: " << speler.level;
    cout << "\t\t    Keys: " << speler.keys;
    gotoxy(39,22);
    cout << "EXP to lvl " << speler.level +1 << ": " << speler.experience;
    gotoxy(0,0);
    //cout << "Map:" << worldx << ", " << worldy << " x: " << speler.x << " y: " << speler.y << char(205);
}

void drawBorder(int minX, int minY, int width, int height)
{
    char spelerplaats[width][height];
    for(int y=0; y<height; y++) //build array
    {
        for(int x=0; x<width; x++)
        {
            spelerplaats[x][y]=' ';
            if(y==0)
                spelerplaats[x][y]=205;
            if(y==height-1)
                spelerplaats[x][y]=205;
            if(x==0)
                spelerplaats[x][y]=186;
            if(x==width-1)
                spelerplaats[x][y]=186;
            if(x==0 && y==0)
                spelerplaats[x][y]=201;
            if(x==0 && y==height-1)
                spelerplaats[x][y]=200;
            if(x==width-1 && y==0)
                spelerplaats[x][y]=187;
            if(x==width-1 && y==height-1)
                spelerplaats[x][y]=188;
        }
    }
    gotoxy(minX, minY);
    for(int y=0; y<height; y++) //draw it
    {
        for(int x=0; x<width; x++)
        {
            gotoxy(minX+x, minY+y);
            cout << spelerplaats[x][y];

        }
    }

//    gotoxy(minX,minY);
//    cout << char(201);
//    for(int x=0; x<width; x++)
//        {
//
//        }
//
//    for(int y=0; y<height-2; y++) //draw it
//    {
//        for(int x=0; x<width; x++)
//        {
//
//        }
//    }
}

void showInventory()
{
    setcolor(yellow, dark_red);
    if(focus!='I')
    {
        drawBorder(25,6,30,10);
        gotoxy(35,7);
        cout << "Inventory";
        gotoxy(29,9);
        cout << "Health potion\tx" << inventory.healthpots;
        gotoxy(29,11);
        cout << "Mana potion\tx" << inventory.manapots;
        gotoxy(29,13);
        cout << "Stamina potion\tx" << inventory.staminapots;

        hideCursor();
        focus='I';
        int button=0;
        int pos=0;
        curs.x=27;
        curs.y=9;
        gotoxy(27,9);
        cout << char(26);
        while(focus=='I')
        {
            gotoxy(curs.x,curs.y);
            cout << char(26);
            hideCursor();
            button=getch();

            if(button==122)//up
            {
                if(pos>0)
                {
                    gotoxy(curs.x,curs.y);
                    cout << " ";
                    pos-=1;
                    curs.y-=2;
                }

            }
            if(button==115)//down
            {
                if(pos<2)
                {
                    gotoxy(curs.x,curs.y);
                    cout << " ";
                    pos+=1;
                    curs.y+=2;
                }

            }
            if(button==105) //i
            {
                focus='O';
            }
            if(button==13)//enter
            {
                Sleep(200);
                switch(pos)
                {
                case 0:
                    if(inventory.healthpots>0)
                    {
                        speler.HP+=50;
                        inventory.healthpots-=1;
                        if(speler.HP>speler.maxHP)
                        {
                            speler.HP=speler.maxHP;
                        }
                        writeStats();
                    }
                    break;
                case 1:
                    if(inventory.manapots>0)
                    {
                        speler.mana+=50;
                        inventory.manapots-=1;
                        if(speler.mana>speler.maxMana)
                        {
                            speler.mana=speler.maxMana;
                        }
                        writeStats();
                    }
                    break;
                case 2:
                    if(inventory.staminapots>0)
                    {
                        speler.stamina+=50;
                        inventory.staminapots-=1;
                        if(speler.stamina>speler.maxStamina)
                        {
                            speler.stamina=speler.maxStamina;
                        }
                        writeStats();
                    }
                    break;
                }
                gotoxy(29,9);
                cout << "Health potion\tx" << inventory.healthpots;
                gotoxy(29,11);
                cout << "Mana potion\tx" << inventory.manapots;
                gotoxy(29,13);
                cout << "Stamina potion\tx" << inventory.staminapots;

                writeStats;
            }
        }
        drawScreen();
        writeStats();
    }

}

void showMap()
{
    setcolor(yellow,dark_red);
    visitedMaps[5][5] ='x';
    drawBorder(28,3,25,15);
    gotoxy(38,4);
    cout << "Map";
    hideCursor();
    focus='M';
    int button=0;
    gotoxy(30, 6);
    for(int y=0; y < 11; y++)
    {
        for(int x=0; x < 11; x++)
        {

            if(worldx == x && worldy == y)
            {
                cout << char(1) << " ";
            }
            else
            {
                cout << visitedMaps[x][y] << " ";
            }

        }
        gotoxy(30, y + 7);
    }
    hideCursor();
    while(focus=='M')
    {
        button=getch();
        if(button==109) //i
        {
            focus='O';
        }
    }
    drawScreen();
    writeStats();

}

void initWorld()
{
    for(int wx=0; wx<11; wx++)
    {
        for(int wy=0; wy<11; wy++)
        {
            for(int x=0; x<77; x++)
            {
                for(int y=0; y<19; y++)
                {
                    worldmap[wx][wy][x][y]=176; //all grass
                    if(wy==0)
                    {
                        worldmap[wx][wy][x][0]='#';
                    }
                    if(wy==10)
                    {
                        worldmap[wx][wy][x][18]='#';
                    }
                    if(wx==0)
                    {
                        worldmap[wx][wy][0][y]='#';
                    }
                    if(wx==10)
                    {
                        worldmap[wx][wy][76][y]='#';
                    }

                    npcmap[wx][wy][x][y]=' '; //empty npcmap
                    //if((rand()%9)== 0)
                }
            }
        }
    }


    srand(time(NULL));

    for(int wx=0; wx<11; wx++)
    {
        for(int wy=0; wy<11; wy++)
        {
            for(int i=0; i<3; i++) //rand water pools
            {
                srand(rand());
                int randx=(rand()%70)+4;
                srand(rand());
                int randy=(rand()%15)+2;
                srand(rand());
                int width=(rand()%30)+5;
                srand(rand());
                int height=(rand()%8)+2;

                for(int h=0; h<height; h++)
                {
                    for(int w=0; w<width; w++)
                    {
                        if((w+randx)<70 && (h+randy)<15)
                        {

                            if(wx!=5 && wy!=5)
                            {
                                worldmap[wx][wy][w+randx][h+randy]=char(178);
                            }


                        }

                    }
                }

            }
        }
    }
    Sleep(10);
    for(int wx=0; wx<11; wx++)
    {
        for(int wy=0; wy<11; wy++)
        {
            for(int i=0; i<25; i++) //rand trees
            {

                srand(rand());
                int randx=(rand()%70)+4;
                srand(rand());
                int randy=(rand()%15)+2;
                if(worldmap[wx][wy][randx][randy]==char(176))
                {
                    worldmap[wx][wy][randx][randy]=char(89);
                }
            }
        }
    }
    Sleep(10);
    for(int wx=0; wx<11; wx++)
    {
        for(int wy=0; wy<11; wy++)
        {
            for(int i=0; i<10; i++) //rand rocks
            {
                srand(rand());
                int randx=(rand()%70)+4;
                srand(rand());
                int randy=(rand()%15)+2;
                if(worldmap[wx][wy][randx][randy]==char(176))
                {
                    worldmap[wx][wy][randx][randy]=char(42);
                }
            }
        }
    }

//    Sleep(10);
//    for(int wx=0; wx<11; wx++)
//    {
//        for(int wy=0; wy<11; wy++)
//        {
//            for(int i=0; i<1; i++) //rand TREASURE
//            {
//                srand(rand());
//                int randx=(rand()%70)+4;
//                srand(rand());
//                int randy=(rand()%15)+2;
//                if(worldmap[wx][wy][randx][randy]==char(176))
//                {
//                    worldmap[wx][wy][randx][randy]='X';
//                }
//            }
//        }
//    }

    for(int wx=0; wx<11; wx++)
    {
        for(int wy=0; wy<11; wy++)
        {
            for(int i=0; i<5; i++) //rand house
            {
                srand(rand());
                int randx=(rand()%55)+10;
                srand(rand());
                int randy=(rand()%10)+4;
                bool check=true;
                for(int y=0; y<4; y++) //check for room
                {
                    for(int x=0; x<6; x++)
                    {
                        if(worldmap[wx][wy][x+randx][y+randy]!=char(176))
                        {
                            check=false;
                        }
                    }
                }
                if(check==true) //if room
                {
                    worldmap[wx][wy][randx][randy]=char(201);//uppleft
                    worldmap[wx][wy][randx+1][randy]=char(205);//up
                    worldmap[wx][wy][randx+2][randy]=char(205);
                    worldmap[wx][wy][randx+3][randy]=char(205);
                    worldmap[wx][wy][randx+4][randy]=char(205);
                    worldmap[wx][wy][randx+5][randy]=char(187);//upright

                    worldmap[wx][wy][randx][randy+1]=char(186);//left
                    worldmap[wx][wy][randx+5][randy+1]=char(186);
                    npcmap[wx][wy][randx+3][randy+1]=char(2); //monster in house

                    worldmap[wx][wy][randx][randy+2]=char(186);//right
                    worldmap[wx][wy][randx+5][randy+2]=char(186);

                    worldmap[wx][wy][randx][randy+3]=char(200);//downleft
                    worldmap[wx][wy][randx+1][randy+3]==char(205);
                    worldmap[wx][wy][randx+2][randy+3]=char(205);
                    worldmap[wx][wy][randx+3][randy+3]=char(205);
                    worldmap[wx][wy][randx+4][randy+3]=char(205);
                    worldmap[wx][wy][randx+5][randy+3]=char(188);
                }
            }
        }
    }




    for(int wx=0; wx<11; wx++)
    {
        for(int wy=0; wy<11; wy++)
        {
            for(int i=0; i<5; i++) //rand monster locations
            {
                srand(rand());
                int randx=(rand()%70)+4;
                srand(rand());
                int randy=(rand()%15)+2;
                if(worldmap[wx][wy][randx][randy]==char(176))
                {
                    npcmap[wx][wy][randx][randy]=char(2);
                }

            }
        }
    }




    for(int y=0; y < 11; y++) //visited maps
    {
        for(int x=0; x < 11; x++)
        {
            visitedMaps[x][y] = ' ';

        }
    }

    //testbosses
    worldmap[0][0][3][3]=char(2);
    worldmap[10][0][75][3]=char(2);
    worldmap[0][10][3][16]=char(2);

    //worldmap[11][11][75][16]=char(1);

}

void levelUp()
{
    setcolor(yellow,dark_red);
    if (speler.level<100)
    {
        Sleep(200);
        focus='L';
        speler.level+=1;
        speler.experience=speler.level*100;
        drawBorder(20,10,40,5);
        gotoxy(22,11);
        cout << "You grew to level " << speler.level << ".";
        gotoxy(22,12);
        cout << "What stat would you like to improve?";
        gotoxy(28,13);
        cout << "Health   Mana   Stamina";
        curs.x=27;
        curs.y=13;
        int choice=1;
        int button=0;
        while(focus=='L')
        {
            gotoxy(curs.x, curs.y);
            cout << char(26);
            hideCursor();
            button=_getch();
            if(button==100) //right button
            {
                gotoxy(curs.x,curs.y);
                cout << " ";
                switch(choice)
                {
                case 1:
                    curs.x=36;
                    choice=2;
                    break;
                case 2:
                    curs.x=43;
                    choice=3;
                    break;
                case 3:
                    curs.x=27;
                    choice=1;
                    break;
                }
            }
            if(button==113) //left button
            {
                gotoxy(curs.x,curs.y);
                cout << " ";
                switch(choice)
                {
                case 1:
                    curs.x=43;
                    choice=3;
                    break;
                case 2:
                    curs.x=27;
                    choice=1;
                    break;
                case 3:
                    curs.x=36;
                    choice=2;
                    break;
                }
            }
            if(button==13) //enter button
            {
                switch(choice)
                {
                case 1:
                    speler.maxHP+=20;
                    speler.HP+=20;
                    break;
                case 2:
                    speler.maxMana+=20;
                    speler.mana+=20;
                    break;
                case 3:
                    speler.maxStamina+=20;
                    speler.stamina+=20;
                    break;
                }
                focus='O';
            }

        }
        drawScreen();
        writeStats();
    }

}
bool checkCollide(int x, int y)
{
    if(worldmap[worldx][worldy][x][y]==char(176))
    {
        return true;
    }
    else
    {
        return false;
    }

}

void drawBattle(Monster enemy)
{
    setcolor(yellow,dark_red);
    drawBorder(10,3,60,15);
    drawBorder(12,12,13,5);
    drawBorder(27,12,40,5);
    drawBorder(12,6,35,4);
    gotoxy(36,4);
    cout << "Battle";
    curs.x=13;
    curs.y=13;
    int button=0;
    gotoxy(14,13);
    cout << "Attack";
    gotoxy(14,14);
    cout << "Spell";
    gotoxy(14,15);
    cout << "Inventory";

    gotoxy(13,7);
    cout << "Enemy: " << enemy.type << "   Lvl: " << enemy.level;
    gotoxy(13,8);
    cout<< "Element: " << enemy.element << "   HP: " << enemy.HP << "/" << enemy.maxHP;
}

void battle(Monster enemy)
{
    PlaySound(TEXT("music/battle.wav"), NULL, SND_LOOP | SND_ASYNC);
    PlaySound(TEXT("music/battle.wav"), NULL, SND_LOOP | SND_ASYNC);
    setcolor(yellow,dark_red);
    focus='B';

    drawBattle(enemy);

    gotoxy(29,13);
    cout << "A " << enemy.type << " appeared!";

    gotoxy(29,15);
    cout << "What will you do?";


    while(focus=='B')
    {

        gotoxy(curs.x, curs.y);
        cout << char(26);
        hideCursor();
        int button=0;
        button=getch();
        gotoxy(curs.x, curs.y);
        cout << " ";
        hideCursor();
        bool invent =false;
        if(button==115) //Z
        {
            if(curs.y<15)
            {
                curs.y+=1;
            }
        }
        if(button==122) //S
        {
            if(curs.y>13)
            {
                curs.y-=1;
            }

        }
        if(button==13) //enter
        {
            int damage=0;
            if(curs.y==13) //chose attack
            {
                damage=attack(melee[0], 0, enemy);
                writeStats();
            }
            if(curs.y==14) //chose spell
            {
                int choice=showSpells();
                damage=attack(spells[choice], 1, enemy);
                writeStats();
                drawBattle(enemy);

            }
            if(curs.y==15)
            {
                showInventory();
                focus='B';
                invent=true;
            }
            if(enemy.HP<0)
            {
                enemy.HP=0;
            }
            drawBattle(enemy);
            if(invent==false)
            {
                gotoxy(29,13);
                cout << battlestr1;
                Sleep(1000);
                hideCursor();
                gotoxy(29,14);
                cout << battlestr2;
                enemy.HP-=damage;
                gotoxy(29,15);
                cout << "                       ";
                hideCursor();
                int button=0;
                button=getch();
            }


            if(enemy.HP>0)
            {
                drawBattle(enemy);
                gotoxy(29,13); //enemy turn
                cout << "The " << enemy.type << " attacks you!";
                int damageEnemy;
                srand(rand());
                int rnd=(rand()%5)-2;
                damageEnemy=(enemy.level * 6)+rnd;
                damageEnemy/=speler.level;
                Sleep(1000);
                hideCursor();
                int hitchance = rand()%100;
                gotoxy(29,14);
                if(hitchance > 80)
                {
                    cout << "...The enemy misses!";
                    damageEnemy=0;
                }
                else
                {
                    cout << "It hits you for " << damageEnemy << " damage!";
                }

                speler.HP-=damageEnemy;
                if(speler.HP<=0)
                {
                    button=getch();
                    dead(enemy);
                }
                Sleep(10);
                srand(time(NULL));
                int chance = rand()%3;
                gotoxy(29,15);
                switch(chance)
                {
                case 1:
                    if(speler.stamina < speler.maxStamina)
                    {
                        cout << "You regained some stamina.";
                        srand(rand());
                        chance = (rand()%10) +1;
                        speler.stamina+=chance;
                        if(speler.stamina > speler.maxStamina)
                        {
                            speler.stamina=speler.maxStamina;
                        }
                    }
                    break;
                case 2:
                    if(speler.mana < speler.maxMana)
                    {
                        cout << "You regained some mana.";
                        srand(rand());
                        chance = (rand()%10) +1;
                        speler.mana+=chance;
                        if(speler.mana > speler.maxMana)
                        {
                            speler.mana=speler.maxMana;
                        }
                    }
                    break;
                case 0:
                    break;
                }

                writeStats();
                int button;
                button=getch();
            }
            else //WON
            {
                goto won;
            }


        }

        //gotoxy(curs.x, curs.y);
        //cout << char(26);
    }

won:
    if(enemy.HP<0)
        enemy.HP=0;

    drawBattle(enemy);
    PlaySound(TEXT("music/victory.wav"), NULL, SND_LOOP | SND_ASYNC);
    srand(rand());
    int rnd=(rand()%20)-10;
    int exp=(enemy.level*40) + rnd;
    gotoxy(29,13);
    cout << "                       ";
    gotoxy(29,14);
    cout << "                       ";
    gotoxy(29,13);
    cout << "The enemy was slain!";
    gotoxy(29,14);
    cout << "You gain " << exp << " experience!";
    gotoxy(29,15);

    srand(rand());
    rnd=rand()%7;

    switch(rnd)
    {
    case 0:
        cout << "You find a health potion!";
        inventory.healthpots+=1;
        break;
    case 1:
        cout << "You find a mana potion!";
        inventory.manapots+=1;
        break;
    case 2:
        cout << "You find a stamina potion!";
        inventory.staminapots+=1;
        break;
    default:
        break;
    }

    speler.experience-=exp;
    writeStats();
    int button=0;
    button=getch();
    if(speler.experience > 0)
    {
        drawScreen();
        focus='O';

    }
    else
    {
        //int temp = speler.experience;
        levelUp();
        //speler.experience += temp;
    }
    focus='O';
    npcmap[worldx][worldy][speler.x-1][speler.y-1]=' '; //remove enemy

    writeStats();


    curs.x=14;
    curs.y=13;
    //writeStats();
    PlaySound(TEXT("music/dungeon.wav"), NULL, SND_LOOP | SND_ASYNC);
}

void initSpells()
{
    // name, type, damage, chance, mana used.
    spells[0]= {"Fire bolt", "Fire", 10, 60, 30};
    spells[1]= {"Water bolt", "Water", 10, 60, 30};
    spells[2]= {"Nature bolt", "Nature", 10, 60, 30};

    spells[3]= {"Fire wave", "Fire", 15, 60, 50};
    spells[4]= {"Water wave", "Water", 15, 60, 50};
    spells[5]= {"Nature wave", "Nature", 15, 60, 50};

    spells[6]= {"Fire storm", "Fire", 20, 60, 80};
    spells[7]= {"Water storm", "Water", 20, 60, 80};
    spells[8]= {"Nature storm", "Nature", 20, 60, 80};

    spells[9]= {"Apocalypse", "Void", 100, 30, 500};


}

void initMelee()
{
    // name, type, damage, chance, stamina used.
    melee[0]= {"to attack", "None", 5, 95, 10}; //default
    melee[1]= {"Punch", "None", 5, 95, 20};

    melee[2]= {"Stab", "None", 5, 95, 20}; //if sword
    melee[3]= {"Slash", "None", 5, 95, 20};
    melee[4]= {"Lunge", "None", 5, 95, 20};

    melee[5]= {"Shoot Arrow", "None", 5, 95, 20}; //if bow

}

void initMonsters()
{
    //name, element, HP, maxHP, level
    monsters[0]= {"Wild Rat", "None", 10, 10, 1};
    monsters[1]= {"Wild Boar", "None", 15, 15, 1};
    monsters[2]= {"Shambling Zombie", "Nature", 25, 25, 2};
    monsters[3]= {"Lesser Demon", "Fire", 30, 30, 3};
    monsters[4]= {"Skeletal Warrior", "None", 45, 45, 5};
    monsters[5]= {"Ice Spirit", "Water", 50, 50, 5};
    monsters[6]= {"Corrupted Spirit", "None", 65, 65, 6};
    monsters[7]= {"Enraged Ent", "Nature", 75, 75, 7};
    monsters[8]= {"Flaming Demon", "Fire", 80, 80, 8};
    monsters[9]= {"Zombie Pirate", "Water", 95, 95, 9};
    monsters[10]= {"Mossy Ghoul", "Nature", 100, 100, 10};
    monsters[11]= {"Flame Overlord", "Fire", 120, 120, 12};
    monsters[12]= {"Aqua Overlord", "Water", 120, 120, 12};
    monsters[13]= {"Nature Overlord", "Nature", 120, 120, 12};
    monsters[14]= {"Lord Jelle", "None", 200, 200, 15};

}


int showSpells()
{

    int returnspell=0;
    int maxspell=speler.maxMana / 20 - 4;
    if(maxspell>10)
        maxspell=10;
    drawBorder(50,3,20,15);
    gotoxy(56,4);
    cout << "Spells";
    gotoxy(54,6);
    for(int i=0; i<maxspell; i++)
    {
        gotoxy(54,i+6);
        cout << spells[i].name;
    }

    hideCursor();
    focus='S';
    int button=0;
    curs.x=53;
    curs.y=6;
    gotoxy(curs.x, curs.y);
    cout << char(26);
    while(focus=='S')
    {
        hideCursor();
        button=getch();
        gotoxy(curs.x, curs.y);
        cout << " ";

        if(button == 122) //Z
        {
            //cout << "up" << endl;
            if(curs.y>6)
            {
                curs.y-=1;
                returnspell-=1;
            }


        }
        if(button==115) //S
        {
            //cout << "down" << endl;
            if(curs.y<6+maxspell-1)
            {
                curs.y+=1;
                returnspell+=1;
            }


        }
        if(button==13) //enter
        {
            focus='B';
        }
        gotoxy(curs.x, curs.y);
        cout << char(26);
    }
    //drawScreen();
    writeStats();
    return returnspell;

}


int attack(Spell move, int type, Monster enemy)
{
    double damage=0;
    srand(rand());
    int rnd=(rand()%5)-2; //variable damage
    switch(type)
    {
    case 0: //attack
        if(speler.stamina >= move.manaUsed)
        {
            srand(rand());
            int chance=rand()%100;
            if(chance > 85)//no hit
            {
                damage= -1;
                speler.stamina -= move.manaUsed;
            }
            else //hit
            {
                //damage=(move.damage * (speler.maxStamina/100))+(rnd * speler.level);
                damage=speler.level * (speler.maxStamina/10) + rnd;
                speler.stamina -= move.manaUsed;
                damage/=(enemy.level+1);
            }

        }
        else
        {
            damage=0;
        }
        break;
    case 1: //spell
        if(speler.mana >= move.manaUsed)
        {
            srand(rand());
            int chance=rand()%100;
            if(chance > move.accuracy)//no hit
            {
                damage= -1;
                speler.mana -= move.manaUsed;
            }
            else //hit
            {
                //damage=((move.damage *10) * (speler.level * 0.1))+rnd +5;
                damage=(speler.level * (move.damage)) +rnd;
                damage/=enemy.level;
                speler.mana -= move.manaUsed;
            }
        }
        else
        {
            damage=0;
        }
        break;
    }

    if(enemy.element=="Fire") //type advantages
    {
        if(move.element=="Water")
        {
            damage *=2;
        }
        if(move.element=="Nature")
        {
            damage/=2;
        }
    }
    if(enemy.element=="Water")
    {
        if(move.element=="Nature")
        {
            damage*=2;
        }
        if(move.element=="Fire")
        {
            damage/=2;
        }
    }
    if(enemy.element=="Nature")
    {
        if(move.element=="Fire")
        {
            damage*=2;
        }
        if(move.element=="Water")
        {
            damage/=2;
        }

    }

    stringstream ss, ss2;
    ss << "you tried " << move.name << "!";
    battlestr1= ss.str();
    if(damage >0)
    {
        ss2 << "You hit for " << (int)damage << " damage!";
        battlestr2=ss2.str();
    }
    if(damage==0)
    {
        if(type==0)
        {
            battlestr2 = "Not enough stamina!";
        }
        else
        {
            battlestr2= "Not enough mana!";
        }
    }
    if(damage==-1)
    {
        battlestr2="...but you missed!";
        damage=0;
    }

    return damage;
}

void moveEnemies()
{
    setcolor(black,dark_green);
    setcolor(dark_green,black);
    for(int y=0; y<18; y++)
    {
        for(int x=0; x<75; x++)
        {
            if(npcmap[worldx][worldy][x][y]==char(2))
            {
                bool hasMoved=false;
                srand(rand());
                int rnd = rand()%2;
                if(rnd==0)
                {
                    if(speler.y-1<y) //if player is higher
                    {
                        if(checkCollide(x,y-1)==true && hasMoved==false && npcmap[worldx][worldy][x][y-1]==' ')
                        {
                            gotoxy(x+1,y+1);
                            setcolor(dark_green,black);
                            cout << char(176);
                            gotoxy(x+1,y);
                            setcolor(black,dark_green);
                            cout << char(2);
                            npcmap[worldx][worldy][x][y]=' ';
                            npcmap[worldx][worldy][x][y-1]=char(2);
                            hasMoved=true;
                        }

                    }
                    if(speler.y-1>y) //if player is lower
                    {
                        if(checkCollide(x,y+1)==true && hasMoved==false && npcmap[worldx][worldy][x][y+1]==' ')
                        {
                            gotoxy(x+1,y+1);
                            setcolor(dark_green,black);
                            cout << char(176);
                            gotoxy(x+1,y+2);
                            setcolor(black,dark_green);
                            cout << char(2);
                            npcmap[worldx][worldy][x][y]=' ';
                            npcmap[worldx][worldy][x][y+1]=char(2);
                            y++;
                            hasMoved=true;
                        }

                    }
                    if(speler.x-1>x) //if player is right
                    {
                        if(checkCollide(x+1,y)==true && hasMoved==false && npcmap[worldx][worldy][x+1][y]==' ')
                        {
                            gotoxy(x+1,y+1);
                            setcolor(dark_green,black);
                            cout << char(176);
                            setcolor(black,dark_green);
                            cout << char(2);
                            npcmap[worldx][worldy][x][y]=' ';
                            npcmap[worldx][worldy][x+1][y]=char(2);
                            x++;
                            hasMoved=true;
                        }
                    }
                    if(speler.x-1<x) //if player is left
                    {
                        if(checkCollide(x-1,y)==true && hasMoved==false && npcmap[worldx][worldy][x-1][y]==' ')
                        {
                            gotoxy(x+1,y+1);
                            setcolor(dark_green,black);
                            cout << char(176);

                            gotoxy(x,y+1);
                            setcolor(black,dark_green);
                            cout << char(2);
                            npcmap[worldx][worldy][x][y]=' ';
                            npcmap[worldx][worldy][x-1][y]=char(2);
                            hasMoved=true;
                        }

                    }

                }
                else
                {
                    if(speler.x-1>x) //if player is right
                    {
                        if(checkCollide(x+1,y)==true && hasMoved==false && npcmap[worldx][worldy][x+1][y]==' ')
                        {
                            gotoxy(x+1,y+1);
                            setcolor(dark_green,black);
                            cout << char(176);
                            setcolor(black,dark_green);
                            cout << char(2);
                            npcmap[worldx][worldy][x][y]=' ';
                            npcmap[worldx][worldy][x+1][y]=char(2);
                            x++;
                            hasMoved=true;
                        }
                    }
                    if(speler.x-1<x) //if player is left
                    {
                        if(checkCollide(x-1,y)==true && hasMoved==false && npcmap[worldx][worldy][x-1][y]==' ')
                        {
                            gotoxy(x+1,y+1);
                            setcolor(dark_green,black);
                            cout << char(176);

                            gotoxy(x,y+1);
                            setcolor(black,dark_green);
                            cout << char(2);
                            npcmap[worldx][worldy][x][y]=' ';
                            npcmap[worldx][worldy][x-1][y]=char(2);
                            hasMoved=true;
                        }

                    }
                    if(speler.y-1<y) //if player is higher
                    {
                        if(checkCollide(x,y-1)==true && hasMoved==false && npcmap[worldx][worldy][x][y-1]==' ')
                        {
                            gotoxy(x+1,y+1);
                            setcolor(dark_green,black);
                            cout << char(176);
                            gotoxy(x+1,y);
                            setcolor(black,dark_green);
                            cout << char(2);
                            npcmap[worldx][worldy][x][y]=' ';
                            npcmap[worldx][worldy][x][y-1]=char(2);
                            hasMoved=true;
                        }

                    }
                    if(speler.y-1>y) //if player is lower
                    {
                        if(checkCollide(x,y+1)==true && hasMoved==false && npcmap[worldx][worldy][x][y+1]==' ')
                        {
                            gotoxy(x+1,y+1);
                            setcolor(dark_green,black);
                            cout << char(176);
                            gotoxy(x+1,y+2);
                            setcolor(black,dark_green);
                            cout << char(2);
                            npcmap[worldx][worldy][x][y]=' ';
                            npcmap[worldx][worldy][x][y+1]=char(2);
                            y++;
                            hasMoved=true;
                        }

                    }

                }
            }
        }
    }
}

void dead(Monster enemy)
{
    setcolor(yellow,dark_red);
    clearScreen();

    PlaySound(TEXT("music/gameover.wav"), NULL, SND_LOOP | SND_ASYNC);
    drawBorder(10,3,60,15);

    gotoxy(33,4);
    cout << "You Died...";
    gotoxy(13,6);
    cout << "Oh dear, it seems you were killed by a " << enemy.type << "!";
    gotoxy(13,7);
    cout << "How unfortunate.";

    gotoxy(13,9);
    cout << "You collected " << speler.keys << " out of three keys,";

    gotoxy(13,10);
    cout << "And you reached level " << speler.level << ".";

    gotoxy(13,12);
    cout << "Each time you play the game, the maps will change";
    gotoxy(13,13);
    cout << "randomly, So each playthrough is different!";

    gotoxy(33,15);
    cout << "GAME OVER";


    while(1)
    {
        int button=0;
        button=getch();
    }
}

bool checkPlayerCollide(int x, int y)
{
    switch(worldmap[worldx][worldy][x][y])
    {
    case char(176): //grass
        return true;
        break;
    case 'X': //treasure
    {
        worldmap[worldx][worldy][x-1][y-1]=char(176);
        worldmap[worldx][worldy][speler.x-1][speler.y-1]=char(176);
        setcolor(yellow,dark_red);
        drawBorder(20,10,40,7);
        gotoxy(35,11);
        cout << "Treasure!";
        gotoxy(22,13);
        cout << "X marks the spot! You find a chest.";
        gotoxy(22,14);
        cout << "You open it to find ";

        srand(time(NULL));
        int pot=(rand()%3)+1;
        srand(rand());
        int amount=(rand()%3)+1;

        cout << amount << " ";
        switch(pot)
        {
        case 1:
            cout << "Health potion(s)!";
            inventory.healthpots+=amount;
            break;
        case 2:
            cout << "Mana potion(s)!";
            inventory.manapots+=amount;
            break;
        case 3:
            cout << "Stamina potion(s)!";
            inventory.staminapots+=amount;
            break;
        }
        hideCursor();
        int button=0;
        button=getch();

        drawScreen();
        return true;
        break;
    }

    case char(2): //boss
        switch(speler.keys)
        {
        case 0:
            battle(monsters[11]);
            speler.keys+=1;
            break;
        case 1:
            battle(monsters[12]);
            speler.keys+=1;
            break;
        case 2:
            battle(monsters[13]);
            speler.keys+=1;
            worldmap[10][10][75][16]=char(1);
            break;
        default:
            break;
        }
        return true;
        break;
    case char(1): //jelle
        battle(monsters[14]);
        win();
        return true;
        break;
    case char(215): //door
        return true;
        break;

    default:
        return false;
        break;
    }
}

void win()
{
    PlaySound(TEXT("music/victory.wav"), NULL, SND_LOOP | SND_ASYNC);
    setcolor(yellow,dark_red);
    drawBorder(10,3,60,15);

    gotoxy(25,4);
    cout << "You Defeated Lord Jelle!";
    gotoxy(13,6);
    cout << "You fought bravely, gathered all 3 keys by beating";
    gotoxy(13,7);
    cout << "Jelle's henchmen, and then proceeded to kill";
    gotoxy(13,8);
    cout << "Jelle himself! The world is once again saved!";
    gotoxy(13,9);
    cout << "Thank you, " << speler.name << "!";
    gotoxy(13,10);
    cout << "You reached level " << speler.level << "; making you the";
    gotoxy(13,11);
    cout << "strongest warrior in the land! THANKS FOR PLAYING!!";

    gotoxy(33,15);
    cout << "GAME OVER";


    while(1)
    {
        int button=0;
        button=getch();
    }
}

void showCheats() //NO CHEATING PLS
{
    setcolor(yellow, dark_red);
    drawBorder(25,6,30,5);
    gotoxy(37,7);
    cout << "Cheat";
    gotoxy(26,9);
    cout << ">";
    string cheat;
    cin >> cheat;
    focus='O';
    drawScreen();
    if(cheat=="kennyisamazing")
    {
        speler.maxHP=1000;
        speler.maxStamina=1000;
        speler.maxMana=1000;

        speler.HP=speler.maxHP;
        speler.mana=speler.maxMana;
        speler.stamina=speler.maxMana;
        speler.level=20;
    }
    if(cheat=="kill")
    {
        dead(monsters[0]);
    }
    if(cheat=="ihazwondagame")
    {
        win();
    }
}
