#include <stdio.h>
#include "sistema.h"
#include "menu.h"
#include "auxiliar.h"
#include "caixa.h"
#include "venda.h"

void menuPrincipal(Sistema *s) {
    int opcao = 0;

    do {
        printf("\n================== BERE IS NICE ==================\n");   
        printf("1. %47s\n", "Cadastros");
        printf("2. %47s\n", "Vendas");
        printf("3. %47s\n", "Abertura de Caixa");
        printf("4. %47s\n", "Fechamento de Caixa");
        printf("5. %47s\n", "Relatorios");
        printf("0. %47s\n", "Sair");

        opcao = lerInteiroIntervalo("Digite o codigo do submenu em que deseja entrar: ", 0, 5);

        switch (opcao) {
        case 1: menuCadastros(s); break;
        
        case 2: menuVendas(s); break;

        case 3: aberturaCaixa(s); break;

        case 4: fechamentoCaixa(s); break;

        case 5:
            printf("\nAVISO: Modulo de Relatorios em construcao.\n");
            break;

        case 0:
            if (s->caixa.caixaAberto == 1) {
                printf("\n==================================================\n");
                printf("AVISO: O sistema esta fechando com caixa ABERTO!\n");
                printf("Os dados de vendas serao preservados nos arquivos.\n");
                printf("Sera necessario reabrir o caixa no proximo inicio.\n");
                printf("==================================================\n");
            } else {
                printf("\nEncerrando o sistema...\n");
            }
            break;

        default: printf("\nERRO: Opcao invalida!\n"); break;
        }
    } while (opcao != 0);
}

void menuCadastros(Sistema *s) {
    int opcao = 0;

    do {
        printf("\n================ MENU DE CADASTROS ===============\n");
        printf("1. %47s\n", "Cadastro de Usuarios");
        printf("2. %47s\n", "Cadastro de Clientes");
        printf("3. %47s\n", "Cadastro de Produtos");
        printf("4. %47s\n", "Cadastro de Categorias");
        printf("0. %47s\n", "Voltar ao Menu Principal");

        opcao = lerInteiroIntervalo("Digite o codigo do submenu em que deseja entrar: ", 0, 4);

        switch (opcao) {
            case 1: cadastrarUsuario(&s->usuarios); break;
            
            case 2: cadastrarCliente(&s->clientes); break;

            case 3: cadastrarProduto(&s->produtos, &s->categorias); break;

            case 4: cadastrarCategoria(&s->categorias); break;
            
            case 0: printf("\nVoltando...\n"); break;

            default: printf("\nERRO: Opcao invalida!\n");
        }
    } while (opcao != 0);
}

void menuVendas(Sistema *s) {
    int opcao = 0;
    do {
        printf("\n================== MENU DE VENDAS ================\n");
        printf("1. %47s\n", "Nova Venda");
        printf("2. %47s\n", "Sangria");
        printf("3. %47s\n", "Pagamento");
        printf("0. %47s\n", "Voltar ao Menu Principal");

        opcao = lerInteiroIntervalo("Digite o codigo do submenu em que deseja entrar: ", 0, 3);

        switch (opcao) {
            case 1: novaVenda(&s->vendas, &s->pagamentos, &s->clientes, &s->produtos, &s->caixa, &s->usuarios); break;

            case 2: realizarSangria(s); break;

            case 3: printf("\nAVISO: Recuperacao de vendas em construcao.\n"); break;

            case 0: printf("\nVoltando...\n"); break;

            default: printf("\nERRO: Opcao invalida!\n");
        }
    } while (opcao != 0);
}