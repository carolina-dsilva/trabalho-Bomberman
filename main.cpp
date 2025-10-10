#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

/// Função da cor de fundo
void setColor(int text, int background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, background * 16 + text);
}
/// Função desenho do mapa
bool carregarMapa(const string& caminho, int m[15][45]) {
    ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo: " << caminho << endl;
        return false;
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 45; j++) {
            if (!(arquivo >> m[i][j])) { // lê o próximo número
                cout << "Erro na leitura do arquivo!" << endl;
                arquivo.close();
                return false;
            }
        }
    }
    arquivo.close();
    return true;
}

void desenhoMapa(int m[15][45], int x, int y, int xInimigo[], int yInimigo[]){
    COORD origin = {0,0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), origin);

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
                        case 5:
                        case 6:
                        case 7:
                        case 8:
                        case 9:
                        case 10:
                        case 11:
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                            cout << char(158);
                            break;
                    }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
            }
        }
        cout<<"\n";
    }
}
///Função para apagar o mapa
void apagaMapa(int m[15][45]){
    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 45; j++){
            m[i][j] = 0;
        }
    }
}
///Função reinicia o mapa
void reiniciaMapa(int m[15][45]){
    if (!carregarMapa("fases/primeiraFase.txt", m)) {
        cout << "Erro ao reiniciar o mapa!" << endl;
    }
}

