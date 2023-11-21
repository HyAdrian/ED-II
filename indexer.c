#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

// ================  STRUCT LISTA LIGADA / NODO E FUNÇÕES ==================================
typedef struct Nodo {
    char palavra[30];
    int qtd;
    struct Nodo *prox;
} Nodo;

typedef struct Nodo_Arvore {
    char documento[30];
    int qtd;
    float tfidf;
    struct Nodo_Arvore *esq;
    struct Nodo_Arvore *dir;
} Nodo_Arvore;

typedef struct Termo {
    char palavra[30];
    unsigned int qtd;
    int nt;
    struct Termo *prox;
} Termo;

typedef struct Valores {
    char documento[30];
    double qtd_termo;
    double tfidf;
    struct Valores *prox;
} Valores;

// Hash
int calcula_tamanho_definitivo(char *nome_arquivo);
int string_hash(char *s, int m);
void toLowerCase(char *str);

// Freq
Nodo *insere_lista(Nodo *N, char *palavra);
Nodo *busca_item_lista(Nodo *N, char *palavra);
Nodo *criar_nodo_maior(const char *palavra, int qtd);
Nodo *inserir_lista_maior(Nodo *lista, const char *palavra, int qtd);
int contar_nodos(Nodo *lista);
void remover_ultimo_nodo(Nodo **lista);
Nodo *encontrar_maiores(Nodo *tabela[], int tamanho_tabela, int n);
void exibe_lista(Nodo *N);
void liberar_lista(Nodo *lista);
int eh_primo(int n);
int prox_primo(int n);

// Search
Nodo_Arvore *criar_nodo_arvore(char *documento, float tfidf);
Nodo_Arvore *inserir_na_arvore(Nodo_Arvore *raiz, Nodo_Arvore *nodo);
void imprimir_em_ordem_decrescente(Nodo_Arvore *raiz);
void liberar_arvore(Nodo_Arvore *raiz);
Termo *criar_nodo_termo();
void insere_lista_termo(Termo **N, char *palavra, int nt);
Termo *busca_item_lista_termo(Termo *N, char *palavra);
void liberar_lista_termo(Termo *lista);
void liberar_tabela_termo(Termo **tabela, int tamanho);

// Opções
void freq(char *arquivocmd, int num_palavras);
int freq_word(char *palavra, char *arquivocmd);
int search(int argc, char *argv[]);


// ============================= PROGRAMA PRINCIPAL ========================================

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "Portuguese");

    char *caminho;
    char *opcao = argv[1];

    caminho = argv[argc-1];


    if(strcmp(opcao, "--freq") == 0) freq(argv[argc-1], atoi(argv[2]));
    else if(strcmp(opcao, "--freq-word")==0) freq_word(argv[2], argv[argc-1]);
    else if(strcmp(opcao, "--search")==0) search(argc, argv);
    else printf("Opção inválida!!");
    return 0;

}

// ====================== FUNÇÕES DO NODO ==================================================

Nodo *insere_lista(Nodo *N, char *palavra) {
    Nodo *novo = (Nodo *)malloc(sizeof(Nodo));
    strcpy(novo->palavra, palavra);
    novo->qtd = 1;
    novo->prox = N;
    return novo;
}

Nodo *busca_item_lista(Nodo *N, char *palavra) {
    Nodo *p = N;
    while (p != NULL) {
        if (strcmp(p->palavra, palavra) == 0) {
            return p;
        }
        p = p->prox;
    }
    return NULL;
}

Nodo *criar_nodo_maior(const char *palavra, int qtd) {
    Nodo *novo = (Nodo *)malloc(sizeof(Nodo));
    if (novo != NULL) {
        strcpy(novo->palavra, palavra);
        novo->qtd = qtd;
        novo->prox = NULL;
    }
    return novo;
}

Nodo *inserir_lista_maior(Nodo *lista, const char *palavra, int qtd) {
    Nodo *novo = criar_nodo_maior(palavra, qtd);
    novo->prox=lista;
    lista=novo;
    return novo;
}

int contar_nodos(Nodo *lista) {
    int count = 0;
    while (lista != NULL) {
        count++;
        lista = lista->prox;
    }
    return count;
}

void remover_ultimo_nodo(Nodo **lista) {
    if (*lista == NULL) {
        return;
    }

    if ((*lista)->prox == NULL) {
        free(*lista);
        *lista = NULL;
        return;
    }

    Nodo *atual = *lista;
    Nodo *anterior = NULL;

    while (atual->prox != NULL) {
        anterior = atual;
        atual = atual->prox;
    }

    free(atual);
    anterior->prox = NULL;
}

