#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>
#include<iostream>

/** Altera a posição de onde o conteúdo do printf vai ser impresso, a partir da posição x e y */
void pointerPositionXY(int x, int y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){ x, y });
}

/** Desenha um item na tela na posição indicada */
void drawIten(int x, int y, char content[]) 
{
	pointerPositionXY(x, y);
    printf("%s", content);
}

/** 
*   Preenche a matrix com o numero passado
*   Depende da porcentagem e o numero maximo a ser inserido
*   int matrix[][]
*   int porcentagem
*   int contador
*   int numero
**/
void inserirItens(int matriz[][10], int porc, int cont, int num) 
{
	int randPorc, contIten = 0;
	
    for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			randPorc = rand() % 100; // Sorteia um numero de 0 a 99
			
            if (randPorc < porc && contIten < cont) {
                contIten++;
                matriz[i][j] = num;
            }
		}
	}
}

/** Preenche a matriz de operação com valores aleatórios das tochas, armadilhas e tesouro */
void preencherOperacao(int matriz[][10])
{	
	// Preenchendo matriz com valores vazios
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
            matriz[i][j] = 0;
		}
	}
	
	inserirItens(matriz, 15, 5, 2);  // Insere as tochas
    inserirItens(matriz, 25, 32, 1); // Insere as armadilhas 

	int goldLinha = (rand() % 10) + 1; // Para iniciar a linha na posição 1 pra cima
	int goldColuna = rand() % 10;

    if (goldLinha > 9) goldLinha = 9; // Não deixa que fique na ultima posição da linha
	
	matriz[goldLinha][goldColuna] = 3; // Insere o tesouro
	
	matriz[0][0] = -1; // Insere o player 1
	matriz[0][9] = -2; // Insere o player 2
}

/** Preenche o campo da interface com '*' */
void preencherInterface(int matriz[][10]) 
{
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            matriz[i][j] = '*';
        }   
    }
}

/** Retorna a quantidade de jogadas que o player vai ter */
int qtdJogar()
{
	int jogar = (rand() % 8) + 3; 
	return jogar;
}

/** Imprime o campoOperacao para verificar onde tem armadilhas
 *	Esse método é para ser usado para testes
 */
void imprimir(int matriz[][10])
{
	pointerPositionXY(0, 18);
	for(int i = 0; i < 10; i++){
		for(int l = 0; l < 10; l++){
			if( matriz[i][l] == 0 )
				printf("* ");
		    else if( matriz[i][l] == 1 )
		    	printf("x ");
		    else if( matriz[i][l] == 2 )
		    	printf("! "); 
		    else if( matriz[i][l] == 3 )
		    	printf("+ ");
		    else
		    	printf("N ");
		}
		printf("\n");
	}
	
	for(int i = 0; i < 10; i++){
		for(int l = 0; l < 10; l++){
			printf("%d ", matriz[i][l]);
		}
		printf("\n");
	}
}

/** Verifica se o player bateu em algum iten
 *  returns:	
 *  0 para nenhum
 *  1 para armadilha
 *  2 para tocha
 *	3 para tesouro
 */
