/**
    Estrutura inicial para um jogo
    versão: 0.1 (Prof. Alex,  Adaptado Prof. Felski)
*/
#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

void explosaoBomba(int m[15][45], int xBomba, int yBomba, int *contBomba){
    if (m[xBomba][yBomba] == 3){
        m[xBomba][yBomba] = 4;
    }
    for(int i = 0; i < 2; i++){
        if(m[xBomba - i][yBomba] == 0 || m[xBomba - i][yBomba] == 2 ){
            m[xBomba - i][yBomba] = 4;
        }
        if(m[xBomba + i][yBomba] == 0 || m[xBomba + i][yBomba] == 2 ){
            m[xBomba + i][yBomba] = 4;
        }
        if(m[xBomba][yBomba - i] == 0 || m[xBomba][yBomba - i] == 2 ){
            m[xBomba][yBomba - i] = 4;
        }
        if(m[xBomba][yBomba + i] == 0 || m[xBomba][yBomba + i] == 2 ){
            m[xBomba][yBomba + i] = 4;
        }
    }
    *contBomba = 2;
}

int main()
{
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
        //FIM: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
        short int CX=0, CY=0;
        COORD coord;
        coord.X = CX;
        coord.Y = CY;
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.
    int m[15][45]={ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                    1,0,0,0,2,2,0,2,2,0,2,0,2,2,0,2,2,0,2,2,0,2,1,0,2,2,0,2,2,0,2,2,0,2,2,0,2,2,0,2,2,0,2,2,1,
                    1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,
                    1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,2,1,
                    1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,2,1,
                    1,0,0,2,2,0,0,0,0,2,1,0,0,2,0,0,0,2,2,0,0,0,0,0,0,1,0,0,0,0,2,2,2,0,1,0,0,0,0,2,0,1,0,0,1,
                    1,2,1,2,1,1,1,2,1,0,1,0,1,2,1,1,1,2,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,2,1,0,1,2,1,
                    1,2,1,0,0,0,2,2,0,0,1,1,1,0,0,0,1,0,0,0,0,2,0,0,0,0,2,2,0,0,0,0,1,0,0,2,0,0,0,0,1,0,0,2,1,
                    1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,2,1,1,1,0,1,2,1,1,1,0,1,0,1,2,1,2,1,0,1,0,1,0,1,
                    1,2,0,2,0,0,0,0,0,0,1,0,0,0,0,2,2,0,1,2,2,0,0,0,0,1,0,0,0,0,0,0,0,2,2,0,2,2,0,0,0,1,0,2,1,
                    1,2,1,2,1,1,1,1,1,0,1,2,1,0,1,1,1,0,1,2,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,2,1,0,1,1,1,0,1,2,1,
                    1,0,0,0,1,0,0,0,2,2,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,2,2,2,0,1,0,0,0,0,0,1,
                    1,2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,0,1,1,2,1,
                    1,2,2,0,2,2,1,2,2,0,2,2,0,2,2,0,2,2,0,2,2,0,2,2,1,2,2,0,2,2,0,2,2,0,2,2,0,2,1,0,2,2,0,2,1,
                    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    int x=3, y=1, xBomba, yBomba;/// Posição inicial do personagem
    int contBomba = 0;/// Contador da bomba
    unsigned long tempoBomba = 0;
    char tecla;/// Variavel para tecla precionada

    while(true){
        ///Posiciona a escrita no iicio do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        ///Imprime o jogo
        for(int i=0;i<15;i++){
            for(int j=0;j<45;j++){
                if(i==x && j==y){
                    cout << char(36); //personagem
                } else {
                    switch (m[i][j]){
                        case 0: cout << " "; break; //caminho
                        case 1: cout << char(219); break; //parede fixa
                        case 2: cout << char(176);break; //parede que quebra
                        case 3: cout << char(42);break; // Bomba
                        case 4: cout << char(64);break;//explosão da bomba
                    }
                }
            }
            cout<<"\n";
        }

        ///executa os movimentos
        if ( _kbhit() ){
            tecla = getch();
            switch(tecla)
            {
                case 72: case 'w': ///cima
                    if(m[x-1][y] == 0 || m[x-1][y] == 4){
                        x--;
                    }
                break;
                case 80: case 's': ///baixo
                    if(m[x+1][y] == 0 || m[x-1][y] == 4){
                        x++;
                    }
                break;
                case 75:case 'a': ///esquerda
                    if(m[x][y-1] == 0 || m[x-1][y] == 4){
                        y--;
                    }
                break;
                case 77: case 'd': ///direita
                    if(m[x][y+1] == 0 || m[x-1][y] == 4){
                        y++;
                    }
                break;
                case 32:
                    if(contBomba == 0){
                        m[x][y] = 3;
                        contBomba = 1;
                        xBomba = x;
                        yBomba = y;
                        tempoBomba = GetTickCount();
                    }
                break;
            }
        }
        if(contBomba == 1 && GetTickCount() - tempoBomba >= 3000){
            explosaoBomba(m, xBomba, yBomba, &contBomba);
            tempoBomba = GetTickCount();
        }
        if(contBomba == 2 && GetTickCount() - tempoBomba >= 3000){
            for(int i=0;i<15;i++){
                for(int j=0;j<45;j++){
                    if(m[i][j] == 4){
                        m[i][j] = 0;
                    }
                }
            }
            contBomba = 0;
        }
    } //fim do laco do jogo

    return 0;
} //fim main
