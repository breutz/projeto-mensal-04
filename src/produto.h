#ifndef PRODUTO_H
#define PRODUTO_H

typedef struct {
    int codigo;
    char nome[30];
} Categoria;

typedef struct {
    Categoria *dados;
    int total;
} ListaCategorias;

typedef struct {
    int codigo;
    char descricao[50];
    int codigoCategoria;
    char nomeCategoria[30];
    float precoCompra;
    float margemLucro;
    float precoVenda;
    int estoque;
    int estoqueMinimo;
} Produto;

typedef struct {
    Produto *dados;
    int total;
} ListaProdutos;

/* --------------     Protótipos de cadastro     -------------- */
void cadastrarCategoria(ListaCategorias *lista);
int adicionarCategoria(ListaCategorias *lista, Categoria novo);
void cadastrarProduto(ListaProdutos *lista, const ListaCategorias *listaCategorias);
void adicionarProduto(ListaProdutos *lista, Produto novo);

/* --------------     Protótipos de ordenação     -------------- */
void listarCategorias(const ListaCategorias *lista);
void listarProdutos(const ListaProdutos *lista);

/* --------------     Protótipos de persitência de dados     -------------- */
int salvarCategorias(const ListaCategorias *lista);
int carregarCategorias(ListaCategorias *lista);
int salvarProdutos(const ListaProdutos *lista);
int carregarProdutos(ListaProdutos *lista);

#endif