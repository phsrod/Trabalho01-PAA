/*  ============================ BIBLIOTECAS ===========================*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*  ============================ CONSTANTES ===========================*/
#define TAM_VETOR 10        /* Altere para 40000 ou 60000 */
#define REPETICOES 10       /* Altere a quantidade de repetições */
#define TAM_MAX 1000000  /* valor máximo dos elementos do vetor */
#define SALTO_MAX 50      /* valor máximo do salto entre elementos */

/* ================= FUNÇÕES AUXILIARES ================= */
void copiarVetor(int origem[], int destino[]) {
    int i;
    for (i = 0; i < TAM_VETOR; i++)
        destino[i] = origem[i];
}

/* ================= FUNÇÃO DE IMPRESSÃO ================= */
void imprimirVetor(int v[]) {
    int i;
    int limite = 50;

    if (TAM_VETOR < limite)
        limite = TAM_VETOR;

    for (i = 0; i < limite; i++)
        printf("%d ", v[i]);

    if (TAM_VETOR > limite)
        printf("...");

    printf("\n");
}

/* ======================= GERAR BASE DE NÚMEROS =================*/
int gerarValorAleatorio() {
    return rand() % TAM_MAX;
}


/* ================= GERAÇÃO DE VETORES ================= */
void gerarVetor(int v[], int tipo) {
    int i;
    int valor;

    /* Primeiro elemento: sempre aceita */
    v[0] = gerarValorAleatorio();

    /* Preenche o restante */
    for (i = 1; i < TAM_VETOR; i++) {

        valor = gerarValorAleatorio();

        if (tipo == 1) {
            /* Crescente */
            while (valor < v[i - 1]) {
                valor = gerarValorAleatorio();
            }
        }
        else if (tipo == 2) {
            /* Decrescente */
            while (valor > v[i - 1]) {
                valor = gerarValorAleatorio();
            }
        }
        /* Aleatório não tem restrição */

        v[i] = valor;
    }
}


/* ================= SHELL SORT (KNUTH 3k+1) ================= */
void shellSort(int v[], long *comparacoes, long *trocas) {
    int i, j, aux;
    int continuar;
    float k;
    int h;

    *comparacoes = 0;
    *trocas = 0;

    k = log(TAM_VETOR + 1) / log(3);
    k = floor(k + 0.5);
    h = (pow(3, k) - 1) / 2;

    while (h >= 1) {
        for (i = h; i < TAM_VETOR; i++) {
            aux = v[i];
            j = i;
            continuar = 1;

            while (j >= h && continuar) {
                (*comparacoes)++;
                if (v[j - h] > aux) {
                    v[j] = v[j - h];
                    (*trocas)++;
                    j = j - h;
                } else {
                    continuar = 0;
                }
            }
            v[j] = aux;
        }
        h = (h - 1) / 3;
    }
}

/* ================= TEMPO ====================*/
double medirTempo(struct timespec inicio, struct timespec fim) {
    double segundos, nanos;
    segundos = fim.tv_sec - inicio.tv_sec;
    nanos = fim.tv_nsec - inicio.tv_nsec;
    return segundos * 1000.0 + nanos / 1000000.0;
}

/* ================= ESTATÍSTICAS =============*/
double calcularMedia(double tempos[]) {
    double soma = 0.0;
    int i;
    for (i = 0; i < REPETICOES; i++)
        soma += tempos[i];
    return soma / REPETICOES;
}

double calcularDesvioPadrao(double tempos[], double media) {
    double soma = 0.0;
    int i;
    for (i = 0; i < REPETICOES; i++)
        soma += pow(tempos[i] - media, 2);
    return sqrt(soma / REPETICOES);
}

double calcularMediaComparacoes(long comparacoes[]) {
    long soma = 0;
    int i;

    for (i = 0; i < REPETICOES; i++) {
        soma += comparacoes[i];
    }

    return (double)soma / REPETICOES;
}

double calcularMediaTrocas(long trocas[]) {
    long soma = 0;
    int i;

    for (i = 0; i < REPETICOES; i++) {
        soma += trocas[i];
    }

    return (double)soma / REPETICOES;
}

