#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"
#include "auxiliar.h"

/* --------------     Funções de cadastro     -------------- */
void cadastrarCliente(ListaClientes *lista) {
    Cliente novo;

    novo.codigo = lista->total + 1;

    lerTexto("Nome completo: ", novo.nome, sizeof(novo.nome));
    lerTexto("Nome social: ", novo.nomeSocial, sizeof(novo.nomeSocial));
    lerTexto("CPF: ", novo.cpf, sizeof(novo.cpf));
    lerTexto("Endereco (Rua e numero): ", novo.endereco, sizeof(novo.endereco));
    lerTexto("Bairro: ", novo.bairro, sizeof(novo.bairro));
    lerTexto("Telefone/WhatsApp: ", novo.telefone, sizeof(novo.telefone));

    adicionarCliente(lista, novo);
    printf("\nCliente '%s' cadastrado com sucesso!\n", novo.nome);
}

int adicionarCliente(ListaClientes *lista, Cliente novo) {
    int novoTotal = lista->total + 1;

    Cliente *temp = realloc(lista->dados, novoTotal * sizeof(Cliente));

    if (temp == NULL) {
        printf("\nErro: Falha ao alocar memoria para Cliente!\n");
        return 0;
    }

    lista->dados = temp;
    lista->dados[lista->total] = novo;
    lista->total = novoTotal;

    return 1;
}

/* --------------     Funções de ordenação     -------------- */
void listarClientes(const ListaClientes *lista) {
    if (lista->total == 0) {
        printf("\nErro: Nao existem clientes cadastrados!\n");
        return;
    }

    ordenarClientesPorNome((ListaClientes *) lista);

    printf("\n============= RELATORIO DE CLIENTES ==============\n");
    
    for (int i = 0; i < lista->total; i++) {
        printf("%-46s#%3d\n", "Cliente:", i + 1);
        printf("%-15s %34.34s\n", "Nome:", lista->dados[i].nome);
        printf("%-15s %34.34s\n", "Nome Social:", lista->dados[i].nomeSocial);
        printf("%-15s %34.34s\n", "CPF:", lista->dados[i].cpf);
        printf("%-15s %34.34s\n", "Endereco:", lista->dados[i].endereco);
        printf("%-15s %34.34s\n", "Bairro:", lista->dados[i].bairro);
        printf("%-15s %34.34s\n", "Telefone:", lista->dados[i].telefone);
        printf("--------------------------------------------------\n");
    }
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

/* --------------     Funções de persistência de dados     -------------- */
int salvarClientes(const ListaClientes *lista) {
    // Grava primeiro um arquivo temporário (conceitos de Gravação Atômica)
    FILE *arq = fopen("clientes.tmp", "wb");
    if (arq == NULL) {
        printf("\nErro: Falha ao criar o aquivo de clientes temporario!\n");
        return 0;
    }

    // Grava a quantidade de clientes e verifica se deu certo, se não, remove o arquivo temporário
    if (fwrite(&lista->total, sizeof(int), 1, arq) != 1) {
        printf("\nErro: Falha ao gravar a quantidade de clientes!\n");
        fclose(arq);
        remove("clientes.tmp");
        return 0;
    }

    // Se tiver clientes cadastrados, grava o bloco no arquivo e verifica se a gravação foi bem sucedida
    if (lista->total > 0 && lista->dados != NULL) {
        if (fwrite(lista->dados, sizeof(Cliente), lista->total, arq) != (size_t)lista->total) {
            printf("\nErro: Falha ao gravar os dados dos clientes!\n");
            fclose(arq);
            remove("clientes.tmp");
            return 0;
        }
    }

    // Se tudo deu certo, fecha o arquivo temporário com os dados salvos com sucesso
    fclose(arq);

    // Faz a troca atômica garantindo 100% de sucesso na gravação do arquivo e verifica
    remove("clientes.dat"); // se o arquivo não existir na primeira vez, a função retona um erro que é ignorado
    if (rename("clientes.tmp", "clientes.dat") != 0) {
        printf("\nErro: Falha ao realizar a troca dos arquivos!\n");
        return 0;
    }

    return 1; // Sucesso
}

int carregarClientes(ListaClientes *lista) {
    FILE *arq = fopen("clientes.dat", "rb");

    if (arq == NULL) {
        lista->dados = NULL;
        lista->total = 0;
        return 1;
    }

    // Se o arquivo existe, lê a quantidade e verifica se deu certo, caso não, fecha o arquivo
    int qtd = 0;
    if (fread(&qtd, sizeof(int), 1, arq) != 1) {
        fclose(arq);
        return 0;
    }

    if (qtd > 0) {
        Cliente *temp = (Cliente *) realloc(lista->dados, qtd * sizeof(Cliente));
        if (temp == NULL) {
            printf("\nErro: Falha ao alocar memoria!\n");
            return 0;
        }

        lista->dados = temp;
        lista->total = qtd;

        // Lê os clientes e carrega do arquivo
        if (fread(lista->dados, sizeof(Cliente), qtd, arq) != (size_t)qtd) {
            printf("\nErro: Falha na leitura do registro de clientes!\n");
            return 0;
        }
    }

    fclose(arq);
    return 1;
}
