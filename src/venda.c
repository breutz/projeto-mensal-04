#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"
#include "venda.h"
#include "auxiliar.h"

void novaVenda(ListaVendas *vendas, ListaPagamentos *pagamentos, const ListaClientes *clientes, ListaProdutos *produtos, Caixa *caixa, const ListaUsuarios *usuarios) {
    if (caixa->caixaAberto == 0) {
        printf("\nERRO: Nao e possivel vender com o caixa fechado!\nAbra o caixa primeiro.\n");
        return;
    }

    if (clientes->total == 0) {
        printf("\nERRO: Nenhum cliente cadastrado no sistema!\n");
        return;
    }

    if (produtos->total == 0) {
        printf("\nERRO: Nenhum produto cadastrado no sistema!\n");
        return;
    }

    Venda v = {0};
    v.numeroVenda = vendas->total + 1;
    v.situacao = 'a';

    // Exibe apenas catálogo compacto (Código + Nome) para escolha rápida
    exibirCatalogoClientes(clientes);

    int codCliente = 0;
    int posCliente = -1;
    do {
        codCliente = lerInteiroIntervalo("Digite o codigo do cliente (0 para cancelar): ", 0, 99999);
        if (codCliente == 0) {
            printf("\nVenda cancelada.\n");
            return;
        }
        posCliente = buscarClientePorCodigo(clientes, codCliente);
        if (posCliente == -1) {
            printf("\nERRO: Cliente nao encontrado!\n");
        }
    } while (posCliente == -1);

    v.codigoCliente = codCliente;
    strcpy(v.nomeCliente, clientes->dados[posCliente].nome);

    // Sugere a data atual do sistema, mas permite informar outra (útil para testes de período)
    char dataHoje[11];
    obterDataAtual(dataHoje);
    printf("\nData atual do sistema: %s\n", dataHoje);
    int usarHoje = lerInteiroIntervalo("Usar data de hoje? (1 - Sim | 0 - Outra data): ", 0, 1);
    if (usarHoje == 1) {
        strcpy(v.data, dataHoje);
    } else {
        lerData("Data da venda (DD/MM/AAAA): ", v.data);
    }

    // Cabeçalho simétrico da venda (50 colunas)
    printf("\n==================================================\n");
    printf("Venda #%-5d | Data: %-10s | Status: %8s\n", v.numeroVenda, v.data, "ABERTO");
    printf("Cliente: %-41.41s\n", v.nomeCliente);
    printf("==================================================\n");

    char continuar = 's';
    do {
        exibirCatalogoProdutos(produtos);

        int codProduto = lerInteiroIntervalo("Digite o codigo do produto (0 para encerrar): ", 0, 99999);
        if (codProduto == 0) {
            break;
        }

        int posProduto = buscarProdutoPorCodigo(produtos, codProduto);
        if (posProduto == -1) {
            printf("\nERRO: Produto nao encontrado!\n");
            continue;
        }

        Produto *p = &produtos->dados[posProduto];

        if (p->estoque <= 0) {
            printf("\nERRO: '%s' com estoque ZERADO! Venda bloqueada.\n", p->descricao);
            continue;
        }

        int qtd = lerInteiroIntervalo("Quantidade desejada: ", 1, 99999);

        if (qtd > p->estoque) {
            printf("\nAVISO: Quantidade indisponivel! Estoque atual: %d\n", p->estoque);
            int aceita = lerInteiroIntervalo("Levar as unidades disponiveis? (1 - Sim | 0 - Nao): ", 0, 1);
            if (aceita == 1) {
                qtd = p->estoque;
            } else {
                printf("\nItem cancelado.\n");
                continue;
            }
        }

        int novoTotal = v.totalItens + 1;
        ItemVenda *temp = (ItemVenda *) realloc(v.itens, novoTotal * sizeof(ItemVenda));
        if (temp == NULL) {
            printf("\nERRO: Falha ao alocar memoria para o item!\n");
            break;
        }

        // RF08: calcula preço de venda no ato da venda
        float precoUnitario = p->precoCompra * (1.0f + (p->margemLucro / 100.0f));
        p->precoVenda = precoUnitario;

        v.itens = temp;
        v.itens[v.totalItens].codigoProduto = p->codigo;
        strcpy(v.itens[v.totalItens].descricao, p->descricao);
        v.itens[v.totalItens].precoVenda = p->precoVenda;
        v.itens[v.totalItens].quantidade = qtd;
        v.itens[v.totalItens].subtotal = p->precoVenda * qtd;

        v.valorTotal += v.itens[v.totalItens].subtotal;
        v.totalItens = novoTotal;

        p->estoque -= qtd;

        if (p->estoque <= p->estoqueMinimo) {
            printf("\nAVISO: '%s' atingiu o estoque minimo! (Restam: %d)\n",
                   p->descricao, p->estoque);
        }

        printf("\n--------------------------------------------------\n");
        printf("%-35s R$ %11.2f\n", "Item adicionado! Subtotal atual:", v.valorTotal);
        printf("--------------------------------------------------\n");

        int opcContinuar = lerInteiroIntervalo("Adicionar outro produto? (1 - Sim | 0 - Nao): ", 0, 1);
        continuar = (opcContinuar == 1) ? 's' : 'n';

    } while (continuar == 's');

    if (v.totalItens == 0) {
        printf("\nAVISO: Nenhum item adicionado. Venda cancelada.\n");
        return;
    }

    while ((v.desconto = lerFloatPositivo("\nPercentual de desconto (0 para nenhum): ")) > 100.0f) {
        printf("\nERRO: Desconto nao pode ser superior a 100%%!\n");
    }

    v.valorFinal = v.valorTotal * (1.0f - (v.desconto / 100.0f));
    printf("\n--------------------------------------------------\n");
    printf("%-35s R$ %11.2f\n", "Subtotal da venda:", v.valorTotal);
    printf("%-35s %13.2f%%\n", "Desconto aplicado:", v.desconto);
    printf("%-35s R$ %11.2f\n", "TOTAL A PAGAR:", v.valorFinal);
    printf("--------------------------------------------------\n");

    processarPagamento(&v, caixa, usuarios, pagamentos);
    adicionarVenda(vendas, v);
}

void processarPagamento(Venda *v, Caixa *caixa, const ListaUsuarios *usuarios, ListaPagamentos *pagamentos) {
    // Será implementado na Etapa 2
    (void)v;
    (void)caixa;
    (void)usuarios;
    (void)pagamentos;
}

int adicionarVenda(ListaVendas *lista, Venda nova) {
    int novoTotal = lista->total + 1;
    Venda *temp = realloc(lista->dados, novoTotal * sizeof(Venda));

    if (temp == NULL) {
        printf("\nERRO: Falha ao alocar memoria para venda!\n");
        return 0;
    }

    lista->dados = temp;
    lista->dados[lista->total] = nova;
    lista->total = novoTotal;

    return 1;
}

int adicionarPagamento(ListaPagamentos *lista, Pagamento novo) {
    int novoTotal = lista->total + 1;
    Pagamento *temp = realloc(lista->dados, novoTotal * sizeof(Pagamento));

    if (temp == NULL) {
        printf("\nERRO: Falha ao alocar memoria para pagamento!\n");
        return 0;
    }

    lista->dados = temp;
    lista->dados[lista->total] = novo;
    lista->total = novoTotal;

    return 1;
}