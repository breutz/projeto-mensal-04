#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usuario.h"
#include "sistema.h"

int main() {
    Sistema s = {0};

    // Inicializa o sistema carregando o arquivo de usuarios (cria o admin se for a primeira vez)
    if (!carregarUsuarios(&s.usuarios)) {
        printf("\nErro: Falha ao carregar o registro de usuarios!\n");
        return 1;
    }

    char login[13];
    char senha[9];

    printf("\nLogin: ");
    scanf(" %12s", login);
    printf("\nSenha: ");
    scanf(" %8s", senha);

    int indice = autenticarUsuario(&s.usuarios, login, senha);

    if (indice != -1) {
        s.usuarioLogado = s.usuarios.dados[indice];
        s.logado = 1;
        printf("\nLogin bem sucedido! Usuario: %s | Hierarquia: %s\n", s.usuarioLogado.login, (s.usuarioLogado.tipo == 1) ? "Administrador" : "Usuario Comum");
    } else {
        printf("\nErro: Login ou Senha invalidos!\n");
        s.logado = 0;
    }

    // Ao sair, garante que os arquivos sejam salvos
    salvarUsuarios(&s.usuarios);

    return 0;
}