/*
 * Proiect IP
 *
 * Facultatea de Informatica Iasi
 * Anul I (2019-2020), Grupa B5
 *
 * (c) 2019-2020 Serban Talpiz & Iustina Filipescu
 */

#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

/// Dimensiunile ferestrei.
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

/// Cate linii si coloane sa aiba o harta/piesa.
#define MAP_ROWS 3
#define MAP_COLS 3

/// Dimensiunea (in pixeli) a unei celule dintr-o harta.
#define MAP_CELL_SIZE 100
/// Dimensiunea (in pixeli) a spatiului dintre celulele unei harti.
#define MAP_CELL_BORDER 8

/// Latimea si inaltimea totala a unei harti din cele 4.
#define MAP_WIDTH (MAP_CELL_SIZE * MAP_COLS + MAP_CELL_BORDER * (MAP_COLS + 1))
#define MAP_HEIGHT (MAP_CELL_SIZE * MAP_ROWS + MAP_CELL_BORDER * (MAP_ROWS + 1))

/// Offset-ul pentru X si Y a hartii din stanga sus.
#define MAP_X_OFFSET (SCREEN_WIDTH / 3 - MAP_WIDTH)
#define MAP_Y_OFFSET (SCREEN_HEIGHT / 2 - MAP_HEIGHT)

/// Dimensiunea (in pixeli) a unei celule dintr-o piesa.
#define PIECE_CELL_SIZE 50
/// Dimensiunea (in pixeli) a spatiului dintre celulele dintr-o piesa.
#define PIECE_CELL_BORDER 4

/// Latimea si inaltimea totala a unei piese din cele 4.
#define PIECE_WIDTH (PIECE_CELL_SIZE * MAP_COLS + PIECE_CELL_BORDER * (MAP_COLS + 1))
#define PIECE_HEIGHT (PIECE_CELL_SIZE * MAP_ROWS + PIECE_CELL_BORDER * (MAP_ROWS + 1))

/// Spatiul dintre piese (pe axa Y).
#define PIECE_PADDING 5

/// Offset-ul pentru X si Y a primei piese de sus.
#define PIECE_X_OFFSET (SCREEN_WIDTH - PIECE_WIDTH - 20)
#define PIECE_Y_OFFSET (SCREEN_HEIGHT / 2 - ((PIECE_HEIGHT * 4) + PIECE_PADDING * 3) / 2)

/// Culoarea de background a hartilor.
#define MAP_BACKGROUND_COLOR COLOR(255, 255, 255)
/// Culoarea unei celule goale.
#define MAP_EMPTY_CELL_COLOR COLOR(0, 255, 0)

/// Marimea patratului care indica piesa selectata.
#define PIECE_SELECTED_SIZE 20
/// Culoarea patratului care indica piesa selectata.
#define PIECE_SELECTED_COLOR COLOR(255, 0, 0)

/// Cele 4 matrici care contin hartile (0 - stanga sus, 1 - dreapta sus, 2 - stanga jos, 3 - dreapta jos).
/// maps[0] iti da prima matrice, maps[1] a doua etc.
int maps[4][MAP_ROWS][MAP_COLS];

/// Cele 4 matrici care contin piesele (0 - cea mai de sus, 1 - sub prima, 2 - sub a doua, 3 - sub a treia).
/// Exact ca si la maps, pieces[0] iti da prima matrice etc.
int pieces[4][MAP_ROWS][MAP_COLS];

/// Cele 4 matrici care contin hartile nivelului, exact ca maps.
/// Aici nu se modifica nimic, decat la incarcarea nivelului. Matricile astea se folosesc pentru a reseta nivelul.
int levelMaps[4][MAP_ROWS][MAP_COLS];

/// Cele 4 matrici care contin piesele nivelului, exact ca pieces.
/// Aici nu se modifica nimic, decat la incarcarea nivelului. Matricile astea se folosesc pentru a reseta nivelul.
int levelPieces[4][MAP_ROWS][MAP_COLS];

