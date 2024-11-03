#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
} Fruta;


typedef struct Nodo {
    Fruta *fruta;
    struct Nodo *prox;
} Nodo;

Nodo *inicio = NULL;


int codigo(int codigo);
void cadastrar();
void listar();
void buscar();
void alterar();
void excluir();
void vender();
void registrar(int codigo, int quantidade, float preco_total);
void liberar();


int main() {
    int opcao;
    
    do {
        printf("\n Joel Frutas \n");
        printf("1 OP - Cadastrar uma fruta\n");
        printf("2 OP -  Listar frutas cadastradas\n");
        printf("3 OP - buscar uma fruta\n");
        printf("4 OP -alterar uma fruta\n");
        printf("5 OP - excluir uma fruta\n");
        printf("6 OP - vender uma fruta\n");
        printf("7 OP - sair\n");
        printf("opcao desejada: ");
        scanf("%d", &opcao);
        getchar(); 
        
        switch(opcao) {
            case 1: cadastrar(); break;
            case 2: listar(); break;
            case 3: buscar(); break;
            case 4: alterar(); break;
            case 5: excluir(); break;
            case 6: vender(); break;
            case 7: liberar(); printf("liberando\n"); break;
            default: printf("Opcao invalida\n");
        }
    } while(opcao != 7);
    
    return 0;
}

int codigo(int codigo) {
    Nodo *atual = inicio;
    while (atual != NULL) {
        if (atual->fruta->codigo == codigo) return 1;
        atual = atual->prox;
    }
    return 0;
}

void cadastrar() {
    Fruta *nova = (Fruta*)malloc(sizeof(Fruta));
    if (!nova) {
        printf("Erro de cadastro\n");
        return;
    }
    
    printf("Digite o codigo da fruta: ");
    scanf("%d", &nova->codigo);
    if (codigo(nova->codigo)) {
        printf("Codigo ja registrado\n");
        free(nova);
        return;
    }
    
    printf("Digite o nome de uma fruta");
    getchar();
    fgets(nova->nome, 50, stdin);
    nova->nome[strcspn(nova->nome, "\n")] = '\0'; 
    
    printf("Digite a quantidade");
    scanf("%d", &nova->quantidade);
    
    printf("Digite o preco ");
    scanf("%f", &nova->preco);
    
    Nodo *novoNodo = (Nodo*)malloc(sizeof(Nodo));
    novoNodo->fruta = nova;
    novoNodo->prox = inicio;
    inicio = novoNodo;
    
    printf("Fruta cadastrada\n");
}


void listar() {
    Nodo *atual = inicio;
    if (!atual) {
        printf("Nenhuma fruta cadastrada\n");
        return;
    }
    
    printf("\n Lista de Frutas cadastradas\n");
    while (atual != NULL) {
        printf("Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n",
               atual->fruta->codigo, atual->fruta->nome, atual->fruta->quantidade, atual->fruta->preco);
        atual = atual->prox;
    }
}


void buscar() {
    int codigo;
    printf("Digite o codigo da fruta: ");
    scanf("%d", &codigo);
    
    Nodo *atual = inicio;
    while (atual != NULL) {
        if (atual->fruta->codigo == codigo) {
            printf("Fruta encontrada: Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n",
                   atual->fruta->codigo, atual->fruta->nome, atual->fruta->quantidade, atual->fruta->preco);
            return;
        }
        atual = atual->prox;
    }
    printf("Fruta nao localizada\n");
}

void alterar() {
    int codigo;
    printf("Digite o codigo da fruta: ");
    scanf("%d", &codigo);
    
    Nodo *atual = inicio;
    while (atual != NULL) {
        if (atual->fruta->codigo == codigo) {
            printf("Digite o novo nome da fruta: ");
            getchar();
            fgets(atual->fruta->nome, 50, stdin);
            atual->fruta->nome[strcspn(atual->fruta->nome, "\n")] = '\0';
            
            printf("Digite a nova quantidade: ");
            scanf("%d", &atual->fruta->quantidade);
            
            printf("Digite o novo preco: ");
            scanf("%f", &atual->fruta->preco);
            
            printf("Fruta alterada\n");
            return;
        }
        atual = atual->prox;
    }
    printf("Fruta nao localizada\n");
}


void excluir() {
    int codigo;
    printf("Digite o codigo da fruta a ser excluida: ");
    scanf("%d", &codigo);
    
    Nodo *atual = inicio, *anterior = NULL;
    while (atual != NULL) {
        if (atual->fruta->codigo == codigo) {
            if (atual->fruta->quantidade > 0) {
                printf("Fruta possui estoque, nao pode ser excluida\n");
                return;
            }
            if (anterior) anterior->prox = atual->prox;
            else inicio = atual->prox;
            
            free(atual->fruta);
            free(atual);
            printf("Fruta excluida com sucesso\n");
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
    printf("Fruta nao localizada.\n");
}


void vender() {
    int codigo, quantidade;
    printf("Digite o codigo da fruta: ");
    scanf("%d", &codigo);
    printf("Digite a quantidade a ser vendida: ");
    scanf("%d", &quantidade);
    
    Nodo *atual = inicio;
    while (atual != NULL) {
        if (atual->fruta->codigo == codigo) {
            if (atual->fruta->quantidade < quantidade) {
                printf("Estoque insuficiente.\n");
                return;
            }
            atual->fruta->quantidade -= quantidade;
            registrar(codigo, quantidade, quantidade * atual->fruta->preco);
            printf("Venda realizada\n");
            return;
        }
        atual = atual->prox;
    }
    printf("Fruta nao localizada.\n");
}


void registrar(int codigo, int quantidade, float preco_total) {
    FILE *arquivo = fopen("vendas.txt", "a");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }
    fprintf(arquivo, "Codigo: %d, Quantidade: %d, Preco total: %.2f\n", codigo, quantidade, preco_total);
    fclose(arquivo);
}


void liberar() {
    Nodo *atual = inicio;
    while (atual != NULL) {
        Nodo *temp = atual;
        atual = atual->prox;
        free(temp->fruta);
        free(temp);
    }
}
