/*  ============================ BIBLIOTECAS ===========================*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*  ============================ CONSTANTES ===========================*/
#define TAM_VETOR 10        /* Altere para 40000 ou 60000 */
#define REPETICOES 10       /* Altere a quantidade de repetições */
#define INICIO_INTERVALO 1  /* Altere o início do intervalo */
#define FIM_INTERVALO 10  /* Altere o fim do intervalo */

/*  ============================ PROTÓTIPOS ===========================*/

/* ==================== VERIFICAÇÃO DO INTERVALO ======================*/
int verificarIntervalo(){
    int ehValido = 1, tamanhoIntervalo = FIM_INTERVALO - INICIO_INTERVALO + 1;
    if (tamanhoIntervalo != TAM_VETOR){
        printf("Erro: O tamanho do intervalo (%d) não corresponde ao tamanho do vetor (%d).\n", tamanhoIntervalo, TAM_VETOR);
        ehValido = 0;
    }
    return ehValido;
}

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

/* ================= GERAÇÃO DE VETORES ================= */
void gerarCrescente(int v[]) {
    int i;
    int valor = INICIO_INTERVALO;

    for (i = 0; i < TAM_VETOR; i++) {
        v[i] = valor;
        valor++;
    }
}

void gerarDecrescente(int v[]) {
    int i;
    int valor = FIM_INTERVALO;

    for (i = 0; i < TAM_VETOR; i++) {
        v[i] = valor, valor--;
    }
}

void gerarAleatorio(int v[]) {
    int i;
    int j;
    int temp;

    gerarCrescente(v);

    /* Fisher-Yates Shuffle */
    for (i = TAM_VETOR - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
}

/* ================= SHELL SORT (KNUTH 3k+1) ================= */
// void shellSort(int v[], long *comparacoes, long *trocas) {
//     int gap = 1;
//     int i, j, temp;
//     int continuar;

//     *comparacoes = 0;
//     *trocas = 0;

//     while (gap < TAM_VETOR / 3)
//         gap = 3 * gap + 1;

//     while (gap >= 1) {
//         for (i = gap; i < TAM_VETOR; i++) {
//             temp = v[i];
//             j = i;
//             continuar = 1;

//             while (j >= gap && continuar) {
//                 (*comparacoes)++;
//                 if (v[j - gap] > temp) {
//                     v[j] = v[j - gap];
//                     (*trocas)++;
//                     j = j - gap;
//                 } else {
//                     continuar = 0;
//                 }
//             }
//             v[j] = temp;
//         }
//         gap = gap / 3;
//     }
// }

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
double medirTempo(struct timespec inicio, struct timespec fim){
    double segundos, nanos;
    segundos = fim.tv_sec - inicio.tv_sec;
    nanos = fim.tv_nsec - inicio.tv_nsec;
    return segundos * 1000.0 + nanos / 1000000.0;
}

/* ================= ESTATÍSTICAS =============*/
double calcularMedia(double tempos[]){
    double soma = 0.0;
    for (int i = 0; i < REPETICOES; i++){
        soma += tempos[i];
    }
    return soma / REPETICOES;
}

double calcularDesvioPadrao(double tempos[], double media){
    double soma = 0.0;
    for (int i = 0; i < REPETICOES; i++){
        soma += pow(tempos[i] - media, 2);
    }
    return sqrt(soma / REPETICOES);
}

/* ================= ARQUIVO ================= */
void salvarResultados(char tipo[], double tempos[], long comparacoes[], long trocas[], double media, double desvio){
    FILE *arquivo;
    time_t agora;
    struct tm *info;
    char nomeArquivo[100];
    int i;
    time(&agora);
    info = localtime(&agora);

    sprintf(nomeArquivo, "../results/shellsort_%s_%02d-%02d-%04d_%02d-%02d-%02d.txt", tipo, info->tm_mday, info->tm_mon + 1, info->tm_year + 1900, info->tm_hour, info->tm_min, info->tm_sec);
    arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de resultados.\n");
        return;
    } else{
        //fprintf(arquivo, "Shell Sort - Sequencia de Knuth (3k+1)\n");
        fprintf(arquivo, "Shell Sort - Sequencia de Knuth Clássica\n");
        fprintf(arquivo, "Tamanho do vetor: %d\n", TAM_VETOR);
        fprintf(arquivo, "Intervalo: [%d, %d]\n", INICIO_INTERVALO, FIM_INTERVALO);
        fprintf(arquivo, "Repeticoes: %d\n\n", REPETICOES);

        for (i = 0; i < REPETICOES; i++) {
        fprintf(arquivo, "Execucao %d\n", i + 1);
        fprintf(arquivo, "Tempo: %.3f ms\n", tempos[i]);
        fprintf(arquivo, "Comparacoes: %ld\n", comparacoes[i]);
        fprintf(arquivo, "Trocas: %ld\n\n", trocas[i]);
        }
        fprintf(arquivo, "Tempo medio: %.3f ms\n", media);
        fprintf(arquivo, "Desvio padrao: %.3f ms\n", desvio);

        fclose(arquivo);

        printf("\nResultados salvos em: %s\n", nomeArquivo);
    }  
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
    int vetorOriginal[TAM_VETOR];
    int vetorTeste[TAM_VETOR];

    double tempos[REPETICOES];
    long comparacoes[REPETICOES];
    long trocas[REPETICOES];

    struct timespec inicio, fim;

    int opcao;
    int i;

    if (!verificarIntervalo()){
        return 0;
    }
    
    srand(time(NULL));

    opcao = menu();
    if (opcao == 0)
        return 0;

    if (opcao == 1)
        gerarCrescente(vetorOriginal);
    else if (opcao == 2)
        gerarDecrescente(vetorOriginal);
    else if (opcao == 3)
        gerarAleatorio(vetorOriginal);
    else
        return 0;

    printf("\nVetor antes da ordenacao:\n");
    imprimirVetor(vetorOriginal);

    for (i = 0; i < REPETICOES; i++) {
        copiarVetor(vetorOriginal, vetorTeste);

        clock_gettime(CLOCK_MONOTONIC, &inicio);
        shellSort(vetorTeste, &comparacoes[i], &trocas[i]);
        clock_gettime(CLOCK_MONOTONIC, &fim);

        tempos[i] = medirTempo(inicio, fim);
    }

    printf("\nVetor apos a ordenacao:\n");
    imprimirVetor(vetorTeste);

    double media = calcularMedia(tempos);
    double desvio = calcularDesvioPadrao(tempos, media);

    if (opcao == 1)
        salvarResultados("crescente", tempos, comparacoes, trocas, media, desvio);
    else if (opcao == 2)
        salvarResultados("decrescente", tempos, comparacoes, trocas, media, desvio);
    else
        salvarResultados("aleatorio", tempos, comparacoes, trocas, media, desvio);

    return 0;
}




