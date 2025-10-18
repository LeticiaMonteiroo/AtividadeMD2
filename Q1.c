//Maria Samara Alves Silva - 231027005
//Leticia da Silva Monteiro - 231026859

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef long long ll;

int eh_primo(ll n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (ll i = 3; i * i <= n; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

int eh_composto_valido(ll n) {
    if (n < 4) return 0;
    if (eh_primo(n)) return 0;
    int cont = 0;
    ll temp = n;
    for (ll i = 2; i * i <= temp; i++) {
        if (temp % i == 0) {
            cont++;
            while (temp % i == 0) temp /= i;
        }
    }
    if (temp > 1) cont++;
    return (cont == 2);
}

ll mdc(ll a, ll b) {
    printf("Calculando MDC(%lld, %lld)\n", a, b);
    ll passo = 1;
    ll a_orig = a, b_orig = b;
    while (b != 0) {
        printf("   Passo %lld: %lld = %lld*(%lld) + %lld\n", passo++, a, b, a/b, a%b);
        ll temp = a % b;
        a = b;
        b = temp;
    }
    printf("   Resultado: MDC(%lld, %lld) = %lld\n", a_orig, b_orig, a);
    return a;
}

ll euclides_estendido(ll a, ll b, ll *x, ll *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    ll x1, y1;
    ll mdc_ = euclides_estendido(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return mdc_;
}

ll inverso_modular(ll a, ll m) {
    printf("Calculando inverso modular de %lld mod %lld\n", a, m);
    ll x, y;
    ll g = euclides_estendido(a, m, &x, &y);
    if (g != 1) {
        printf("   ERRO: MDC(%lld, %lld) = %lld != 1, nao existe inverso!\n", a, m, g);
        return -1;
    }
    ll inv = (x % m + m) % m;
    printf("   Inverso modular de %lld mod %lld = %lld\n", a, m, inv);
    printf("   Verificacao: (%lld * %lld) mod %lld = %lld\n", a, inv, m, (a * inv) % m);
    return inv;
}

ll calcular_phi(ll n) {
    ll result = n;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

ll exponenciacao_modular_binaria(ll base, ll expoente, ll modulo) {
    if (modulo == 1) return 0;
    ll resultado = 1;
    base = base % modulo;
    printf("   Exponenciacao binaria: ");
    int passo = 0;
    while (expoente > 0) {
        if (expoente % 2 == 1) {
            resultado = (resultado * base) % modulo;
            printf("M");
        }
        base = (base * base) % modulo;
        expoente /= 2;
        passo++;
    }
    printf(" (%d passos)\n", passo);
    return resultado;
}

ll exponenciacao_modular(ll base, ll expoente, ll modulo, int *teorema_usado) {
    printf(">> Calculando %lld^%lld mod %lld\n", base, expoente, modulo);
    if (eh_primo(modulo)) {
        *teorema_usado = 1;
        printf("   Aplicando Pequeno Teorema de Fermat (n primo)\n");
        ll exp_red = expoente % (modulo - 1);
        printf("   Expoente reduzido: %lld mod %lld = %lld\n", expoente, modulo-1, exp_red);
        return exponenciacao_modular_binaria(base, exp_red, modulo);
    } else {
        ll mdc_val = mdc(base, modulo);
        if (mdc_val == 1) {
            *teorema_usado = 2;
            printf("   Aplicando Teorema de Euler (MDC(base, n) = 1)\n");
            ll phi = calcular_phi(modulo);
            ll exp_red = expoente % phi;
            printf("   phi(%lld) = %lld\n", modulo, phi);
            printf("   Expoente reduzido: %lld mod %lld = %lld\n", expoente, phi, exp_red);
            return exponenciacao_modular_binaria(base, exp_red, modulo);
        } else {
            *teorema_usado = 3;
            printf("   Aplicando Divisao Euclidiana (MDC(base, n) != 1)\n");
            return exponenciacao_modular_binaria(base, expoente, modulo);
        }
    }
}

ll pollard_rho(ll n) {
    printf("Pollard Rho para n = %lld\n", n);
    if (n % 2 == 0) {
        printf("   Fator 2 encontrado\n");
        return 2;
    }
    ll x = 2, y = 2, d = 1;
    int iteracao = 0;
    while (d == 1) {
        x = (x * x + 1) % n;
        y = (y * y + 1) % n;
        y = (y * y + 1) % n;
        ll diferenca = (x > y) ? (x - y) : (y - x);
        d = mdc(diferenca, n);
        iteracao++;
        printf("   Iteracao %d: x=%lld, y=%lld, |x-y|=%lld, MDC=%lld\n", iteracao, x, y, diferenca, d);
        if (d > 1 && d < n) {
            printf("   * Fator nao trivial encontrado: %lld\n", d);
            return d;
        }
        if (iteracao > 100) {
            printf("   Limite de iteracoes atingido\n");
            break;
        }
    }
    if (d == n) {
        printf("   Fator trivial, usando fallback...\n");
        for (ll i = 3; i * i <= n; i += 2) {
            if (n % i == 0) return i;
        }
    }
    return d;
}

int char_para_codigo(char c) {
    if (c == ' ') return 0;
    c = toupper(c);
    if (c >= 'A' && c <= 'Z') 
        return 11 + (c - 'A');
    return 0; 
}

char codigo_para_char(int codigo) {
    if (codigo == 0) return ' ';
    if (codigo >= 11 && codigo <= 36) 
        return 'A' + (codigo - 11);
    return '?';
}

void criar_blocos(const char *msg, int blocos[], int *num_blocos) {
    *num_blocos = 0;
    int len = strlen(msg);
    printf(">> Pre-codificacao (blocos de 2 digitos):\n");
    for (int i = 0; i < len; i++) {
        blocos[*num_blocos] = char_para_codigo(msg[i]);
        printf("   Bloco %d ('%c'): %02d\n", i + 1, msg[i], blocos[*num_blocos]);
        (*num_blocos)++;
    }
}

void decodificar_blocos(int blocos[], int num_blocos, char *msg) {
    int pos = 0;
    for (int i = 0; i < num_blocos; i++) {
        msg[pos++] = codigo_para_char(blocos[i]);
    }
    msg[pos] = '\0';
}

int main() {
    printf("=== QUESTAO 1 - RSA COM POLLARD RHO ===\n\n");
    ll N1, N2;
    do {
        printf("Digite N1 (produto de primos distintos, 100-9999): ");
        scanf("%lld", &N1);
        if (N1 < 100 || N1 > 9999) {
            printf("   ERRO: N1 deve estar entre 100 e 9999!\n");
            continue;
        }
        if (!eh_composto_valido(N1)) {
            printf("   ERRO: N1 deve ser produto de dois primos distintos!\n");
            continue;
        }
        break;
    } while (1);
    do {
        printf("Digite N2 (produto de primos distintos, 100-9999): ");
        scanf("%lld", &N2);
        if (N2 < 100 || N2 > 9999) {
            printf("   ERRO: N2 deve estar entre 100 e 9999!\n");
            continue;
        }
        if (!eh_composto_valido(N2)) {
            printf("   ERRO: N2 deve ser produto de dois primos distintos!\n");
            continue;
        }
        if (N1 == N2) {
            printf("   ERRO: N2 deve ser diferente de N1!\n");
            continue;
        }
        break;
    } while (1);

    printf("\n=== ETAPA 1: FATORACAO (METODO rho DE POLLARD) ===\n");
    ll p = pollard_rho(N1);
    ll q1 = N1 / p;
    if (!eh_primo(q1)) {
        printf(">> Segundo fator de N1 nao e primo, refinando...\n");
        ll temp = q1;
        q1 = pollard_rho(temp);
        p = temp / q1;
    }
    ll q = pollard_rho(N2);
    ll q2 = N2 / q;
    if (!eh_primo(q2)) {
        printf(">> Segundo fator de N2 nao e primo, refinando...\n");
        ll temp = q2;
        q2 = pollard_rho(temp);
        q = temp / q2;
    }
    printf("\n=== FATORES ENCONTRADOS ===\n");
    printf("N1 = %lld = %lld x %lld\n", N1, p, q1);
    printf("N2 = %lld = %lld x %lld\n", N2, q, q2);
    printf("Usando p = %lld (de N1) e q = %lld (de N2)\n", p, q);
    printf("\n=== ETAPA 2: GERACAO DAS CHAVES RSA ===\n");
    ll n = p * q;
    ll phi = (p - 1) * (q - 1);
    printf("n = p x q = %lld x %lld = %lld\n", p, q, n);
    printf("phi(n) = (p-1) x (q-1) = %lld x %lld = %lld\n", p-1, q-1, phi);
    ll e;
    printf("\nBuscando expoente publico e (MDC(e, phi(n)) = 1)...\n");
    for (e = 2; e < phi; e++) { 
        if (mdc(e, phi) == 1) {
            break;
        }
        if (e > 10000) { 
            e = 65537;
            break;
        }
    }
    printf("Expoente publico e = %lld\n", e);
    ll d = inverso_modular(e, phi);
    if (d == -1) {
        printf("ERRO CRITICO: Nao foi possivel encontrar inverso modular!\n");
        return 1;
    }
    printf("\n=== CHAVES GERADAS ===\n");
    printf("CHAVE PUBLICA: (n=%lld, e=%lld)\n", n, e);
    printf("CHAVE PRIVADA: (n=%lld, d=%lld)\n", n, d);
    getchar(); 
    char msg_original[100];
    printf("\nDigite a mensagem (somente letras e espacos): ");
    fgets(msg_original, sizeof(msg_original), stdin);
    msg_original[strcspn(msg_original, "\n")] = 0;
    for (int i = 0; msg_original[i]; i++) {
        msg_original[i] = toupper(msg_original[i]);
    }
    int blocos[100], num_blocos;
    criar_blocos(msg_original, blocos, &num_blocos);
    printf("\n=== ETAPA 3: CRIPTOGRAFIA ===\n");
    ll texto_cifrado[100];
    for (int i = 0; i < num_blocos; i++) {
        printf("\n--- Criptografando Bloco %d: %02d ---\n", i + 1, blocos[i]);
        int teorema;
        texto_cifrado[i] = exponenciacao_modular(blocos[i], e, n, &teorema);
        printf("Resultado: %02d -> %lld\n", blocos[i], texto_cifrado[i]);
    }
    printf("\nMensagem criptografada (numerica): ");
    for (int i = 0; i < num_blocos; i++) 
        printf("%lld ", texto_cifrado[i]);
    printf("\n");
    printf("\n=== DESCRIPTOGRAFIA ===\n");
    int texto_decifrado[100];
    ll d_reduzido = d % phi;
    printf(">> Usando expoente 'd' reduzido para descriptografia\n");
    printf("   Teorema de Euler (Generalizado/RSA)\n");
    printf("   d_red = %lld mod %lld = %lld\n", d, phi, d_reduzido);
    for (int i = 0; i < num_blocos; i++) {
        printf("\n--- Descriptografando Bloco %d: %lld ---\n", i + 1, texto_cifrado[i]);
        texto_decifrado[i] = exponenciacao_modular_binaria(texto_cifrado[i], d_reduzido, n);
        printf("Resultado: %lld -> %02d\n", texto_cifrado[i], texto_decifrado[i]);
    }
    char msg_decifrada[100];
    decodificar_blocos(texto_decifrado, num_blocos, msg_decifrada);
    printf("\n=== VERIFICACAO FINAL ===\n");
    printf("Mensagem original:  '%s'\n", msg_original);
    printf("Mensagem decifrada: '%s'\n", msg_decifrada);
    if (strcmp(msg_original, msg_decifrada) == 0) {
        printf("Mensagem recuperada perfeitamente!\n");
    } else {
        printf("As mensagens nao coincidem!\n");
    }
    printf("\n=== FIM DO PROGRAMA ===\n");
    return 0;
}