int verificaColisao(int campoOperacao[][10], int currentX[], int currentY[], int player, int vida[]) 
{
	int arrayPosition, arrayPosition2, player2;
	
	if (player == 1) {
		arrayPosition = 0;
		arrayPosition2 = 1;
		player2 = 2;
	} else {
		arrayPosition = 1;
		arrayPosition2 = 0;
		player2 = 1;
	}
	
	int x = currentX[arrayPosition] / 2;
	int y = currentY[arrayPosition];
	
	// Mostra o valor do campo de operação onde o player pisou
	// Serve apenas para validar o jogo
	// pointerPositionXY(0, 12);
	// printf("X = %d, Y = %d, campo = %d", x, y, campoOperacao[y][x]);
	
	
	// Não encontrou nada
	if (campoOperacao[y][x] == 0 || campoOperacao[y][x] == -1 || campoOperacao[y][x] == -2) { 
		return 0;
	} 
	
	// Encontrou uma armadilha
	if (campoOperacao[y][x] == 1) { 
		// Info que o player pisou na armadilha
		pointerPositionXY(0, 13);
		printf("O PLAYER 0%d PISOU NUMA ARMADILHA, PERDEU 1 PONTO DE VIDA(HP)", player);
		
		// Info e diminuindo vida do player
		if (player == 1) pointerPositionXY(30, 1);
		else pointerPositionXY(30, 3);
		printf("PLAYER 0%d    HP:%d/5\n", player, --vida[player - 1]);
		
		campoOperacao[y][x] = 0; // Desabilitando armadilha
		
		// Verifica se o player morreu
		if (vida[player - 1] == 0) {
			system("CLS");
			printf("\n\n==========================================");
			printf("\n\n              VOCE MORREU");
			printf("\n\n\n\n         VITORIA DO PLAYER 0%d ", player2);
			printf("\n\n==========================================");
			printf("\n\n\n\nCLIQUE EM QUALQUER TECLA PARA SAIR");	
		}  
		
		return 1;
	} 
	
	// Encontrou uma tocha
	if (campoOperacao[y][x] == 2) { 
		pointerPositionXY(0, 13);
		printf("O PLAYER 0%d ENCONTROU UMA TOCHA                              ", player);
		
		int cima, baixo, esquerda, direita, x2, y2;
		// Item a ser impresso
		char meuItem[] = " ";  
		
		// Posições do outro player
		x2 = currentX[arrayPosition2] / 2;
		y2 = currentY[arrayPosition2];
		// Posições para verificar os itens ao redor do player atual
		cima = y - 1;
		baixo = y + 1;
		esquerda = x - 1;
		direita = x + 1;
		
		// Verifica se existe uma posição a cima
		if (cima >= 0) { 
			if (x == x2 && cima == y2) { // Verifica se o outo player está a cima do player atual
				if (player == 1) {
					meuItem[0] = '2';
				} else {
					meuItem[0] = '1';
				}
			}
			else if (campoOperacao[cima][x] <= 0) meuItem[0] = ' ';
			else if (campoOperacao[cima][x] == 1) meuItem[0] = 'x';
			else if (campoOperacao[cima][x] == 2) meuItem[0] = '!';
			else if (campoOperacao[cima][x] == 3) meuItem[0] = '+';
			
			drawIten((x + x), cima, meuItem);
		} 
		
		// Verifica se existe uma posição abaixo
		if (baixo <= 9) {
			if (x == x2 && baixo == y2) { // Verifica se o outo player está a cima do player atual
				if (player == 1) {
					meuItem[0] = '2';
				} else {
					meuItem[0] = '1';
				}
			}
			else if (campoOperacao[baixo][x] <= 0) meuItem[0] = ' ';
			else if (campoOperacao[baixo][x] == 1) meuItem[0] = 'x';
			else if (campoOperacao[baixo][x] == 2) meuItem[0] = '!';
			else if (campoOperacao[baixo][x] == 3) meuItem[0] = '+';
			

			drawIten((x + x), baixo, meuItem);
		}
		
		// Verifica se existe uma posição a esquerda
		if (esquerda >= 0) {
			if (esquerda == x2 && y == y2) { // Verifica se o outo player está a esquerda do player atual
				if (player == 1) {
					meuItem[0] = '2';
				} else {
					meuItem[0] = '1';
				}
			}
			else if (campoOperacao[y][esquerda] <= 0) meuItem[0] = ' ';
			else if (campoOperacao[y][esquerda] == 1) meuItem[0] = 'x';
			else if (campoOperacao[y][esquerda] == 2) meuItem[0] = '!';
			else if (campoOperacao[y][esquerda] == 3) meuItem[0] = '+';
	
			drawIten((esquerda + esquerda), y, meuItem);
		}
		
		// Verifica se existe uma posição a direita
		if (direita <= 9) {
			if (direita == x2 && y == y2) { // Verifica se o outo player está a direita do player atual
				if (player == 1) {
					meuItem[0] = '2';
				} else {
					meuItem[0] = '1';
				}
			}
			else if (campoOperacao[y][direita] <= 0) meuItem[0] = ' ';
			else if (campoOperacao[y][direita] == 1) meuItem[0] = 'x';
			else if (campoOperacao[y][direita] == 2) meuItem[0] = '!';
			else if (campoOperacao[y][direita] == 3) meuItem[0] = '+';

			drawIten((direita + direita), y, meuItem);
		}
		
		// Desativa tocha
		campoOperacao[y][x] = 0;
		
		return 2;
	} 
	
	// Encontrou o tesouro
	if (campoOperacao[y][x] == 3) { 
		system("CLS");
		printf("\n\n==========================================");
		printf("\n\n        O PLAYER 0%d ACHOU O TESOURO", player);
		printf("\n\n\n\n                 VITORIA");
		printf("\n\n==========================================");
		printf("\n\n\n\nCLIQUE EM QUALQUER TECLA PARA SAIR");
		
		campoOperacao[y][x] = 0;
		
		return 3;
	}
	
	return 0;
}

