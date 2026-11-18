#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produto.h"
#include "auxiliar.h"

/* --------------     Funções de cadastro     -------------- */
void cadastrarCategoria(ListaCategorias *lista) {
    Categoria nova;

    nova.codigo = lista->total + 1;

    lerTexto("Nome da Categoria: ", nova.nome, sizeof(nova.nome));
    
    adicionarCategoria(lista, nova);
    printf("\nCategoria '%s' cadastrada com sucesso!\n", nova.nome);
}

int adicionarCategoria(ListaCategorias *lista, Categoria novo) {
    int novoTotal = lista->total + 1;

    Categoria *temp = realloc(lista->dados, novoTotal * sizeof(Categoria));

    if (temp == NULL) {
        printf("\nErro: Falha ao alocar memoria para Categoria!\n");
        return 0;
    }

    lista->dados = temp;
    lista->dados[lista->total] = novo;
    lista->total = novoTotal;

    return 1;
}

void cadastrarProduto(ListaProdutos *lista, const ListaCategorias *listaCategorias) {
    if (listaCategorias->total == 0) {
        printf("\nErro: Nenhuma categoria cadastrada!\n");
        return;
    }

    Produto novo;

    novo.codigo = lista->total + 1000;

    lerTexto("Descricao: ", novo.descricao, sizeof(novo.descricao));

    listarCategorias(listaCategorias);

    int codigoCategoria = lerInteiroIntervalo("Informe o codigo da categoria: ", 1, listaCategorias->total);

    strcpy(novo.nomeCategoria, listaCategorias->dados[codigoCategoria - 1].nome);
    novo.codigoCategoria = codigoCategoria;

    novo.precoCompra = lerFloatPositivo("Preco de compra: R$ ");
    novo.margemLucro = lerFloatPositivo("Margem de lucro (%): ");

    novo.precoVenda = novo.precoCompra * (1.0f + (novo.margemLucro / 100.0f));
    printf("Preco de venda calculado: R$ %.2f\n", novo.precoVenda);

    novo.estoque = lerInteiroIntervalo("Estoque: ", 0, 99999);
    novo.estoqueMinimo = lerInteiroIntervalo("Estoque minimo: ", 0, 99999);

    adicionarProduto(lista, novo);
}

int adicionarProduto(ListaProdutos *lista, Produto novo) {
    int novoTotal = lista->total + 1;

    Produto *temp = realloc(lista->dados, novoTotal * sizeof(Produto));

    if (temp == NULL) {
        printf("\nErro: Falha ao alocar memoria para Produto!\n");
        return 0;
    }

    lista->dados = temp;
    lista->dados[lista->total] = novo;
    lista->total = novoTotal;

    return 1;
}

/* --------------     Funções de ordenação     -------------- */
void listarCategorias(const ListaCategorias *lista) {
    if (lista->total == 0) {
        printf("\nErro: Nao existem categorias cadastradas!\n");
        return;
    }

    printf("\n============ RELATORIO DE CATEGORIAS =============\n");

    for (int i = 0; i < lista->total; i++) {
        printf("%-46s#%3d\n", "Categoria:", i + 1);
        printf("%-15s %34d\n", "Codigo:", lista->dados[i].codigo);
        printf("%-15s %34.34s\n", "Nome:", lista->dados[i].nome);
        printf("--------------------------------------------------\n");
    }
}

void listarProdutos(const ListaProdutos *lista) {
    if (lista->total == 0) {
        printf("\nErro: Nao existem produtos cadastrados!\n");
        return;
    }

    printf("\n============= RELATORIO DE PRODUTOS ==============\n");

    for (int i = 0; i < lista->total; i++) {
        printf("%-46s#%3d\n", "Produto:", i + 1);
        printf("%-35s%15d\n", "Codigo:", lista->dados[i].codigo);
        printf("%-15s %34.34s\n", "Descricao:", lista->dados[i].descricao);
        printf("%-15s %34.34s\n", "Categoria:", lista->dados[i].nomeCategoria);
        printf("%-41sR$ %6.2f\n", "Preco de compra:", lista->dados[i].precoCompra);
        printf("%-35s%14.2f%%\n", "Margem de lucro:", lista->dados[i].margemLucro);
        printf("%-41sR$ %6.2f\n", "Preco de venda:", lista->dados[i].precoVenda);
        printf("%-35s%15d\n", "Estoque atual:", lista->dados[i].estoque);
        printf("%-35s%15d\n", "Estoque minimo:", lista->dados[i].estoqueMinimo);
        printf("--------------------------------------------------\n");
    }
}

/* --------------     Funções de persistência de dados     -------------- */
int salvarProdutos(const ListaProdutos *lista) {
    // Grava primeiro um arquivo temporário (conceitos de Gravação Atômica)
    FILE *arq = fopen("produtos.tmp", "wb");
    if (arq == NULL) {
        printf("\nErro: Falha ao criar o aquivo de produtos temporario!\n");
        return 0;
    }

    // Grava a quantidade de produtos e verifica se deu certo, se não, remove o arquivo temporário
    if (fwrite(&lista->total, sizeof(int), 1, arq) != 1) {
        printf("\nErro: Falha ao gravar a quantidade de produtos!\n");
        fclose(arq);
        remove("produtos.tmp");
        return 0;
    }

    // Se tiver produtos cadastrados, grava o bloco no arquivo e verifica se a gravação foi bem sucedida
    if (lista->total > 0 && lista->dados != NULL) {
        if (fwrite(lista->dados, sizeof(Produto), lista->total, arq) != (size_t)lista->total) {
            printf("\nErro: Falha ao gravar os dados dos produtos!\n");
            fclose(arq);
            remove("produtos.tmp");
            return 0;
        }
    }

    // Se tudo deu certo, fecha o arquivo temporário com os dados salvos com sucesso
    fclose(arq);

    // Faz a troca atômica garantindo 100% de sucesso na gravação do arquivo e verifica
    remove("produtos.dat"); // se o arquivo não existir na primeira vez, a função retona um erro que é ignorado
    if (rename("produtos.tmp", "produtos.dat") != 0) {
        printf("\nErro: Falha ao realizar a troca dos arquivos!\n");
        return 0;
    }

    return 1; // Sucesso
}

int carregarProdutos(ListaProdutos *lista) {
    FILE *arq = fopen("produtos.dat", "rb");

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
        Produto *temp = (Produto *) realloc(lista->dados, qtd * sizeof(Produto));
        if (temp == NULL) {
            printf("\nErro: Falha ao alocar memoria!\n");
            return 0;
        }

        lista->dados = temp;
        lista->total = qtd;

        // Lê os produtos e carrega do arquivo
        if (fread(lista->dados, sizeof(Produto), qtd, arq) != (size_t)qtd) {
            printf("\nErro: Falha na leitura do registro de produtos!\n");
            return 0;
        }
    }

    fclose(arq);
    return 1;
}
