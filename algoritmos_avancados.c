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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* ----------------------------
   Configurações da hash table
   ---------------------------- */
#define HASH_SIZE 31    // tamanho pequeno e primo

/* ----------------------------
   ESTRUTURAS
   ---------------------------- */

/* Nó da árvore binária que representa cada sala da mansão.
   Cada sala possui um nome, uma pista (string, pode ser vazia)
   e ponteiros para filhos esquerdo/direito. Também há um
   campo 'coletada' para evitar recolher a mesma pista várias vezes.
*/
typedef struct Sala {
    char nome[64];
    char pista[128];    // pista estática associada à sala (pode ser "")
    int coletada;       // 0 = não coletada ainda, 1 = já coletada
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* Nó da BST que armazena pistas coletadas (ordenadas por string) */
typedef struct PistaNode {
    char pista[128];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

/* Estrutura para encadeamento na tabela hash (associa pista -> suspeito) */
typedef struct HashNode {
    char pista[128];
    char suspeito[64];
    struct HashNode *prox;
} HashNode;

/* ----------------------------
   PROTÓTIPOS
   ---------------------------- */

/* criarSala:
   Cria dinamicamente uma sala (nó da árvore) com nome e pista.
*/
Sala* criarSala(const char *nome, const char *pista);

/* explorarSalas:
   Permite a navegação interativa pela árvore da mansão a partir de salaInicial.
   Ao entrar em uma sala, se houver pista e não tiver sido coletada, adiciona à BST.
*/
void explorarSalas(Sala *salaInicial, PistaNode **raizPistas, HashNode *tabela[]);

/* inserirPista (adicionarPista):
   Insere uma pista (string) na BST de pistas coletadas em ordem alfabética.
   Retorna a raiz (padrão de BST).
*/
PistaNode* inserirPista(PistaNode *raiz, const char *pista);

/* exibirPistas:
   Faz in-order traversal da BST mostrando as pistas armazenadas.
*/
void exibirPistas(PistaNode *raiz);

/* HASH: inserirNaHash
   Insere par (pista -> suspeito) na tabela hash (encadeamento).
*/
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito);

/* HASH: encontrarSuspeito
   Dada uma pista (string), retorna o suspeito associado (ou NULL).
*/
const char* encontrarSuspeito(HashNode *tabela[], const char *pista);

/* verificarSuspeitoFinal:
   Percorre a BST de pistas coletadas e conta quantas pistas apontam
   para o suspeito acusado. Retorna o número de pistas que apontam para ele.
*/
int verificarSuspeitoFinal(PistaNode *raiz, HashNode *tabela[], const char *acusado);

/* Funções utilitárias: criação de hash, hash function, liberar memória */
unsigned long hashString(const char *str);
HashNode** criarTabelaHash();
void liberarTabelaHash(HashNode *tabela[]);
void liberarPistas(PistaNode *raiz);
void liberarSala(Sala *root);

/* ----------------------------
   IMPLEMENTAÇÃO
   ---------------------------- */

/* criarSala: cria dinamicamente uma sala com nome e pista */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    if (!s) {
        fprintf(stderr, "Erro: memória insuficiente ao criar sala\n");
        exit(1);
    }
    strncpy(s->nome, nome, sizeof(s->nome)-1);
    s->nome[sizeof(s->nome)-1] = '\0';
    if (pista && strlen(pista) > 0) {
        strncpy(s->pista, pista, sizeof(s->pista)-1);
        s->pista[sizeof(s->pista)-1] = '\0';
    } else {
        s->pista[0] = '\0';
    }
    s->coletada = 0;
    s->esquerda = s->direita = NULL;
    return s;
}

/* inserirPista: insere pista na BST (ordem alfabética) */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (pista == NULL || strlen(pista) == 0) return raiz;

    if (raiz == NULL) {
        PistaNode *n = (PistaNode*) malloc(sizeof(PistaNode));
        if (!n) { fprintf(stderr, "Erro: malloc\n"); exit(1); }
        strncpy(n->pista, pista, sizeof(n->pista)-1);
        n->pista[sizeof(n->pista)-1] = '\0';
        n->esq = n->dir = NULL;
        return n;
    }

    int cmp = strcasecmp(pista, raiz->pista);
    if (cmp < 0) raiz->esq = inserirPista(raiz->esq, pista);
    else if (cmp > 0) raiz->dir = inserirPista(raiz->dir, pista);
    // Se igual, não insere duplicata (consideramos única)
    return raiz;
}