/// Vectorii pentru piesele si hartile utilizate deja.
/// isUsedPiece[i] = True (1), daca piesa i a fost folosita. False (0) in caz contrar.
/// isUsedPiece[i] = True (1), daca harta i a fost ocupata. False (0) in caz contrar.
bool isUsedPiece[4], isUsedMap[4];

/// Contine numarul piesei selectate.
/// -1 inseamna ca nici o piesa nu este selectata.
int selectedPiece = -1;

/// Deseneaza o piesa cu un anumit numar (0 - cea mai de sus, 1 - sub prima piesa, etc).
void drawPiece(int number) {
    int xOffset = PIECE_X_OFFSET;
    int yOffset = PIECE_Y_OFFSET + number * (PIECE_HEIGHT + PIECE_PADDING);
    /// Y-ul se modifica in functie de a cata piesa este. X-ul este mereu acelasi.

    setfillstyle(SOLID_FILL, MAP_BACKGROUND_COLOR);

    bar(xOffset, yOffset, xOffset + PIECE_WIDTH, yOffset + PIECE_HEIGHT);


    /// Deseneaza continutul piesei doar daca nu a fost deja folosita.
    if(!isUsedPiece[number]) {
        for(int i = 0; i < MAP_ROWS; ++i) {
            for(int j = 0; j < MAP_COLS; ++j) {
                switch(pieces[number][i][j]) {
                    case 0: {
                        /// Gol (Permis).
                        setfillstyle(SOLID_FILL, MAP_EMPTY_CELL_COLOR);

                        int xStart = xOffset + PIECE_CELL_SIZE * j + PIECE_CELL_BORDER * (j + 1);
                        int yStart = yOffset + PIECE_CELL_SIZE * i + PIECE_CELL_BORDER * (i + 1);

                        bar(xStart, yStart, xStart + PIECE_CELL_SIZE, yStart + PIECE_CELL_SIZE);

                        break;
                    }
                    case 1: {
                        /// Piesa.
                        setfillstyle(SOLID_FILL, COLOR(0, 0, 255));

                        int xStart = xOffset + PIECE_CELL_SIZE * j + PIECE_CELL_BORDER * (j + 1);
                        int yStart = yOffset + PIECE_CELL_SIZE * i + PIECE_CELL_BORDER * (i + 1);

                        bar(xStart, yStart, xStart + PIECE_CELL_SIZE, yStart + PIECE_CELL_SIZE);

                        break;
                    }
                    case 2: {
                        /// Interzis.
                        setfillstyle(SOLID_FILL, COLOR(255, 0, 0));

                        int xStart = xOffset + PIECE_CELL_SIZE * j + PIECE_CELL_BORDER * (j + 1);
                        int yStart = yOffset + PIECE_CELL_SIZE * i + PIECE_CELL_BORDER * (i + 1);

                        bar(xStart, yStart, xStart + PIECE_CELL_SIZE, yStart + PIECE_CELL_SIZE);

                        break;
                    }
                    case 3: {
                        /// Interzis.
                        setfillstyle(SOLID_FILL, COLOR(255, 0, 0));

                        int xStart = xOffset + PIECE_CELL_SIZE * j + PIECE_CELL_BORDER * (j + 1);
                        int yStart = yOffset + PIECE_CELL_SIZE * i + PIECE_CELL_BORDER * (i + 1);

                        bar(xStart, yStart, xStart + PIECE_CELL_SIZE, yStart + PIECE_CELL_SIZE);

                        break;
                    }
                    case 4: {
                        /// Permis.
                        setfillstyle(SOLID_FILL, MAP_EMPTY_CELL_COLOR);

                        int xStart = xOffset + PIECE_CELL_SIZE * j + PIECE_CELL_BORDER * (j + 1);
                        int yStart = yOffset + PIECE_CELL_SIZE * i + PIECE_CELL_BORDER * (i + 1);

                        bar(xStart, yStart, xStart + PIECE_CELL_SIZE, yStart + PIECE_CELL_SIZE);

                        break;
                    }
                    case 5: {
                        /// Permis.
                        setfillstyle(SOLID_FILL, MAP_EMPTY_CELL_COLOR);

                        int xStart = xOffset + PIECE_CELL_SIZE * j + PIECE_CELL_BORDER * (j + 1);
                        int yStart = yOffset + PIECE_CELL_SIZE * i + PIECE_CELL_BORDER * (i + 1);

                        bar(xStart, yStart, xStart + PIECE_CELL_SIZE, yStart + PIECE_CELL_SIZE);

                        break;
                    }
                    default: {
                        break;

                    }
                }
            }
        }
    }
}

