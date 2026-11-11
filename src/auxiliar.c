#include <stdio.h>
#include <string.h>
#include "auxiliar.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int lerInteiroIntervalo(const char *mensagem, int min, int max) {
    int valor, resultado;

    while (1) { // = while(true)
        printf("%s", mensagem);
        resultado = scanf("%d", &valor);

        // Valida se o usuário digitou letras ou simbolos
        if (resultado != 1) {
            printf("\nErro: Entrada Invalida! Digite apenas numeros!\n\n");
            limparBuffer();
            continue;
        }

        limparBuffer();

        // Valida se o usuário digitou dentro do intervalo permitido
        if (valor < min || valor > max) {
            printf("\nErro: Opcao Invalida! Diginte um valor entre %d e %d!\n\n", min, max);
            continue;
        }

        return valor; // Retorna o número válido e seguro.
    }
}

float lerFloatPositivo(const char *mensagem) {
    float valor;
    int resultado;
    while (1) {
        printf("%s", mensagem);
        resultado = scanf("%f", &valor);

        // Valida se o usuário digitou letras ou simbolos
        if (resultado != 1) {
            printf("\nErro: Entrada invalida! Digite apenas numeros!\n\n");
            limparBuffer();
            continue;
        }

        limparBuffer();

        // Valida se o usuario digitou números negativos
        if (valor < 0.0f) {
            printf("\nErro: Valor nao pode ser negativo!\n\n");
            continue;
        }

        return valor; // Retorna o número válido e seguro
    }
}

void lerTexto(const char *mensagem, char *destino, int tamanhoMaximo) {
    char formato[20];

    // Cria uma máscara de leitura dinâmica de tamanho para não estourar memória (ex: %12s)
    sprintf(formato, " %%%ds", tamanhoMaximo - 1);

    printf("%s", mensagem);
    scanf(formato, destino);
    limparBuffer();
}