Nodo *encontrar_maiores(Nodo *tabela[], int tamanho_tabela, int n) {
    Nodo *maiores = NULL;

    for (int i = 0; i < tamanho_tabela; i++) {
        Nodo *lista = tabela[i];
        while (lista != NULL) {
            Nodo *atual = maiores;
            Nodo *anterior = NULL;

                // ENCONTRAR A POSIÇÃO CORRETA
            while (atual != NULL && atual->qtd >= lista->qtd) {
                anterior = atual;
                atual = atual->prox;
            }
            // INSERE NA POS CORRETA
            Nodo *novo = criar_nodo_maior(lista->palavra, lista->qtd);
            if (novo != NULL) {
                if (anterior == NULL) {
                    // INICIO
                    novo->prox = maiores;
                    maiores = novo;
                } else {
                    // MEIO OU FIM
                    novo->prox = atual;
                    anterior->prox = novo;
                }
            }

            // REMOVE ULTIMO NODO
            if (contar_nodos(maiores) > n) remover_ultimo_nodo(&maiores);

            lista = lista->prox;
        }
    }

    return maiores;
}

void exibe_lista(Nodo *N) {
    Nodo *aux = N;
    while (aux != NULL) {
        printf("%s %d\n", aux->palavra, aux->qtd);
        aux = aux->prox;
    }
}

void liberar_lista(Nodo *lista) {
    Nodo *atual = lista;
    while (atual != NULL) {
        Nodo *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

int eh_primo(int n) {
    int i;
    if (n <= 1)
        return 0;
    if (n <= 3)
        return 1;
    if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0)
        return 0;
    for (i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }
    return 1;
}

int prox_primo(int n) {
    while (1) {
        n++;
        if (eh_primo(n)) {
            return n;
        }
    }
}

int calcula_tamanho_definitivo(char *nome_arquivo) {
    long num_palavras;
    long tamanho_arquivo_kbytes;
    int tamanho_inicial_tabela;
    int tamanho_tabela_primo;

    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        printf("Erro: não foi possível abrir o arquivo: '%s'\n", nome_arquivo);
        return 1;
    }

    fseek(arquivo, 0, SEEK_END);
    tamanho_arquivo_kbytes = ftell(arquivo) / 1024;

    fclose(arquivo);

    num_palavras = (long)((tamanho_arquivo_kbytes * 170));
    tamanho_inicial_tabela = (int)(num_palavras / 3);
    tamanho_tabela_primo = prox_primo(tamanho_inicial_tabela);

    return tamanho_tabela_primo;
}

int string_hash(char *s, int m) {
    char c;
    int p = 31;
    int p_pow = 1;
    unsigned int hash_value = 0;

    while ((c = *s++)) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return (hash_value);
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}


// ============ --freq =========================================

void freq(char *arquivocmd, int num_palavras) {

    int qtd_palavras = num_palavras;
    int hash;
    int tamanho = 0;
    tamanho = calcula_tamanho_definitivo(arquivocmd);
    printf("Tamanho da tabela hash: %d\n", tamanho);

    const char delim[] = " !\"#$%&'()*\"\'+,-./:;<=>?@[\\]^_`{|}~\v\t\n\r0123456789\0";
    char buffer[1024*1024];

    Nodo **tabela, **palavras; // ARRAY DE PONTEIROS DE NODOS / TABELA HASH
    tabela = (Nodo **)malloc(tamanho * sizeof(Nodo *));
    Nodo *aux;
    palavras = (Nodo **)malloc(qtd_palavras * sizeof(Nodo));

    if (palavras == NULL) {
        fprintf(stderr, "Erro ao alocar memória para palavras\n");
        return;
    }

    int fd = open(arquivocmd, O_RDONLY);

    if (fd == -1) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    ssize_t n;

    for (int j = 0; j < tamanho; j++) {
        tabela[j] = NULL;
    }

    lseek(fd, 0, SEEK_SET);
    while ((n = read(fd, buffer, sizeof(buffer))) > 0)
    {
        char *rest = buffer;
        toLowerCase(rest);
        for (ssize_t i = 0; i < n; i++)
        {
            if (strchr(delim, buffer[i]) != NULL)
            {
                buffer[i] = '\0';
                if (strlen(rest) > 2) {
                    hash = string_hash(rest, tamanho);
                    aux = busca_item_lista(tabela[hash], rest);
                    if (aux != NULL) {
                        aux->qtd++;
                    } else {
                        tabela[hash] = inserir_lista_maior(tabela[hash], rest, 1);
                    }
                }
                rest = buffer + i + 1;
            }
        }
    }

    Nodo *maiores = encontrar_maiores(tabela, tamanho, qtd_palavras);
    exibe_lista(maiores);

    for (int i = 0; i < tamanho; i++) {
        liberar_lista(tabela[i]);
    }

    free(tabela);

    for (int i = 0; i < qtd_palavras; i++) {
        free(palavras[i]);
    }
    free(palavras);

    liberar_lista(maiores);

    close(fd);

    return;
}