/// Deseneaza o harta cu un anumit numar (0 - stanga sus, 1 - dreapta sus, etc).
void drawMap(int number) {
    int xOffset = MAP_X_OFFSET + (number % 2) * MAP_WIDTH;
    int yOffset = MAP_Y_OFFSET + (number / 2) * MAP_HEIGHT;
    /// Calculeaza X si Y in functie de numarul hartii.

    setfillstyle(SOLID_FILL, MAP_BACKGROUND_COLOR);

    bar(xOffset, yOffset, xOffset + MAP_WIDTH, yOffset + MAP_HEIGHT);

    /// Deseneaza continutul hartii.
    for(int i = 0; i < MAP_ROWS; ++i) {
        for(int j = 0; j < MAP_COLS; ++j) {
            switch(maps[number][i][j]) {
                case 0: {
                    /// Gol (Permis).
                    setfillstyle(SOLID_FILL, MAP_EMPTY_CELL_COLOR);

                    int xStart = xOffset + MAP_CELL_SIZE * j + MAP_CELL_BORDER * (j + 1);
                    int yStart = yOffset + MAP_CELL_SIZE * i + MAP_CELL_BORDER * (i + 1);

                    bar(xStart, yStart, xStart + MAP_CELL_SIZE, yStart + MAP_CELL_SIZE);

                    break;
                }
                case 1: {
                    /// Piesa.
                    setfillstyle(SOLID_FILL, COLOR(0, 0, 255));

                    int xStart = xOffset + MAP_CELL_SIZE * j + MAP_CELL_BORDER * (j + 1);
                    int yStart = yOffset + MAP_CELL_SIZE * i + MAP_CELL_BORDER * (i + 1);

                    bar(xStart, yStart, xStart + MAP_CELL_SIZE, yStart + MAP_CELL_SIZE);

                    break;
                }
                case 2: {
                    /// Interzis.
                    setfillstyle(SOLID_FILL, RED);

                    int xStart = xOffset + MAP_CELL_SIZE * j + MAP_CELL_BORDER * (j + 1);
                    int yStart = yOffset + MAP_CELL_SIZE * i + MAP_CELL_BORDER * (i + 1);

                    bar(xStart, yStart, xStart + MAP_CELL_SIZE, yStart + MAP_CELL_SIZE);

                    break;
                }
                case 3: {
                    /// Interzis.
                    setfillstyle(SOLID_FILL, YELLOW);

                    int xStart = xOffset + MAP_CELL_SIZE * j + MAP_CELL_BORDER * (j + 1);
                    int yStart = yOffset + MAP_CELL_SIZE * i + MAP_CELL_BORDER * (i + 1);

                    bar(xStart, yStart, xStart + MAP_CELL_SIZE, yStart + MAP_CELL_SIZE);

                    break;
                }
                case 4: {
                    /// Permis.
                    setfillstyle(SOLID_FILL, MAP_EMPTY_CELL_COLOR);

                    int xStart = xOffset + MAP_CELL_SIZE * j + MAP_CELL_BORDER * (j + 1);
                    int yStart = yOffset + MAP_CELL_SIZE * i + MAP_CELL_BORDER * (i + 1);

                    bar(xStart, yStart, xStart + MAP_CELL_SIZE, yStart + MAP_CELL_SIZE);

                    break;
                }
                case 5: {
                    /// Permis.
                    setfillstyle(SOLID_FILL, MAP_EMPTY_CELL_COLOR);

                    int xStart = xOffset + MAP_CELL_SIZE * j + MAP_CELL_BORDER * (j + 1);
                    int yStart = yOffset + MAP_CELL_SIZE * i + MAP_CELL_BORDER * (i + 1);

                    bar(xStart, yStart, xStart + MAP_CELL_SIZE, yStart + MAP_CELL_SIZE);

                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
}

/// Selecteaza o piesa cu un anumit numar (0 - prima, 1 - sub prima, etc)
void selectPiece(int number) {
    selectedPiece = number;

    setfillstyle(SOLID_FILL, COLOR(0, 0, 0));

    /// Atentie! Numar magic '50' ! Daca modifici marimea patratului indicator, trebuie sa modifici si aici distanta
    /// dintre el si piese.
    int xOffset = PIECE_X_OFFSET - 50;
    int yOffset = 0;

    /// Traseaza o bara neagra pentru a sterge vechiul patrat (de la Y = 0 la inaltimea ferestrei, ca sa fie sigur).
    bar(xOffset, yOffset, xOffset + PIECE_SELECTED_SIZE, yOffset + SCREEN_HEIGHT);

    /// Daca a selectat o piesa (adica nu este cazul -1), deseneaza patratul indicator.
    if(selectedPiece >= 0) {
        setfillstyle(SOLID_FILL, PIECE_SELECTED_COLOR);

        int xOffset = PIECE_X_OFFSET - 50;
        int yOffset = PIECE_Y_OFFSET + number * (PIECE_HEIGHT + PIECE_PADDING) + PIECE_HEIGHT / 2 - PIECE_SELECTED_SIZE / 2;

        bar(xOffset, yOffset, xOffset + PIECE_SELECTED_SIZE, yOffset + PIECE_SELECTED_SIZE);
    }
}

/// Deseneaza toate hartile.
void drawMaps() {
    for(int i = 0; i < 4; ++i)
        drawMap(i);
}

/// Deseneaza toate piesele.
void drawPieces() {
    for(int i = 0; i < 4; ++i)
        drawPiece(i);
    selectPiece(0);
}


/// Reseteaza nivelul.
/// Matricile cu harti si cu piese sunt reinitializate la cele stocate in nivel (matricile 'levelMaps' si 'levelPieces').
void resetLevel() {
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < MAP_ROWS; ++j) {
            for(int k = 0; k < MAP_COLS; ++k) {
                maps[i][j][k] = levelMaps[i][j][k];
                pieces[i][j][k] = levelPieces[i][j][k];
            }
        }
    }

    /// Culoarea neagra.
    setfillstyle(SOLID_FILL, COLOR(0, 0, 0));

    /// Curata ecranul.
    bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    /// Reinitiealizeaza vectorii isUsed la 0 (pentru ca piesele si hartile sunt resetate).
    for(int i = 0; i < 4; ++i) {
        isUsedPiece[i] = 0;
        isUsedMap[i] = 0;
    }
    /// Redeseneaza hartile si piesele.
    drawMaps();
    drawPieces();
}



void finishedLevel() {
    bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    settextstyle(BOLD_FONT,HORIZ_DIR,7);
    bgiout<<"NIVEL TERMINAT";
    outstreamxy(370,200);
    settextstyle(BOLD_FONT,HORIZ_DIR,5);
    bgiout<<"FELICITARI !!";
    outstreamxy(500,300);

    setfillstyle(SOLID_FILL,LIGHTGRAY);
    bar(530,490,750,555);
    setfillstyle(SOLID_FILL,BLACK);
    bar(540,500,740,545);
    settextstyle(4,HORIZ_DIR,2);
    bgiout<<"IESI DIN JOC";
    outstreamxy(550,510);
    while(1){
        if(ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if(x >= 540 && x <= 720 && y >= 500 && y <= 545)
                closegraph();
        }
    }
}

/// Verifica daca playerul a castigat.
/// Daca nu, nu face nimic.
/// Se apeleaza doar cand playerul a pus toate piesele.
void checkForWin() {
    int count2=0,count3=0;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < MAP_ROWS; ++j) {
            for(int k = 0; k < MAP_COLS; ++k) {
                if(maps[i][j][k] == 2)
                    count2++;
                if(maps[i][j][k] == 3)
                    count3++;
            }
        }
    }

    if(count2 == 2 && count3 == 3) {
        /// Playerul a castigat.
        finishedLevel();
    }
        else
        {
            settextstyle(BOLD_FONT, HORIZ_DIR, 5);
            bgiout<<"MAI INCEARCA";
            outstreamxy(320,300);
            delay(2000);
            resetLevel();
        }


}

