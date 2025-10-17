#include <stdio.h>

int mdc(int a, int b) {
    printf("Calculando mdc(%d, %d):\n", a, b);
    while (b != 0) {
        int resto = a % b;
        printf("  %d mod %d = %d\n", a, b, resto);
        a = b;
        b = resto;
    }
    printf("  mdc = %d\n\n", a);
    return a;
}

// Função de Euclides estendido para encontrar o inverso modular
int inverso_modular(int a, int m) {
    printf("Calculando inverso modular de %d mod %d:\n", a, m);
    int m0 = m, x0 = 0, x1 = 1, q, t;
    
    if (m == 1) return 0;

    int A = a, B = m;  // Guardar valores originais

    // CORREÇÃO: usar while (m != 0) evita divisão por zero e segue o algoritmo padrão
    while (m != 0) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
        printf("  q=%d, x0=%d, x1=%d, a=%d, m=%d\n", q, x0, x1, a, m);
    }

    // 'a' agora é o mdc; se gcd != 1, não existe inverso
    if (a != 1) {
        printf("  Aviso: gcd != 1 (gcd = %d). Inverso não existe.\n\n", a);
        return -1;
    }

    if (x1 < 0) {
        printf("  Ajustando inverso negativo: %d + %d = ", x1, m0);
        x1 += m0;
        printf("%d\n", x1);
    }

    printf("  Inverso modular de %d mod %d = %d\n\n", A, B, x1);
    return x1;
}

// Função para calcular a^b mod n usando exponenciação binária
int powMod(int base, int exp, int mod) {
    printf("Calculando %d^%d mod %d:\n", base, exp, mod);
    long long res = 1;
    long long b = base % mod;
    int e = exp;
    
    while (e > 0) {
        if (e % 2 == 1) {
            printf("  Expoente %d é ímpar: res = (%lld * %lld) mod %d = ", e, res, b, mod);
            res = (res * b) % mod;
            printf("%lld\n", res);
        } else {
            printf("  Expoente %d é par\n", e);
        }
        printf("  Elevando base ao quadrado: %lld^2 mod %d = ", b, mod);
        b = (b * b) % mod;
        printf("%lld\n", b);
        e /= 2;
    }
    printf("  Resultado: %lld\n\n", res);
    return (int)res;
}

// Função para verificar se um número é primo
int eh_primo(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Função para calcular fi(n) (Totiente de Euler)
int fiEuler(int n) {
    printf("Calculando φ(%d):\n", n);
    int resultado = n;
    int temp = n;
    
    for (int i = 2; i * i <= temp; i++) {
        if (temp % i == 0) {
            printf("  Fator primo: %d\n", i);
            while (temp % i == 0)
                temp /= i;
            resultado -= resultado / i;
            printf("  φ parcial = %d\n", resultado);
        }
    }
    if (temp > 1) {
        printf("  Fator primo: %d\n", temp);
        resultado -= resultado / temp;
    }
    printf("  φ(%d) = %d\n\n", n, resultado);
    return resultado;
}

int main() {
    int H, G, Zn, x, n1;
    
    printf("=== QUESTÃO 04 - DIVISÃO MODULAR E POTÊNCIA MODULAR ===\n\n");
    printf("Digite H G Zn x n1: ");
    if (scanf("%d %d %d %d %d", &H, &G, &Zn, &x, &n1) != 5) {
        printf("Entrada inválida.\n");
        return 1;
    }
    
    printf("\nValores inseridos: H=%d, G=%d, Zn=%d, x=%d, n1=%d\n\n", H, G, Zn, x, n1);

    printf("--- ETAPA 1: DIVISÃO MODULAR H ÷ G (mod Zn) ---\n");

    // Verificar se G tem inverso modular em Zn
    if (mdc(G, Zn) != 1) {
        printf("ERRO: Não existe inverso modular, pois mdc(%d, %d) != 1\n", G, Zn);
        return 0;
    }

    int inv = inverso_modular(G, Zn);
    if (inv == -1) {
        printf("ERRO: inverso não existe (gcd != 1)\n");
        return 0;
    }

    int resultado_div = (H * inv) % Zn;
    if (resultado_div < 0) resultado_div += Zn;
    
    printf("Divisão modular: (%d * %d) mod %d = %d\n", H, inv, Zn, resultado_div);
    printf("Portanto, %d ÷ %d ≡ %d (mod %d)\n\n", H, G, resultado_div, Zn);

    printf("--- ETAPA 2: CÁLCULO DE a^x mod n1 ---\n");
    printf("a = %d, x = %d, n1 = %d\n", resultado_div, x, n1);

    int a = resultado_div;
    int exp_reduzido = x;
    char teorema_usado[50] = "Divisão Euclidiana";

    // Verificar qual teorema aplicar
    if (eh_primo(n1)) {
        printf("n1 = %d é primo → Aplicando PEQUENO TEOREMA DE FERMAT\n", n1);
        printf("Pelo Pequeno Teorema de Fermat: a^(n1-1) ≡ 1 (mod n1)\n");
        exp_reduzido = x % (n1 - 1);
        if (exp_reduzido == 0 && x > 0) exp_reduzido = n1 - 1;
        sprintf(teorema_usado, "Pequeno Teorema de Fermat");
        printf("Expoente reduzido: %d mod %d = %d\n", x, n1 - 1, exp_reduzido);
    } else if (mdc(a, n1) == 1) {
        int phi = fiEuler(n1);
        printf("n1 não é primo, mas mdc(a, n1) = 1 → Aplicando TEOREMA DE EULER\n");
        printf("Pelo Teorema de Euler: a^φ(n1) ≡ 1 (mod n1)\n");
        exp_reduzido = x % phi;
        if (exp_reduzido == 0 && x > 0) exp_reduzido = phi;
        sprintf(teorema_usado, "Teorema de Euler");
        printf("Expoente reduzido: %d mod %d = %d\n", x, phi, exp_reduzido);
    } else {
        printf("Aplicando DIVISÃO EUCLIDIANA (nenhum teorema aplicável)\n");
        sprintf(teorema_usado, "Divisão Euclidiana");
    }

    printf("\n--- CÁLCULO FINAL (%s) ---\n", teorema_usado);
    int resultado_final = powMod(a, exp_reduzido, n1);
    
    printf("=== RESULTADO FINAL ===\n");
    printf("%d^%d mod %d = %d\n", a, x, n1, resultado_final);
    
    return 0;
}
