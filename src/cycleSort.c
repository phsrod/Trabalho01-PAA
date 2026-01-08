/*  ============================ BIBLIOTECAS ===========================*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*  ============================ CONSTANTES ===========================*/
#define TAM_VETOR 20000        /* Altere para 40000 ou 60000 */
#define REPETICOES 10       /* Altere a quantidade de repetições */
#define TAM_MAX 100000        /* valor máximo dos elementos do vetor */

/* ================= FUNÇÕES AUXILIARES ================= */
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #define limpar_tela() system("cls")
#else
    #define limpar_tela() system("clear")
#endif

void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar(); // consome \n pendente
    getchar(); // espera ENTER
}

int confirmar(const char *mensagem) {
    char op;
    printf("\n%s (s/n): ", mensagem);
    scanf(" %c", &op);
    return (op == 's' || op == 'S');
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

/* ================= FUNÇÃO SHUFFLE ================= */
void shuffle(int v[], int n) {
    int i, j, temp;
    for (i = n - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
}

/* ================= GERAÇÃO DE VETORES ================= */
void gerarVetor(int v[], int tipo) {
    int i;
    
    if (tipo == 1) {
        /* CRESCENTE - números crescentes dentro de TAM_MAX */
        int valor = rand() % (TAM_MAX / 10);  // Começa baixo
        
        for (i = 0; i < TAM_VETOR; i++) {
            v[i] = valor;
            // Incremento controlado para não ultrapassar TAM_MAX
            int incremento = 1 + (rand() % (TAM_MAX / TAM_VETOR));
            valor += incremento;
            
            // Garante que não ultrapasse TAM_MAX
            if (valor > TAM_MAX) {
                valor = TAM_MAX - (rand() % (TAM_MAX / 10));
            }
        }
    }
    else if (tipo == 2) {
        /* DECRESCENTE - números decrescentes dentro de TAM_MAX */
        // Começa com valor alto, mas dentro de TAM_MAX
        int valor = TAM_MAX - (rand() % (TAM_MAX / 10));
        
        for (i = 0; i < TAM_VETOR; i++) {
            v[i] = valor;
            // Decremento controlado
            int decremento = 1 + (rand() % (TAM_MAX / TAM_VETOR));
            valor -= decremento;
            
            // Garante que não fique negativo
            if (valor < 0) {
                valor = rand() % (TAM_MAX / 10);
            }
        }
    }
    else if (tipo == 3) {
        /* ALEATÓRIO - dentro de TAM_MAX */
        for (i = 0; i < TAM_VETOR; i++) {
            v[i] = rand() % TAM_MAX;
        }
        
        // Embaralha o vetor para garantir aleatoriedade, pode colocar em laço se desejar mais aleatoriedade
        shuffle(v, TAM_VETOR);
    }
}


/* ================= CYCLE SORT ================= */
void cycleSort(int v[], long *comparacoes, long *trocas) {
    *comparacoes = 0;
    *trocas = 0;

    for (int ciclo_inicio = 0; ciclo_inicio < TAM_VETOR - 1; ciclo_inicio++){
        int item = v[ciclo_inicio];
        int pos = ciclo_inicio;
        
        for (int j = ciclo_inicio + 1; j < TAM_VETOR; j++){
            (*comparacoes)++;
            if (v[j] < item){
                pos++;
            }
        }
        
        if (pos != ciclo_inicio){
            while (item == v[pos]){
                pos++;
                (*comparacoes)++;
            }
            
            int temp = v[pos];
            v[pos] = item;
            item = temp;
            (*trocas)++;
            
            while (pos != ciclo_inicio){
                pos = ciclo_inicio;
                
                for (int j = ciclo_inicio + 1; j < TAM_VETOR; j++){
                    (*comparacoes)++;
                    if (v[j] < item){
                        pos++;
                    }
                }
                
                while (pos < TAM_VETOR && item == v[pos]){
                    pos++;
                    (*comparacoes)++;
                }
                
                temp = v[pos];
                v[pos] = item;
                item = temp;
                (*trocas)++;
            }
        }
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
void salvarResultados(char tipo[], double tempos[], long comparacoes[], long trocas[], double mediaTempo, double desvioTempo, double mediaComparacoes, double mediaTrocas) {

    FILE *arquivo;
    time_t agora;
    struct tm *info;
    char nomeArquivo[150];
    int i;

    time(&agora);
    info = localtime(&agora);

    sprintf(nomeArquivo,
            "../results/cyclesort_%s_%02d-%02d-%04d_%02d-%02d-%02d.txt", tipo, info->tm_mday, info->tm_mon + 1, info->tm_year + 1900, info->tm_hour, info->tm_min, info->tm_sec);

    arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de resultados.\n");
        return;
    }

    /* ================= CABECALHO ================= */
    fprintf(arquivo, "============================================================\n");
    fprintf(arquivo, "                        CYCLE SORT                          \n");
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

    limpar_tela();
    printf("=====================================\n");
    printf("         CYCLE SORT - MENU            \n");
    printf("=====================================\n");
    printf(" 1 - Vetor Crescente\n");
    printf(" 2 - Vetor Decrescente\n");
    printf(" 3 - Vetor Aleatório\n");
    printf(" 0 - Sair\n");
    printf("=====================================\n");
    printf(" Escolha uma opção: ");
    scanf("%d", &opcao);

    return opcao;
}

void processarResultados(int opcao, double tempos[], long comparacoes[], long trocas[]) {

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

    printf("\nResultados salvos com sucesso!\n");
    pausar();
}

void executarExperimento(int opcao, double tempos[], long comparacoes[], long trocas[]) {

    int vetor[TAM_VETOR];
    struct timespec inicio, fim;

    for (int i = 0; i < REPETICOES; i++) {

        gerarVetor(vetor, opcao);
        
        printf("=====================================\n");
        printf(" Execução %d\n", i + 1);
        printf("=====================================\n");

        printf("Vetor antes da ordenação:\n");
        imprimirVetor(vetor);
        
        clock_gettime(CLOCK_MONOTONIC, &inicio);
        cycleSort(vetor, &comparacoes[i], &trocas[i]);
        clock_gettime(CLOCK_MONOTONIC, &fim);
        
        tempos[i] = medirTempo(inicio, fim);
        
        printf("\nVetor após a ordenação:\n");
        imprimirVetor(vetor);
    }
}

void executarOpcao(int opcao) {

    double tempos[REPETICOES];
    long comparacoes[REPETICOES];
    long trocas[REPETICOES];

    limpar_tela();
    printf("Opção selecionada: %d\n", opcao);

    if (confirmar("Deseja iniciar a execução?")) {
        executarExperimento(opcao, tempos, comparacoes, trocas);
        processarResultados(opcao, tempos, comparacoes, trocas);
    }
}


int main() {
    int opcao;
    int executando = 1;

    srand(time(NULL));

    while (executando) {

        opcao = menu();

        switch (opcao) {

            case 0:
                limpar_tela();
                printf("Encerrando o programa...\n");
                pausar();
                executando = 0;
                break;

            case 1:
            case 2:
            case 3:
                executarOpcao(opcao);
                break;

            default:
                limpar_tela();
                printf("Opção inválida!\n");
                pausar();
        }
    }

    return 0;
}