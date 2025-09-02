#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// -----------------------------------------------------------------------------
//                       CONSTANTES GLOBAIS
// -----------------------------------------------------------------------------
#define TOTAL_TERRITORIOS 5
#define TOTAL_MISSOES 3
#define TAM_NOME 30
#define TAM_COR 10

// -----------------------------------------------------------------------------
//                       ESTRUTURA DE DADOS
// -----------------------------------------------------------------------------

typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// -----------------------------------------------------------------------------
//                       PROTÓTIPOS DAS FUNÇÕES
// -----------------------------------------------------------------------------

// Setup e gerenciamento de memória
Territorio* alocarMapa(int tamanho);
void inicializarTerritorios(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa);

// Interface com o usuário
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa, int tamanho);
void exibirMissao(int idMissao);

// Lógica do jogo
void faseDeAtaque(Territorio* mapa, int tamanho);
void simularAtaque(Territorio* mapa, int origem, int destino);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, int tamanho, int idMissao);

// Funções utilitárias
void limparBufferEntrada();

// -----------------------------------------------------------------------------
//                       FUNÇÃO PRINCIPAL
// -----------------------------------------------------------------------------

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio* mapa = alocarMapa(TOTAL_TERRITORIOS);
    if (!mapa) {
        printf("Erro ao alocar memória para o mapa.\n");
        return 1;
    }

    inicializarTerritorios(mapa, TOTAL_TERRITORIOS);

    char corJogador[TAM_COR] = "Vermelho";
    int idMissao = sortearMissao();
    int opcao;
    int venceu = 0;

    do {
        exibirMapa(mapa, TOTAL_TERRITORIOS);
        exibirMissao(idMissao);
        exibirMenuPrincipal();

        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, TOTAL_TERRITORIOS);
                break;
            case 2:
                venceu = verificarVitoria(mapa, TOTAL_TERRITORIOS, idMissao);
                if (venceu)
                    printf("\n🎉 Parabéns! Você cumpriu sua missão e venceu o jogo!\n\n");
                else
                    printf("\nAinda não cumpriu a missão. Continue lutando!\n\n");
                break;
            case 0:
                printf("Encerrando o jogo. Até a próxima!\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

        if (!venceu && opcao != 0) {
            printf("Pressione Enter para continuar...");
            getchar();
        }

    } while (opcao != 0 && !venceu);

    liberarMemoria(mapa);
    return 0;
}
Territorio* alocarMapa(int tamanho) {
    return (Territorio*) calloc(tamanho, sizeof(Territorio));
}

void inicializarTerritorios(Territorio* mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("Território %d\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);
        printf("Cor do exército: ");
        scanf(" %[^\n]", mapa[i].cor);
        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
    }
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

void exibirMapa(const Territorio* mapa, int tamanho) {
    printf("\n===== MAPA ATUAL =====\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Território %d - Nome: %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=======================\n\n");
}

void exibirMenuPrincipal() {
    printf("1 - Atacar território inimigo\n");
    printf("2 - Verificar missão\n");
    printf("0 - Sair do jogo\n");
}

void exibirMissao(int idMissao) {
    printf("\n🎯 MISSÃO SECRETA: ");
    switch (idMissao) {
        case 0:
            printf("Conquistar 3 territórios.\n");
            break;
        case 1:
            printf("Destruir o exército Azul.\n");
            break;
        case 2:
            printf("Conquistar todos os territórios com mais de 5 tropas.\n");
            break;
        default:
            printf("Missão desconhecida.\n");
    }
}

void faseDeAtaque(Territorio* mapa, int tamanho) {
    int origem, destino;
    printf("Informe o número do território de origem (1 a %d): ", tamanho);
    scanf("%d", &origem);
    printf("Informe o número do território de destino (1 a %d): ", tamanho);
    scanf("%d", &destino);
    limparBufferEntrada();

    if (origem >= 1 && origem <= tamanho && destino >= 1 && destino <= tamanho && origem != destino) {
        simularAtaque(mapa, origem - 1, destino - 1);
    } else {
        printf("Entrada inválida.\n");
    }
}

void simularAtaque(Territorio* mapa, int origem, int destino) {
    if (mapa[origem].tropas <= 1) {
        printf("Você precisa de mais de 1 tropa para atacar.\n");
        return;
    }

    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    printf("Atacante (%s) rolou: %d\n", mapa[origem].nome, dadoAtaque);
    printf("Defensor (%s) rolou: %d\n", mapa[destino].nome, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("Ataque bem-sucedido!\n");
        mapa[destino].tropas--;
        if (mapa[destino].tropas <= 0) {
            strcpy(mapa[destino].cor, mapa[origem].cor);
            mapa[destino].tropas = 1;
            mapa[origem].tropas--;
            printf("Território conquistado!\n");
        }
    } else {
        printf("Ataque falhou!\n");
        mapa[origem].tropas--;
    }
}

int sortearMissao() {
    return rand() % TOTAL_MISSOES;
}

int verificarVitoria(const Territorio* mapa, int tamanho, int idMissao) {
    int contador = 0;

    switch (idMissao) {
        case 0: // Conquistar 3 territórios (vermelhos)
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(mapa[i].cor, "Vermelho") == 0) contador++;
            }
            return (contador >= 3);
        case 1: // Destruir o exército Azul (nenhum território azul)
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(mapa[i].cor, "Azul") == 0) return 0;
            }
            return 1;
        case 2: // Conquistar todos com mais de 5 tropas
            for (int i = 0; i < tamanho; i++) {
                if (mapa[i].tropas > 5 && strcmp(mapa[i].cor, "Vermelho") != 0)
                    return 0;
            }
            return 1;
    }

    return 0;
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
