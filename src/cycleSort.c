/*  ============================ BIBLIOTECAS ===========================*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

/*  ============================ CONSTANTES ===========================*/
#define TAM_VETOR 20000 /* Usado apenas nas opções 1-3 do menu */
#define REPETICOES 10   /* Número total de repetições */
#define QTD_DESCARTES 1 /* Quantidade de repetições descartadas (normalmente 1 para warm-up) */
#define TAM_MAX 100000  /* valor máximo dos elementos do vetor */

/* ================= ESTRUTURAS ================= */
typedef struct
{
    char algoritmo[50];
    char cenario[20];
    int tamanho;
    int repeticao;
    double tempo;
    long comparacoes;
    long trocas;
} ResultadoCSV;

typedef struct
{
    char cenario[20];
    int tamanho;
    double media_tempo;
    double desvio_tempo;
    double media_comparacoes;
    double media_trocas;
} Estatisticas;

/* ================= FUNÇÕES AUXILIARES ================= */
#ifdef _WIN32
#define limpar_tela() system("cls")
#else
#define limpar_tela() system("clear")
#endif

/**
 * @brief Pausa a execução até que o usuário pressione ENTER.
 * @return void
*/
void pausar()
{
    printf("\nPressione ENTER para continuar...");
    getchar();
    getchar();
}

/**
 * @brief Solicita confirmação do usuário.
 * @param mensagem Mensagem a ser exibida para confirmação.
 * @return 1 se o usuário confirmar (s/S), 0 caso contrário.
*/
int confirmar(const char *mensagem)
{
    char op;
    printf("\n%s (s/n): ", mensagem);
    scanf(" %c", &op);
    return (op == 's' || op == 'S');
}

/**
 * @brief Imprime todos os elementos do vetor ou os primeiros 50 elementos se o tamanho for maior que 50.
 * @param v Vetor a ser impresso.
 * @param tamanho Tamanho do vetor.
 * @return void 
*/
void imprimirVetor(int v[], int tamanho)
{
    int limite = (tamanho < 50) ? tamanho : 50;

    for (int i = 0; i < limite; i++)
        printf("%d ", v[i]);

    if (tamanho > limite)
        printf("...");

    printf("\n");
}

/**
 * @brief Embaralha os elementos do vetor.
 * @param v Vetor a ser embaralhado.
 * @param n Tamanho do vetor.
 * @return void 
*/
void shuffle(int v[], int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
}

/**
 * @brief Gera um vetor com elementos em ordem crescente, decrescente ou aleatória.
 * @param v Vetor a ser preenchido.
 * @param tipo Tipo de vetor: 1 - crescente, 2 - decrescente, 3 - aleatório.
 * @param tamanho Tamanho do vetor.
 * @return void
*/
void gerarVetor(int v[], int tipo, int tamanho)
{
    if (tipo == 1)
    { /* CRESCENTE */
        int valor = rand() % (TAM_MAX / 10);
        for (int i = 0; i < tamanho; i++)
        {
            v[i] = valor;
            int incremento = 1 + (rand() % (TAM_MAX / tamanho));
            valor += incremento;
            if (valor > TAM_MAX)
                valor = TAM_MAX - (rand() % (TAM_MAX / 10));
        }
    }
    else if (tipo == 2)
    { /* DECRESCENTE */
        int valor = TAM_MAX - (rand() % (TAM_MAX / 10));
        for (int i = 0; i < tamanho; i++)
        {
            v[i] = valor;
            int decremento = 1 + (rand() % (TAM_MAX / tamanho));
            valor -= decremento;
            if (valor < 0)
                valor = rand() % (TAM_MAX / 10);
        }
    }
    else if (tipo == 3)
    { /* ALEATÓRIO */
        for (int i = 0; i < tamanho; i++)
            v[i] = rand() % TAM_MAX;
        shuffle(v, tamanho);
    }
}

