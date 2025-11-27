    #include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   STRUCT DA ÁRVORE BINÁRIA
   Representa cada cômodo (sala) da mansão.
   ============================================================ */
typedef struct Sala {
    char nome[40];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* ============================================================
   criarSala()
   Cria dinamicamente uma sala com o nome informado.
   ============================================================ */
Sala* criarSala(const char nome[]) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/* ============================================================
   explorarSalas()
   Permite ao jogador explorar interativamente a mansão.
   O jogador inicia no Hall e escolhe ir:
       - esquerda (e)
       - direita  (d)
       - sair     (s)
   A exploração termina quando o jogador chega a um nó-folha.
   ============================================================ */
void explorarSalas(Sala *salaAtual) {

    char opcao;

    while (salaAtual != NULL) {

        printf("\nVocê está em: **%s**\n", salaAtual->nome);

        // Se for nó-folha, a exploração termina
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Você chegou ao fim do caminho! Não há mais saídas.\n");
            return;
        }

        printf("Escolha para onde ir:\n");
        if (salaAtual->esquerda != NULL) printf("  (e) Esquerda -> %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita  != NULL) printf("  (d) Direita  -> %s\n", salaAtual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (salaAtual->esquerda != NULL)
                salaAtual = salaAtual->esquerda;
            else
                printf("Não há caminho à esquerda!\n");
        }
        else if (opcao == 'd' || opcao == 'D') {
            if (salaAtual->direita != NULL)
                salaAtual = salaAtual->direita;
            else
                printf("Não há caminho à direita!\n");
        }
        else if (opcao == 's' || opcao == 'S') {
            printf("\nExploração encerrada pelo jogador.\n");
            return;
        }
        else {
            printf("Opção inválida! Use apenas e, d ou s.\n");
        }
    }
}

/* ============================================================
   main()
   Monta o mapa fixo da mansão usando a função criarSala() e
   dá início à exploração.
   ============================================================ */
int main() {

    /* ----------------------------------------------
       Construção manual da árvore binária da mansão
       (Mapa pré-definido pela Enigma Studios)
       ---------------------------------------------- */

    Sala *hall        = criarSala("Hall de Entrada");
    Sala *salaEstar   = criarSala("Sala de Estar");
    Sala *biblioteca  = criarSala("Biblioteca");
    Sala *cozinha     = criarSala("Cozinha");
    Sala *jardim      = criarSala("Jardim");
    Sala *porao       = criarSala("Porão");
    Sala *quartoSecreto = criarSala("Quarto Secreto");

    // Conectando as salas (Exemplo de mapa)
    hall->esquerda        = salaEstar;
    hall->direita         = biblioteca;

    salaEstar->esquerda   = cozinha;
    salaEstar->direita    = jardim;

    biblioteca->esquerda  = porao;
    biblioteca->direita   = quartoSecreto;

    /* ----------------------------------------------
       Início da exploração
       ---------------------------------------------- */
    printf("=====================================\n");
    printf("   DETECTIVE QUEST – Exploração da Mansão\n");
    printf("=====================================\n");

    explorarSalas(hall);

    printf("\nObrigado por jogar Detective Quest!\n");

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   STRUCT DA MANSÃO (ÁRVORE BINÁRIA)
   Cada sala contém:
      - nome da sala
      - pista (pode ser vazia)
      - ponteiros esquerda/direita
   ============================================================ */
typedef struct Sala {
    char nome[40];
    char pista[80];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* ============================================================
   STRUCT DA ÁRVORE BST DE PISTAS
   Cada nó contém:
      - texto da pista
      - ponteiros esq/dir
   ============================================================ */
typedef struct PistaNode {
    char pista[80];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

/* ============================================================
   criarSala()
   Cria dinamicamente uma sala com nome e pista opcional.
   ============================================================ */
Sala* criarSala(const char nome[], const char pista[]) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro de alocação ao criar sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/* ============================================================
   inserirPista()
   Insere uma pista na BST de forma ordenada por ordem alfabética.
   ============================================================ */
PistaNode* inserirPista(PistaNode *raiz, const char pista[]) {

    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}

/* ============================================================
   exibirPistas()
   Exibe as pistas coletadas em ordem alfabética (in-order).
   ============================================================ */
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esq);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}

/* ============================================================
   explorarSalasComPistas()
   Controla a navegação do jogador pela mansão, coletando pistas.
   ============================================================ */
void explorarSalasComPistas(Sala *salaAtual, PistaNode **arvorePistas) {

    char opcao;

    while (1) {
        printf("\nVocê está em: **%s**\n", salaAtual->nome);

        /* Coleta automática da pista da sala atual */
        if (strlen(salaAtual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", salaAtual->pista);
            *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        /* Mostrar opções */
        printf("\nEscolha o próximo caminho:\n");
        if (salaAtual->esquerda != NULL)
            printf("  (e) Ir para a esquerda → %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)
            printf("  (d) Ir para a direita  → %s\n", salaAtual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (salaAtual->esquerda != NULL)
                salaAtual = salaAtual->esquerda;
            else
                printf("Não há caminho à esquerda!\n");
        }
        else if (opcao == 'd' || opcao == 'D') {
            if (salaAtual->direita != NULL)
                salaAtual = salaAtual->direita;
            else
                printf("Não há caminho à direita!\n");
        }
        else if (opcao == 's' || opcao == 'S') {
            printf("\nExploração encerrada.\n");
            return;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}

/* ============================================================
   main()
   Cria a mansão, inicializa a BST de pistas
   e inicia a exploração.
   ============================================================ */
int main() {

    /* ------------------------------------------------------
       Construção do mapa fixo da mansão (árvore binária)
       ------------------------------------------------------ */

    Sala *hall = criarSala("Hall de Entrada", "Pegadas suspeitas no tapete.");

    Sala *salaEstar   = criarSala("Sala de Estar", "Um livro deixado aberto.");
    Sala *biblioteca  = criarSala("Biblioteca", "Um mapa antigo dobrado.");
    Sala *cozinha     = criarSala("Cozinha", "Uma faca fora do lugar.");
    Sala *jardim      = criarSala("Jardim", "Galhos quebrados recentemente.");
    Sala *porao       = criarSala("Porão", "Uma lanterna descarregada.");
    Sala *escritorio  = criarSala("Escritório", "Documentos parcialmente queimados.");

    hall->esquerda = salaEstar;
    hall->direita  = biblioteca;

    salaEstar->esquerda = cozinha;
    salaEstar->direita  = jardim;

    biblioteca->esquerda = porao;
    biblioteca->direita  = escritorio;

    /* Árvore BST de pistas começa vazia */
    PistaNode *pistasColetadas = NULL;

    printf("=============================================\n");
    printf("    DETECTIVE QUEST – Sistema de Pistas\n");
    printf("=============================================\n");

    explorarSalasComPistas(hall, &pistasColetadas);

    /* Exibir pistas coletadas */
    printf("\n=============================================\n");
    printf("Pistas coletadas (ordem alfabética):\n");
    printf("=============================================\n");
    exibirPistas(pistasColetadas);

    printf("\nObrigado por jogar Detective Quest!\n");

    return 0;
}