//  ============ --freq-word ====================================

int freq_word(char *palavra, char *arquivocmd)
{
    int contador = 0;
    int fd = open(arquivocmd, O_RDONLY);
    char *palavra_busca = palavra;
    char buffer[1024*1024];
    const char delim[] = " !\"#$%&'()*\"\'+,-./:;<=>?@[\\]^_`{|}~\v\t\n\r0123456789\0"; // DELIMITADORES
    ssize_t n;

    if (fd == -1) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    toLowerCase(palavra_busca);
    lseek(fd, 0, SEEK_SET);

    while ((n = read(fd, buffer, sizeof(buffer))) > 0)
    {
        char *rest = buffer;
        toLowerCase(rest);

        for (ssize_t i = 0; i < n; i++)
        {
            if (strchr(delim, buffer[i]) != NULL) {
                buffer[i] = '\0';
                if (strcmp(rest, palavra_busca) == 0 && strlen(rest) > 2) {contador++;}
                rest = buffer + i + 1;
            }
        }
    }

    close(fd);
    printf("%s: %d", palavra_busca, contador);

    return 0;
}


// ============ --search ========================================
Nodo_Arvore *criar_nodo_arvore(char *documento, float tfidf) {
    Nodo_Arvore *novo = (Nodo_Arvore *)malloc(sizeof(Nodo_Arvore));
    if (novo == NULL) {
        perror("Error allocating memory for Nodo");
        exit(EXIT_FAILURE);
    }

    strcpy(novo->documento, documento);
    novo->tfidf = tfidf;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

Nodo_Arvore *inserir_na_arvore(Nodo_Arvore *raiz, Nodo_Arvore *nodo) {
    if (raiz == NULL) {
        return nodo;
    }

    if (nodo->tfidf >= raiz->tfidf) {
        raiz->dir = inserir_na_arvore(raiz->dir, nodo);
    } else {
        raiz->esq = inserir_na_arvore(raiz->esq, nodo);
    }

    return raiz;
}

void imprimir_em_ordem_decrescente(Nodo_Arvore *raiz) {
    if (raiz != NULL) {
        imprimir_em_ordem_decrescente(raiz->dir);
        printf("Documento %s: TF-IDF: %.30f\n", raiz->documento, raiz->tfidf);
        imprimir_em_ordem_decrescente(raiz->esq);
    }
}

void liberar_arvore(Nodo_Arvore *raiz) {
    if (raiz != NULL) {
        liberar_arvore(raiz->esq);
        liberar_arvore(raiz->dir);
        free(raiz);
    }
}

Termo *criar_nodo_termo() {
    return NULL;
}

void insere_lista_termo(Termo **N, char *palavra, int nt) {
    Termo *novo = (Termo *)malloc(sizeof(Termo));
    if (novo == NULL) {
        perror("Error allocating memory for new termo");
        exit(EXIT_FAILURE);
    }

    strcpy(novo->palavra, palavra);
    novo->qtd = 1;
    novo->nt = nt;
    novo->prox = *N;
    *N = novo;
}

Termo *busca_item_lista_termo(Termo *N, char *palavra) {
    Termo *p = N;
    while (p != NULL) {
        if (strcmp(p->palavra, palavra) == 0) {
            return p;
        }
        p = p->prox;
    }
    return NULL;
}

void liberar_lista_termo(Termo *lista) {
    Termo *atual = lista;
    while (atual != NULL) {
        Termo *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

void liberar_tabela_termo(Termo **tabela, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        liberar_lista_termo(tabela[i]);
    }
    free(tabela);
}

int search(int argc, char *argv[]) {
    int qtd_arquivos = argc - 3;
    int tamanho = prox_primo(qtd_arquivos / 4);
    int nt = 0;
    int *qtd_por_termo = NULL;
    char delim[] = " !\"#$%&'()*\"\'+,-./:;<=>?@[\\]^_`{|}~\v\t\n\r0123456789";
    char *token = strtok(argv[2], delim);
    Nodo_Arvore *arvore = NULL;
    Termo **termo;
    termo = (Termo **)malloc(tamanho * sizeof(Termo *));
    Valores **array_tamanho = (Valores **)malloc(qtd_arquivos * sizeof(Valores *));

    if (termo == NULL || array_tamanho == NULL) {
        perror("Error allocating memory for termo or array_tamanho");
        return EXIT_FAILURE;
    }


    if (token == NULL) {
        fprintf(stderr, "Error: Invalid input for stopwords_file\n");
        free(termo);
        free(array_tamanho);
        return EXIT_FAILURE;
    }

    for (int j = 0; j < qtd_arquivos; j++) {
        termo[j] = criar_nodo_termo();
        array_tamanho[j] = (Valores *)malloc(sizeof(Valores));
        if (array_tamanho[j] == NULL) {
            perror("Error allocating memory for array_tamanho[j]");
            exit(EXIT_FAILURE);
        }
    }

    while (token != NULL) {
        if (strlen(token) > 2) {
            toLowerCase(token);
            int hash = string_hash(token, tamanho);
            insere_lista_termo(&termo[hash], token, nt);
            nt++;
        }
        token = strtok(NULL, delim);
    }

    qtd_por_termo = (int*) malloc (sizeof(int*)*nt);
    for(int i=0; i<nt; i++){
        qtd_por_termo[i] = 0;
    }

    for (int a = 3; a < argc; a++) {
        unsigned int contador_pal = 0;
        int fd = open(argv[a], O_RDONLY);
        double soma_tf = 0.0;

        if (fd == -1) {
            perror("Error opening file");
            return EXIT_FAILURE;
        }

        char buffer[1024 * 1024];
        const char delim[] = " !\"#$%&'()*\"\'+,-./:;<=>?@[\\]^_`{|}~\v\t\n\r";

        ssize_t n;

        while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
            buffer[n] = '\0';
            char *rest = buffer;
            toLowerCase(rest);
            Termo *busca;

            for (ssize_t i = 0; i < n; i++) {
                if (strchr(delim, buffer[i]) != NULL) {
                    buffer[i] = '\0';
                    int hash = string_hash(rest, tamanho);
                    busca = busca_item_lista_termo(termo[hash], rest);
                    if (busca != NULL) {
                        busca->qtd++;
                    }
                    contador_pal++;
                    rest = buffer + i + 1;
                }
            }
        }

        close(fd);

        for (int i = 0; i < tamanho; i++) {
            Termo *p = termo[i];

            while (p != NULL) {
                if(p->qtd>0){
                    qtd_por_termo[p->nt]++;
                    double tf = (double)p->qtd / contador_pal;
		    printf("tf: %.30f\n", tf);
                    if(soma_tf>0) soma_tf = soma_tf * tf;
                    else soma_tf = soma_tf + tf;
                    p->qtd = 0;
                }
                    p = p->prox;
            }
            array_tamanho[a - 3]->qtd_termo = soma_tf;
        }
	printf("soma_tf: %.30f\n", soma_tf);
    }
	

    printf("%d\n\n", qtd_arquivos);
    for(int i=0; i<qtd_arquivos; i++){
        double soma_idf = 0.0;
	for(int j=0; j<nt; j++){
            double idf;
            idf = log(qtd_arquivos/(double)qtd_por_termo[j]);
	    printf("idf: %.30f\n", idf);
            if(soma_idf > 0) soma_idf = soma_idf * idf;
            else soma_idf = soma_idf + idf;
        }
        array_tamanho[i]->tfidf = (array_tamanho[i]->qtd_termo * soma_idf)/nt;
        printf("soma_idf: %.30f\n", soma_idf);
	printf("tfidf: %.30f\n", array_tamanho[i]->tfidf);
	soma_idf = 0.0;
    }
	
    arvore = criar_nodo_arvore(argv[3], array_tamanho[0]->tfidf);

    for(int i=1; i<qtd_arquivos; i++){
        Nodo_Arvore *p = criar_nodo_arvore(argv[3+i], array_tamanho[i]->tfidf);
        inserir_na_arvore(arvore, p);
    }

    imprimir_em_ordem_decrescente(arvore);

    for (int j = 0; j < qtd_arquivos; j++) {
        free(array_tamanho[j]);
    }
    free(array_tamanho);

    liberar_tabela_termo(termo, tamanho);
    liberar_arvore(arvore);

    return 0;
}
