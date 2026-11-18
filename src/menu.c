#include <stdio.h>
#include "menu.h"
#include "auxiliar.h"

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
        
        case 2:
            //
            break;

        case 3:
            //
            break;

        case 4:
            //
            break;

        case 5:
            //
            break;
        
        case 0: printf("\nSaindo...\n"); break;

        default: printf("\nErro: Opcao Invalida!\n"); break;
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

        default: printf("\nErro: Opcao Invalida!\n"); break;
        }
    } while (opcao != 0);
}