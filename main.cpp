/// Bibliotecas
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
using namespace std;

/// Função da cor de fundo
void setColor(int text, int background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hConsole, background * 16 + text);
}
/// Função de explosão da bomba
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
                    1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,0,2,1,
                    1,0,0,2,2,0,0,0,0,2,1,0,0,2,0,0,0,2,2,0,0,0,0,0,0,1,0,0,0,0,2,2,2,0,1,0,0,0,0,2,0,0,1,0,1,
                    1,2,1,2,1,1,1,2,1,0,1,0,1,2,1,1,1,2,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,2,1,0,0,2,1,
                    1,2,1,0,0,0,2,2,0,0,1,1,1,0,0,0,1,0,0,0,0,2,0,0,0,0,2,2,0,0,0,0,1,0,0,2,0,0,0,0,1,0,0,2,1,
                    1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,2,1,1,1,0,1,2,1,1,1,0,1,0,1,2,1,2,1,0,1,0,1,0,1,
                    1,2,0,2,0,0,0,0,0,0,1,0,0,0,0,2,2,0,1,2,2,0,0,0,0,1,0,0,0,0,0,0,0,2,2,0,2,2,0,0,0,1,0,2,1,
                    1,2,1,2,1,1,1,1,1,0,1,2,1,0,1,1,1,0,1,2,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,2,1,0,1,1,1,0,1,2,1,
                    1,0,0,0,1,0,0,0,2,2,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,2,2,2,0,1,0,0,0,0,0,1,
                    1,2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,0,1,1,2,1,
                    1,2,2,0,2,2,1,2,2,0,2,2,0,2,2,0,2,2,0,2,2,0,2,2,1,2,2,0,2,2,0,2,2,0,2,2,0,2,1,0,2,2,0,2,1,
                    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    int x=3, y=1, xBomba, yBomba;  e y da bomba 
    int contBomba = 0;
    int xInimigo[3] = {10, 4, 3}; 
    int yInimigo[3] = {10, 20, 42};
    unsigned long tempoAtual = GetTickCount(); 
    unsigned long tempoUltimoMovimentoInimigos = 0; 
    const unsigned long intervaloMovimentoInimigos = 1000;
    unsigned long tempoBomba = 0; 
    char tecla; 

    /// Laço principal do jogo
    while(true){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);c 
        tempoAtual = GetTickCount(); 
        
        /// Movimentação do inimigos
        if (tempoAtual - tempoUltimoMovimentoInimigos >= intervaloMovimentoInimigos) { // o tempo atual será menor que o ultimo movimento no caso 0 quando isso for maior que 1000 ele executa o if
            for(int k = 0; k < 3; k++){ 
                int direcao = rand() % 4; 
                int novoX = xInimigo[k]; 
                int novoY = yInimigo[k]; 

                for (int passos = 0; passos < 3; passos++) {
                    int proximoX = novoX; 
                    int proximoY = novoY;

                    // defini a direção dos movimentos
                    switch(direcao){ 
                        case 0: proximoX--; break; 
                        case 1: proximoX++; break;
                        case 2: proximoY--; break;
                        case 3: proximoY++; break;
                    }

                    if (proximoX >= 0 && proximoX < 15 && proximoY >= 0 && proximoY < 45 && m[proximoX][proximoY] == 0) {//se proximox for maior que 0 e menor que 15 e proximoy maior que 0 e menor que 45 e a posição de proximo x e y igual a 0 ele executa o if
                        novoX = proximoX; 
                        novoY = proximoY;
                    } else {
                        break;
                    }
                }

                xInimigo[k] = novoX; 
                yInimigo[k] = novoY; 
            }
            tempoUltimoMovimentoInimigos = tempoAtual; 
        }

        /// Desenha o mapa
        for(int i=0;i<15;i++){
            for(int j=0;j<45;j++){ 
                bool desenhouInimigo = false;
                for (int k = 0; k < 3; k++) { 
                    if (xInimigo[k] != -1 && yInimigo[k] != -1 && i == xInimigo[k] && j == yInimigo[k]) { 
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY); 
                        cout << char(60); 
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        desenhouInimigo = true;
                        break;
                    }
                }
                if(!desenhouInimigo){
                    if(i==x && j==y){ 
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); 
                        cout << char(36); 
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); 
                    } else { 
                        switch (m[i][j]){ 
                            case 0: 
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY); 
                                cout << " "; 
                                break;
                            case 1: 
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_INTENSITY); 
                                cout << char(219); 
                                break;
                            case 2: 
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_INTENSITY);
                                cout << char(178);
                                break;
                            case 3: 
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY);
                                cout << char(42);
                                break;
                            case 4:
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                                cout << char(64); 
                                break;
                        }
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    }
                }
            }
            cout<<"\n";
        }

        /// movimento
        if ( _kbhit() ){
            tecla = getch();
            switch(tecla){
                case 72: case 'w': 
                    if(m[x-1][y] == 0 || m[x-1][y] == 4){ 
                        x--; 
                    }
                    break;
                case 80: case 's': 
                    if(m[x+1][y] == 0 || m[x+1][y] == 4){ 
                       x++; 
                    }
                    break;
                case 75: case 'a':
                    if(m[x][y-1] == 0 || m[x][y-1] == 4){
                        y--;
                    }
                    break;
                case 77: case 'd': 
                    if(m[x][y+1] == 0 || m[x][y+1] == 4){
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

        if(contBomba == 1 && GetTickCount() - tempoBomba >= 1500){ 
            explosaoBomba(m, xBomba, yBomba, &contBomba); 
            tempoBomba = GetTickCount();
        }

        if(contBomba == 2 && GetTickCount() - tempoBomba >= 1700){ 
            for(int i=0;i<15;i++){ 
                for(int j=0;j<45;j++){
                    if(m[i][j] == 4){ 
                        m[i][j] = 0; 
                    }
                }
            }
            contBomba = 0; 
        }

        
        for (int k = 0; k < 3; k++) { 
            if (xInimigo[k] != -1 && yInimigo[k] != -1) { 
                if (m[xInimigo[k]][yInimigo[k]] == 4) { 
                    xInimigo[k] = -1; 
                    yInimigo[k] = -1;
                }
            }
        }

        bool encostouInimigo = false; 
        for (int k = 0; k < 3; k++) { 
            if (xInimigo[k] != -1 && yInimigo[k] != -1) { 
                if (x == xInimigo[k] && y == yInimigo[k]) { 
                    encostouInimigo = true;  
                    break;
                }
            }
        }
        
        bool venceu = true; 
        for (int k = 0; k < 3; k++) { 
            if (xInimigo[k] != -1 && yInimigo[k] != -1) { 
                venceu = false; 
                break;
            }
        }

        if (venceu) { 
            for(int i = 0; i < 15; i++){ 
                for(int j = 0; j < 45; j++){ 
                    m[i][j] = 0; 
                }
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            for(int i = 0; i < 15; i++){ 
                for(int j = 0; j < 45; j++){
                    cout << " "; 
                }
                cout << "\n"; 
            }
            COORD topo; 
            topo.X = 15; 
            topo.Y = 2;  
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), topo);
            cout << "\n\nVOCE VENCEU!\n"; 
            Sleep(3000); 
            break;
        }
        if (m[x][y] == 4 || encostouInimigo) { 
            for(int i = 0; i < 15; i++){ 
                for(int j = 0; j < 45; j++){ 
                    m[i][j] = 0;  
                }
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); 
            for(int i = 0; i < 15; i++){ 
                for(int j = 0; j < 45; j++){
                    cout << " "; 
                }
                cout << "\n"; 
            }
            COORD topo; 
            topo.X = 15;
            topo.Y = 2;+
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), topo);// não entendi
            cout << "VOCE MORREU!"; 
            Sleep(3000); 
            break;
        }
    }
    return 0;
}