/// Pune o piesa peste o harta.
/// La fel ca si mai sus, numerele reprezinta a cata piesa respectiv a cata harta.
void placePiece(int pieceNumber, int mapNumber) {
    /// Inlocuieste continutul hartii cu cel al piesei.
    for(int i = 0; i < MAP_ROWS; ++i) {
        for(int j = 0; j < MAP_COLS; ++j) {
            /// Daca piesa are celule care sunt goale, atunci harta pastreaza ce avea inainte.
            if(pieces[pieceNumber][i][j] != 0)
                maps[mapNumber][i][j] = pieces[pieceNumber][i][j];
        }
    }

    /// Marcheaza harta si piesa ca fiind folosite.
    isUsedMap[mapNumber] = 1;
    isUsedPiece[pieceNumber] = 1;

    /// Redeseneaza doar harta si piesa care au fost folosite.
    drawMap(mapNumber);
    drawPiece(pieceNumber);

    /// Selecteaza prima piesa disponibila. Daca nu exista, nu va selecta nimic si va verifica daca playerul a castigat.
    for(int i = 0; i < 4; ++i) {
        if(!isUsedPiece[i]) {
            selectPiece(i);
            return;
        }
    }

    selectPiece(-1);
    checkForWin();
    return;
}

/// Roteste o piesa cu un numar (ca si mai sus, 0 = cea mai de sus, etc) la 90 de grade, in sensul acelor de ceasornic.
void rotatePiece(int number) {
    for(int i = 0; i < MAP_ROWS / 2; ++i) {
        for(int j = i; j < MAP_COLS - i - 1; ++j) {
            int temp = pieces[number][i][j];

            pieces[number][i][j] = pieces[number][MAP_ROWS - 1 - j][i];
            pieces[number][MAP_ROWS - 1 - j][i] = pieces[number][MAP_ROWS - 1 - i][MAP_ROWS - 1 - j];
            pieces[number][MAP_ROWS - 1 - i][MAP_ROWS - 1- j] = pieces[number][j][MAP_ROWS - 1 - i];
            pieces[number][j][MAP_ROWS - 1 - i] = temp;
        }
    }
}


