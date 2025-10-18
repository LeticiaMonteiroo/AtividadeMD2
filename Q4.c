/* 
  ATIVIDADE MD2 - QUESTÃO 04
  Nome: SEU_NOME
  Matrícula: SUA_MATRICULA
  Observações: implementação em C sem uso de bibliotecas prontas para mdc/inverso/exponenciação.
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

// --------------------------------------------------
// [Função] mdc (para uso interno, sem prints)
// --------------------------------------------------
int mdc(int a, int b) {
    a = abs(a);
    b = abs(b);
    while (b != 0) {
        int resto = a % b;
        a = b;
        b = resto;
    }
    return a;
}

// --------------------------------------------------
// [Função] mdccmPassos (com prints para passo a passo)
// --------------------------------------------------
int mdccmPassos(int a, int b) {
    a = abs(a);
    b = abs(b);
    printf("[PASSO 1] Calculando mdc(%d, %d):\n", a, b);
    while (b != 0) {
        int resto = a % b;
        printf("  [MDC] %d mod %d = %d\n", a, b, resto);
        a = b;
        b = resto;
    }
    printf("  [MDC] Resultado: mdc = %d\n\n", a);
    return a;
}

// --------------------------------------------------
// [Função] eh_primo
// --------------------------------------------------
int eh_primo(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; (long long)i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// --------------------------------------------------
// [Função] fiEuler
// --------------------------------------------------
int fiEuler(int n) {
    printf("[PASSO 4] Calculando φ(%d):\n", n);
    int resultado = n;
    int temp = n;
    for (int i = 2; (long long)i * i <= temp; i++) {
        if (temp % i == 0) {
            printf("  [PHI] Fator primo: %d\n", i);
            while (temp % i == 0) temp /= i;
            resultado -= resultado / i;
            printf("  [PHI] φ parcial = %d\n", resultado);
        }
    }
    if (temp > 1) {
        printf("  [PHI] Fator primo: %d\n", temp);
        resultado -= resultado / temp;
    }
    printf("  [PHI] φ(%d) = %d\n\n", n, resultado);
    return resultado;
}

// --------------------------------------------------
// [Função] inverseModular (CORRIGIDA)
// --------------------------------------------------
int inverseModular(int a, int m) {
    printf("[PASSO 2] Calculando inverso modular de %d mod %d:\n", a, m);

    if (m <= 1) {
        printf("  [ERRO] Módulo inválido (%d).\n", m);
        return -1;
    }

    int m0 = m;
    a = a % m0;
    if (a < 0) a += m0;

    int x0 = 0, x1 = 1;
    int temp_m = m, temp_a = a;
    int q, t;

    printf("  [INV] Iniciando Algoritmo Estendido de Euclides:\n");

    while (temp_m != 0) {
        q = temp_a / temp_m;
        t = temp_m;
        temp_m = temp_a % temp_m;
        temp_a = t;

        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
        printf("    [INV] q=%d, x0=%d, x1=%d, a=%d, m=%d\n", q, x0, x1, temp_a, temp_m);
    }

    // 🔍 CORREÇÃO: Verificar se o inverso existe APÓS o algoritmo
    if (temp_a != 1) {
        printf("  [ERRO] mdc(%d, %d) != 1 → inverso não existe.\n\n", a, m0);
        return -1;
    }

    if (x1 < 0) {
        printf("  [INV] Ajustando inverso negativo: %d + %d = ", x1, m0);
        x1 += m0;
        printf("%d\n", x1);
    }

    printf("  [INV] Inverso modular de %d mod %d = %d\n\n", a, m0, x1);
    return x1;
}

// --------------------------------------------------
// [Função] powMod
// --------------------------------------------------
int powMod(int base, int exp, int mod) {
    printf("[PASSO 3] Calculando %d^%d mod %d:\n", base, exp, mod);

    if (mod == 1) {
        printf("  [POW] Módulo = 1 → resultado = 0\n\n");
        return 0;
    }
    if (mod <= 0) {
        printf("  [ERRO] Módulo inválido (%d)\n\n", mod);
        return -1;
    }
    if (exp == 0) {
        printf("  [POW] Expoente = 0 → resultado = 1\n\n");
        return 1;
    }

    int base_mod = base % mod;
    if (base_mod < 0) base_mod += mod;
    printf("  [POW] Base normalizada: %d mod %d = %d\n", base, mod, base_mod);

    if (base_mod == 0 && exp > 0) {
        printf("  [POW] Base ≡ 0 (mod %d) → resultado = 0\n\n", mod);
        return 0;
    }

    int exp_reduzido = exp;
    char teorema[50] = "Divisão Euclidiana";

    if (eh_primo(mod)) {
        if (mdc(base_mod, mod) == 1) {
            printf("  [POW] n1 primo e mdc(a,n1)=1 → Aplicando PEQUENO TEOREMA DE FERMAT\n");
            printf("  [POW] Pelo Pequeno Teorema de Fermat: a^(n1-1) ≡ 1 (mod n1)\n");
            exp_reduzido = exp % (mod - 1);
            if (exp_reduzido == 0 && exp > 0) exp_reduzido = mod - 1;
            sprintf(teorema, "Pequeno Teorema de Fermat");
            printf("  [POW] Expoente reduzido: %d mod %d = %d\n", exp, mod - 1, exp_reduzido);
        } else {
            printf("  [POW] n1 primo mas mdc(a,n1) ≠ 1 → Aplicando DIVISÃO EUCLIDIANA\n");
        }
    } else {
        if (mdc(base_mod, mod) == 1) {
            int phi = fiEuler(mod);
            printf("  [POW] n1 não primo mas mdc(a,n1)=1 → Aplicando TEOREMA DE EULER\n");
            printf("  [POW] Pelo Teorema de Euler: a^φ(n1) ≡ 1 (mod n1)\n");
            exp_reduzido = exp % phi;
            if (exp_reduzido == 0 && exp > 0) exp_reduzido = phi;
            sprintf(teorema, "Teorema de Euler");
            printf("  [POW] Expoente reduzido: %d mod %d = %d\n", exp, phi, exp_reduzido);
        } else {
            printf("  [POW] mdc(a,n1) ≠ 1 → Aplicando DIVISÃO EUCLIDIANA\n");
        }
    }

    printf("  [POW] Método aplicado: %s\n", teorema);
    printf("  [POW] Expoente final: %d\n", exp_reduzido);

    long long res = 1;
    long long b = base_mod;
    int e = exp_reduzido, passo = 1;

    printf("  [POW] Iniciando exponenciação binária:\n");
    while (e > 0) {
        if (e & 1) {
            printf("    [POW-%02d] Expoente %d é ímpar: res = (%lld * %lld) mod %d = ", passo, e, res, b, mod);
            res = (res * b) % mod;
            printf("%lld\n", res);
        } else {
            printf("    [POW-%02d] Expoente %d é par\n", passo, e);
        }
        printf("    [POW-%02d] Elevando base: %lld^2 mod %d = ", passo, b, mod);
        b = (b * b) % mod;
        printf("%lld\n", b);
        e >>= 1;
        passo++;
    }
    printf("  [POW] Resultado final: %lld\n\n", res);
    return (int)res;
}

// --------------------------------------------------
// [main] (CORRIGIDO)
// --------------------------------------------------
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    int H, G, Zn, x, n1;
    printf("=== QUESTÃO 04 - DIVISÃO MODULAR E POTÊNCIA MODULAR ===\n\n");

    printf("Insira H: "); scanf("%d", &H);
    printf("Insira G: "); scanf("%d", &G);
    printf("Insira Zn: "); scanf("%d", &Zn);
    printf("Insira x: "); scanf("%d", &x);
    printf("Insira n1: "); scanf("%d", &n1);
    printf("\n");

    printf("[ENTRADA] H=%d, G=%d, Zn=%d, x=%d, n1=%d\n\n", H, G, Zn, x, n1);

    printf("--- ETAPA 1: DIVISÃO MODULAR ---\n");
    if (Zn <= 1) { 
        printf("[ERRO] Zn deve ser >= 2\n"); 
        return 0; 
    }

    // 🔍 CORREÇÃO: Verificar MDC apenas uma vez
    if (mdccmPassos(G, Zn) != 1) {
        printf("[ERRO] Não existe inverso modular (mdc != 1)\n");
        return 0;
    }

    int inv = inverseModular(G, Zn);
    if (inv == -1) { 
        // O erro já foi mostrado dentro da função inverseModular
        return 0; 
    }

    int a = (int)(((long long)H * inv) % Zn);
    if (a < 0) a += Zn;

    printf("[RESULTADO ETAPA 1] %d ÷ %d ≡ %d (mod %d)\n", H, G, a, Zn);
    printf("  (Cálculo: %d * %d mod %d = %d)\n\n", H, inv, Zn, a);

    printf("--- ETAPA 2: CÁLCULO DE a^x mod n1 ---\n\n");
    if (n1 <= 1) { 
        printf("[ERRO] n1 deve ser >= 2\n"); 
        return 0; 
    }

    int resultado = powMod(a, x, n1);
    printf("=== RESULTADO FINAL ===\n");
    printf("%d^%d mod %d = %d\n", a, x, n1, resultado);
    return 0;
}