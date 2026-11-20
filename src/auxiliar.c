#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "auxiliar.h"

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int lerInteiroIntervalo(const char *mensagem, int min, int max) {
    int valor, resultado;

    while (1) {
        printf("%s", mensagem);
        resultado = scanf("%d", &valor);

        // Valida se o usuário digitou letras ou símbolos
        if (resultado != 1) {
            printf("\nERRO: Entrada invalida! Digite apenas numeros.\n\n");
            limparBuffer();
            continue;
        }

        limparBuffer();

        // Valida se o usuário digitou dentro do intervalo permitido
        if (valor < min || valor > max) {
            printf("\nERRO: Opcao invalida! Digite entre %d e %d.\n\n", min, max);
            continue;
        }

        return valor; // Retorna o número válido e seguro
    }
}

float lerFloatPositivo(const char *mensagem) {
    float valor;
    int resultado;

    while (1) {
        printf("%s", mensagem);
        resultado = scanf("%f", &valor);

        // Valida se o usuário digitou letras ou símbolos
        if (resultado != 1) {
            printf("\nERRO: Entrada invalida! Digite apenas numeros.\n\n");
            limparBuffer();
            continue;
        }

        limparBuffer();

        // Valida se o usuário digitou números negativos
        if (valor < 0.0f) {
            printf("\nERRO: O valor nao pode ser negativo!\n\n");
            continue;
        }

        return valor; // Retorna o número válido e seguro
    }
}

void lerTexto(const char *mensagem, char *destino, int tamanhoMaximo) {
    while (1) {
        printf("%s", mensagem);

        if (fgets(destino, tamanhoMaximo, stdin) != NULL) {
            // Verifica se o usuário digitou mais caracteres do que o limite
            if (strchr(destino, '\n') == NULL) {
                limparBuffer(); // Descarta o excesso que sobrou no teclado
            } else {
                // Remove o '\n' (Enter) do final da string
                destino[strcspn(destino, "\n")] = '\0';
            }

            // Verifica se a string não está vazia ou apenas com espaços
            int apenasEspacos = 1;
            for (int i = 0; destino[i] != '\0'; i++) {
                if (destino[i] != ' ' && destino[i] != '\t') {
                    apenasEspacos = 0;
                    break;
                }
            }

            if (destino[0] != '\0' && !apenasEspacos) {
                break; // Texto válido, sai do laço
            }
        }

        printf("\nERRO: O campo nao pode ficar vazio!\n\n");
    }
}

void obterDataAtual(char *destino) {
    time_t agora = time(NULL);
    struct tm *tempoLocal = localtime(&agora);

    // Formata no padrão DD/MM/AAAA (tm_mon vai de 0 a 11; tm_year conta a partir de 1900)
    sprintf(destino, "%02d/%02d/%04d",
            tempoLocal->tm_mday,
            tempoLocal->tm_mon + 1,
            tempoLocal->tm_year + 1900);
}

int validarData(const char *data) {
    if (strlen(data) != 10) {
        return 0;
    }

    // Confere formato DD/MM/AAAA (barras nas posições 2 e 5; restante apenas dígitos)
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (data[i] != '/') return 0;
        } else {
            if (!isdigit((unsigned char)data[i])) return 0;
        }
    }

    int dia = 0, mes = 0, ano = 0;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        return 0;
    }

    if (ano < 2000 || ano > 2100 || mes < 1 || mes > 12) {
        return 0;
    }

    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Regra do ano bissexto para fevereiro
    int bissexto = (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
    if (bissexto) {
        diasNoMes[1] = 29;
    }

    if (dia < 1 || dia > diasNoMes[mes - 1]) {
        return 0;
    }

    return 1;
}

void lerData(const char *mensagem, char *destino) {
    char temp[30];

    while (1) {
        lerTexto(mensagem, temp, sizeof(temp));
        if (validarData(temp)) {
            strcpy(destino, temp);
            return;
        }
        printf("\nERRO: Data invalida! Use o formato DD/MM/AAAA.\n\n");
    }
}

int dataParaInteiro(const char *data) {
    int dia = 0, mes = 0, ano = 0;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        return 0;
    }
    // Converte para AAAAMMDD para facilitar comparacoes de periodo (>= e <=)
    return (ano * 10000) + (mes * 100) + dia;
}