/* exibirPistas: in-order traversal para mostrar pistas em ordem alfabética */
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esq);
    printf("  - %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}

/* hashString: djb2 hash */
unsigned long hashString(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

/* criarTabelaHash: aloca e inicializa a tabela de HASH */
HashNode** criarTabelaHash() {
    HashNode **t = (HashNode**) calloc(HASH_SIZE, sizeof(HashNode*));
    if (!t) { fprintf(stderr, "Erro: calloc\n"); exit(1); }
    return t;
}

/* inserirNaHash: insere (pista -> suspeito) */
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    unsigned long h = hashString(pista) % HASH_SIZE;
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    if (!novo) { fprintf(stderr, "Erro: malloc\n"); exit(1); }
    strncpy(novo->pista, pista, sizeof(novo->pista)-1);
    novo->pista[sizeof(novo->pista)-1] = '\0';
    strncpy(novo->suspeito, suspeito, sizeof(novo->suspeito)-1);
    novo->suspeito[sizeof(novo->suspeito)-1] = '\0';
    novo->prox = tabela[h];
    tabela[h] = novo;
}

/* encontrarSuspeito: busca a pista na tabela e retorna o suspeito (ou NULL) */
const char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    unsigned long h = hashString(pista) % HASH_SIZE;
    HashNode *cur = tabela[h];
    while (cur) {
        if (strcasecmp(cur->pista, pista) == 0) return cur->suspeito;
        cur = cur->prox;
    }
    return NULL;
}

/* explorarSalas:
   Navega interativamente pela mansão, coletando pistas automaticamente.
   Parâmetros:
     - salaInicial: nó onde a exploração começa (Hall)
     - raizPistas: ponteiro para a raiz da BST onde as pistas coletadas serão inseridas
     - tabela: tabela hash já preenchida com associações (opcional aqui)
*/
void explorarSalas(Sala *salaInicial, PistaNode **raizPistas, HashNode *tabela[]) {
    if (!salaInicial) return;

    Sala *atual = salaInicial;
    char opcao;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        /* Se a sala tem pista e ainda não foi coletada, colete-a e insira na BST */
        if (strlen(atual->pista) > 0 && !atual->coletada) {
            printf(">>> Você encontrou uma pista: \"%s\"\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
            atual->coletada = 1;
        } else if (strlen(atual->pista) > 0 && atual->coletada) {
            printf("Pista já coletada anteriormente nesta sala.\n");
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        /* Mostrar opções de navegação */
        printf("\nEscolha:\n");
        if (atual->esquerda) printf("  (e) Ir para a esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita)  printf("  (d) Ir para a direita  -> %s\n", atual->direita->nome);
        printf("  (s) Encerrar exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("Não há caminho à esquerda.\n");
        } else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita) atual = atual->direita;
            else printf("Não há caminho à direita.\n");
        } else if (opcao == 's' || opcao == 'S') {
            printf("\nExploração finalizada pelo jogador.\n");
            return;
        } else {
            printf("Opção inválida. Use e, d ou s.\n");
        }
    }
}

/* verificarSuspeitoFinal:
   Conta quantas pistas coletadas apontam para 'acusado' usando a tabela hash.
   Retorna o número de pistas que apontam para o acusado.
*/
int verificarSuspeitoFinal(PistaNode *raiz, HashNode *tabela[], const char *acusado) {
    if (!raiz) return 0;
    int count = 0;

    /* Percorre em-order e conta */
    // esquerda
    count += verificarSuspeitoFinal(raiz->esq, tabela, acusado);

    // nó atual
    const char *sus = encontrarSuspeito(tabela, raiz->pista);
    if (sus != NULL && strcasecmp(sus, acusado) == 0) count++;

    // direita
    count += verificarSuspeitoFinal(raiz->dir, tabela, acusado);
    return count;
}

/* liberarTabelaHash: libera toda a tabela */
void liberarTabelaHash(HashNode *tabela[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *cur = tabela[i];
        while (cur) {
            HashNode *tmp = cur;
            cur = cur->prox;
            free(tmp);
        }
    }
    free(tabela);
}

/* liberarPistas: libera BST de pistas */
void liberarPistas(PistaNode *raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esq);
    liberarPistas(raiz->dir);
    free(raiz);
}

/* liberarSala: libera árvore de salas (post-order) */
void liberarSala(Sala *root) {
    if (!root) return;
    liberarSala(root->esquerda);
    liberarSala(root->direita);
    free(root);
}

/* ----------------------------
   FUNÇÃO main: monta mapa, tabela de pistas->suspeitos e inicia jogo
   ---------------------------- */
