#include <stdlib.h>
#include <stdio.h>
#include "omp.h"

#define true 1
#define false 0

typedef struct omp_mcs_barrier_node {
  volatile int32_t parentsense;
  int32_t num_children;
  volatile int8_t child_notready[4];
  volatile int8_t *parent_child_not_ready_ptr;
  volatile int32_t *child_sense_ptrs[2];
  int32_t dummy;
} omp_mcs_barrier_node_t;

typedef struct omp_mcs_barrier {
  int32_t N;
  omp_mcs_barrier_node_t *nodes;
  int32_t *local_senses;
} omp_mcs_barrier_t;

void omp_mcs_barrier_init(omp_mcs_barrier_t *barrier, int num_threads) {
  barrier->N = num_threads;
  barrier->nodes = (omp_mcs_barrier_node_t *) malloc(sizeof(omp_mcs_barrier_node_t) * num_threads);
  if (barrier->nodes == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  barrier->local_senses = (int32_t *) malloc(sizeof(int32_t) * num_threads);
  if (barrier->local_senses == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < num_threads; i++) {
    barrier->local_senses[i] = true;

    omp_mcs_barrier_node_t *node = &barrier->nodes[i];
    node->parentsense = false;

    int first_child = 4 * i + 1;
    node->num_children = 0;
    if (first_child >= num_threads) {
      node->num_children = num_threads - first_child;
      if (node->num_children > 4) {
        node->num_children = 4;
      }
    }

    for (int j = 0; i < 4; i++) {
      if (j < node->num_children) {
        node->child_notready[j] = true;
      } else {
        node->child_notready[j] = false;
      }
    }

    if (i != 0) {
      node->parent_child_not_ready_ptr = &(barrier->nodes[(i - 1) / 4].child_notready[(i - 1) % 4]);
    }


    if ((2 * i + 2) < num_threads) {
      node->child_sense_ptrs[0] = &(barrier->nodes[2 * i + 1].parentsense);
      node->child_sense_ptrs[1] = &(barrier->nodes[2 * i + 2].parentsense);
    } else if ((2 * i + 1) < num_threads) {
      node->child_sense_ptrs[0] = &(barrier->nodes[2 * i + 1].parentsense);
      node->child_sense_ptrs[1] = &node->dummy;
    } else {
      node->child_sense_ptrs[0] = &node->dummy;
      node->child_sense_ptrs[1] = &node->dummy;
    }

  }
}

void omp_mcs_barrier_destroy(omp_mcs_barrier_t *barrier) {
  free(barrier->nodes);
  free(barrier->local_senses);
}

void omp_mcs_barrier(omp_mcs_barrier_t *barrier) {
  int32_t id = omp_get_thread_num();
  omp_mcs_barrier_node_t *my_node = &barrier->nodes[id];
  int32_t *sense = &barrier->local_senses[id];
  if (my_node->num_children > 0) {
    volatile int32_t *children_not_ready = (volatile int32_t *) &my_node->child_notready;
    while (*children_not_ready != 0) { };
    for (int i = 0; i < my_node->num_children; i++) {
      my_node->child_notready[i] = true;
    }
  }

  if (id != 0) {
    *my_node->parent_child_not_ready_ptr = false;
    while (my_node->parentsense != *sense);
  }

  *my_node->child_sense_ptrs[0] = *sense;
  *my_node->child_sense_ptrs[1] = *sense;
  *sense = !*sense;
}