///Função Movimento
void movimento(int m[15][45], int &x, int &y, int &contBomba, int &xBomba, int &yBomba, unsigned long &tempoBomba, char &tecla ){
    if ( _kbhit() ){
        tecla = getch();
        switch((unsigned char)tecla){
            case 72: case 'w':
                if (x-1 >= 0 && (m[x-1][y] == 0 || m[x-1][y] > 3)){
                    x--;
                }
                break;
            case 80: case 's':
                if (x+1 < 15 && (m[x+1][y] == 0 || m[x+1][y] > 3)){
                    x++;
                }
                break;
            case 75: case 'a':
                if (y-1 >= 0 && (m[x][y-1] == 0 || m[x][y-1] > 3)){
                    y--;
                }
                break;
            case 77: case 'd':
                if (y+1 < 45 && (m[x][y+1] == 0 || m[x][y+1] > 3)){
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
}

//void coletarItem(int m[15][45], int x, int y, int &itemColetado, int &vidas){
  //  for(int i = 0; i < 15; i++){
    //    for(int j = 0; j < 45; j++){
      //      if(m[i][j] == 5){
        //        m[i][j] = 4;
          //      item = 5;
           // }
    //    }
  //  }
//}
/// Função movimento Inimigos
void movimentoInimigos(int m[15][45], int xInimigo[], int yInimigo[]){
    for(int k = 0; k < 3; k++){
        int direcao = rand() % 4;
        int novoX = xInimigo[k];
        int novoY = yInimigo[k];

        for (int passos = 0; passos < 3; passos++) {
            int proximoX = novoX;
            int proximoY = novoY;

            // define a direção dos movimentos
            switch(direcao){
                case 0: proximoX--; break;
                case 1: proximoX++; break;
                case 2: proximoY--; break;
                case 3: proximoY++; break;
            }

            if (proximoX >= 0 && proximoX < 15 && proximoY >= 0 && proximoY < 45 && m[proximoX][proximoY] == 0) {
                novoX = proximoX;
                novoY = proximoY;
            } else {
                break;
            }
        }

        xInimigo[k] = novoX;
        yInimigo[k] = novoY;
    }
}
/// Função de explosão da bomba
void explosaoBomba(int m[15][45], int xBomba, int yBomba, int &contBomba){
    if (m[xBomba][yBomba] == 3){
        m[xBomba][yBomba] = 4;
    }

    int dx[] = {-1, 1,  0, 0};
    int dy[] = { 0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int x = xBomba;
        int y = yBomba;

        for (int passo = 1; passo <= 3; passo++) {
            x += dx[i];
            y += dy[i];
            if (x < 0 || x >= 15 || y < 0 || y >= 45){
                break;
            }
            if (m[x][y] == 1){
                break;
            }
            if(m[x][y] == 2 && x == 5 && y == 17){
                m[x][y] = 5;
                break;
            }else  if(m[x][y] == 2 && x == 12 && y == 1){
                m[x][y] = 6;
                break;
            }else if(m[x][y] == 2 && x == 9 && y == 16){
                m[x][y] = 7;
                break;
            }else if(m[x][y] == 2 && x == 13 && y == 27){
                m[x][y] = 8;
                break;
            }else if(m[x][y] == 2 && x == 3 && y == 20){
                m[x][y] = 9;
                break;
            }else if(m[x][y] == 2 && x == 6 && y == 36){
                m[x][y] = 10;
                break;
            }else if(m[x][y] == 2 && x == 1 && y == 32){
                m[x][y] = 11;
                break;
            }
            m[x][y] = 4;
        }
    }
    contBomba = 2;
}
///Função atualiza a bomba
void atualizarBomba(int m[15][45], int &contBomba, unsigned long &tempoBomba, int xBomba, int yBomba) {
    if (contBomba == 0){
        return;
    }
    unsigned long tempoAtual = GetTickCount();
    if (contBomba == 1 && tempoAtual - tempoBomba >= 1500) {
        explosaoBomba(m, xBomba, yBomba, contBomba);
        tempoBomba = tempoAtual;
    } else if (contBomba == 2 && tempoAtual - tempoBomba >= 1700) {
        for(int i = 0; i < 15; i++){
            for(int j = 0; j < 45; j++){
                if (m[i][j] == 4){
                    m[i][j] = 0;
                }
            }
        }
        contBomba = 0;
    }
}
void limparTela() {
    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int main()
{
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
    short int CX=0, CY=0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.

    int m[15][45];
    if (!carregarMapa("fases/primeiraFase.txt", m)) {
        cout << "Nao foi possivel carregar o mapa. Saindo..." << endl;
        return 1;
    }

    int x = 3, y = 1, xBomba = 0, yBomba = 0;
    int contBomba = 0;
    int xInimigo[3] = {10, 4, 3};
    int yInimigo[3] = {10, 20, 42};
    int vidas = 3;
    int itemColetado = 0;

    unsigned long tempoAtual = GetTickCount();
    unsigned long tempoUltimoMovimentoInimigos = 0;
    const unsigned long intervaloMovimentoInimigos = 1000;
    unsigned long tempoBomba = 0;
    char tecla;
    bool jogo = true;

    /// Laço principal do jogo
    while(jogo){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        tempoAtual = GetTickCount();

        if (tempoAtual - tempoUltimoMovimentoInimigos >= intervaloMovimentoInimigos) {
            movimentoInimigos(m, xInimigo, yInimigo);
            tempoUltimoMovimentoInimigos = tempoAtual;
        }

        desenhoMapa(m, x, y, xInimigo, yInimigo);
        movimento(m, x, y, contBomba, xBomba, yBomba, tempoBomba, tecla);
        //coletarItem(m, x, y, itemColetado, vidas, velocidade);
        atualizarBomba(m, contBomba, tempoBomba, xBomba, yBomba);


        bool encostouInimigo = false;
        for (int k = 0; k < 3; k++) {
            if (xInimigo[k] != -1 && yInimigo[k] != -1) {
                if (x == xInimigo[k] && y == yInimigo[k]) {
                    encostouInimigo = true;
                    break;
                }
            }
        }
        if (m[x][y] == 4 || encostouInimigo) {
            if(vidas > 0){
                vidas -= 1;
                x = 3;
                y = 1;
            }
        }

        if (vidas == 0){
            int continuar;
            cout << "Voce morreu! Quer continuar? (1 = Sim / 0 = Nao):";
            cin >> continuar;

            if (continuar == 0){
                jogo = false;
            }else{
                vidas = 3;
                x = 3; y = 1;
                xInimigo[0] = 10; yInimigo[0] = 10;
                xInimigo[1] = 4; yInimigo[1] = 20;
                xInimigo[2] = 3; yInimigo[2] = 42;

                reiniciaMapa(m);
                //limparTela();
            }
        }

        for (int k = 0; k < 3; k++) {
            if (xInimigo[k] != -1 && yInimigo[k] != -1) {
                if (m[xInimigo[k]][yInimigo[k]] == 4) {
                    xInimigo[k] = -1;
                    yInimigo[k] = -1;
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
            apagaMapa(m);
            desenhoMapa(m, -1, -1, xInimigo, yInimigo);

            COORD topo;
            topo.X = 15;
            topo.Y = 2;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), topo);
            cout << "\n\nVOCE VENCEU!\n";
            Sleep(3000);
            break;
        }
    }
    return 0;
}