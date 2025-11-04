#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <map>
using namespace std;

//Variáveis globais para os itens bônus
const int ITEM_FIRE_UP      = 9;
const int ITEM_EXTRA_BOMB   = 10;
const int ITEM_EXTRA_LIFE   = 11;
int fireLevelGlobal = 1;
int bonusExtraBombJ1 = 0, bonusExtraBombJ2 = 0; //quantidade de bomba extra ativo para cada jogador
int bonusExtraLifeJ1 = 0, bonusExtraLifeJ2 = 0; //quantidade de vida extra ativo para cada jogador
// Temporizadores para controlar a duração do bônus de bomba extra, unsigned long para armazenar milissegundos
unsigned long tempoExtraBombJ1 = 0;
unsigned long tempoExtraBombJ2 = 0;

struct Jogador {
    string nome;
    int pontuacao = 0;
    int bombas = 0;
    int movimentos = 0;
};
void setColor(int text, int background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, background * 16 + text);
}
void apagaMapa(int m[15][45]){
    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 45; j++){
            m[i][j] = 0;
        }
    }
}
///Função para armazenar os jogadores
void salvarJogador(const Jogador &jogador1, const Jogador &jogador2, int numJogadores) {
    ofstream arquivo("salvarDados.txt", ios::app); // cria ofstream chamado arquivo, para escrever no arquivo chamado salvarDados.txt
    if (!arquivo) {
        cout << "Erro ao abrir arquivo!" << endl;
        return;
    }
    //Recebe a data e hora atual
    time_t agora = time(0);// retorna tempo atual em segundos
    tm *ltm = localtime(&agora);// converte o tempo para a data e hora local
    //armazenar a data e hora
    char dataStr[20];
    sprintf(dataStr, "%02d/%02d/%04d %02d:%02d:%02d",
            ltm->tm_mday, ltm->tm_mon + 1, 1900 + ltm->tm_year,
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

    // Salva os dados do Jogador 1
    arquivo << "Data: " << dataStr // recebe do char dataStr
            << " | Jogador: " << jogador1.nome // recebe do struct Jogador
            << " | Pontuacao: " << jogador1.pontuacao
            << " | Bombas: " << jogador1.bombas
            << " | Movimentos: " << jogador1.movimentos
            << "\n";
    // Se tiver jogador 2, salva seus dados também
    if(numJogadores == 2) {
        arquivo << "Data: " << dataStr
                << " | Jogador: " << jogador2.nome
                << " | Pontuacao: " << jogador2.pontuacao
                << " | Bombas: " << jogador2.bombas
                << " | Movimentos: " << jogador2.movimentos
                << "\n";
    }
    arquivo.close();
}

void mostrarRanking() {
    system("cls");
    ifstream arquivo("salvarDados.txt"); //Cria um objeto ifstream para ler o arquivo
    if (!arquivo) {
        cout << "Nenhum dado salvo ainda!\n";
        _getch();
        return;
    }
    map<string, int> rankingMap; //relaciona jogador - maior pontuação, caso tenha vários registros do mesmo jogador sera considerado somente a maior pontuação
    string linha;
    while (getline(arquivo, linha)) {
        size_t posNome = linha.find("Jogador:");  //procura nas linhas Jogador:
        size_t posPont = linha.find("Pontuacao:");//procura nas linhas Pontuacao:
        //Verifica se ambos os termos foram encontrados na linha.
        if (posNome != string::npos && posPont != string::npos) {
            string nome = linha.substr(posNome + 9, linha.find("|", posNome) - (posNome + 9));
            int pontos = stoi(linha.substr(posPont + 11, linha.find("|", posPont) - (posPont + 11)));
            //verifica se o jogador já existe, se não adiciona, se sim atualiza a pontuação
            if (rankingMap.find(nome) == rankingMap.end() || pontos > rankingMap[nome]) {
                rankingMap[nome] = pontos;
            }
        }
    }
    arquivo.close();
    if (rankingMap.empty()) {
        cout << "Nenhum registro encontrado!\n";
        _getch();
        return;
    }

    // Transforma map em vetor para ordenar
    vector<pair<string,int>> ranking(rankingMap.begin(), rankingMap.end());
    //ordena o vetor ranking de forma decrescente
    sort(ranking.begin(), ranking.end(),
         [](auto &a, auto &b) { return a.second > b.second; });

    cout << "===== RANKING =====\n\n";
    for (int i = 0; i < ranking.size() && i < 10; i++) {
        cout << i + 1 << "� - " << ranking[i].first
             << " | Pontos: " << ranking[i].second << "\n";
    }
    cout << "\nPressione qualquer tecla para voltar...";
    _getch();
}

void desenhoMapa(int m[15][45],  int x1, int y1, int x2, int y2, int numJogadores, int xInimigo[], int yInimigo[], int numInimigos,
                 int vidas1, int vidas2, int pontuacao1, int pontuacao2, int mov1, int mov2, int bombas1, int bombas2, bool bonusAtivo, unsigned long tempoBonus) {
    COORD origin = {0,0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), origin);
    //Informações do jogador 1
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Branco
    cout << "VIDAS J1:" << vidas1  << " PONTOS J1:" << pontuacao1
         << " BOMBAS J1:" << bombas1 << " MOVIMENTO J1:" << mov1;
    //informações caso tenha o jogador 2
    if(numJogadores==2) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Laranja
        cout << "\nVIDAS J2:" << vidas2 << " PONTOS J2:" << pontuacao2
             << " BOMBAS J2:" << bombas2  << " MOVIMENTO J2:" << mov2;
    }
    cout << "\n";
    // Desenho Mapa
    for(int i=0;i<15;i++){
        for(int j=0;j<45;j++){
            bool desenhouInimigo = false; // garantir que não tenha desenhado outro elemento no lugar do inimigo.
            // Inimigos
            for (int k = 0; k < numInimigos; k++) {
                if (xInimigo[k] != -1 && yInimigo[k] != -1 && i == xInimigo[k] && j == yInimigo[k]) { //Verifica se a posição (i,j) tem algum inimigo.
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    cout << char(60);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    desenhouInimigo = true;
                    break;
                }
            }
            if(!desenhouInimigo){ //Se não desenhou inimigo
                if(x1>=0 && i==x1 && j==y1){ // verifica se é o jogador 1.
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    cout << char(36); // $
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
                //verifica se é o jogador 2.
                else if(numJogadores==2 && x2>=0 && i==x2 && j==y2){
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Laranja
                    cout << char(36); // $
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // padr�o
                }
                // se a posição for tiver uma bomba (*)
                else if(m[i][j]==7 && !( (i==x1 && j==y1) || (numJogadores==2 && i==x2 && j==y2) )) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_RED | FOREGROUND_INTENSITY); // Vermelho
                    cout << "*";
                }
                // se a posição for explosão (8)
                else if(m[i][j]==8 && !( (i==x1 && j==y1) || (numJogadores==2 && i==x2 && j==y2) )) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_RED | FOREGROUND_INTENSITY); // Vermelho
                    cout << "@";
                }//verificam os itens
                else if(m[i][j]==ITEM_FIRE_UP){
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_RED | FOREGROUND_INTENSITY); // vermelho
                    cout << "F"; // Fire Up
                }
                else if(m[i][j]==ITEM_EXTRA_BOMB){
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_GREEN | FOREGROUND_INTENSITY); // verde
                    cout << "B"; // Extra Bomb
                }
                else if(m[i][j]==ITEM_EXTRA_LIFE){
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_BLUE | FOREGROUND_INTENSITY); // azul
                    cout << "V"; // Extra Life
                }
                else { // os outros elementos do mapa
                    switch (m[i][j]){
                        case 0:
                            cout << " ";
                            break;
                        case 1:
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Verde forte
                            cout << char(219);
                            break;
                        case 2:
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                FOREGROUND_GREEN); // Verde claro
                            cout << char(178);
                            break;
                        case 3:
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                FOREGROUND_RED | FOREGROUND_INTENSITY);
                            cout << char(42);
                            break;
                        case 4:
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_RED | FOREGROUND_INTENSITY);
                            cout << char(64);
                            break;
                        case 5:
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Laranja/Amarelo
                            cout << char(206);
                            break;
                        default:
                            cout << " ";
                    }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // padr�o
                }
            }
        }
        cout << "\n";
    }
}

