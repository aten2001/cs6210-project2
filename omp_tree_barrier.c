#include <stdio.h>
#include <stdint.h>
#include "omp_tree_barrier.h"

int construct_tree_aux(int start_id, int end_id, omp_tree_barrier_node_t* parent, omp_tree_barrier_node_t* nodes,
                       tree_barrier_thread_data_t* threads, int free_node_ptr) {
  omp_tree_barrier_node_t* node = &nodes[free_node_ptr++];
  node->parent = parent;
  node->sense = false;
  node->count = node->k = 2;
  if ((end_id - start_id) == 1) {
    threads[start_id].mynode = node;
    threads[end_id].mynode = node;
  } else {
    free_node_ptr = construct_tree_aux(start_id, (start_id + end_id) / 2, node, nodes, threads, free_node_ptr);
    if ((end_id - start_id) == 2) {
      threads[end_id].mynode = node;
    } else {
      free_node_ptr = construct_tree_aux((start_id + end_id) / 2 + 1, end_id, node, nodes, threads, free_node_ptr);
    }
  }
  return free_node_ptr;
}

void construct_tree(int num_threads, omp_tree_barrier_node_t* nodes, tree_barrier_thread_data_t* threads) {
  if (num_threads == 1) {
    omp_tree_barrier_node_t* root = &nodes[0];
    root->parent = NULL;
    root->sense = false;
    root->count = root->k = 1;
    threads[0].mynode = root;
  } else {
    construct_tree_aux(0, num_threads - 1, NULL, nodes, threads, 0);
  }
}

void omp_tree_barrier_init(omp_tree_barrier_t *barrier, int num_threads) {
  barrier->N = num_threads;
  posix_memalign((void**)&barrier->nodes, 64, sizeof(omp_tree_barrier_node_t) * barrier->N);

#ifdef CACHE_PADDING
  assert((barrier->nodes % 64) == 0)
  assert(sizeof(omp_tree_barrier_node_t) == 64);
#endif
  if (barrier->nodes == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  posix_memalign((void**)&barrier->threads, 64, sizeof(tree_barrier_thread_data_t) * barrier->N);
#ifdef CACHE_PADDING
  assert((barrier->threads % 64) == 0)
  assert(sizeof(tree_barrier_thread_data_t) == 64);
#endif
  if (barrier->threads == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < barrier->N; i++) {
    barrier->threads[i].sense = true;
  }

  construct_tree(num_threads, barrier->nodes, barrier->threads);
}

void omp_tree_barrier_destroy(omp_tree_barrier_t *barrier) {
  free(barrier->threads);
  free(barrier->nodes);
}

void omp_tree_barrier(omp_tree_barrier_t *barrier) {
  int32_t* local_sense = &(barrier->threads[omp_get_thread_num()].sense);
  omp_tree_barrier_aux(barrier->threads[omp_get_thread_num()].mynode, local_sense);
  *local_sense = !*local_sense;
}

void omp_tree_barrier_aux(omp_tree_barrier_node_t* node, int32_t* local_sense) {
  if (__sync_fetch_and_sub(&node->count, 1) == 1) {
    if (node->parent) {
      omp_tree_barrier_aux(node->parent, local_sense);
    }
    node->count = node->k;
    node->sense = !node->sense;
  }
  while (node->sense != *local_sense);
}

