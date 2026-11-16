#ifndef SISTEMA_H
#define SISTEMA_H

#include "usuario.h"
#include "cliente.h"
#include "produto.h"

typedef struct {
    ListaUsuarios usuarios;
    ListaClientes clientes;
    ListaCategorias categorias;
    ListaProdutos produtos;
    Usuario usuarioLogado; // Guarda os dados do usuario logado no momento
    int logado; // 0 = Ninguem logado, 1 = Usuario autenticado com sucesso
} Sistema;

#endif