/// Initializeaza nivelul.
void initLevel() {

        levelMaps[0][0][0] = 0;
        levelMaps[0][0][1] = 4;
        levelMaps[0][0][2] = 4;

        levelMaps[0][1][0] = 2;
        levelMaps[0][1][1] = 0;
        levelMaps[0][1][2] = 4;

        levelMaps[0][2][0] = 4;
        levelMaps[0][2][1] = 0;
        levelMaps[0][2][2] = 0;

        levelMaps[1][0][0] = 4;
        levelMaps[1][0][1] = 4;
        levelMaps[1][0][2] = 0;

        levelMaps[1][1][0] = 0;
        levelMaps[1][1][1] = 2;
        levelMaps[1][1][2] = 4;

        levelMaps[1][2][0] = 4;
        levelMaps[1][2][1] = 3;
        levelMaps[1][2][2] = 0;

        levelMaps[2][0][0] = 4;
        levelMaps[2][0][1] = 4;
        levelMaps[2][0][2] = 3;

        levelMaps[2][1][0] = 2;
        levelMaps[2][1][1] = 4;
        levelMaps[2][1][2] = 0;

        levelMaps[2][2][0] = 3;
        levelMaps[2][2][1] = 4;
        levelMaps[2][2][2] = 4;

        levelMaps[3][0][0] = 0;
        levelMaps[3][0][1] = 0;
        levelMaps[3][0][2] = 0;

        levelMaps[3][1][0] = 4;
        levelMaps[3][1][1] = 4;
        levelMaps[3][1][2] = 4;

        levelMaps[3][2][0] = 2;
        levelMaps[3][2][1] = 3;
        levelMaps[3][2][2] = 4;

        levelPieces[0][0][0] = 1;
        levelPieces[0][0][1] = 0;
        levelPieces[0][0][2] = 1;

        levelPieces[0][1][0] = 1;
        levelPieces[0][1][1] = 0;
        levelPieces[0][1][2] = 1;

        levelPieces[0][2][0] = 1;
        levelPieces[0][2][1] = 1;
        levelPieces[0][2][2] = 1;

        levelPieces[1][0][0] = 1;
        levelPieces[1][0][1] = 1;
        levelPieces[1][0][2] = 1;

        levelPieces[1][1][0] = 0;
        levelPieces[1][1][1] = 1;
        levelPieces[1][1][2] = 0;

        levelPieces[1][2][0] = 1;
        levelPieces[1][2][1] = 1;
        levelPieces[1][2][2] = 1;

        levelPieces[2][0][0] = 1;
        levelPieces[2][0][1] = 1;
        levelPieces[2][0][2] = 0;

        levelPieces[2][1][0] = 1;
        levelPieces[2][1][1] = 1;
        levelPieces[2][1][2] = 1;

        levelPieces[2][2][0] = 1;
        levelPieces[2][2][1] = 0;
        levelPieces[2][2][2] = 1;

        levelPieces[3][0][0] = 0;
        levelPieces[3][0][1] = 1;
        levelPieces[3][0][2] = 1;

        levelPieces[3][1][0] = 1;
        levelPieces[3][1][1] = 1;
        levelPieces[3][1][2] = 1;

        levelPieces[3][2][0] = 1;
        levelPieces[3][2][1] = 1;
        levelPieces[3][2][2] = 0;
        /// END   ----------------------------------------------------------------------------------------------

        /// Initializeaza vectorii cu hartile si piesele folosite la 0.
        for(int i = 0; i < 4; ++i) {
            isUsedPiece[i] = 0;
            isUsedMap[i] = 0;
        }

        /// Curata ecranul si deseneaza nivelul.
        resetLevel();

}