/**
 * @brief Implementa o algoritmo Cycle Sort para ordenar um vetor.
 * @param v Vetor a ser ordenado.
 * @param tamanho Tamanho do vetor.
 * @param comparacoes Ponteiro para armazenar o número de comparações realizadas.
 * @param trocas Ponteiro para armazenar o número de trocas realizadas.
 * @return void
 */
void cycleSort(int v[], int tamanho, long *comparacoes, long *trocas)
{
    *comparacoes = 0;
    *trocas = 0;

    for (int ciclo_inicio = 0; ciclo_inicio < tamanho - 1; ciclo_inicio++)
    {
        int item = v[ciclo_inicio];
        int pos = ciclo_inicio;

        for (int j = ciclo_inicio + 1; j < tamanho; j++)
        {
            (*comparacoes)++;
            if (v[j] < item)
            {
                pos++;
            }
        }

        if (pos != ciclo_inicio)
        {
            while (item == v[pos])
            {
                pos++;
                (*comparacoes)++;
            }

            int temp = v[pos];
            v[pos] = item;
            item = temp;
            (*trocas)++;

            while (pos != ciclo_inicio)
            {
                pos = ciclo_inicio;

                for (int j = ciclo_inicio + 1; j < tamanho; j++)
                {
                    (*comparacoes)++;
                    if (v[j] < item)
                    {
                        pos++;
                    }
                }

                while (pos < tamanho && item == v[pos])
                {
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

/**
 * @brief Mede o tempo decorrido entre dois instantes.
 * @param inicio Instante inicial.
 * @param fim Instante final.
 * @return Tempo decorrido em milissegundos.
*/
double medirTempo(struct timespec inicio, struct timespec fim)
{
    return (fim.tv_sec - inicio.tv_sec) * 1000.0 +
           (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;
}

/**
 * @brief Calcula a média dos valores do tipo double em um intervalo do vetor.
 * @param valores Vetor de valores do tipo double.
 * @param inicio Índice inicial do intervalo.
 * @param fim Índice final do intervalo.
 * @return Média dos valores no intervalo.
 */
double calcularMediaDouble(double valores[], int inicio, int fim)
{
    double soma = 0.0;
    for (int i = inicio; i <= fim; i++)
        soma += valores[i];
    return soma / (fim - inicio + 1);
}

/**
 * @brief Calcula o desvio padrão dos valores em um intervalo do vetor.
 * @param valores Vetor de valores.
 * @param inicio Índice inicial do intervalo.
 * @param fim Índice final do intervalo.
 * @param media Média dos valores no intervalo.
 * @return Desvio padrão dos valores no intervalo.
 */
double calcularDesvioPadraoDouble(double valores[], int inicio, int fim, double media)
{
    double soma = 0.0;
    for (int i = inicio; i <= fim; i++)
        soma += pow(valores[i] - media, 2);
    return sqrt(soma / (fim - inicio + 1));
}

/**
 * @brief Calcula a média dos valores do tipo long em um intervalo do vetor.
 * @param valores Vetor de valores do tipo long.
 * @param inicio Índice inicial do intervalo.
 * @param fim Índice final do intervalo.
 * @return Média dos valores no intervalo convertida para double.
 */
double calcularMediaLong(long valores[], int inicio, int fim)
{
    long soma = 0;
    for (int i = inicio; i <= fim; i++)
        soma += valores[i];
    return (double)soma / (fim - inicio + 1);
}

/**
 * @brief Salva os resultados gerais em um arquivo CSV.
 * @param resultados Vetor de resultados individuais.
 * @param num_resultados Número de resultados individuais.
 * @param estatisticas Vetor de estatísticas calculadas.
 * @param num_estatisticas Número de estatísticas calculadas.
 * @return void
 */
void salvarCSVGeral(ResultadoCSV resultados[], int num_resultados, Estatisticas estatisticas[], int num_estatisticas)
{
    time_t agora = time(NULL);
    struct tm *info = localtime(&agora);
    char nomeCSV[150];

    sprintf(nomeCSV, "../results/files/cycleSort/cyclesort_geral_%04d-%02d-%02d_%02d-%02d-%02d.csv",
            info->tm_year + 1900, info->tm_mon + 1, info->tm_mday,
            info->tm_hour, info->tm_min, info->tm_sec);

    FILE *csv = fopen(nomeCSV, "w");
    if (!csv)
    {
        printf("Erro ao criar o arquivo CSV.\n");
        return;
    }

    fprintf(csv, "algoritmo;cenario;tamanho;repeticao;tempo_ms;comparacoes;trocas\n");

    for (int i = 0; i < num_resultados; i++)
    {
        fprintf(csv, "%s;%s;%d;%d;%.3f;%ld;%ld\n",
                resultados[i].algoritmo, resultados[i].cenario,
                resultados[i].tamanho, resultados[i].repeticao,
                resultados[i].tempo, resultados[i].comparacoes,
                resultados[i].trocas);
    }

    fprintf(csv, "\nESTATISTICAS (descartadas as %d primeiras repeticoes)\n", QTD_DESCARTES);
    fprintf(csv, "algoritmo;cenario;tamanho;media_tempo_ms;desvio_tempo_ms;media_comparacoes;media_trocas\n");

    for (int i = 0; i < num_estatisticas; i++)
    {
        fprintf(csv, "Cycle Sort;%s;%d;%.3f;%.3f;%.2f;%.2f\n",
                estatisticas[i].cenario, estatisticas[i].tamanho,
                estatisticas[i].media_tempo, estatisticas[i].desvio_tempo,
                estatisticas[i].media_comparacoes, estatisticas[i].media_trocas);
    }

    fclose(csv);
    printf("\nCSV geral salvo em: %s\n", nomeCSV);
}

/**
 * @brief Salva os resultados detalhados de um experimento em um arquivo TXT.
 * @param tipo Tipo de vetor (crescente, decrescente, aleatório).
 * @param tamanho Tamanho do vetor.
 * @param tempos Vetor de tempos de execução.
 * @param comparacoes Vetor de números de comparações.
 * @param trocas Vetor de números de trocas.
 * @param mediaTempo Média dos tempos de execução.
 * @param desvioTempo Desvio padrão dos tempos de execução.
 * @param mediaComparacoes Média dos números de comparações.
 * @param mediaTrocas Média dos números de trocas.
 * @return void
 */
void salvarResultados(char tipo[], int tamanho, double tempos[], long comparacoes[], long trocas[],
                      double mediaTempo, double desvioTempo, double mediaComparacoes, double mediaTrocas)
{
    time_t agora = time(NULL);
    struct tm *info = localtime(&agora);
    char nomeArquivo[150];

    sprintf(nomeArquivo, "../results/files/cycleSort/cyclesort_%s_%d_%02d-%02d-%04d_%02d-%02d-%02d.txt",
            tipo, tamanho, info->tm_mday, info->tm_mon + 1,
            info->tm_year + 1900, info->tm_hour, info->tm_min, info->tm_sec);

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo)
    {
        printf("Erro ao criar o arquivo TXT.\n");
        return;
    }

    fprintf(arquivo, "============================================================\n");
    fprintf(arquivo, "                      CYCLE SORT                            \n");
    fprintf(arquivo, "============================================================\n\n");
    fprintf(arquivo, "Configuracoes do experimento:\n");
    fprintf(arquivo, "------------------------------------------------------------\n");
    fprintf(arquivo, "Tamanho do vetor : %d\n", tamanho);
    fprintf(arquivo, "Repeticoes       : %d (descartadas %d primeiras)\n", REPETICOES, QTD_DESCARTES);
    fprintf(arquivo, "Repeticoes validas: %d\n", REPETICOES - QTD_DESCARTES);
    fprintf(arquivo, "Tipo de vetor    : %s\n\n", tipo);
    fprintf(arquivo, "Resultados individuais:\n");
    fprintf(arquivo, "------------------------------------------------------------\n");

    for (int i = 0; i < REPETICOES; i++)
    {
        fprintf(arquivo, "Execucao %2d", i + 1);
        if (i < QTD_DESCARTES)
        {
            fprintf(arquivo, " (DESCARTADA)");
        }
        fprintf(arquivo, "\n");
        fprintf(arquivo, "  Tempo        : %8.3f ms\n", tempos[i]);
        fprintf(arquivo, "  Comparacoes  : %8ld\n", comparacoes[i]);
        fprintf(arquivo, "  Trocas       : %8ld\n\n", trocas[i]);
    }

    fprintf(arquivo, "Resumo estatistico (descartadas %d primeiras execucoes):\n", QTD_DESCARTES);
    fprintf(arquivo, "------------------------------------------------------------\n");
    fprintf(arquivo, "Tempo medio           : %.3f ms\n", mediaTempo);
    fprintf(arquivo, "Desvio padrao (tempo) : %.3f ms\n\n", desvioTempo);
    fprintf(arquivo, "Media de comparacoes  : %.2f\n", mediaComparacoes);
    fprintf(arquivo, "Media de trocas       : %.2f\n", mediaTrocas);
    fprintf(arquivo, "\n============================================================\n");

    fclose(arquivo);
    printf("  TXT salvo: %s\n", nomeArquivo);
}

/**
 * @brief Executa o teste geral com todos os cenários e tamanhos.
 * @return void
 */
void executarTesteGeral()
{
    int tamanhos[] = {20000, 40000, 60000};
    char *nomes_cenarios[] = {"crescente", "decrescente", "aleatorio"};
    int tipos_cenarios[] = {1, 2, 3};

    const int TOTAL_COMBINACOES = 3 * 3;
    const int TOTAL_EXECUCOES = TOTAL_COMBINACOES * REPETICOES;

    ResultadoCSV *resultados = malloc(TOTAL_EXECUCOES * sizeof(ResultadoCSV));
    Estatisticas *estatisticas = malloc(TOTAL_COMBINACOES * sizeof(Estatisticas));

    if (!resultados || !estatisticas)
    {
        printf("Erro ao alocar memoria!\n");
        free(resultados);
        free(estatisticas);
        return;
    }

    int idx_resultado = 0;
    int idx_estatistica = 0;

    printf("\n========================================\n");
    printf("        INICIANDO TESTE GERAL           \n");
    printf("========================================\n\n");

    for (int t = 0; t < 3; t++)
    {
        int tamanho = tamanhos[t];

        for (int c = 0; c < 3; c++)
        {
            printf("\n[CENARIO: %s | TAMANHO: %d]\n", nomes_cenarios[c], tamanho);
            printf("Executando %d repeticoes (serao descartadas %d)...\n", REPETICOES, QTD_DESCARTES);

            double tempos[REPETICOES];
            long comparacoes[REPETICOES];
            long trocas[REPETICOES];

            for (int r = 0; r < REPETICOES; r++)
            {
                int *vetor = malloc(tamanho * sizeof(int));
                if (!vetor)
                {
                    printf("Erro ao alocar vetor!\n");
                    break;
                }

                gerarVetor(vetor, tipos_cenarios[c], tamanho);

                struct timespec inicio, fim;
                clock_gettime(CLOCK_MONOTONIC, &inicio);
                cycleSort(vetor, tamanho, &comparacoes[r], &trocas[r]);
                clock_gettime(CLOCK_MONOTONIC, &fim);

                tempos[r] = medirTempo(inicio, fim);

                strcpy(resultados[idx_resultado].algoritmo, "Cycle Sort");
                strcpy(resultados[idx_resultado].cenario, nomes_cenarios[c]);
                resultados[idx_resultado].tamanho = tamanho;
                resultados[idx_resultado].repeticao = r + 1;
                resultados[idx_resultado].tempo = tempos[r];
                resultados[idx_resultado].comparacoes = comparacoes[r];
                resultados[idx_resultado].trocas = trocas[r];
                idx_resultado++;

                if ((r + 1) % 5 == 0)
                    printf("  Completadas: %d/%d\n", r + 1, REPETICOES);

                free(vetor);
            }

            // Calcular estatísticas descartando as primeiras QTD_DESCARTES repetições
            int inicio_estat = QTD_DESCARTES;
            int fim_estat = REPETICOES - 1;
            int repeticoes_validas = REPETICOES - QTD_DESCARTES;

            if (repeticoes_validas > 0)
            {
                double media_tempo = calcularMediaDouble(tempos, inicio_estat, fim_estat);
                double desvio_tempo = calcularDesvioPadraoDouble(tempos, inicio_estat, fim_estat, media_tempo);
                double media_comparacoes = calcularMediaLong(comparacoes, inicio_estat, fim_estat);
                double media_trocas = calcularMediaLong(trocas, inicio_estat, fim_estat);

                strcpy(estatisticas[idx_estatistica].cenario, nomes_cenarios[c]);
                estatisticas[idx_estatistica].tamanho = tamanho;
                estatisticas[idx_estatistica].media_tempo = media_tempo;
                estatisticas[idx_estatistica].desvio_tempo = desvio_tempo;
                estatisticas[idx_estatistica].media_comparacoes = media_comparacoes;
                estatisticas[idx_estatistica].media_trocas = media_trocas;
                idx_estatistica++;

                salvarResultados(nomes_cenarios[c], tamanho,
                                 tempos,
                                 comparacoes,
                                 trocas,
                                 media_tempo, desvio_tempo,
                                 media_comparacoes, media_trocas);

                printf("  Concluido! Media: %.2f ms (apos descarte de %d execucoes)\n",
                       media_tempo, QTD_DESCARTES);
            }
            else
            {
                printf("  Erro: Nenhuma repeticao valida apos descarte!\n");
            }
        }
    }

    salvarCSVGeral(resultados, idx_resultado, estatisticas, idx_estatistica);

    free(resultados);
    free(estatisticas);

    printf("\n========================================\n");
    printf("     TESTE GERAL CONCLUIDO!             \n");
    printf("========================================\n");
    pausar();
}

/* ================= FUNÇÕES PARA OPÇÕES 1-3 (usam TAM_VETOR fixo) ================= */
/**
 * @brief Executa o experimento para um tamanho fixo de vetor.
 * @param tipo Tipo de vetor: 1 - crescente, 2 - decrescente, 3 - aleatório.
 * @param tempos Vetor para armazenar os tempos de execução.
 * @param comparacoes Vetor para armazenar o número de comparações.
 * @param trocas Vetor para armazenar o número de trocas.
 * @param tamanho Tamanho do vetor.
 * @return void
 */
void executarExperimentoTamanhoFixo(int tipo, double tempos[], long comparacoes[], long trocas[], int tamanho)
{
    int *vetor = malloc(tamanho * sizeof(int));
    struct timespec inicio, fim;

    for (int i = 0; i < REPETICOES; i++)
    {
        gerarVetor(vetor, tipo, tamanho);

        printf("=====================================\n");
        printf(" Execução %d", i + 1);
        if (i < QTD_DESCARTES)
        {
            printf(" (SERA DESCARTADA)");
        }
        printf("\n");
        printf("=====================================\n");

        printf("Vetor antes da ordenação:\n");
        imprimirVetor(vetor, tamanho);

        clock_gettime(CLOCK_MONOTONIC, &inicio);
        cycleSort(vetor, tamanho, &comparacoes[i], &trocas[i]);
        clock_gettime(CLOCK_MONOTONIC, &fim);

        tempos[i] = medirTempo(inicio, fim);

        printf("\nVetor após a ordenação:\n");
        imprimirVetor(vetor, tamanho);
    }

    free(vetor);
}

/**
 * @brief Processa e salva os resultados do experimento.
 * @param opcao Tipo de vetor: 1 - crescente, 2 - decrescente, 3 - aleatório.
 * @param tempos Vetor para armazenar os tempos de execução.
 * @param comparacoes Vetor para armazenar o número de comparações.
 * @param trocas Vetor para armazenar o número de trocas.
 * @return void
 */
void processarResultados(int opcao, double tempos[], long comparacoes[], long trocas[])
{
    // Calcular estatísticas descartando as primeiras QTD_DESCARTES repetições
    int inicio_estat = QTD_DESCARTES;
    int fim_estat = REPETICOES - 1;
    int repeticoes_validas = REPETICOES - QTD_DESCARTES;

    if (repeticoes_validas <= 0)
    {
        printf("Erro: Nenhuma repeticao valida apos descarte!\n");
        pausar();
        return;
    }

    double media = calcularMediaDouble(tempos, inicio_estat, fim_estat);
    double desvio = calcularDesvioPadraoDouble(tempos, inicio_estat, fim_estat, media);
    double mediaComparacoes = calcularMediaLong(comparacoes, inicio_estat, fim_estat);
    double mediaTrocas = calcularMediaLong(trocas, inicio_estat, fim_estat);

    char *tipo;
    if (opcao == 1)
        tipo = "crescente";
    else if (opcao == 2)
        tipo = "decrescente";
    else
        tipo = "aleatorio";

    salvarResultados(tipo, TAM_VETOR, tempos, comparacoes, trocas,
                     media, desvio, mediaComparacoes, mediaTrocas);

    printf("\nResultados salvos com sucesso!\n");
    printf("Estatisticas calculadas com %d repeticoes validas (descartadas %d primeiras)\n",
           repeticoes_validas, QTD_DESCARTES);
    pausar();
}

/* ================= MENU ================= */
/**
 * @brief Exibe o menu e obtém a opção do usuário.
 * @return Opção selecionada pelo usuário.
 */
int menu()
{
    int opcao;
    limpar_tela();
    printf("=====================================\n");
    printf("         CYCLE SORT - MENU           \n");
    printf("=====================================\n");
    printf(" Configuracoes atuais:\n");
    printf("  Repeticoes: %d\n", REPETICOES);
    printf("  Descartes: %d\n", QTD_DESCARTES);
    printf("  Repeticoes validas: %d\n", REPETICOES - QTD_DESCARTES);
    printf("=====================================\n");
    printf(" 1 - Vetor Crescente\n");
    printf(" 2 - Vetor Decrescente\n");
    printf(" 3 - Vetor Aleatorio\n");
    printf(" 4 - Teste Geral (todos os casos)\n");
    printf(" 0 - Sair\n");
    printf("=====================================\n");
    printf(" Escolha uma opcao: ");
    scanf("%d", &opcao);
    return opcao;
}

/**
 * @brief Executa a opção selecionada pelo usuário.
 * @param opcao Opção selecionada.
 * @return void
 */
void executarOpcao(int opcao)
{
    double tempos[REPETICOES];
    long comparacoes[REPETICOES];
    long trocas[REPETICOES];

    limpar_tela();
    printf("Opcao selecionada: %d\n", opcao);

    if (confirmar("Deseja iniciar a execucao?"))
    {
        if (opcao == 4)
        {
            executarTesteGeral();
        }
        else
        {
            executarExperimentoTamanhoFixo(opcao, tempos, comparacoes, trocas, TAM_VETOR);
            processarResultados(opcao, tempos, comparacoes, trocas);
        }
    }
}

/* ================= MAIN ================= */
int main()
{
    srand(time(NULL));
    int executando = 1;

    // Validação das constantes
    if (QTD_DESCARTES >= REPETICOES)
    {
        printf("ERRO: QTD_DESCARTES (%d) deve ser menor que REPETICOES (%d)!\n",
               QTD_DESCARTES, REPETICOES);
        printf("Por favor, ajuste as constantes no codigo.\n");
        pausar();
        return 1;
    }

    while (executando)
    {
        int opcao = menu();

        switch (opcao)
        {
        case 0:
            limpar_tela();
            printf("Encerrando o programa...\n");
            pausar();
            executando = 0;
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            executarOpcao(opcao);
            break;

        default:
            limpar_tela();
            printf("Opcao invalida!\n");
            pausar();
        }
    }

    return 0;
}