void movimentoMultiplayer( int m[15][45], int &x1, int &y1, int &contBomba1, int &xBomba1, int &yBomba1, unsigned long &tempoBomba1, int &contBomba1b, int &xBomba1b, int &yBomba1b, unsigned long &tempoBomba1b,
    int &x2, int &y2, int &contBomba2, int &xBomba2, int &yBomba2, unsigned long &tempoBomba2, int &contBomba2b, int &xBomba2b, int &yBomba2b, unsigned long &tempoBomba2b, Jogador &j1, Jogador &j2, int numJogadores, char &tecla){

    if (_kbhit()){
        tecla = getch();
        if (tecla == -32 || tecla == 224) {
            char setinha = getch();
            //teclas jogador 1
            switch(setinha){
                case 72: // cima
                    if (x1-1>=0 && (m[x1-1][y1]==0 || m[x1-1][y1]>3)) { x1--; j1.movimentos++; }
                    break;
                case 80: // baixo
                    if (x1+1<15 && (m[x1+1][y1]==0 || m[x1+1][y1]>3)) { x1++; j1.movimentos++; }
                    break;
                case 75: // esquerda
                    if (y1-1>=0 && (m[x1][y1-1]==0 || m[x1][y1-1]>3)) { y1--; j1.movimentos++; }
                    break;
                case 77: // direita
                    if (y1+1<45 && (m[x1][y1+1]==0 || m[x1][y1+1]>3)) { y1++; j1.movimentos++; }
                    break;
            }
        }else {
            //teclas jogador 2
            switch(tecla){
                case 'w': case 'W':
                    if(numJogadores==2 && x2-1>=0 && (m[x2-1][y2]==0 || m[x2-1][y2]>3)) { x2--; j2.movimentos++; }
                    break;
                case 's': case 'S':
                    if(numJogadores==2 && x2+1<15 && (m[x2+1][y2]==0 || m[x2+1][y2]>3)) { x2++; j2.movimentos++; }
                    break;
                case 'a': case 'A':
                    if(numJogadores==2 && y2-1>=0 && (m[x2][y2-1]==0 || m[x2][y2-1]>3)) { y2--; j2.movimentos++; }
                    break;
                case 'd': case 'D':
                    if(numJogadores==2 && y2+1<45 && (m[x2][y2+1]==0 || m[x2][y2+1]>3)) { y2++; j2.movimentos++; }
                    break;
                case 32: // espaço bomba jogador 1
                    if (bonusExtraBombJ1 ? ( (contBomba1==0) || (contBomba1b==0) ) : (contBomba1==0) ) { // Se o jogador tem bônus de bomba extra, pode colocar uma segunda bomba
                        if (contBomba1 == 0) {
                            if (m[x1][y1] == 0 || m[x1][y1] > 3) {
                                m[x1][y1] = 3;
                                contBomba1 = 1;
                                xBomba1 = x1; yBomba1 = y1;
                                tempoBomba1 = GetTickCount();
                                j1.bombas++;
                            }
                        }else if (bonusExtraBombJ1 && contBomba1b == 0) {
                            if (m[x1][y1] == 0 || m[x1][y1] > 3) {
                                m[x1][y1] = 3;
                                contBomba1b = 1;
                                xBomba1b = x1; yBomba1b = y1;
                                tempoBomba1b = GetTickCount();
                                j1.bombas++;
                            }
                        }
                    }
                    break;
                case 'f': case 'F': // bomba do jogador 2
                    if (numJogadores == 2) {
                        if (bonusExtraBombJ2 ? ( (contBomba2==0) || (contBomba2b==0) ) : (contBomba2==0) ) {
                            if (contBomba2 == 0) {
                                if (m[x2][y2] == 0 || m[x2][y2] > 3) {
                                    m[x2][y2] = 3;
                                    contBomba2 = 1;
                                    xBomba2 = x2; yBomba2 = y2;
                                    tempoBomba2 = GetTickCount();
                                    j2.bombas++;
                                }
                            } else if (bonusExtraBombJ2 && contBomba2b == 0) {
                                if (m[x2][y2] == 0 || m[x2][y2] > 3) {
                                    m[x2][y2] = 3;
                                    contBomba2b = 1;
                                    xBomba2b = x2; yBomba2b = y2;
                                    tempoBomba2b = GetTickCount();
                                    j2.bombas++;
                                }
                            }
                        }
                    }
                    break;
            }
        }
    }
}
void movimentoInimigos(int m[15][45], int xInimigo[], int yInimigo[], int numInimigos){
    for(int k=0;k<numInimigos;k++){
        if(xInimigo[k]==-1) continue; // Se a posição x do inimigo é -1, significa que está "morto"
        int direcao=rand()%4; int novoX=xInimigo[k], novoY=yInimigo[k];//escolhe uma direção aleatoria
        int dx[]={-1,1,0,0}; int dy[]={0,0,-1,1};
        for(int passo=0;passo<3;passo++){ // Permite que o inimigo se mova até 3 passos na direção escolhida
            int px=novoX+dx[direcao], py=novoY+dy[direcao]; //calcula nova posição
            //verifica se a nova posição é valida
            if(px>=0 && px<15 && py>=0 && py<45 && (m[px][py]==0 || m[px][py]==5 || m[px][py]==6)){
                novoX=px; novoY=py;
            } else break;
        }
        xInimigo[k]=novoX; yInimigo[k]=novoY;// Atualiza a posição final do inimigo na matriz
    }
}
void explosaoBomba(int m[15][45], int xBomba, int yBomba, int &contBomba, int xPortal, int yPortal){
    if(!(xBomba==xPortal && yBomba==yPortal) && m[xBomba][yBomba]==3) m[xBomba][yBomba]=4;// Se a bomba não está sobre o portal e a célula é uma bomba ativa (3),transforma a célula em explosão (4)
    int dx[]={-1,1,0,0}, dy[]={0,0,-1,1};

    for(int i=0;i<4;i++){
        int xx=xBomba, yy=yBomba;
        for(int passo=1;passo<=fireLevelGlobal;passo++){
            xx += dx[i]; yy += dy[i]; // Avança a explosão passo a passo na direção i
            if(xx<0 || xx>=15 || yy<0 || yy>=45) break;//se sair dos limites do mapa
            if(m[xx][yy]==1) break; // parede solida
            if(xx==xPortal && yy==yPortal) continue;// não destroi portal

            // Se for bloco quebravel
            if(m[xx][yy] == 2){
                int chance = rand() % 100; //sorteia para gerar itens
                if(chance < 15) m[xx][yy] = ITEM_FIRE_UP;
                else if(chance < 30) m[xx][yy] = ITEM_EXTRA_BOMB;
                else if(chance < 45) m[xx][yy] = ITEM_EXTRA_LIFE;
                else m[xx][yy] = 0; // apenas limpa o bloco sem bonus
                break;
            }
            m[xx][yy] = 4;
        }
    }
    contBomba=2;
}
void atualizarBomba(int m[15][45], int &contBomba, unsigned long &tempoBomba, unsigned long &tempoExplosao, int xBomba, int yBomba, int xPortal, int yPortal){
    if (contBomba == 0) return;

    unsigned long tempoAtual = GetTickCount();

    if (contBomba == 1 && tempoAtual - tempoBomba >= 1500) {
        explosaoBomba(m, xBomba, yBomba, contBomba, xPortal, yPortal);
        tempoExplosao = tempoAtual; // come�a a contar tempo da explos�o agora
    }
    else if (contBomba == 2 && tempoAtual - tempoExplosao >= 700) { // reduzido para 700ms ap�s explos�o
        for (int i = 0; i < 15; i++)
            for (int j = 0; j < 45; j++)
                if (m[i][j] == 4)
                    m[i][j] = 0;

        if (xPortal >= 0 && yPortal >= 0)
            m[xPortal][yPortal] = 5;
        contBomba = 0;
    }
}
void atualizarBombaInimigo(int m[15][45], int &contBomba, unsigned long &tempoBomba, unsigned long &tempoExplosao, int &xBomba, int &yBomba){
    if(contBomba==0) return;
    unsigned long tempoAtual=GetTickCount();

    if(contBomba==1 && tempoAtual-tempoBomba>=1500){
        if(m[xBomba][yBomba]==7) m[xBomba][yBomba]=8; //explode
        int dx[]={-1,1,0,0}, dy[]={0,0,-1,1};
        for(int i=0;i<4;i++){
            int xx=xBomba, yy=yBomba;
            for(int passo=1;passo<=fireLevelGlobal;passo++){
                xx+=dx[i]; yy+=dy[i];
                if(xx<0 || xx>=15 || yy<0 || yy>=45) break;
                if(m[xx][yy]==1) break;
                if(m[xx][yy]==5 || m[xx][yy]==6) continue;
                m[xx][yy]=8;
            }
        }
        contBomba=2;
        tempoExplosao=tempoAtual;
    } else if(contBomba==2 && tempoAtual-tempoExplosao>=700){
        for(int i=0;i<15;i++)
            for(int j=0;j<45;j++)
                if(m[i][j]==8) m[i][j]=0;
        contBomba=0; xBomba=-1; yBomba=-1;
    }
}
void geraMapa(int m[15][45], int xJogador1, int yJogador1, int xJogador2, int yJogador2, int xInimigo[], int yInimigo[], int &xPortal, int &yPortal, int numInimigos, int dificuldade, int numJogadores) {
    int chanceSolida = 15;
    int chanceQuebravel = 15;

    if (dificuldade == 2) { // nivel medio
        chanceSolida = 30;
        chanceQuebravel = 35;
    } else if (dificuldade == 3) { // nivel dificil
        chanceSolida = 30;
        chanceQuebravel = 45;
    }

    auto eProximoAoJogador = [&](int i, int j)->bool{ //verificar se a célula (i,j) está próxima de algum jogador
        // verifica proximidade ao jogador1
        if(i==xJogador1 && j==yJogador1) return true;
        if(i==xJogador1-1 && j==yJogador1) return true;
        if(i==xJogador1+1 && j==yJogador1) return true;
        if(i==xJogador1 && j==yJogador1-1) return true;
        if(i==xJogador1 && j==yJogador1+1) return true;
        if(numJogadores==2){// verifica proximidade ao jogador2
            if(i==xJogador2 && j==yJogador2) return true;
            if(i==xJogador2-1 && j==yJogador2) return true;
            if(i==xJogador2+1 && j==yJogador2) return true;
            if(i==xJogador2 && j==yJogador2-1) return true;
            if(i==xJogador2 && j==yJogador2+1) return true;
        }
        return false;
    };

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 45; j++) {
            if (i == 0 || i == 14 || j == 0 || j == 44) {
                m[i][j] = 1; // borda
            }
            else if (eProximoAoJogador(i,j)) {
                m[i][j] = 0; // espaço ao redor dos jogadores
            }
            else if (rand() % 100 < chanceSolida)
                m[i][j] = 1; // parede solida
            else if (rand() % 100 < chanceQuebravel)
                m[i][j] = 2; // parede quebravel
            else
                m[i][j] = 0;
        }
    }
    // Gera portal
    do {
        xPortal = rand() % 13 + 1;
        yPortal = rand() % 43 + 1;
    } while (m[xPortal][yPortal] != 0);
    m[xPortal][yPortal] = 5;

    // Posiciona inimigos
    for (int k = 0; k < numInimigos; k++) {
        int xi, yi;
        do {
            xi = rand() % 13 + 1;
            yi = rand() % 43 + 1;
        } while (m[xi][yi] != 0);
        xInimigo[k] = xi;
        yInimigo[k] = yi;
    }
}