/// Loop-ul care asteapta input.
void inputLoop() {
    while(true) {
        /// Click dreapta.
        if(ismouseclick(WM_RBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_RBUTTONDOWN, x, y);

            if(x >= PIECE_X_OFFSET && x <= PIECE_X_OFFSET + PIECE_WIDTH) {
                /// Pe o piesa.

                for(int i = 0; i < 4; ++i) {
                    if(y >= PIECE_Y_OFFSET + i * (PIECE_HEIGHT + PIECE_PADDING) && y <= PIECE_Y_OFFSET + i * (PIECE_HEIGHT + PIECE_PADDING) + PIECE_HEIGHT) {
                        if(!isUsedPiece[i]) {
                            rotatePiece(i);
                            drawPiece(i);
                            selectPiece(i);
                        }
                    }
                }
            }
        }
        /// Click stanga.
        if(ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if(x >= PIECE_X_OFFSET && x <= PIECE_X_OFFSET + PIECE_WIDTH) {
                /// Pe o piesa.

                for(int i = 0; i < 4; ++i) {
                    if(y >= PIECE_Y_OFFSET + i * (PIECE_HEIGHT + PIECE_PADDING) && y <= PIECE_Y_OFFSET + i * (PIECE_HEIGHT + PIECE_PADDING) + PIECE_HEIGHT) {
                        if(!isUsedPiece[i])
                            selectPiece(i);
                    }
                }
            } else if(x >= MAP_X_OFFSET && x <= MAP_X_OFFSET + MAP_WIDTH) {
                /// Pe o harta din stanga.

                if(y >= MAP_Y_OFFSET && y <= MAP_Y_OFFSET + MAP_HEIGHT) {
                    /// Pe harta din stanga sus.

                    if(!isUsedMap[0])
                        placePiece(selectedPiece, 0);
                } else if(y > MAP_Y_OFFSET + MAP_HEIGHT && y <= MAP_Y_OFFSET + 2 * MAP_HEIGHT) {
                    /// Pe harta din stanga jos.

                    if(!isUsedMap[2])
                        placePiece(selectedPiece, 2);
                }
            } else if(x >= MAP_X_OFFSET + MAP_WIDTH && x <= MAP_X_OFFSET + 2 * MAP_WIDTH) {
                /// Pe o harta din dreapta.

                if(y >= MAP_Y_OFFSET && y <= MAP_Y_OFFSET + MAP_HEIGHT) {
                    /// Pe harta din dreapta sus.

                    if(!isUsedMap[1])
                        placePiece(selectedPiece, 1);
                } else if(y > MAP_Y_OFFSET + MAP_HEIGHT && y <= MAP_Y_OFFSET + 2 * MAP_HEIGHT) {
                    /// Pe harta din dreapta jos.

                    if(!isUsedMap[3])
                        placePiece(selectedPiece, 3);
                }
            }
        }
        /// Tasta R.
        if(GetAsyncKeyState('R')) {
            resetLevel();
            delay(10);
        }
        /// Delay de 1ms.
        delay(1);
    }
}




