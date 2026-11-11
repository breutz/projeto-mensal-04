#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct {
    int codigo;
    char nome[60];
    char nomeSocial[60];
    char cpf[15];
    char endereco[80]; // Rua e Número
    char bairro [40];
    char telefone[20];
} Cliente;

typedef struct {
    Cliente *dados;
    int total;
} ListaClientes;

/* --------------     Protótipos de cadastro     -------------- */
void cadastrarCliente(ListaClientes *lista);
int adicionarCliente(ListaClientes *lista, Cliente novo);

/* --------------     Protótipos de ordenação     -------------- */
void listarClientes(const ListaClientes *lista);

/* --------------     Protótipos de persitência de dados     -------------- */
int salvarClientes(const ListaClientes *lista);
int carregarClientes(ListaClientes *lista);

#endif