/* ================= ARQUIVO ================= */
void salvarResultados(char tipo[],
                      double tempos[],
                      long comparacoes[],
                      long trocas[],
                      double mediaTempo,
                      double desvioTempo,
                      double mediaComparacoes,
                      double mediaTrocas) {

    FILE *arquivo;
    time_t agora;
    struct tm *info;
    char nomeArquivo[150];
    int i;

    time(&agora);
    info = localtime(&agora);

    sprintf(nomeArquivo,
            "../results/shellsort_%s_%02d-%02d-%04d_%02d-%02d-%02d.txt",
            tipo,
            info->tm_mday,
            info->tm_mon + 1,
            info->tm_year + 1900,
            info->tm_hour,
            info->tm_min,
            info->tm_sec);

    arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de resultados.\n");
        return;
    }

    /* ================= CABECALHO ================= */
    fprintf(arquivo, "============================================================\n");
    fprintf(arquivo, "               SHELL SORT - SEQUENCIA DE KNUTH               \n");
    fprintf(arquivo, "============================================================\n\n");

    fprintf(arquivo, "Configuracoes do experimento:\n");
    fprintf(arquivo, "------------------------------------------------------------\n");
    fprintf(arquivo, "Tamanho do vetor : %d\n", TAM_VETOR);
    fprintf(arquivo, "Repeticoes       : %d\n", REPETICOES);
    fprintf(arquivo, "Tipo de vetor    : %s\n\n", tipo);

    /* ================= RESULTADOS POR EXECUCAO ================= */
    fprintf(arquivo, "Resultados individuais:\n");
    fprintf(arquivo, "------------------------------------------------------------\n");

    for (i = 0; i < REPETICOES; i++) {
        fprintf(arquivo, "Execucao %2d\n", i + 1);
        fprintf(arquivo, "  Tempo        : %8.3f ms\n", tempos[i]);
        fprintf(arquivo, "  Comparacoes  : %8ld\n", comparacoes[i]);
        fprintf(arquivo, "  Trocas       : %8ld\n\n", trocas[i]);
    }

    /* ================= ESTATISTICAS ================= */
    fprintf(arquivo, "Resumo estatistico:\n");
    fprintf(arquivo, "------------------------------------------------------------\n");
    fprintf(arquivo, "Tempo medio           : %.3f ms\n", mediaTempo);
    fprintf(arquivo, "Desvio padrao (tempo) : %.3f ms\n\n", desvioTempo);

    fprintf(arquivo, "Media de comparacoes  : %.2f\n", mediaComparacoes);
    fprintf(arquivo, "Media de trocas       : %.2f\n", mediaTrocas);

    fprintf(arquivo, "\n============================================================\n");

    fclose(arquivo);

    printf("\nResultados salvos em: %s\n", nomeArquivo);
}


/* ================= MENU ================= */
int menu() {
    int opcao;

    printf("\n=== SHELL SORT ===\n");
    printf("1 - Vetor Crescente\n");
    printf("2 - Vetor Decrescente\n");
    printf("3 - Vetor Aleatorio\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &opcao);

    return opcao;
}

/* ================= MAIN ================= */
int main() {
    int base[TAM_MAX];
    int vetor[TAM_VETOR];

    double tempos[REPETICOES];
    long comparacoes[REPETICOES];
    long trocas[REPETICOES];

    struct timespec inicio, fim;

    int opcao;
    int i;

    srand(time(NULL));

    opcao = menu();
    if (opcao == 0)
        return 0;

    for (i = 0; i < REPETICOES; i++) {
        gerarVetor(vetor, opcao);

        printf("\nExecução %d:\n", i + 1);
        printf("Vetor antes da ordenacao:\n");
        imprimirVetor(vetor);

        clock_gettime(CLOCK_MONOTONIC, &inicio);
        shellSort(vetor, &comparacoes[i], &trocas[i]);
        clock_gettime(CLOCK_MONOTONIC, &fim);

        tempos[i] = medirTempo(inicio, fim);

        printf("\nVetor apos a ordenacao:\n");
        imprimirVetor(vetor);
    }

    double media = calcularMedia(tempos);
    double desvio = calcularDesvioPadrao(tempos, media);
    double mediaComparacoes = calcularMediaComparacoes(comparacoes);
    double mediaTrocas = calcularMediaTrocas(trocas);

    if (opcao == 1)
        salvarResultados("crescente", tempos, comparacoes, trocas, media, desvio, mediaComparacoes, mediaTrocas);
    else if (opcao == 2)
        salvarResultados("decrescente", tempos, comparacoes, trocas, media, desvio, mediaComparacoes, mediaTrocas);
    else
        salvarResultados("aleatorio", tempos, comparacoes, trocas, media, desvio, mediaComparacoes, mediaTrocas);

    return 0;
}
