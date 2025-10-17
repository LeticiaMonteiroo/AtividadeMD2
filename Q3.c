#include <stdio.h>
#include <math.h>

// Função para potenciação inteira (evita problemas de precisão do pow())
long long potencia_inteira(long long base, int expoente) {
    long long resultado = 1;
    for (int i = 0; i < expoente; i++) {
        resultado *= base;
    }
    return resultado;
}

int main() {
    int N;
    printf("Digite um inteiro N (1 <= N <= 100000): ");
    scanf("%d", &N);

    if (N < 1 || N > 100000) {
        printf("Erro: N esta fora do intervalo valido (1 a 100000).\n");
        return 1;
    }

    // Caso especial N = 1
    if (N == 1) {
        printf("\n=== PASSO 1: FATORACAO PRIMA ===\n");
        printf("N = 1 nao possui fatores primos.\n");
        printf("\n=== PASSO 2: CALCULO DE τ(N) ===\n");
        printf("τ(1) = 1\n");
        printf("\n=== PASSO 3: CALCULO DE σ(N) ===\n");
        printf("σ(1) = 1\n");
        printf("\n=== PASSO 4: CALCULO DA RAZAO DE EFICIENCIA ===\n");
        printf("Razao de Eficiencia = 1.00\n");
        return 0;
    }

    int n_copia = N;
    int fatores_primos[100];
    int expoentes[100];
    int total_fatores = 0;

    printf("\n=== PASSO 1: FATORACAO PRIMA (Trial Division) ===\n");

    // Fatorar por 2 separadamente
    if (n_copia % 2 == 0) {
        int count = 0;
        while (n_copia % 2 == 0) {
            n_copia /= 2;
            count++;
        }
        fatores_primos[total_fatores] = 2;
        expoentes[total_fatores] = count;
        printf("Fator primo encontrado: 2 com expoente %d\n", count);
        total_fatores++;
    }

    // Fatorar por números ímpares
    for (int i = 3; i * i <= n_copia; i += 2) {
        if (n_copia % i == 0) {
            int count = 0;
            while (n_copia % i == 0) {
                n_copia /= i;
                count++;
            }
            fatores_primos[total_fatores] = i;
            expoentes[total_fatores] = count;
            printf("Fator primo encontrado: %d com expoente %d\n", i, count);
            total_fatores++;
        }
    }

    // Último fator primo (se restar)
    if (n_copia > 1) {
        fatores_primos[total_fatores] = n_copia;
        expoentes[total_fatores] = 1;
        printf("Fator primo encontrado: %d com expoente 1\n", n_copia);
        total_fatores++;
    }

    printf("\n=== PASSO 2: CALCULO DE τ(N) (Numero de Divisores) ===\n");
    long long tau = 1;
    for (int i = 0; i < total_fatores; i++) {
        printf("τ atual = %lld × (%d + 1) = ", tau, expoentes[i]);
        tau *= (expoentes[i] + 1);
        printf("%lld (apos fator %d^%d)\n", tau, fatores_primos[i], expoentes[i]);
    }

    printf("\n=== PASSO 3: CALCULO DE σ(N) (Soma dos Divisores) ===\n");
    long long sigma = 1;
    for (int i = 0; i < total_fatores; i++) {
        long long p = fatores_primos[i];
        int a = expoentes[i];
        
        // Calcular σ(p^a) = (p^(a+1) - 1) / (p - 1)
        long long numerador = potencia_inteira(p, a + 1) - 1;
        long long denominador = p - 1;
        long long termo = numerador / denominador;
        
        printf("Para fator %lld^%d: σ = (%lld^(%d+1) - 1) / (%lld - 1) = ", p, a, p, a, p);
        printf("(%lld - 1) / %lld = %lld / %lld = %lld\n", 
               potencia_inteira(p, a + 1), denominador, numerador, denominador, termo);
        
        printf("σ atual = %lld × %lld = ", sigma, termo);
        sigma *= termo;
        printf("%lld\n", sigma);
    }

    printf("\n=== PASSO 4: CALCULO DA RAZAO DE EFICIENCIA ===\n");
    double razao = (double)sigma / (double)tau;

    printf("σ(N) = %lld\n", sigma);
    printf("τ(N) = %lld\n", tau);
    printf("Razao de Eficiencia = σ(N) / τ(N) = %.2f\n", razao);

    return 0;
}