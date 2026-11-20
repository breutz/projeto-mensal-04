#ifndef VENDA_H
#define VENDA_H

#include "cliente.h"
#include "produto.h"
#include "usuario.h"
#include "caixa.h"

typedef struct {
    int codigoProduto;
    char descricao[50];
    float precoVenda;
    int quantidade;
    float subtotal;
} ItemVenda;

typedef struct {
    int numeroVenda;
    int codigoCliente;
    char nomeCliente[60];
    char data[11]; // "DD/MM/AAAA"
    ItemVenda *itens; // Vetor dinâmico de itens da venda!
    int totalItens;
    float valorTotal;
    float desconto;
    float valorFinal;
    char situacao; // 'a' = Aberta, 'p' = Paga
} Venda;


typedef struct {
    int numeroVenda;
    float valorPago;
    char tipo[3]; // "d" = Dinheiro, "c" = Cartão, "m" = Misto
    float parcelaDinheiro;
    float parcelaCartao;
} Pagamento;


typedef struct {
    Venda *dados;
    int total;
} ListaVendas;

typedef struct {
    Pagamento *dados;
    int total;
} ListaPagamentos;

/* --------------     Protótipos de Vendas e Pagamentos     -------------- */
void novaVenda(ListaVendas *vendas, ListaPagamentos *pagamentos, const ListaClientes *clientes, ListaProdutos *produtos, Caixa *caixa, const ListaUsuarios *usuarios);
void processarPagamento(Venda *v, Caixa *caixa, const ListaUsuarios *usuarios, ListaPagamentos *pagamentos);
void recuperarVendaAberta(ListaVendas *vendas, Caixa *caixa, const ListaUsuarios *usuarios, ListaPagamentos *pagamentos);
int adicionarVenda(ListaVendas *lista, Venda nova);
int adicionarPagamento(ListaPagamentos *lista, Pagamento novo);

#endif