void gerarItemBonus(int m[15][45]) {
    static int bonus[15][45] = {0}; // permanece na memória mesmo após sair da função

    vector<int> itens = {ITEM_FIRE_UP, ITEM_EXTRA_BOMB, ITEM_EXTRA_LIFE};// Vetor com os tipos de itens que serão gerados

    for (int item : itens) {
        int x, y;
        do {
            x = rand() % 15;
            y = rand() % 45;
        } while (m[x][y] != 2 || bonus[x][y] != 0);// Sorteia coordenadas aleatórias até encontrar um bloco quebrável (2) e que ainda não tenha item atribuído
        bonus[x][y] = item;// recebe um item nessa posição
    }
}
char telaGameOver(){
    system("cls");
    cout<<"\n\n============== GAME OVER ==============\n";
    cout<<"M - Voltar ao Menu\nS - Sair\n";
    while(true){
        char c=_getch();
        if(c=='M'||c=='m') return 'M';
        if(c=='S'||c=='s') return 'S';
    }
}

void iniciarJogo(int dificuldade, Jogador &jogador1, Jogador &jogador2, int numJogadores) {
    /// TRECHO DE CONFIGURAÇÃO DE CURSOR: NÃO MODIFICAR
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
    short int CX = 0, CY = 0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;
    /// FIM DO TRECHO DE CURSOR

    // Tabela de inimigos por fase e dificuldade
    int inimigosPorFase[3][3] = {
        {2, 3, 3},   // Fácil
        {3, 4, 5},   // Médio
        {5, 6, 7}    // Difícil
    };

    // Configuração de inimigos inteligentes (bombas automáticas)
    const int MAX_INIMIGOS_INTELIGENTES = 2;
    int xBombaInimigo[2] = {-1, -1};        // posição X da bomba inimiga
    int yBombaInimigo[2] = {-1, -1};        // posição Y da bomba inimiga
    int contBombaInimigo[2] = {0, 0};       // contador de bombas ativas
    unsigned long tempoBombaInimigo[2] = {0, 0};   // tempo em que bomba foi colocada
    unsigned long tempoExplosaoInimigo[2] = {0, 0}; // tempo da explosão

    // Vidas iniciais dos jogadores
    int vidas1 = 3;
    int vidas2 = 3;

    // RESET DE BÔNUS GLOBAIS
    bonusExtraBombJ1 = bonusExtraBombJ2 = 0;
    bonusExtraLifeJ1 = bonusExtraLifeJ2 = 0;
    fireLevelGlobal = 1;

    // Loop pelas fases do jogo (1 a 3)
    for (int fase = 1; fase <= 3; fase++) {
        // Reset de bônus e nível de fogo a cada fase
        bonusExtraBombJ1 = bonusExtraBombJ2 = 0;
        bonusExtraLifeJ1 = bonusExtraLifeJ2 = 0;
        fireLevelGlobal = 1;

        // Define número de inimigos para a fase atual e dificuldade
        int numInimigos = inimigosPorFase[dificuldade - 1][fase - 1];

        // Matriz do mapa
        int m[15][45];
        apagaMapa(m); // limpa o mapa

        // Posicionamento inicial dos jogadores
        int baseX1 = 3, baseY1 = 1;
        int baseX2 = baseX1 + 1, baseY2 = baseY1;

        int x1 = (vidas1 > 0) ? baseX1 : -1;
        int y1 = (vidas1 > 0) ? baseY1 : -1;
        int x2 = (numJogadores == 1 || vidas2 <= 0) ? -1 : baseX2;
        int y2 = (numJogadores == 1 || vidas2 <= 0) ? -1 : baseY2;

        // Bombas do jogador 1
        int xBomba1 = -1, yBomba1 = -1, contBomba1 = 0;
        unsigned long tempoBomba1 = 0, tempoExplosao1 = 0;
        int xBomba1b = -1, yBomba1b = -1, contBomba1b = 0;
        unsigned long tempoBomba1b = 0, tempoExplosao1b = 0;

        // Bombas do jogador 2
        int xBomba2 = -1, yBomba2 = -1, contBomba2 = 0;
        unsigned long tempoBomba2 = 0, tempoExplosao2 = 0;
        int xBomba2b = -1, yBomba2b = -1, contBomba2b = 0;
        unsigned long tempoBomba2b = 0, tempoExplosao2b = 0;

        unsigned long tempoUltMovInim = 0; // para controlar movimento inimigos

        // Vetor de inimigos
        int xInimigo[10], yInimigo[10];
        for (int i = 0; i < 10; i++) xInimigo[i] = yInimigo[i] = -1;

        // Portal
        int xPortal = -1, yPortal = -1;

        // Bônus e multiplicador
        bool bonusAtivo = false;
        unsigned long tempoBonus = 0;
        int multiplicadorDePontos = 1;

        // Gera mapa e itens bônus
        geraMapa(m, x1, y1, x2, y2, xInimigo, yInimigo, xPortal, yPortal, numInimigos, dificuldade, numJogadores);
        gerarItemBonus(m);

        bool faseAtiva = true;
        while (faseAtiva) {
            unsigned long tempoAtual = GetTickCount();

            // Move cursor para o início do mapa
            COORD coord = {0, 0};
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

            // Movimento dos inimigos a cada 1 segundo
            if (tempoAtual - tempoUltMovInim >= 1000) {
                movimentoInimigos(m, xInimigo, yInimigo, numInimigos);
                tempoUltMovInim = tempoAtual;
            }

            // Movimento dos jogadores
            char tecla = 0;
            movimentoMultiplayer(m,
                                x1, y1,
                                contBomba1, xBomba1, yBomba1, tempoBomba1,
                                contBomba1b, xBomba1b, yBomba1b, tempoBomba1b,
                                x2, y2,
                                contBomba2, xBomba2, yBomba2, tempoBomba2,
                                contBomba2b, xBomba2b, yBomba2b, tempoBomba2b,
                                jogador1, jogador2, numJogadores, tecla);

            // --- Jogador 1 pega bônus ---
            if (x1 >= 0 && m[x1][y1] >= ITEM_FIRE_UP && m[x1][y1] <= ITEM_EXTRA_LIFE) {
                string mensagem = "";
                switch (m[x1][y1]) {
                    case ITEM_FIRE_UP:
                        fireLevelGlobal++;
                        mensagem = "Poder da explosao aumentado!";
                        break;
                    case ITEM_EXTRA_BOMB:
                        bonusExtraBombJ1 = 1;
                        tempoExtraBombJ1 = GetTickCount();
                        mensagem = "+1 bomba extra por 10 segundos!";
                        break;
                    case ITEM_EXTRA_LIFE:
                        vidas1++;
                        mensagem = "+1 vida!";
                        break;
                }
                jogador1.pontuacao += 250; // pontos por pegar bônus
                m[x1][y1] = 0; // remove bônus do mapa

                // Exibe mensagem temporária
                COORD posMsg = {0, 20};
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posMsg);
                cout << "                                             ";
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posMsg);
                cout << mensagem;
                Sleep(800);
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posMsg);
                cout << "                                             ";
            }

            // --- Jogador 2 pega bônus ---
            if (numJogadores == 2 && x2 >= 0 && m[x2][y2] >= ITEM_FIRE_UP && m[x2][y2] <= ITEM_EXTRA_LIFE) {
                string mensagem = "";
                switch (m[x2][y2]) {
                    case ITEM_FIRE_UP:
                        fireLevelGlobal++;
                        mensagem = "Poder da explosao aumentado!";
                        break;
                    case ITEM_EXTRA_BOMB:
                        bonusExtraBombJ2 = 1;
                        tempoExtraBombJ2 = GetTickCount();
                        mensagem = "+1 bomba extra por 10 segundos!";
                        break;
                    case ITEM_EXTRA_LIFE:
                        vidas2++;
                        mensagem = "+1 vida!";
                        break;
                }
                jogador2.pontuacao += 250;
                m[x2][y2] = 0;

                // Exibe mensagem temporária
                COORD posMsg = {0, 21};
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posMsg);
                cout << "                                             ";
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posMsg);
                cout << mensagem;
                Sleep(800);
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posMsg);
                cout << "                                             ";
            }

            // Desativa bônus extra bombas após 10 segundos
            if (bonusExtraBombJ1 && GetTickCount() - tempoExtraBombJ1 > 10000) bonusExtraBombJ1 = 0;
            if (bonusExtraBombJ2 && GetTickCount() - tempoExtraBombJ2 > 10000) bonusExtraBombJ2 = 0;

            // Desenha o mapa e HUD
            desenhoMapa(m, x1, y1, x2, y2, numJogadores, xInimigo, yInimigo, numInimigos,
                        vidas1, vidas2, jogador1.pontuacao, jogador2.pontuacao,
                        jogador1.movimentos, jogador2.movimentos,
                        jogador1.bombas, jogador2.bombas, bonusAtivo, tempoBonus);

            // Atualiza bombas dos jogadores
            atualizarBomba(m, contBomba1, tempoBomba1, tempoExplosao1, xBomba1, yBomba1, xPortal, yPortal);
            if (bonusExtraBombJ1) atualizarBomba(m, contBomba1b, tempoBomba1b, tempoExplosao1b, xBomba1b, yBomba1b, xPortal, yPortal);

            if(numJogadores==2){
                atualizarBomba(m, contBomba2, tempoBomba2, tempoExplosao2, xBomba2, yBomba2, xPortal, yPortal);
                if (bonusExtraBombJ2) atualizarBomba(m, contBomba2b, tempoBomba2b, tempoExplosao2b, xBomba2b, yBomba2b, xPortal, yPortal);
            }

            // Bombas inimigas
            if(fase == 3){
                for(int i=0;i<MAX_INIMIGOS_INTELIGENTES;i++){
                    int idx = numInimigos - MAX_INIMIGOS_INTELIGENTES + i;
                    if(idx < 0) continue;
                    if(contBombaInimigo[i]==0 && xInimigo[idx]!=-1){
                        if(rand()%50==0){
                            int xi = xInimigo[idx];
                            int yi = yInimigo[idx];
                            if(m[xi][yi]==0){
                                m[xi][yi]=7; // bomba inimiga
                                xBombaInimigo[i]=xi; yBombaInimigo[i]=yi;
                                contBombaInimigo[i]=1;
                                tempoBombaInimigo[i]=GetTickCount();
                            }
                        }
                    }
                }
            }

            for(int i=0;i<MAX_INIMIGOS_INTELIGENTES;i++){
                atualizarBombaInimigo(m, contBombaInimigo[i], tempoBombaInimigo[i], tempoExplosaoInimigo[i], xBombaInimigo[i], yBombaInimigo[i]);
            }

            // Colisão com inimigos ou explosões
            bool jogador1Morreu = false;
            bool jogador2Morreu = false;

            for(int k=0;k<10;k++){
                if(xInimigo[k]!=-1 && yInimigo[k]!=-1){
                    if(x1>=0 && x1==xInimigo[k] && y1==yInimigo[k]) jogador1Morreu = true;
                    if(numJogadores==2 && x2>=0 && x2==xInimigo[k] && y2==yInimigo[k]) jogador2Morreu = true;
                }
            }

            // explosões (4) ou bombas (7,8)
            if(x1>=0 && (m[x1][y1] == 4 || m[x1][y1] == 8 || m[x1][y1] == 7)) jogador1Morreu = true;
            if(numJogadores==2 && x2>=0 && (m[x2][y2] == 4 || m[x2][y2] == 8 || m[x2][y2] == 7)) jogador2Morreu = true;

            // Processa mortes
            if(jogador1Morreu){
                vidas1--;
                jogador1.pontuacao = max(0,jogador1.pontuacao-50);
                if(vidas1>0){
                    x1 = 3; y1 = 1;
                    contBomba1 = contBomba1b = 0;
                    xBomba1 = yBomba1 = xBomba1b = yBomba1b = -1;
                    for (int i = 0; i < 15; i++)
                        for (int j = 0; j < 45; j++)
                            if (m[i][j] == 4) m[i][j] = 0;
                } else{
                    x1 = -1; y1 = -1;

                    COORD posMsg = {0, 22};
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posMsg);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                    cout << "Jogador 1 (" << jogador1.nome << ") perdeu todas as vidas!";
                    Sleep(1500);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posMsg);
                    cout << "                                              ";
                }
            }
            if(jogador2Morreu){
                vidas2--;
                jogador2.pontuacao = max(0,jogador2.pontuacao-50);
                if(vidas2>0){
                    x2 = 4;
                    y2 = 1;
                    contBomba2 = contBomba2b = 0;
                    xBomba2 = yBomba2 = xBomba2b = yBomba2b = -1;
                    for (int i = 0; i < 15; i++)
                        for (int j = 0; j < 45; j++)
                            if (m[i][j] == 4) m[i][j] = 0;
                } else {
                    x2 = -1; y2 = -1;

                    COORD posMsg = {0, 22};
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posMsg);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                    cout << "Jogador 2 (" << jogador2.nome << ") perdeu todas as vidas!";
                    Sleep(1500);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posMsg);
                    cout << "                                              ";
                }
            }

            // Inimigos mortos pelas explos�es do jogador (explode marcado com 4)
            for(int k=0; k<10; k++){
                if(xInimigo[k] != -1 && yInimigo[k] != -1 && m[xInimigo[k]][yInimigo[k]] == 4){

                    bool bomba1Proxima = ( (xBomba1!=-1 && abs(xInimigo[k] - xBomba1) <= 5 && abs(yInimigo[k] - yBomba1) <= 5)
                                       || (xBomba1b!=-1 && abs(xInimigo[k] - xBomba1b) <= 5 && abs(yInimigo[k] - yBomba1b) <= 5) );
                    bool bomba2Proxima = ( (numJogadores==2) &&
                                          ( (xBomba2!=-1 && abs(xInimigo[k] - xBomba2) <= 5 && abs(yInimigo[k] - yBomba2) <= 5)
                                         || (xBomba2b!=-1 && abs(xInimigo[k] - xBomba2b) <= 5 && abs(yInimigo[k] - yBomba2b) <= 5) ) );

                    if (bomba1Proxima && (!bomba2Proxima || numJogadores == 1)) {
                        jogador1.pontuacao += 100 * multiplicadorDePontos;
                    }
                    else if (numJogadores == 2 && bomba2Proxima) {
                        jogador2.pontuacao += 100 * multiplicadorDePontos;
                    }

                    if (m[xInimigo[k]][yInimigo[k]] == 0){
                        m[xInimigo[k]][yInimigo[k]] = 4; // garante que o @ apare�a
                    }
                    xInimigo[k] = -1;
                    yInimigo[k] = -1;
                }
            }

            // Verifica se todos inimigos morreram -> avan�a fase automaticamente se ao menos um jogador vivo
            bool todosMortos = true;
            for(int k=0;k<10;k++){
                if(xInimigo[k]!=-1){
                    todosMortos = false;
                    break;
                }
            }
            bool algumVivo = (x1>=0) || (numJogadores==2 && x2>=0);

            // Verifica se algum jogador pisou no portal
            bool jogadorPisouPortal = false;
            if(x1>=0 && x1==xPortal && y1==yPortal) jogadorPisouPortal = true;
            if(numJogadores==2 && x2>=0 && x2==xPortal && y2==yPortal) jogadorPisouPortal = true;

            for(int k = 0; k < numInimigos; k++){
                // s� processa inimigos vivos
                if(xInimigo[k] != -1 && yInimigo[k] != -1){
                    // verifica se a bomba 1 atingiu o inimigo
                    if(m[xInimigo[k]][yInimigo[k]] == 4){ // 4 = explos�o
                        // d� pontos ao jogador respons�vel
                        if(xBomba1 == xInimigo[k] && yBomba1 == yInimigo[k] && x1 >= 0){
                            jogador1.pontuacao += 100 * multiplicadorDePontos;
                        }
                        else if(numJogadores == 2 && xBomba2 == xInimigo[k] && yBomba2 == yInimigo[k] && x2 >= 0){
                            jogador2.pontuacao += 100 * multiplicadorDePontos;
                        }

                        // limpa a posi��o do inimigo no mapa
                        m[xInimigo[k]][yInimigo[k]] = 0;

                        // marca inimigo como morto
                        xInimigo[k] = -1;
                        yInimigo[k] = -1;
                    }
                }
            }


            // Avan�a fase apenas se todos inimigos morreram e algum jogador pisou no portal
            if(todosMortos && jogadorPisouPortal && algumVivo){
                faseAtiva = false;
            }


            // Se ambos morreram (vidas de ambos <=0 ou posi��es == -1 e sem vidas), fim de jogo
            bool ambosSemVida = false;
            if(numJogadores==2){
                if((vidas1<=0 && x1<0) && (vidas2<=0 && x2<0)) ambosSemVida = true;
            } else {
                if(vidas1<=0 && x1<0) ambosSemVida = true;
            }
            if(ambosSemVida){
                // chamar game over: pergunta voltar ao menu ou sair
                char c = telaGameOver();
                if(c=='M') return;
                if(c=='S') exit(0);
            }

            // Se apenas um jogador restante com vidas ==0? manter comportamento: se o outro morrer depois, vai cair no ambosSemVida
            // Sleep do loop
            Sleep(20);
        } // fim while faseAtiva
        salvarJogador(jogador1, jogador2, numJogadores);
    } // fim for fases

    cout << "\nParabens! Voce concluiu as 3 fases da dificuldade!\nPressione qualquer tecla para voltar ao menu...";
    _getch();
}

