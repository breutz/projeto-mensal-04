#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usuario.h"
#include "cliente.h"
#include "produto.h"
#include "sistema.h"
#include "auxiliar.h"
#include "menu.h"

int main() {
    Sistema s = {0};

    // Carrega todos os dados dos arquivos
    carregarUsuarios(&s.usuarios);
    carregarClientes(&s.clientes);
    carregarProdutos(&s.produtos);

    // Tela de Login e Autenticação
    char login[13], senha[9];

    printf("\n============== BERE IS NICE - LOGIN ==============\n");
    printf("\nLogin: ");
    scanf(" %12s", login);
    printf("\nSenha: ");
    scanf(" %8s", senha);

    int indice = autenticarUsuario(&s.usuarios, login, senha);

    if (indice != -1) {
        s.usuarioLogado = s.usuarios.dados[indice];
        s.logado = 1;
        printf("\nLogin bem sucedido! Usuario: %s (%s)\n",
            s.usuarioLogado.login,
            (s.usuarioLogado.tipo == 1) ? "Administrador" : "Usuario Comum");
        
            menuPrincipal(&s);

    } else {
        printf("\nErro: Login ou Senha invalidos!\n");
        s.logado = 0;
    }

    // Ao sair, garante que os arquivos sejam salvos
    salvarUsuarios(&s.usuarios);
    salvarClientes(&s.clientes);
    salvarProdutos(&s.produtos);

    printf("\nSistema encerrado com seguranca. Bere is Nice!\n");
    return 0;
}