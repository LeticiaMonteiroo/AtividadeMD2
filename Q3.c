#include <stdio.h>
#include <math.h>

// Função para calcular p^exp de forma segura (evitando overflow quando possível)
long long potencia_segura(long long p, int exp) {
    long long resultado = 1;
    for (int i = 0; i < exp; i++) {
        // Verificar possível overflow antes de multiplicar
        if (resultado > LLONG_MAX / p) {
            printf("Aviso: Possível overflow no cálculo de potência\n");
        }
        resultado *= p;
    }
    return resultado;
}

int main() {
    int N;
    printf("Digite um inteiro N (1 <= N <= 100000): ");
    scanf("%d", &N);

    // Validação da entrada
    if (N < 1 || N > 100000) {
        printf("Erro: N está fora do intervalo válido (1 a 100000).\n");
        return 1;
    }

    // Caso especial N = 1
    if (N == 1) {
        printf("\n=== PASSO 1: FATORAÇÃO PRIMA ===\n");
        printf("N = 1 não possui fatores primos.\n");
        printf("\n=== PASSO 2: CÁLCULO DE TAU(N) ===\n");
        printf("tau(1) = 1\n");
        printf("\n=== PASSO 3: CÁLCULO DE SIGMA(N) ===\n");
        printf("sigma(1) = 1\n");
        printf("\n=== PASSO 4: CÁLCULO DA RAZÃO DE EFICIÊNCIA ===\n");
        printf("Razão de eficiência = 1.00\n");
        return 0;
    }

    int n_copia = N;
    int fatores_primos[100]; // Armazena os fatores primos de N
    int expoentes[100];      // Armazena os expoentes correspondentes
    int total_fatores = 0;   // Contador de fatores encontrados

    printf("\n=== PASSO 1: FATORAÇÃO PRIMA (Trial Division) ===\n");

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

    printf("\n=== PASSO 2: CÁLCULO DE TAU(N) (Número de Divisores) ===\n");
    long long tau = 1;
    for (int i = 0; i < total_fatores; i++) {
        printf("tau atual = %lld * (%d + 1) = ", tau, expoentes[i]);
        tau *= (expoentes[i] + 1);
        printf("%lld (após fator %d^%d)\n", tau, fatores_primos[i], expoentes[i]);
    }

    printf("\n=== PASSO 3: CÁLCULO DE SIGMA(N) (Soma dos Divisores) ===\n");
    long long sigma = 1;
    for (int i = 0; i < total_fatores; i++) {
        long long p = fatores_primos[i];
        int a = expoentes[i];
        
        // Cálculo seguro de sigma(p^a) = 1 + p + p^2 + ... + p^a
        long long termo = 1; // Começa com 1
        long long potencia_atual = 1;
        
        printf("Para fator %lld^%d:\n", p, a);
        printf("  Calculando sigma(p^a) = 1");
        
        for (int j = 1; j <= a; j++) {
            potencia_atual *= p;
            termo += potencia_atual;
            if (j <= a) {
                printf(" + %lld", potencia_atual);
            }
        }
        printf(" = %lld\n", termo);
        
        printf("  sigma acumulado = %lld * %lld = ", sigma, termo);
        sigma *= termo;
        printf("%lld\n", sigma);
    }

    printf("\n=== PASSO 4: CÁLCULO DA RAZÃO DE EFICIÊNCIA ===\n");
    double razao = (double)sigma / (double)tau;
    printf("sigma(N) = %lld\n", sigma);
    printf("tau(N) = %lld\n", tau);
    printf("Razão de eficiência = %.2f\n", razao);

    return 0;
}