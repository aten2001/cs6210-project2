#include <stdlib.h>
#include <stdio.h>
#include "omp.h"

#define true 1
#define false 0

typedef struct omp_tree_barrier_node {
  int32_t k;
  int32_t count;
  int32_t sense;
  struct omp_tree_barrier_node *parent;
#ifdef PADDING
  int32_t padding[PADDING];
#endif
} omp_tree_barrier_node_t;

typedef struct omp_tree_barrier {
  int N;
  omp_tree_barrier_node_t *nodes;
  int *local_senses;
} omp_tree_barrier_t;

void omp_tree_barrier_init(omp_tree_barrier_t *barrier, int num_threads) {
  barrier->N = num_threads;
  barrier->nodes = (omp_tree_barrier_node_t *) malloc(sizeof(omp_tree_barrier_node_t) * barrier->N);
  if (barrier->nodes == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }
  barrier->local_senses = (int *) malloc(sizeof(int) * barrier->N);
  if (barrier->local_senses == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < barrier->N; i++) {
    barrier->local_senses[i] = true;
  }

  for (int i = 0; i < barrier->N; i++) {
    omp_tree_barrier_node_t *node = &barrier->nodes[i];
    node->k = 2;
    node->sense = false;
    if (i == 0) {
      node->parent = NULL;
    } else {
      node->parent = &barrier->nodes[i / 2];
    }
  }
}

void omp_tree_barrier_destroy(omp_tree_barrier_t *barrier) {
  free(barrier->local_senses);
  free(barrier->nodes);
}

void omp_tree_barrier_aux(omp_tree_barrier_node_t *node, int *local_sense) {
#pragma omp atomic
  node->count--;
  if (node->count == 0) {
    if (node->parent) {
      omp_tree_barrier_aux(node->parent, local_sense);
    }
    node->count = node->k;
    node->sense = !node->sense;
  }
  while (node->sense != *local_sense);
}

void omp_tree_barrier(omp_tree_barrier_t *barrier) {
  omp_tree_barrier_node_t *my_node = &barrier->nodes[omp_get_thread_num() / 2];
  omp_tree_barrier_aux(my_node, &barrier->local_senses[omp_get_thread_num()]);
}

