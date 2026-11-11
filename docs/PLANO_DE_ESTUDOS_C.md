# Plano de Estudos Prático — C e Estruturas de Dados
**Foco:** Disciplina de Algoritmos e Estrutura de Dados & Projeto Mensal 4 (Sistema da Berê)  
**Fonte Principal:** Material oficial da disciplina (`Aula_C-Estruturas-de-Dados-Lineares.pdf`) e Requisitos do Berê 4.  
**Metodologia:** Direto ao ponto, código em C puro executável, análise de complexidade Big-O e memória, sem pseudocódigo.

---

## 🧭 O Pilar Central da Disciplina: Complexidade Big-O e Custo de Memória

A exigência central do professor é saber **comparar o custo de tempo e de memória** de cada estrutura para justificar escolhas técnicas:

| Estrutura | Acesso por Índice | Inserção | Remoção | Busca por Valor | Custo de Memória (Overhead) | Quando usar no Berê 4 |
| :--- | :---: | :---: | :---: | :---: | :--- | :--- |
| **Lista Sequencial** (Vetor / `realloc`) | **$O(1)$** | Fim: $O(1)$<br>Início/Meio: $O(n)$ | Fim: $O(1)$<br>Início/Meio: $O(n)$ | Linear: $O(n)$<br>Binária: $O(\log n)$ | **Mínimo:** apenas o espaço dos dados brutos. | Acesso direto por posição, relatórios e tabelas estáveis. |
| **Lista Encadeada Simples** | $O(n)$ | Início: **$O(1)$**<br>Fim: $O(n)$ | Início: **$O(1)$**<br>Meio/Fim: $O(n)$ | $O(n)$ | **Médio:** +1 ponteiro (`*proximo`) por nó alocado. | Cadastros flexíveis onde o tamanho muda muito e inserções no início são frequentes. |
| **Lista Duplamente Encadeada** | $O(n)$ | Início/Fim: **$O(1)$** (com `tail`) | Início/Fim: **$O(1)$** (com `tail`) | $O(n)$ | **Alto:** +2 ponteiros (`*ant`, `*prox`) por nó + `head/tail`. | Navegação bidirecional (ex: avançar/voltar produtos no terminal). |
| **Fila (FIFO)** | Só a frente: $O(1)$ | Enfileirar (fim): **$O(1)$** | Desenfileirar (início): **$O(1)$** | $O(n)$ | Vetor circular ou nós encadeados. | Fila de pedidos/vendas aguardando pagamento (**RF13**). |
| **Pilha (LIFO)** | Só o topo: $O(1)$ | Push (topo): **$O(1)$** | Pop (topo): **$O(1)$** | $O(n)$ | Vetor ou nós encadeados. | Desfazer última ação (Undo) no carrinho de compras. |

---

## 📅 Cronograma Atualizado de Estudos e Produção

### Semana 1 (Atual) — Listas Sequenciais vs. Listas Encadeadas & Arquivos
* **Teoria Oficial:**
  * Diferença física de armazenamento contíguo (vetor) vs. disperso (nós e ponteiros).
  * Por que vetor tem acesso direto $O(1)$ e lista encadeada exige percurso $O(n)$.
  * Inserção e remoção no início, meio e fim (custo de deslocamento no vetor vs. religamento de ponteiros).
  * Persistência dos 5 arquivos binários obrigatórios (**RF18**).
* **Aplicação no Berê 4:**
  * Inicializar a arquitetura modular em `src/` (`autenticacao`, `cliente`, `produto`, `venda`, `caixa`, `relatorios`).
  * Implementar módulo de persistência e validação de arquivos.

### Semana 2 — Listas Duplamente Encadeadas, Filas e Pilhas
* **Teoria Oficial:**
  * Lista dupla com `head` e `tail` (operações $O(1)$ nas duas pontas).
  * Fila circular (evitando desperdício de espaço no vetor linear) e Fila dinâmica.
  * Pilha dinâmica e balanceamento/histórico.
* **Aplicação no Berê 4:**
  * Gerenciamento de carrinho de compras e vendas abertas/recuperação (**RF13**).
  * Sistema de sangria e caixa administrativo (**RF03, RF14, RF16**).

### Semana 3 — Ordenação e Relatórios
* **Teoria Oficial:**
  * Métodos de ordenação (Bubble sort, Insertion sort, seleção).
  * Relatórios com filtros de data e ordenação alfabética (**REL01 a REL09**).
* **Aplicação no Berê 4:**
  * Implementação dos 9 relatórios obrigatórios com ordenação alfabética e rankings.

### Semana 4 — Árvores Binárias e Finalização
* **Teoria Oficial:**
  * Árvores Binárias de Busca (BST): busca em $O(\log n)$.
  * Percursos em-ordem, pré-ordem e pós-ordem.
* **Aplicação no Berê 4:**
  * Otimização de busca por código de produto e cliente.
  * Casos de teste (**CT01 a CT15**), documentação e manual do usuário.

---

## 🏗️ Plano de Produção Inicial do Berê 4 (Passo a Passo)

Para manter a promessa de você fazer o código sozinho e no seu ritmo, vamos avançar módulo por módulo:

1. **Passo 1 (Arquitetura e Contrato):**
   * Criar os arquivos de cabeçalho (`.h`) em `src/` com as structs exigidas no enunciado:
     * `usuario.h`: login (8-12 carac.), senha (6-8 carac.), tipo 1 (admin) e tipo 2 (comum).
     * `cliente.h`: código, nome, nome social, CPF, endereço, bairro, telefone.
     * `produto.h`: código, descrição, categoria, compra, margem, venda, estoque, mínimo.
2. **Passo 2 (Módulo de Autenticação - RF02, RF03, RF04):**
   * Implementar `usuario.c` com validação de tamanho de credenciais e permissão administrativa.
3. **Passo 3 (Persistência Inicial - RF18):**
   * Criar rotinas para salvar e carregar usuários e clientes em arquivos.
4. **Passo 4 (Cadastros Dinâmicos - RF05, RF07):**
   * Aplicar as estruturas estudadas (Lista Sequencial dinâmica ou Lista Encadeada) para gerenciar clientes e produtos sem limite fixo.
