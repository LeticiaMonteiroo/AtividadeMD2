#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;

/* ---------------------------------------------------------
   Funcoes auxiliares
   --------------------------------------------------------- */

/* Teste de primalidade nao necessario aqui (mantido se precisar) */

/* MDC com exibicao passo a passo (Algoritmo de Euclides)
   Recebe dois inteiros positivos e imprime cada resto calculado.
*/
int mdcComPassos(int a, int b) {
    int resto;
    printf("      -> Calculando MDC(%d, %d) via Algoritmo de Euclides:\n", a, b);
    while (b != 0) {
        resto = a % b;
        printf("         Passo: %d = %d * (%d) + %d  (resto = %d)\n", a, a / b, b, resto, resto);
        a = b;
        b = resto;
    }
    printf("      -> Resultado do MDC: %d\n", a);
    return a;
}

/* MMC entre dois inteiros usando MMC(a,b) = |a*b| / MDC(a,b)
   Imprime justificativa breve e o calculo.
*/
ll mmcDois(int a, int b) {
    if (a == 0 || b == 0) return 0;
    printf("      -> Calculando MMC(%d, %d) usando MMC = |a*b| / MDC(a,b)\n", a, b);
    int m = mdcComPassos(abs(a), abs(b));
    ll prod = (ll) a * (ll) b;
    ll mmc = llabs(prod) / m;
    printf("      -> Produto = %lld, MDC = %d, MMC = %lld\n", prod, m, mmc);
    return mmc;
}

/* MMC acumulado de um array de ciclos:
   faz MMC iterativo: mmc = MMC(mmc, ciclos[i])
   Imprime cada passo acumulado.
*/
ll mmcAcumulado(int ciclos[], int N) {
    ll acumulado = ciclos[0];
    printf("   -> Inicializando MMC acumulado = %d\n", ciclos[0]);
    for (int i = 1; i < N; i++) {
        printf("   -> Passo acumulado %d: calcular MMC(%lld, %d)\n", i, acumulado, ciclos[i]);
        acumulado = mmcDois((int)acumulado, ciclos[i]);
        printf("   -> MMC parcial apos incluir ciclo %d = %lld\n\n", i+1, acumulado);
        /* se acumulado ja exceder 50, podemos continuar calculando (por requisito de exibicao)
           mas sabemos que nao ha solucao dentro de 1..50 se acumulado > 50. */
    }
    return acumulado;
}

/* ---------------------------------------------------------
   Programa principal
   --------------------------------------------------------- */
int main() {
    int N;
    int ciclos[10];

    /* ETAPA 1: Entrada e validacao */
    printf("ETAPA 1: Entrada de Dados\n");
    printf("-------------------------\n");
    printf("Passo 1.1 - Ler N (1 <= N <= 10)\n");
    printf("Digite o numero de chaves (N): ");
    if (scanf("%d", &N) != 1) {
        printf("Erro: entrada invalida para N.\n");
        return 1;
    }
    if (N < 1 || N > 10) {
        printf("Erro: N deve estar entre 1 e 10.\n");
        return 1;
    }

    printf("\nPasso 1.2 - Ler os %d ciclos Ci (2 <= Ci <= 20)\n", N);
    for (int i = 0; i < N; i++) {
        printf("  Ciclo %d: ", i+1);
        if (scanf("%d", &ciclos[i]) != 1) {
            printf("Erro: entrada invalida para ciclo %d.\n", i+1);
            return 1;
        }
        if (ciclos[i] < 2 || ciclos[i] > 20) {
            printf("Erro: cada ciclo Ci deve estar entre 2 e 20.\n");
            return 1;
        }
    }

    /* ETAPA 2: Calculo do MMC (com exibicao passo a passo) */
    printf("\nETAPA 2: Calculo do Minimo Multiplo Comum (MMC)\n");
    printf("------------------------------------------------\n");
    printf("Passo 2.1 - Justificativa: o primeiro ano em que todos os ciclos coincidem\n");
    printf("e o MMC(C1, C2, ..., CN). Vamos calcular MMC iterativamente e mostrar os passos.\n\n");

    ll resultado_mmc = mmcAcumulado(ciclos, N);
    printf("Resultado final (MMC de todos os ciclos) = %lld\n", resultado_mmc);

    /* ETAPA 3: Verificacao no intervalo 1..50 (iteracoes visiveis) */
    printf("\nETAPA 3: Verificacao do limite de anos (1 a 50)\n");
    printf("------------------------------------------------\n");

    /* Mostramos as iteracoes ano a ano ate 50 ou ate encontrar o primeiro ano sincronizado */
    int ano_encontrado = 0;
    printf("Passo 3.1 - Verificando anos de 1 a 50:\n");
    for (int ano = 1; ano <= 50; ano++) {
        int todos_multiplos = 1;
        printf("  Verificacao ano %d: ", ano);
        for (int j = 0; j < N; j++) {
            if (ano % ciclos[j] != 0) {
                /* nao e multiplo deste ciclo */
                printf("%d nao e multiplo de %d", ano, ciclos[j]);
                todos_multiplos = 0;
                break;
            } else {
                /* e multiplo deste ciclo; se for o ultimo, sera multiplo de todos */
                if (j == N-1) {
                    printf("%d e multiplo de todos (passou em todos os ciclos)", ano);
                } else {
                    /* apenas indicacao parcial */
                    printf("%d e multiplo de %d, ", ano, ciclos[j]);
                }
            }
        }
        printf("\n");

        if (todos_multiplos) {
            ano_encontrado = ano;
            printf("  -> Ano sincronizado encontrado: %d (parar verificacoes)\n", ano_encontrado);
            break;
        }
    }

    /* Conclusao: imprimir resultado conforme enunciado */
    printf("\nETAPA 4: Conclusao\n");
    printf("------------------\n");
    if (ano_encontrado > 0 && ano_encontrado <= 50) {
        printf("Resultado Final: O primeiro ano X > 0 em que todas as chaves sao utilizadas simultaneamente e: %d\n", ano_encontrado);
    } else {
        printf("Resultado Final: Nao existe ano valido entre 1 e 50 em que todas as chaves sejam usadas simultaneamente.\n");
    }

    /* Confirmacao adicional: comparar com MMC calculado */
    printf("\nPasso final - Confirmacao:\n");
    if (resultado_mmc > 0 && resultado_mmc <= 50) {
        printf("  Observacao: MMC calculado = %lld. Como MMC <= 50, o primeiro ano sincronizado e %lld (coincide com a verificacao por anos).\n", resultado_mmc, resultado_mmc);
    } else {
        printf("  Observacao: MMC calculado = %lld. Como MMC > 50, nao ha ano dentro do limite 1..50 que seja o MMC.\n", resultado_mmc);
    }

    return 0;
}
