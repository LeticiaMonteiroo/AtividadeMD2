#include <stdio.h>

/**
 * @brief Calcula o Máximo Divisor Comum (MDC) entre dois números usando o Algoritmo de Euclides.
 *
 * A função exibe cada passo do cálculo, conforme exigido nas especificações da atividade.
 *
 * @param a O primeiro número.
 * @param b O segundo número.
 * @return O MDC de a e b.
 */
int mdcComPassos(int a, int b) {
    int resto;
    printf("   -> Calculando MDC(%d, %d) via Algoritmo de Euclides:\n", a, b);
    while (b != 0) {
        resto = a % b;
        printf("      - Passo: %d mod %d = %d\n", a, b, resto);
        a = b;
        b = resto;
    }
    printf("   -> Resultado do MDC: %d\n", a);
    return a;
}

/**
 * @brief Calcula o Mínimo Múltiplo Comum (MMC) de dois números.
 *
 * Utiliza a fórmula MMC(a, b) = (|a * b|) / MDC(a, b).
 *
 * @param a O primeiro número.
 * @param b O segundo número.
 * @return O MMC de a e b.
 */
int mmc(int a, int b) {
    // Justificativa do Método: O MMC é fundamental para encontrar o primeiro ponto de
    // sincronização entre dois ciclos periódicos. A relação com o MDC é a forma
    // mais eficiente de calculá-lo.
    if (a == 0 || b == 0) {
        return 0;
    }
    return (a * b) / mdcComPassos(a, b);
}

int main() {
    int N; // Número de chaves
    int ciclos[10]; // Array para armazenar os ciclos C_i
    int i;
    int resultado_mmc;

    // --- PASSO 1: ENTRADA DE DADOS E VALIDAÇÃO ---
    // [cite: 307]
    printf("ETAPA 1: Entrada de Dados\n");
    printf("-------------------------\n");
    printf("Digite o numero de chaves (N), com 1 <= N <= 10: ");
    scanf("%d", &N);

    // Validação da entrada N [cite: 90]
    if (N < 1 || N > 10) {
        printf("Erro: O numero de chaves deve estar entre 1 e 10.\n");
        return 1; // Encerra o programa com erro
    }

    printf("Digite os %d ciclos de ativacao (C_i), com 2 <= C_i <= 20:\n", N);
    for (i = 0; i < N; i++) {
        printf("Ciclo %d: ", i + 1);
        scanf("%d", &ciclos[i]);
        // Validação da entrada C_i [cite: 91]
        if (ciclos[i] < 2 || ciclos[i] > 20) {
            printf("Erro: O valor de cada ciclo deve estar entre 2 e 20.\n");
            return 1; // Encerra o programa com erro
        }
    }

    // --- PASSO 2: CÁLCULO DO MMC ---
    // [cite: 296, 303]
    printf("\nETAPA 2: Calculo do Minimo Multiplo Comum (MMC)\n");
    printf("-------------------------------------------------\n");
    printf("O objetivo e encontrar o primeiro ano em que todos os ciclos sao multiplos simultaneamente.\n");
    printf("Isso e, por definicao, o MMC de todos os ciclos inseridos.\n\n");

    // O cálculo do MMC é feito de forma iterativa: MMC(a, b, c) = MMC(MMC(a, b), c)
    resultado_mmc = ciclos[0];
    for (i = 1; i < N; i++) {
        printf("Calculando o MMC acumulado entre %d e %d...\n", resultado_mmc, ciclos[i]);
        resultado_mmc = mmc(resultado_mmc, ciclos[i]);
        printf("MMC parcial atual: %d\n\n", resultado_mmc);
    }
    
    printf("O MMC final de todos os ciclos e: %d\n", resultado_mmc);


    // --- PASSO 3: VERIFICAÇÃO DO LIMITE E SAÍDA ---
    // [cite: 308]
    printf("\nETAPA 3: Conclusao\n");
    printf("-------------------\n");
    printf("Verificando se o resultado (%d) esta dentro do limite de 50 anos...\n", resultado_mmc);

    if (resultado_mmc > 0 && resultado_mmc <= 50) {
        // [cite: 89]
        printf("\nResultado Final: O primeiro ano futuro em que todas as chaves podem ser utilizadas simultaneamente e: %d\n", resultado_mmc);
    } else {
        // [cite: 88, 94]
        printf("\nResultado Final: Nao foi possivel encontrar um ano, dentro do limite de 1 a 50, em que todas as chaves pudessem ser usadas simultaneamente.\n");
    }

    return 0;
}