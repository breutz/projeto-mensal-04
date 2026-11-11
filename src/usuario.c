#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usuario.h"
#include "auxiliar.h"


/* --------------     Funções de cadastro     -------------- */
void cadastrarUsuario(ListaUsuarios *lista) {
    Usuario novo;

    do {
        lerTexto("\nNome de usuario (8 a 12 caracteres): ", novo.login, sizeof(novo.login));
        lerTexto("\nSenha (6 a 8 caracteres): ", novo.senha, sizeof(novo.senha));
    } while (validarCredenciais(novo.login, novo.senha) != 1);

    novo.tipo = lerInteiroIntervalo("\nHierarquia (1 = Admin, 2 = Comum): ", 1, 2);

    adicionarUsuario(lista, novo);
    printf("\nUsuario '%s' cadastrado com sucesso!\n", novo.login);
}

void adicionarUsuario(ListaUsuarios *lista, Usuario novo) {
    int novoTotal = lista->total + 1;

    Usuario *temp = realloc(lista->dados, novoTotal * sizeof(Usuario));
    
    if (temp == NULL) {
        printf("\nErro: Falha ao alocar memoria para Usuario!\n");
        return;
    }

    lista->dados = temp;
    lista->dados[lista->total] = novo;
    lista->total = novoTotal;
}

/* --------------     Funções de autenticação     -------------- */
int validarCredenciais(const char login[], const char senha[]) {
    int tamanhoLogin = strlen(login);
    int tamanhoSenha = strlen(senha);

    if (tamanhoLogin < 8 || tamanhoLogin > 12) {
        printf("\nErro: Login precisa conter entre 8 e 12 caracteres!\n");
        return 0;
    } else if (tamanhoSenha < 6 || tamanhoSenha > 8) {
        printf("\nErro: Senha precisa conter entre 6 e 8 caracteres!\n");
        return 0;
    } else {
        printf("\nUsuario cadastrado com sucesso!\n");
        return 1;
    }
}

int autenticarUsuario(const ListaUsuarios *lista, const char login[], const char senha[]) {
    for(int i = 0; i < lista->total; i++) {
        if (strcmp(login, lista->dados[i].login) == 0 && strcmp(senha, lista->dados[i].senha) == 0) {
            return i; // Retorna o indice do usuario logado
        }
    }
    return -1;
}

int autorizarAdmin(const ListaUsuarios *lista) {
    char login[13], senha[9];
    printf("\nAutorizacao de administrador necessaria\n");
    printf("\nLogin Admin: ");
    scanf(" %12s", login);
    printf("\nSenha Admin: ");
    scanf(" %8s", senha);

    int indice = autenticarUsuario(lista, login, senha);
    if (indice != -1 && lista->dados[indice].tipo == 1) {
        printf("\nOperacao autorizada pelo admin: %s.\n", lista->dados[indice].login);
        return 1;
    }

    printf("\nErro: Autorizacao recusada! Credenciais invalidas ou usuario sem permissoes administrativas!\n");
    return 0;
}

/* --------------     Funções de persistência de dados     -------------- */
int salvarUsuarios(const ListaUsuarios *lista) {
    // Grava primeiro um arquivo temporário (conceitos de Gravação Atômica)
    FILE *arq = fopen("usuarios.tmp", "wb");
    if (arq == NULL) {
        printf("\nErro: Falha ao criar o aquivo de usuarios temporario!\n");
        return 0;
    }

    // Grava a quantidade de usuarios e verifica se deu certo, se não, remove o arquivo temporário
    if (fwrite(&lista->total, sizeof(int), 1, arq) != 1) {
        printf("\nErro: Falha ao gravar a quantidade de usuarios!\n");
        fclose(arq);
        remove("usuarios.tmp");
        return 0;
    }

    // Se tiver usuarios cadastrados, grava o bloco no arquivo e verifica se a gravação foi bem sucedida
    if (lista->total > 0 && lista->dados != NULL) {
        if (fwrite(lista->dados, sizeof(Usuario), lista->total, arq) != (size_t)lista->total) {
            printf("\nErro: Falha ao gravar os dados dos usuarios!\n");
            fclose(arq);
            remove("usuarios.tmp");
            return 0;
        }
    }

    // Se tudo deu certo, fecha o arquivo temporário com os dados salvos com sucesso
    fclose(arq);

    // Faz a troca atômica garantindo 100% de sucesso na gravação do arquivo e verifica
    remove("usuarios.dat"); // se o arquivo não existir na primeira vez, a função retona um erro que é ignorado
    if (rename("usuarios.tmp", "usuarios.dat") != 0) {
        printf("\nErro: Falha ao realizar a troca dos arquivos!\n");
        return 0;
    }

    return 1; // Sucesso
}

int carregarUsuarios(ListaUsuarios *lista) {
    FILE *arq = fopen("usuarios.dat", "rb");

    // Caso o arquivo não exista (primeira vez que o sistema roda), define um admin para cadastro de novos usuários
    if (arq == NULL) {
        Usuario adminPadrao = {"admin123", "123456", 1};
        adicionarUsuario(lista, adminPadrao);
        return salvarUsuarios(lista); // Ja gera o arquivo "usuarios.dat" com o admin inicial
    }

    // Se o arquivo existe, lê a quantidade e verifica se deu certo, caso não, fecha o arquivo
    int qtd = 0;
    if (fread(&qtd, sizeof(int), 1, arq) != 1) {
        fclose(arq);
        return 0;
    }

    if (qtd > 0) {
        Usuario *temp = (Usuario *) realloc(lista->dados, qtd * sizeof(Usuario));
        if (temp == NULL) {
            printf("\nErro: Falha ao alocar memoria!\n");
            return 0;
        }

        lista->dados = temp;
        lista->total = qtd;

        // Lê os usuarios e carrega do arquivo
        if (fread(lista->dados, sizeof(Usuario), qtd, arq) != (size_t)qtd) {
            printf("\nErro: Falha na leitura do registro de usuarios!\n");
            return 0;
        }
    }

    fclose(arq);
    return 1;
}