void instructiuni() {
    setfillstyle(SOLID_FILL,BLACK);
    bar(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    settextstyle(4,HORIZ_DIR,6);
    bgiout<<"OBIECTIVUL JOCULUI:"<<endl;
    outstreamxy(150,160);
    settextstyle(4,HORIZ_DIR,3);
    bgiout<<"ARANJEAZA PIESELE ASTFEL INCAT SA RAMANA"<<endl;
    outstreamxy(150,250);
    settextstyle(4,HORIZ_DIR,3);
    bgiout<<"NEACOPERITE 3 PATRATE GALBENE SI 2 PATRATE ROSII."<<endl;
    outstreamxy(150,280);
    settextstyle(4,HORIZ_DIR,3);
    bgiout<<"MULT SUCCES !!"<<endl;
    outstreamxy(150,310);
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    bar(480,490,730,555);
    setfillstyle(SOLID_FILL,BLACK);
    bar(490,500,720,545);
    settextstyle(4,HORIZ_DIR,2);
    bgiout<<"DU-TE LA JOC";
    outstreamxy(500,510);
    while(1) {
        if(ismouseclick(WM_LBUTTONDOWN)) {
            int a, b;
            getmouseclick(WM_LBUTTONDOWN, a, b);

            if(a >= 540 && a <= 720)
                if(b >= 500 && b <= 545)
                    initLevel();
                    inputLoop();
        }
    }
}

void menu() {
    setfillstyle(SOLID_FILL,BLACK);
    bar(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    settextstyle(BOLD_FONT,HORIZ_DIR,7);
    bgiout<<"PIRATES HIDE-AND-SEEK";
    outstreamxy(200,200);

    setfillstyle(SOLID_FILL,LIGHTGRAY);
    bar(530,490,735,555);
    setfillstyle(SOLID_FILL,BLACK);
    bar(540,500,725,545);
    settextstyle(4,HORIZ_DIR,2);
    bgiout<<"START JOC";
    outstreamxy(550,510);


    while(1) {
        if(ismouseclick(WM_LBUTTONDOWN)) {
            int a, b;
            getmouseclick(WM_LBUTTONDOWN, a, b);

            if(a >= 540 && a <= 720)
                if(b >= 500 && b <= 545)
                    instructiuni();
        }
    }
}
int main()
{
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    PlaySoundA("muzica.wav", NULL, SND_FILENAME|SND_ASYNC|SND_LOOP );
    /// Incepe jocul.
    menu();

    /// Asteapta input.
    inputLoop();

    getch();
    closegraph();
    return 0;
}