/// MAIN
int main(){
    srand(time(0));
    char opcaoMenu;
    int numJogadores;
    Jogador jogador1, jogador2;

    while(true){
        system("cls");
        // Reseta cor para menu preto
        setColor(15, 0); // texto branco, fundo preto

        cout<<"===== BOMBERMAN =====\n";
        cout<<"1 - Jogar\n2 - Como Jogar\n3 - Ranking\n4 - Sair\n";
        cout<<"Escolha uma opcao: "; cin>>opcaoMenu;
        if(opcaoMenu=='1'){
            system("cls");
            setColor(15,0);
            cout << "Quantos jogadores? (1 ou 2): ";
            cin >> numJogadores;
            if(numJogadores < 1) numJogadores = 1;
            if(numJogadores > 2) numJogadores = 2;

            cout << "Digite o nome do Jogador 1: ";
            cin >> jogador1.nome;

            if(numJogadores == 2){
                cout << "Digite o nome do Jogador 2: ";
                cin >> jogador2.nome;
            }

            int dif;
            system("cls");
            setColor(15,0); // garante fundo preto na escolha de dificuldade
            cout<<"Escolha a dificuldade:\n1 - Facil\n2 - Medio\n3 - Dificil\nEscolha: ";cin>>dif;
            if(dif>=1 && dif<=3) iniciarJogo(dif, jogador1, jogador2, numJogadores);
        } else if(opcaoMenu=='2'){
            system("cls");
            setColor(15,0); // garante fundo preto
            cout<<"===== COMO JOGAR =====\n";
            cout<<"Objetivo: Chegar no portal sem perder todas as vidas.\n";
            cout<<"Controles:\n  Jogador1 - Setas -> mover, ESPACO -> colocar bomba\n  Jogador2 - W/A/S/D -> mover, F -> colocar bomba\n\n";
            cout <<"===== ITENS =====\n";
            cout <<"Onde encontrar:\nOs itens sao espalhados pelo mapa de forma aleatoria embaixo das paredes.\n";
            cout <<"TIPOS DE BONUS:\nB - aumenta a quantidade de bombas que o jogador pode colocar no mapa.\n" ;
            cout <<"F - a explosao da bomba aumenta em +1 fogo.\nV o jogador ganha +1 vida.\n";
            cout << "\n==== PONTOS ====\n";
            cout<<"Evite inimigos e explosoes, pode gerar -50 pontos.\nGanhe 250 pontos por item bonus coletado.\n";
            cout<<"Ganhe 100 pontos por cada inimigo eliminado.\n\nPressione qualquer tecla para voltar ao menu...";

            _getch();
        } else if(opcaoMenu =='3'){
            mostrarRanking();
        }else if(opcaoMenu=='4') break;

    }
    return 0;
}