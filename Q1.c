#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;

/* -------------------- Funções Matemáticas -------------------- */

/* Teste de primalidade simples */
int eh_primo(ll n) {
    if (n < 2) return 0;
    for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

/* Cálculo manual do MDC (Algoritmo de Euclides) mostrando passo a passo */
ll calcular_mdc(ll a, ll b) {
    printf("\n>> Calculando o MDC(%lld, %lld) usando o Algoritmo de Euclides:\n", a, b);
    ll resto;
    while (b != 0) {
        resto = a % b;
        printf("   %lld = %lld * (%lld) + %lld\n", a, b, a / b, resto);
        a = b;
        b = resto;
    }
    printf(">> Resultado: MDC = %lld\n", a);
    return a;
}

/* Função f(x) = (x² + 1) mod n para o Pollard Rho */
ll funcao_g(ll x, ll n) {
    return (x * x + 1) % n;
}

/* Exponenciação modular binária */
ll exponenciacao_modular_binaria(ll base, ll expoente, ll modulo) {
    ll resultado = 1 % modulo;
    ll passo = 0;
    printf("Método: exponenciação modular binária\n");
    
    while (expoente > 0) {
        printf("Passo %lld: base = %lld, expoente = %lld", ++passo, base, expoente);
        if (expoente % 2 == 1) {
            resultado = (resultado * base) % modulo;
            printf(" -> multiplica: resultado = %lld", resultado);
        }
        base = (base * base) % modulo;
        expoente = expoente / 2;
        printf("\n");
    }
    printf(">> Resultado final = %lld\n", resultado);
    return resultado;
}

/* Exponenciação modular com seleção automática do teorema */
ll exponenciacao_modular_teorema(ll base, ll expoente, ll modulo, int *teorema_usado) {
    printf("\n>> Calculando %lld^%lld mod %lld\n", base, expoente, modulo);
    
    /* Verifica qual teorema aplicar */
    if (eh_primo(modulo)) {
        printf("Aplicando Pequeno Teorema de Fermat (n é primo)\n");
        *teorema_usado = 1;
        /* Pequeno Teorema de Fermat: a^(p-1) ≡ 1 mod p */
        ll exp_reduzido = expoente % (modulo - 1);
        printf("Expoente reduzido: %lld mod %lld = %lld\n", expoente, modulo - 1, exp_reduzido);
        return exponenciacao_modular_binaria(base, exp_reduzido, modulo);
    }
    else {
        /* Calcula MDC para verificar se podemos usar Euler */
        ll mdc_valor = calcular_mdc(base, modulo);
        if (mdc_valor == 1) {
            printf("Aplicando Teorema de Euler (MDC(base, n) = 1)\n");
            *teorema_usado = 2;
            return exponenciacao_modular_binaria(base, expoente, modulo);
        }
        else {
            printf("Aplicando Divisão Euclidiana (caso geral - MDC ≠ 1)\n");
            *teorema_usado = 3;
            return exponenciacao_modular_binaria(base, expoente, modulo);
        }
    }
}

/* Pollard Rho - fatoração de número composto */
ll pollard_rho(ll n) {
    printf("\n--- Iniciando Pollard Rho para N = %lld ---\n", n);
    printf("Função de iteração: g(x) = (x² + 1) mod N\n");
    printf("Semente inicial: x0 = 2\n");
    
    ll x = 2, y = 2, d = 1;
    int iteracoes = 0;

    while (d == 1) {
        x = funcao_g(x, n);
        y = funcao_g(funcao_g(y, n), n);
        
        ll diferenca = x - y;
        if (diferenca < 0) diferenca = -diferenca;

        printf("\nIteração %d:\n", ++iteracoes);
        printf("  x = %lld, y = %lld\n", x, y);
        printf("  |x - y| = %lld\n", diferenca);
        
        d = calcular_mdc(diferenca, n);
        printf("  MDC(|x - y|, N) = %lld\n", d);

        if (d == n) {
            printf("\nPollard Rho falhou - MDC = N\n");
            return -1;
        }
    }

    printf("\n>> Fator não trivial encontrado: %lld\n", d);
    return d;
}

/* Algoritmo de Euclides estendido (para inverso modular) */
ll euclides_estendido(ll a, ll b, ll *x, ll *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    
    ll x1, y1;
    ll mdc = euclides_estendido(b, a % b, &x1, &y1);
    
    *x = y1;
    *y = x1 - (a / b) * y1;
    
    return mdc;
}

/* Cálculo do inverso modular manualmente */
ll inverso_modular(ll a, ll m) {
    printf("\n>> Calculando o inverso modular de %lld mod %lld\n", a, m);
    printf("Usando Algoritmo Estendido de Euclides:\n");
    
    ll x, y;
    ll mdc = euclides_estendido(a, m, &x, &y);
    
    printf("Equação: %lld*%lld + %lld*%lld = %lld\n", a, x, m, y, mdc);
    
    if (mdc != 1) {
        printf("ERRO: Não existe inverso, pois MDC(%lld, %lld) = %lld ≠ 1\n", a, m, mdc);
        return -1;
    }
    
    /* Ajusta para resultado positivo */
    ll inv = (x % m + m) % m;
    printf("Inverso encontrado: %lld\n", inv);
    printf("Verificação: %lld * %lld mod %lld = %lld\n", a, inv, m, (a * inv) % m);
    
    return inv;
}

/* Conversões de caracteres */
int letra_para_codigo(char c) {
    if (c == ' ') return 0;
    if (c >= 'A' && c <= 'Z') return 11 + (c - 'A');
    if (c >= 'a' && c <= 'z') return 11 + (c - 'a');
    return -1;
}

char codigo_para_letra(int codigo) {
    if (codigo == 0) return ' ';
    if (codigo >= 11 && codigo <= 36) return 'A' + (codigo - 11);
    return '?';
}

/* -------------------- Programa Principal -------------------- */
int main() {
    ll N1, N2;
    printf("=== Questão 1 – RSA com Pollard Rho ===\n");
    printf("Digite N1 (deve ser produto de dois primos distintos, 100-9999): ");
    scanf("%lld", &N1);
    printf("Digite N2 (deve ser produto de dois primos distintos, 100-9999): ");
    scanf("%lld", &N2);

    /* validações */
    if (N1 < 100 || N1 > 9999 || N2 < 100 || N2 > 9999) {
        printf("\nERRO: N1 e N2 devem estar entre 100 e 9999.\n");
        return 0;
    }
    
    if (N1 == N2) {
        printf("\nERRO: N1 e N2 devem ser diferentes.\n");
        return 0;
    }

    printf("\n=== ETAPA 1: Fatoração usando Método ρ de Pollard ===\n");
    
    ll p = pollard_rho(N1);
    if (p == -1) {
        printf("\nERRO: Falha na fatoração de N1.\n");
        return 0;
    }
    ll q1 = N1 / p;
    
    ll q = pollard_rho(N2);
    if (q == -1) {
        printf("\nERRO: Falha na fatoração de N2.\n");
        return 0;
    }
    ll q2 = N2 / q;

    /* Verifica se os fatores são primos */
    if (!eh_primo(p) || !eh_primo(q1)) {
        printf("\nERRO: Fatores de N1 não são primos.\n");
        return 0;
    }
    
    if (!eh_primo(q) || !eh_primo(q2)) {
        printf("\nERRO: Fatores de N2 não são primos.\n");
        return 0;
    }

    printf("\n=== Fatores encontrados: ===\n");
    printf("N1 = %lld = %lld × %lld\n", N1, p, q1);
    printf("N2 = %lld = %lld × %lld\n", N2, q, q2);
    printf("Usando p = %lld (de N1) e q = %lld (de N2)\n", p, q);

    printf("\n=== ETAPA 2: Geração das Chaves RSA ===\n");
    
    ll n = p * q;
    ll phi = (p - 1) * (q - 1);

    printf("Cálculos:\n");
    printf("  n = p × q = %lld × %lld = %lld\n", p, q, n);
    printf("  φ(n) = (p-1) × (q-1) = %lld × %lld = %lld\n", p-1, q-1, phi);

    /* Escolhe o menor E > 1 tal que MDC(E, phi) = 1 */
    ll e;
    for (e = 2; e < phi; e++) {
        if (calcular_mdc(e, phi) == 1) {
            break;
        }
    }
    printf("\nExpoente público e encontrado: %lld\n", e);

    /* Calcula o expoente privado D */
    ll d = inverso_modular(e, phi);
    if (d == -1) {
        printf("ERRO: Não foi possível calcular o expoente privado D.\n");
        return 0;
    }

    printf("\n=== Chaves RSA Geradas: ===\n");
    printf("Chave pública: (n=%lld, e=%lld)\n", n, e);
    printf("Chave privada: (n=%lld, d=%lld)\n", n, d);

    printf("\n=== ETAPA 3: Codificação e Decodificação ===\n");
    
    getchar(); /* consumir newline */
    char mensagem[256];
    printf("Digite a mensagem (apenas letras maiúsculas, minúsculas e espaços): ");
    fgets(mensagem, sizeof(mensagem), stdin);
    mensagem[strcspn(mensagem, "\n")] = 0;

    printf("\n--- Pré-codificação ---\n");
    printf("Sistema: A=11, B=12, ..., Z=36, ESPAÇO=00\n");
    
    int blocos[256], tamanho = 0;
    for (int i = 0; mensagem[i]; i++) {
        int codigo = letra_para_codigo(mensagem[i]);
        if (codigo >= 0) {
            blocos[tamanho++] = codigo;
            printf("'%c' -> %02d\n", mensagem[i], codigo);
        }
    }

    printf("\n--- Criptografia (M → C) ---\n");
    printf("Fórmula: C ≡ M^e (mod n)\n");
    
    ll criptografado[256];
    for (int i = 0; i < tamanho; i++) {
        int teorema;
        printf("\nBloco %d: M = %02d\n", i + 1, blocos[i]);
        criptografado[i] = exponenciacao_modular_teorema(blocos[i], e, n, &teorema);
        printf("Resultado: C = %lld\n", criptografado[i]);
    }

    printf("\n--- Mensagem Criptografada ---\n");
    printf("Representação numérica: ");
    for (int i = 0; i < tamanho; i++) {
        printf("%lld ", criptografado[i]);
    }
    printf("\n");

    printf("Tentativa de representação em caracteres: ");
    for (int i = 0; i < tamanho; i++) {
        /* Converte explicitamente para int antes de usar como char */
        int valor_char = (int)criptografado[i];
        if (valor_char >= 11 && valor_char <= 36) {
            printf("%c", 'A' + (valor_char - 11));
        } else if (valor_char == 0) {
            printf(" ");
        } else {
            printf("?");
        }
    }
    printf("\n");

    printf("\n--- Resumo da Criptografia ---\n");
    printf("Mensagem original: %s\n", mensagem);
    printf("Pré-codificação:   ");
    for (int i = 0; i < tamanho; i++) {
        printf("%02d ", blocos[i]);
    }
    printf("\n");
    printf("Criptografada:     ");
    for (int i = 0; i < tamanho; i++) {
        printf("%02lld ", criptografado[i]);
    }
    printf("\n");

    printf("\n--- Descriptografia (C → M) ---\n");
    printf("Fórmula: M ≡ C^d (mod n)\n");
    
    int decodificado[256];
    for (int i = 0; i < tamanho; i++) {
        int teorema;
        printf("\nBloco %d: C = %lld\n", i + 1, criptografado[i]);
        decodificado[i] = (int)exponenciacao_modular_teorema(criptografado[i], d, n, &teorema);
        printf("Resultado: M' = %02d\n", decodificado[i]);
    }

    printf("\n--- Reconversão numérica em texto ---\n");
    printf("Mensagem decodificada: ");
    for (int i = 0; i < tamanho; i++) {
        printf("%c", codigo_para_letra(decodificado[i]));
    }
    printf("\n");

    /* Verificação final */
    printf("\n=== Verificação ===\n");
    printf("Mensagem original: %s\n", mensagem);
    printf("Mensagem decifrada: ");
    for (int i = 0; i < tamanho; i++) {
        printf("%c", codigo_para_letra(decodificado[i]));
    }
    printf("\n");
    
    int iguais = 1;
    for (int i = 0; i < tamanho; i++) {
        if (letra_para_codigo(mensagem[i]) != decodificado[i]) {
            iguais = 0;
            break;
        }
    }
    
    if (iguais) {
        printf("✓ CONFIRMAÇÃO: Mensagem decifrada é IDÊNTICA à original!\n");
    } else {
        printf("✗ ERRO: Mensagem decifrada NÃO coincide com a original!\n");
    }

    printf("\n=== FIM DO PROGRAMA ===\n");
    return 0;
}