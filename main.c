#include <stdio.h>
#include <stdlib.h>

typedef struct IgnorePath {
  int len;
  int *vals;
} IgnorePath;

typedef struct IgnoreSet {
  int len;
  IgnorePath **paths;
} IgnoreSet;

typedef struct Datatype {
  int nkids;
  char *desc;
  IgnoreSet *set;
  struct Datatype **kids;
} Datatype;

typedef struct Stack {
  struct Stack *previous;
  int number;
} Stack;

int
repeated(Stack *lead, IgnoreSet *set) {
  if (!set) {
    return 0;
  }
  for(int i = 0; i < set->len; i++) {
    Stack *curr = lead;
    for(int p = 0; p < set->paths[i]->len; p++) {
      if (curr == NULL)
	goto cont;
      if (set->paths[i]->vals[p] != curr->number) {
	goto cont;
      }
      curr = curr->previous;
    }
    return 1;
  cont:
    ;
  }
  for(; lead != NULL; lead = lead->previous) printf("%d; ", lead->number);
  printf("\n");
  return 0;
}

Datatype *
kcopy(Datatype *src, int depth, Stack *lag) {
  Datatype *dst = malloc(sizeof(Datatype));
  if (!src) {
    dst->nkids = 0; dst->desc = NULL; dst->kids = NULL;
  } else {
    dst->nkids = src->nkids;
    dst->desc = src->desc;
    dst->kids = calloc(dst->nkids, sizeof(Datatype));
    Stack *lead = malloc(sizeof(Stack));
    lead->previous = lag;
    if (depth > 0) {
      for (int i = 0; i < dst->nkids; i++) {
	lead->number = i;
	if (!repeated(lead, src->set)) {
	  dst->kids[i] = kcopy(src->kids[i], depth-1, lead);
	}
      }
    }
  }
  return dst;
}

void
fshow(int fd, int tabs, Datatype *node) {
  for (int i = 0; i < tabs; i++) dprintf(fd, "\t");
  dprintf(fd, "%s\n", node->desc);
  for (int i = 0; i < node->nkids; i++)
    if (node->kids[i])
      fshow(fd, tabs+1, node->kids[i]);
}

void
tree(Datatype **node) {
  Datatype *tree = malloc(sizeof(Datatype));
  tree->nkids = 3; tree->desc = "Node"; tree->kids = NULL;
  int *zo = malloc(sizeof(int)*2);
  zo[0] = 0; zo[1] = 1;
  IgnorePath *a = malloc(sizeof(IgnorePath));
  a->len = 2; a->vals = zo;
  int *oz = malloc(sizeof(int)*2);
  oz[0] = 1; oz[0] = 0;
  IgnorePath *b = malloc(sizeof(IgnorePath));
  b->len = 2; b->vals = oz;
  IgnoreSet *set = malloc(sizeof(IgnoreSet));
  IgnorePath **paths = malloc(sizeof(IgnorePath*)*2);
  paths[0] = a; paths[1] = b;
  set->len = 2; set->paths = paths;
  tree->set = set;
  Datatype *val = malloc(sizeof(Datatype));
  val->nkids = 0; val->desc = "Val"; val->kids = NULL; val->set = NULL;
  tree->kids = malloc(sizeof(Datatype*)*tree->nkids);
  tree->kids[0] = tree; tree->kids[1] = tree; tree->kids[2] = val;
  *node = tree;
}

int
main(int argc, char *argv[]) {
  Datatype *ex;
  tree(&ex);
  fshow(0, 0, kcopy(ex, 3, NULL));
}
