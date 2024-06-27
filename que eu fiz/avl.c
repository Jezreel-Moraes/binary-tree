#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct no {
  struct no* pai;
  struct no* esquerda;
  struct no* direita;
  int valor;
  int altura;
} No;

typedef struct arvore {
  struct no* raiz;
} Arvore;

void balanceamento(Arvore*, No*);
int altura(No*);
int fb(No*);
No* rsd(Arvore*, No*);
No* rse(Arvore*, No*);
No* rdd(Arvore*, No*);
No* rde(Arvore*, No*);

int max(int a, int b) { return a > b ? a : b; }

Arvore* criar() {
  Arvore* arvore = malloc(sizeof(Arvore));
  arvore->raiz = NULL;

  return arvore;
}

int vazia(Arvore* arvore) { return arvore->raiz == NULL; }

void adicionar(Arvore* arvore, int valor) {
  No* no = arvore->raiz;

  while (no != NULL) {
    if (valor > no->valor) {
      if (no->direita != NULL) {
        no = no->direita;
      } else {
        break;
      }
    } else {
      if (no->esquerda != NULL) {
        no = no->esquerda;
      } else {
        break;
      }
    }
  }

  No* novo = malloc(sizeof(No));
  novo->valor = valor;
  novo->pai = no;
  novo->esquerda = NULL;
  novo->direita = NULL;
  novo->altura = 1;

  if (no == NULL) {
    arvore->raiz = novo;
  } else {
    if (valor > no->valor) {
      no->direita = novo;
    } else {
      no->esquerda = novo;
    }

    balanceamento(arvore, no);
  }
}

No* localizar(No* no, int valor) {
  while (no != NULL) {
    if (no->valor == valor) {
      return no;
    }

    no = valor < no->valor ? no->esquerda : no->direita;
  }

  return NULL;
}

void percorrer(No* no, void (*callback)(int)) {
  if (no != NULL) {
    percorrer(no->esquerda, callback);
    callback(no->valor);
    percorrer(no->direita, callback);
  }
}

void visitar(int valor) { printf("%d ", valor); }

void balanceamento(Arvore* arvore, No* no) {
  while (no != NULL) {
    no->altura = max(altura(no->esquerda), altura(no->direita)) + 1;
    int fator = fb(no);

    if (fator > 1) {  // árvore mais pesada para esquerda
      // rotação para a direita
      if (fb(no->esquerda) > 0) {
        printf("RSD %d -> ", no->valor);
        rsd(arvore, no);  // rotação simples a direita, pois o FB do filho
                          // tem sinal igual
      } else {
        printf("RDD %d -> ", no->valor);
        rdd(arvore, no);  // rotação dupla a direita, pois o FB do filho tem
                          // sinal diferente
      }
    } else if (fator < -1) {  // árvore mais pesada para a direita
      // rotação para a esquerda
      if (fb(no->direita) < 0) {
        printf("RSE %d -> ", no->valor);
        rse(arvore, no);  // rotação simples a esquerda, pois o FB do
                          // filho tem sinal igual
      } else {
        printf("RDE %d -> ", no->valor);
        rde(arvore, no);  // rotação dupla a esquerda, pois o FB do filho tem
                          // sinal diferente
      }
    }

    no = no->pai;
  }
}

int altura(No* no) { return no != NULL ? no->altura : 0; }

int fb(No* no) {
  int esquerda = 0, direita = 0;

  if (no->esquerda != NULL) {
    esquerda = no->esquerda->altura;
  }

  if (no->direita != NULL) {
    direita = no->direita->altura;
  }

  return esquerda - direita;
}

No* rse(Arvore* arvore, No* no) {
  No* pai = no->pai;
  No* direita = no->direita;

  if (direita->esquerda != NULL) {
    direita->esquerda->pai = no;
  }

  no->direita = direita->esquerda;
  no->pai = direita;

  direita->esquerda = no;
  direita->pai = pai;

  if (pai == NULL) {
    arvore->raiz = direita;
  } else {
    if (pai->esquerda == no) {
      pai->esquerda = direita;
    } else {
      pai->direita = direita;
    }
  }

  no->altura = max(altura(no->esquerda), altura(no->direita)) + 1;
  direita->altura =
      max(altura(direita->esquerda), altura(direita->direita)) + 1;

  return direita;
}

No* rsd(Arvore* arvore, No* no) {
  No* pai = no->pai;
  No* esquerda = no->esquerda;

  if (esquerda->direita != NULL) {
    esquerda->direita->pai = no;
  }

  no->esquerda = esquerda->direita;
  no->pai = esquerda;

  esquerda->direita = no;
  esquerda->pai = pai;

  if (pai == NULL) {
    arvore->raiz = esquerda;
  } else {
    if (pai->esquerda == no) {
      pai->esquerda = esquerda;
    } else {
      pai->direita = esquerda;
    }
  }

  no->altura = max(altura(no->esquerda), altura(no->direita)) + 1;
  esquerda->altura =
      max(altura(esquerda->esquerda), altura(esquerda->direita)) + 1;

  return esquerda;
}

No* rde(Arvore* arvore, No* no) {
  no->direita = rsd(arvore, no->direita);
  return rse(arvore, no);
}

No* rdd(Arvore* arvore, No* no) {
  no->esquerda = rse(arvore, no->esquerda);
  return rsd(arvore, no);
}

No* minValorNo(No* no) {
  No* atual = no;
  while (atual->esquerda != NULL) atual = atual->esquerda;
  return atual;
}

void remover(Arvore* arvore, int valor) {
  No* no = localizar(arvore->raiz, valor);
  if (no == NULL) return;

  No* pai = no->pai;
  No* substituto;

  if (no->esquerda == NULL || no->direita == NULL) {
    substituto = no->esquerda ? no->esquerda : no->direita;

    if (substituto != NULL) substituto->pai = pai;

    if (pai == NULL) {
      arvore->raiz = substituto;
    } else {
      if (no == pai->esquerda) {
        pai->esquerda = substituto;
      } else {
        pai->direita = substituto;
      }
    }
  } else {
    No* sucessor = minValorNo(no->direita);
    no->valor = sucessor->valor;
    remover(arvore, sucessor->valor);
    return;
  }

  free(no);

  balanceamento(arvore, pai);
}

int main() {
  Arvore* a = criar();

  for (int i = 0; i < 10000; i++) {
    int operacao = 0;
    int valor = rand() % 100000;

    adicionar(a, valor);
  }

  printf("In-order: ");
  percorrer(a->raiz, visitar);
  return 0;
}
