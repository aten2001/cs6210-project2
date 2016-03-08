#ifndef PROJECT2_OMP_TREE_BARRIER_H
#define PROJECT2_OMP_TREE_BARRIER_H

#include <stdlib.h>
#include <stdint.h>
#include "omp.h"

#define true 1
#define false 0

#define TREE_BARRIER_NODE_PADDING 44
#define TREE_BARRIER_THREAD_DATA_PADDING 52

typedef struct omp_tree_barrier_node {
  int32_t k;
  int32_t count;
  volatile int32_t sense;
  struct omp_tree_barrier_node* parent;
#ifdef CACHE_PADDING
  int8_t padding[TREE_BARRIER_NODE_PADDING];
#endif
} omp_tree_barrier_node_t;

typedef struct tree_barrier_thread_data {
  int32_t sense;
  omp_tree_barrier_node_t* mynode;
#ifdef CACHE_PADDING
  int8_t padding[TREE_BARRIER_THREAD_DATA_PADDING];
#endif
} tree_barrier_thread_data_t;

typedef struct omp_tree_barrier {
  int N;
  omp_tree_barrier_node_t* nodes;
  tree_barrier_thread_data_t* threads;
} omp_tree_barrier_t;

void omp_tree_barrier_init(omp_tree_barrier_t* barrier, int num_threads);

void omp_tree_barrier_destroy(omp_tree_barrier_t* barrier);

void omp_tree_barrier(omp_tree_barrier_t* barrier);

void omp_tree_barrier_aux(omp_tree_barrier_node_t* node, int32_t* local_sense);

#endif //PROJECT2_OMP_TREE_BARRIER_H