/** Verifica a tecla clicada e altera a posição do jogador
 *  Retorna 1 se a posição mudou e 0 se não mudou */
int changeMove(char key, int currentX[], int currentY[], int position, int player) 
{	
	// Pega o valor da tecla inserida
	key = getch(); 
	
	char w, a, s, d;
	int auxX = currentX[position]; // Guardando os valores originais
	int auxY = currentY[position]; // Idem
	
	key = toupper(key); // Passando valor do teclado para maiusculo
	
	if (player == 1) {
		w = 'W';
		a = 'A';
		s = 'S';
		d = 'D';
	} else { // Player 2
		w = '8';
		a = '4';
		s = '5';
		d = '6';
	}

	// Andando com o player
	if (key == w) { // Subir
		
		currentY[position]--;
		if (currentY[position] <= 0) currentY[position] = 0;	
		
	} else if (key == a) { // Esquerda
		
		currentX[position] = currentX[position] - 2;
		if (currentX[position] <= 0 ) currentX[position] = 0;
		
	} else if (key == s) { // Descer
		
		currentY[position]++;
		if (currentY[position] >= 9) currentY[position] = 9;
		
	} else if (key == d) { // Direita
		
		if (currentX[position] == 0) 		currentX[position] = 2;
		else if (currentX[position] != 18)  currentX[position] = currentX[position] + 2;
		
		if (currentX[position] > 18) currentX[position] = 18; // Para evitar bugs, se for maior que a ultima posição, retorna para a ultima
	}
	
	// Verificando se o player atual não vai colidir com o outro player
	int positionP2 = 0; // Posição do array currentX e currentY
	if (player == 1) positionP2 = 1;
	
	if (currentX[position] == currentX[positionP2] && currentY[position] == currentY[positionP2]) {
		// Altera as posições para as originais
		currentX[position] = auxX;
		currentY[position] = auxY;
	}
	
	// Verificando se o player não conseguiu se mover
	if (auxX == currentX[position] && auxY == currentY[position]) return 0;
	
	return 1;
}

