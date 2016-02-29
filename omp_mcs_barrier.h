//
// Created by manatunga on 2/29/16.
//

#ifndef PROJECT2_OMP_MCS_BARRIER_H
#define PROJECT2_OMP_MCS_BARRIER_H

#include <stdlib.h>
#include "omp.h"

#define true 1
#define false 0
#define MCS_NODE_PADDING 0;

typedef struct omp_mcs_barrier_node {
  volatile int32_t parentsense;
  int32_t num_children;
  volatile int8_t child_notready[4];
  volatile int8_t* parent_child_not_ready_ptr;
  volatile int32_t* child_sense_ptrs[2];
  int32_t dummy;
#if MCS_NODE_PADDING
  int32_t padding[MCS_NODE_PADDING];
#endif
} omp_mcs_barrier_node_t;

typedef struct omp_mcs_barrier {
  int32_t N;
  omp_mcs_barrier_node_t* nodes;
  int32_t* local_senses;
} omp_mcs_barrier_t;

void omp_mcs_barrier_init(omp_mcs_barrier_t* barrier, int num_threads);

void omp_mcs_barrier_destroy(omp_mcs_barrier_t* barrier);

void omp_mcs_barrier(omp_mcs_barrier_t* barrier);


#endif //PROJECT2_OMP_MCS_BARRIER_H
