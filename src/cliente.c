#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"
#include "auxiliar.h"

/* --------------     Funções de cadastro     -------------- */
void cadastrarCliente(ListaClientes *lista) {
    Cliente novo;

    // Garante código único mesmo se a lista estiver fora de ordem
    int maiorCodigo = 0;
    for (int i = 0; i < lista->total; i++) {
        if (lista->dados[i].codigo > maiorCodigo) {
            maiorCodigo = lista->dados[i].codigo;
        }
    }
    novo.codigo = maiorCodigo + 1;

    printf("\n=============== CADASTRO DE CLIENTE ==============\n");

    lerTexto("Nome completo: ", novo.nome, sizeof(novo.nome));
    lerTexto("Nome social (ou '-' se nao tiver): ", novo.nomeSocial, sizeof(novo.nomeSocial));
    lerTexto("CPF: ", novo.cpf, sizeof(novo.cpf));
    lerTexto("Endereco (Rua e numero): ", novo.endereco, sizeof(novo.endereco));
    lerTexto("Bairro: ", novo.bairro, sizeof(novo.bairro));
    lerTexto("Telefone/WhatsApp: ", novo.telefone, sizeof(novo.telefone));

    if (adicionarCliente(lista, novo)) {
        printf("\nCliente '%s' (Cod. %d) cadastrado com sucesso!\n", novo.nome, novo.codigo);
    }
}

int adicionarCliente(ListaClientes *lista, Cliente novo) {
    int novoTotal = lista->total + 1;

    Cliente *temp = realloc(lista->dados, novoTotal * sizeof(Cliente));

    if (temp == NULL) {
        printf("\nERRO: Falha ao alocar memoria para Cliente!\n");
        return 0;
    }

    lista->dados = temp;
    lista->dados[lista->total] = novo;
    lista->total = novoTotal;

    return 1;
}

/* --------------     Funções de consulta, listagem e ordenação     -------------- */
// Exibe lista compacta (50 colunas) para seleção rápida durante a venda
void exibirCatalogoClientes(const ListaClientes *lista) {
    if (lista->total == 0) {
        printf("\nAVISO: Nao existem clientes cadastrados!\n");
        return;
    }

    printf("\n============== CATALOGO DE CLIENTES ==============\n");
    printf("%-4s | %-43s\n", "COD", "NOME DO CLIENTE");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < lista->total; i++) {
        printf("%-4d | %-43.43s\n", lista->dados[i].codigo, lista->dados[i].nome);
    }
    printf("==================================================\n");
}

void listarClientes(const ListaClientes *lista) {
    if (lista->total == 0) {
        printf("\nAVISO: Nao existem clientes cadastrados!\n");
        return;
    }

    // Cria cópia temporária para ordenar sem quebrar o contrato 'const' da lista original
    ListaClientes copia = {0};
    copia.dados = (Cliente *) malloc(lista->total * sizeof(Cliente));
    if (copia.dados == NULL) {
        printf("\nERRO: Falha ao alocar memoria para o relatorio!\n");
        return;
    }
    memcpy(copia.dados, lista->dados, lista->total * sizeof(Cliente));
    copia.total = lista->total;

    ordenarClientesPorNome(&copia);

    printf("\n============= RELATORIO DE CLIENTES ==============\n");

    for (int i = 0; i < copia.total; i++) {
        printf("%-46s#%3d\n", "Cliente:", i + 1);
        printf("%-35s%15d\n", "Codigo:", copia.dados[i].codigo);
        printf("%-15s %34.34s\n", "Nome:", copia.dados[i].nome);
        printf("%-15s %34.34s\n", "Nome Social:", copia.dados[i].nomeSocial);
        printf("%-15s %34.34s\n", "CPF:", copia.dados[i].cpf);
        printf("%-15s %34.34s\n", "Endereco:", copia.dados[i].endereco);
        printf("%-15s %34.34s\n", "Bairro:", copia.dados[i].bairro);
        printf("%-15s %34.34s\n", "Telefone:", copia.dados[i].telefone);
        printf("--------------------------------------------------\n");
    }

    free(copia.dados);
}

void ordenarClientesPorNome(ListaClientes *lista) {
    if (lista->total > 1) {
        for (int i = 0; i < lista->total - 1; i++) {
            for (int j = 0; j < lista->total - 1 - i; j++) {
                if (strcmp(lista->dados[j].nome, lista->dados[j + 1].nome) > 0) {
                    Cliente temp = lista->dados[j];
                    lista->dados[j] = lista->dados[j + 1];
                    lista->dados[j + 1] = temp;
                }
            }
        }
    }
}

// Busca um cliente pelo codigo e retorna seu indice no vetor (-1 se nao encontrar)
int buscarClientePorCodigo(const ListaClientes *lista, int codigo) {
    for (int i = 0; i < lista->total; i++) {
        if (lista->dados[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

/* --------------     Funções de persistência de dados     -------------- */
int salvarClientes(const ListaClientes *lista) {
    // Grava primeiro em arquivo temporário (Gravação Atômica)
    FILE *arq = fopen("clientes.tmp", "wb");
    if (arq == NULL) {
        printf("\nERRO: Falha ao criar o arquivo temporario de clientes!\n");
        return 0;
    }

    // Grava a quantidade de clientes e verifica se deu certo
    if (fwrite(&lista->total, sizeof(int), 1, arq) != 1) {
        printf("\nERRO: Falha ao gravar a quantidade de clientes!\n");
        fclose(arq);
        remove("clientes.tmp");
        return 0;
    }

    // Se houver clientes cadastrados, grava o bloco inteiro
    if (lista->total > 0 && lista->dados != NULL) {
        if (fwrite(lista->dados, sizeof(Cliente), lista->total, arq) != (size_t)lista->total) {
            printf("\nERRO: Falha ao gravar os dados dos clientes!\n");
            fclose(arq);
            remove("clientes.tmp");
            return 0;
        }
    }

    fclose(arq);

    // Faz a troca atômica garantindo integridade do arquivo
    remove("clientes.dat"); // Se o arquivo não existir na primeira vez, o retorno é ignorado
    if (rename("clientes.tmp", "clientes.dat") != 0) {
        printf("\nERRO: Falha ao realizar a troca dos arquivos!\n");
        return 0;
    }

    return 1;
}

int carregarClientes(ListaClientes *lista) {
    FILE *arq = fopen("clientes.dat", "rb");

    if (arq == NULL) {
        lista->dados = NULL;
        lista->total = 0;
        return 1;
    }

    int qtd = 0;
    if (fread(&qtd, sizeof(int), 1, arq) != 1) {
        fclose(arq);
        return 0;
    }

    if (qtd > 0) {
        Cliente *temp = (Cliente *) realloc(lista->dados, qtd * sizeof(Cliente));
        if (temp == NULL) {
            printf("\nERRO: Falha ao alocar memoria para clientes!\n");
            fclose(arq);
            return 0;
        }

        if (fread(temp, sizeof(Cliente), qtd, arq) != (size_t)qtd) {
            printf("\nERRO: Falha na leitura do registro de clientes!\n");
            free(temp);
            fclose(arq);
            return 0;
        }

        lista->dados = temp;
        lista->total = qtd;
    }

    fclose(arq);
    return 1;
}