/** Inicia a porte gráfica e controle dos players */
void start(int campoInterface[][10], int campoOperacao[][10]) 
{	
	// Iniciando variáveis
	// Posições de cada player
	int currentX[] = { 0, 18 }, currentY[] = { 0, 0 }, currentPlayer = 1;
	// Vidas de cada player
	int vida[2] = { 5, 5 };
	// Tecla inserida pelo player
	char key = 'i';
	// Variaveis auxiliares das posições do player que está jogando
	int auxX = 0, auxY = 0;
	// Se uma posição foi alterada
	bool changed = false;
	// Se o player conseguiu se mover
	int moved = 1;
	// Quantos movimentos o player da vez pode jogar
	int contChanged = qtdJogar();
	// Player que ganhou
	int win = 0;
	// Guarda o valor quando o player colide
	int colisao;

	// Desenhando mapa
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
        	if (i == 0) pointerPositionXY(i, j);
			else pointerPositionXY(i + i, j);
            printf("%c ", campoInterface[i][j]);
        }   
    }
    
    // Imprime o campo de operação para validar o jogo
    // imprimir(campoOperacao);
    
    // Desenhando players
    drawIten(0, 0, "1");  
    drawIten(18, 0, "2"); 
    
    // Desenhando info dos players
    pointerPositionXY(30, 1);
	printf("PLAYER 01    HP:%d/5\n", vida[0]);
	pointerPositionXY(30, 3);
	printf("PLAYER 02    HP:%d/5\n", vida[1]);
	pointerPositionXY(0, 16);
	printf("PLAYER 0%d PODE ANDAR %d CASAS ", currentPlayer, contChanged);
	
	// Iniciando ciclo de jogo, onde espera os comandos dos players
	while(win == 0){ 
		// Fica no loop até que o player insira um comando de tecla
		while(win == 0 && (key = kbhit()) == false) { 
			if (changed) {
				if (currentPlayer == 1) { // Lógica para o player 1
					drawIten(auxX, auxY, " "); // Desenha um espaço vazia na posição antiga do player
					drawIten(currentX[0], currentY[0], "1"); // Desenha o player na sua próxima posição
					colisao = verificaColisao(campoOperacao, currentX, currentY, 1, vida); // Verifica se o player colidiu com algum item
					contChanged--;
					// Verifica se o player ainda tem jogadas
					if (contChanged <= 0) {
						currentPlayer = 2; // Mudando para o player 2
						contChanged = qtdJogar();
					}
					// Verifica se o player ganhou
					if (colisao == 3) win = 1;
					// Verifica se o player morreu
					if (vida[0] == 0) win = 2;
				} else { // Lógica para o player 2
					drawIten(auxX, auxY, " "); // Desenha um espaço vazia na posição antiga do player
					drawIten(currentX[1], currentY[1], "2"); // Desenha o player na sua próxima posição
					colisao = verificaColisao(campoOperacao, currentX, currentY, 2, vida); // Verifica se o player colidiu com algum item
					contChanged--;
					// Verifica se o player ainda tem jogadas
					if (contChanged <= 0) {
						currentPlayer = 1;
						contChanged = qtdJogar();;
					}
					// Verifica se o player ganhou
					if (colisao == 3) win = 2;
					// Verifica se o player morreu
					if (vida[1] == 0) win = 1;
				}
				
				// Caso nenhum player tenha ganhado ainda, imprime a info de jogadas do próximo player
				if (win == 0) {
					pointerPositionXY(0, 16);
					printf("PLAYER 0%d PODE ANDAR %d CASAS ", currentPlayer, contChanged);
				}
				
				changed = false;
			}
		} // Fim do loop esperando comando do player
	
		if (currentPlayer == 1) { // Lógica para o player 1
			auxX = currentX[0];
			auxY = currentY[0];
			moved = changeMove(key, currentX, currentY, 0, 1); // Retorna 1 se o player se moveu, e 0 caso não tenha se movido
			// Verifica se o player se moveu
			if (moved == 0) changed = false;
			else changed = true;
		} else { // Lógica para o player 2
			auxX = currentX[1];
			auxY = currentY[1];
			moved = changeMove(key, currentX, currentY, 1, 2); // Retorna 1 se o player se moveu, e 0 caso não tenha se movido
			// Verifica se o player se moveu
			if (moved == 0) changed = false;
			else changed = true;
		}
	}
}

int main()
{
    // caminho   = 0
    // armadilha = 1
    // tocha     = 2
    // tesouro   = 3

	srand(time(NULL));

	// Campo desenhado na tela
    int campoInterface[10][10];
    // Campo de itens não desenhado
	int campoOperacao[10][10];
	
    preencherInterface(campoInterface);
    preencherOperacao(campoOperacao);
    
    start(campoInterface, campoOperacao); 
    
    pointerPositionXY(0, 17);
    
	return 0;
}