int main() {
    /* Inicializa random (não essencial aqui, pois pistas são estáticas) */
    srand((unsigned) time(NULL));

    /* -------------------------
       Montagem fixa da mansão (árvore binária)
       Cada sala tem uma pista estática (pode ser string vazia se não houver pista)
       ------------------------- */
    Sala *hall = criarSala("Hall de Entrada", "Pegadas molhadas perto do tapete.");
    Sala *vestibulo = criarSala("Vestíbulo", "Um lenço com monograma 'M' foi encontrado.");
    Sala *salaEstar = criarSala("Sala de Estar", "Vidro quebrado e um bilhete rasgado.");
    Sala *biblioteca = criarSala("Biblioteca", "Marcas de cinzas em um documento.");
    Sala *cozinha = criarSala("Cozinha", "Resíduos de veneno na xícara.");
    Sala *jardim = criarSala("Jardim", "Rastro de pneus recentes.");
    Sala *porao = criarSala("Porão", "Pegadas pequenas e um anel prateado.");
    Sala *escritorio = criarSala("Escritório", "Carta ameaçadora endereçada ao falecido.");
    Sala *quarto = criarSala("Quarto", "Caixa trancada com letras 'C' gravadas.");

    /* Conexões (mapa de exemplo) */
    hall->esquerda = vestibulo;
    hall->direita  = salaEstar;

    vestibulo->esquerda = biblioteca;
    vestibulo->direita  = cozinha;

    salaEstar->esquerda = jardim;
    salaEstar->direita  = porao;

    biblioteca->esquerda = escritorio;
    biblioteca->direita  = quarto;

    /* -------------------------
       Preenche tabela hash com associações pista -> suspeito
       (essas associações são definidas pela lógica do jogo/roteiro)
       ------------------------- */
    HashNode **tabela = criarTabelaHash();

    inserirNaHash(tabela, "Pegadas molhadas perto do tapete.", "Criado");
    inserirNaHash(tabela, "Um lenço com monograma 'M' foi encontrado.", "Mordomo");
    inserirNaHash(tabela, "Vidro quebrado e um bilhete rasgado.", "Jardineiro");
    inserirNaHash(tabela, "Marcas de cinzas em um documento.", "Escritora");
    inserirNaHash(tabela, "Resíduos de veneno na xícara.", "Cozinheira");
    inserirNaHash(tabela, "Rastro de pneus recentes.", "Motorista");
    inserirNaHash(tabela, "Pegadas pequenas e um anel prateado.", "Noiva");
    inserirNaHash(tabela, "Carta ameaçadora endereçada ao falecido.", "Escritora");
    inserirNaHash(tabela, "Caixa trancada com letras 'C' gravadas.", "Cozinheira");

    /* Observação: múltiplas pistas podem apontar para o mesmo suspeito (ex: Escritoria/Cozinheira) */

    /* -------------------------
       Inicia exploração e coleta de pistas
       ------------------------- */
    PistaNode *raizPistas = NULL;

    printf("=============================================\n");
    printf(" DETECTIVE QUEST - Investigação Final\n");
    printf(" Explore a mansão e colete pistas.\n");
    printf("=============================================\n");

    explorarSalas(hall, &raizPistas, tabela);

    /* -------------------------
       Mostrar pistas coletadas
       ------------------------- */
    printf("\n=============================================\n");
    printf("Pistas coletadas (ordenadas):\n");
    printf("=============================================\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista coletada.\n");
    } else {
        exibirPistas(raizPistas);
    }

    /* -------------------------
       Fase de acusação
       ------------------------- */
    char acusado[64];
    printf("\nQuem você acusa? Digite o nome do suspeito: ");
    // limpar buffer e ler linha
    getchar(); // consome eventual '\n' remanescente
    if (fgets(acusado, sizeof(acusado), stdin) == NULL) acusado[0] = '\0';
    acusado[strcspn(acusado, "\n")] = '\0'; // remove newline

    // Conta quantas pistas apontam para o acusado
    int cont = verificarSuspeitoFinal(raizPistas, tabela, acusado);

    printf("\nResultado da acusação contra '%s':\n", acusado);
    printf("Pistas que apontam para esse suspeito: %d\n", cont);

    if (cont >= 2) {
        printf("ACUSADO SUSTENTADO: Existem evidências suficientes (>=2 pistas) para acusar %s.\n", acusado);
    } else {
        printf("ACUSAÇÃO FRACA: Menos de 2 pistas apontam para %s. A acusação não se sustenta.\n", acusado);
    }

    /* -------------------------
       Limpeza de memória
       ------------------------- */
    liberarPistas(raizPistas);
    liberarTabelaHash(tabela);
    liberarSala(hall);

    printf("\nFim do jogo. Obrigado por investigar!\n");
    return 0;
}