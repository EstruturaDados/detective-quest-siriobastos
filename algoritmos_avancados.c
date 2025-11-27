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