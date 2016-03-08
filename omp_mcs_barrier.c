#include <stdio.h>
#include "omp_mcs_barrier.h"
#include <assert.h>


void omp_mcs_barrier_init(omp_mcs_barrier_t* barrier, int num_threads) {
  barrier->N = num_threads;
  posix_memalign((void**)&barrier->nodes, 64, sizeof(omp_mcs_barrier_node_t) * num_threads);

  if (barrier->nodes == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

#ifdef CACHE_PADDING
  assert(((long)barrier->nodes % 64) == 0);
  assert(sizeof(omp_mcs_barrier_node_t) == 64);
#endif

  for (int i = 0; i < num_threads; i++) {
    omp_mcs_barrier_node_t* node = &barrier->nodes[i];
    node->parentsense = false;
    node->sense = true;

    int first_child = 4 * i + 1;
    node->num_children = 0;
    if (first_child < num_threads) {
      node->num_children = num_threads - first_child;
      if (node->num_children > 4) {
        node->num_children = 4;
      }
    }

    for (int j = 0; j < 4; j++) {
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

void omp_mcs_barrier_destroy(omp_mcs_barrier_t* barrier) {
  free(barrier->nodes);
}

void omp_mcs_barrier(omp_mcs_barrier_t* barrier) {
  int32_t id = omp_get_thread_num();
  omp_mcs_barrier_node_t* my_node = &barrier->nodes[id];
  if (my_node->num_children > 0) {
    volatile int32_t* children_not_ready = (volatile int32_t*) &my_node->child_notready;
    while (*children_not_ready != 0) { };
    for (int i = 0; i < my_node->num_children; i++) {
      my_node->child_notready[i] = true;
    }
  }

  if (id != 0) {
    *my_node->parent_child_not_ready_ptr = false;
    while (my_node->parentsense != my_node->sense);
  }

  *my_node->child_sense_ptrs[0] = my_node->sense;
  *my_node->child_sense_ptrs[1] = my_node->sense;
  my_node->sense = !my_node->sense;
}
