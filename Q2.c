#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

/* MDC com exibicao passo a passo */
ll mdcComPassos(ll a, ll b) {
    ll resto;
    a = llabs(a);
    b = llabs(b);
    printf("      -> Calculando MDC(%lld, %lld) via Algoritmo de Euclides:\n", a, b);
    while (b != 0) {
        resto = a % b;
        printf("         Passo: %lld mod %lld = %lld\n", a, b, resto);
        a = b;
        b = resto;
    }
    printf("      -> Resultado do MDC: %lld\n", a);
    return a;
}

/* MMC entre dois inteiros com protecao/precisao */
ll mmcDois(ll a, ll b) {
    if (a == 0 || b == 0) return 0;
    
    a = llabs(a);
    b = llabs(b);
    printf("      -> Calculando MMC(%lld, %lld)\n", a, b);
    ll m = mdcComPassos(a, b);
    
    /* calcular (a / m) * b para reduzir chance de overflow */
    ll fator = a / m;
    ll mmc = fator * b;
    mmc = llabs(mmc);
    printf("      -> MMC = (%lld / %lld) * %lld = %lld\n", a, m, b, mmc);
    return mmc;
}

/* MMC acumulado com otimizacao: se ultrapassar 50, sabemos que final >= atual */
ll mmcAcumulado(int ciclos[], int N) {
    ll acumulado = ciclos[0];
    printf("   -> Inicializando MMC acumulado = %lld (ciclo 1 = %d)\n", acumulado, ciclos[0]);
    
    for (int i = 1; i < N; i++) {
        printf("   -> Passo %d: calcular MMC(%lld, %d)\n", i+1, acumulado, ciclos[i]);
        
        if (acumulado > 50) {
            printf("   -> MMC parcial (%lld) ja ultrapassa 50. Interrompendo calculo (final sera >= este valor).\n", acumulado);
            break;
        }
        
        acumulado = mmcDois(acumulado, (ll)ciclos[i]);
        printf("   -> MMC parcial apos incluir ciclo %d = %lld\n\n", i+1, acumulado);
        
        if (acumulado > 50) {
            printf("   -> MMC (%lld) ultrapassou limite de 50 anos. Calculo interrompido.\n", acumulado);
            break;
        }
    }
    return acumulado;
}

/* Verifica se um ano é múltiplo de todos os ciclos */
int ehMultiploDeTodos(int ano, int ciclos[], int N) {
    for (int i = 0; i < N; i++) {
        if (ano % ciclos[i] != 0) return 0;
    }
    return 1;
}

/* Programa principal */
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

    /* ETAPA 2: Calculo do MMC */
    printf("\nETAPA 2: Calculo do Minimo Multiplo Comum (MMC)\n");
    printf("------------------------------------------------\n");
    printf("Passo 2.1 - Justificativa: o primeiro ano em que todos os ciclos coincidem\n");
    printf("e o MMC(C1, C2, ..., CN). Vamos calcular MMC iterativamente.\n\n");

    ll resultado_mmc = mmcAcumulado(ciclos, N);
    printf("Resultado final (MMC de todos os ciclos) = %lld\n", resultado_mmc);

    /* ETAPA 3: Verificacao no intervalo 1..50 */
    printf("\nETAPA 3: Verificacao do limite de anos (1 a 50)\n");
    printf("------------------------------------------------\n");

    int ano_encontrado = 0;
    printf("Passo 3.1 - Verificando anos de 1 a 50:\n");
    
    int inicio_verificacao = 1;
    if (resultado_mmc > 0 && resultado_mmc <= 50) {
        inicio_verificacao = (int)resultado_mmc;
        printf("  Otimizacao: MMC = %lld, comecando verificacao no ano %d\n", resultado_mmc, inicio_verificacao);
    }
    
    for (int ano = inicio_verificacao; ano <= 50; ano++) {
        if (ehMultiploDeTodos(ano, ciclos, N)) {
            printf("  Ano %d: ✓ MULTIPLO DE TODOS OS CICLOS\n", ano);
            ano_encontrado = ano;
            break;
        } else {
            printf("  Ano %d: nao e multiplo de todos os ciclos\n", ano);
        }
    }

    /* ETAPA 4: Conclusao */
    printf("\nETAPA 4: Conclusao\n");
    printf("------------------\n");
    if (ano_encontrado > 0) {
        printf("Resultado Final: O primeiro ano X > 0 em que todas as chaves sao utilizadas simultaneamente e: %d\n", ano_encontrado);
    } else {
        printf("Resultado Final: Nao existe ano valido entre 1 e 50 em que todas as chaves sejam usadas simultaneamente.\n");
    }

    /* Confirmacao adicional */
    printf("\nPasso final - Confirmacao:\n");
    if (resultado_mmc > 0 && resultado_mmc <= 50 && ano_encontrado == resultado_mmc) {
        printf("  Observacao: MMC calculado = %lld. O primeiro ano sincronizado e %lld.\n", resultado_mmc, resultado_mmc);
    } else if (resultado_mmc > 50 && ano_encontrado > 0) {
        printf("  Observacao: MMC calculado = %lld (maior que 50), mas foi encontrado um ano multiplo comum (%d) dentro do limite.\n", resultado_mmc, ano_encontrado);
    } else if (resultado_mmc > 50 && ano_encontrado == 0) {
        printf("  Observacao: MMC calculado = %lld (maior que 50). Nenhum ano multiplo comum encontrado no limite 1..50.\n", resultado_mmc);
    }

    return 0;
}
