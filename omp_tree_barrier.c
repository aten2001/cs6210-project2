#include <stdio.h>
#include "omp_tree_barrier.h"


void omp_tree_barrier_init(omp_tree_barrier_t *barrier, int num_threads) {
  barrier->N = num_threads;
  barrier->nodes = (omp_tree_barrier_node_t *) malloc(sizeof(omp_tree_barrier_node_t) * barrier->N);
  if (barrier->nodes == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }
  barrier->threads = (tree_barrier_thread_data_t*) malloc(sizeof(tree_barrier_thread_data_t) * barrier->N);
  if (barrier->threads == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < barrier->N; i++) {
    barrier->threads[i].sense = true;
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
  free(barrier->threads);
  free(barrier->nodes);
}

void omp_tree_barrier(omp_tree_barrier_t *barrier) {
  omp_tree_barrier_node_t *my_node = &barrier->nodes[omp_get_thread_num() / 2];
  omp_tree_barrier_aux(my_node, &(barrier->threads[omp_get_thread_num()].sense));
  barrier->threads[omp_get_thread_num()].sense = !barrier->threads[omp_get_thread_num()].sense;
}

void omp_tree_barrier_aux(omp_tree_barrier_node_t* node, int32_t* local_sense) {
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

