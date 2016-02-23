#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "omp.h"

#define true 1
#define false 0

static inline int32_t log2ceil(int32_t value) {
  if (value == 0) {
    return -1;
  }

  int32_t loop_val = value;
  int32_t count = 1;
  while (loop_val >>= 1) {
    ++count;
  }

  if (value > (1 << count)) {
    ++count;
  }
  return count;
}

typedef enum {
  UNKNOWN, WINNER, LOSER, BYE, CHAMPION, DROPOUT
} role_t;

typedef struct round {
  role_t role;
  int32_t *opponent;
  int32_t flag;
} round_t;

typedef struct thread_data {
  int32_t sense;
  round_t *rounds;
} thread_data_t;

typedef struct omp_tournament_barrier {
  int32_t N;
  int32_t num_rounds;
  thread_data_t *threads;
} omp_tournament_barrier_t;

void omp_tournament_barrier_init(omp_tournament_barrier_t *barrier, int num_threads) {
  barrier->N = num_threads;
  barrier->threads = (thread_data_t *) malloc(sizeof(thread_data_t) * num_threads);

  if (barrier->threads == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < barrier->N; i++) {
    thread_data_t *thread_data = &barrier->threads[i];
    thread_data->sense = true;
    thread_data->rounds = (round_t *) malloc(sizeof(round_t) * barrier->num_rounds);

    if (thread_data->rounds == NULL) {
      perror("malloc failed");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < barrier->N; i++) {
    thread_data_t *thread_data = &barrier->threads[i];
    for (int k = 0; k < barrier->num_rounds; k++) {
      thread_data->rounds[k].flag = false;
      thread_data->rounds[k].role = UNKNOWN;

      if (k > 0) {
        if ((i % (1 << k)) == 0) {
          if ((i + (1 << (k - 1))) < barrier->N) {
            if ((1 << k) < barrier->N) {
              thread_data->rounds[k].role = WINNER;
              int32_t opponent_id = i + (1 << (k - 1));
              thread_data_t *opponent = &barrier->threads[opponent_id];
              thread_data->rounds[k].opponent = &(opponent->rounds[k].flag);
            }
          } else {
            thread_data->rounds[k].role = BYE;
          }
        } else if ((i % (1 << k)) == (1 << (k - 1))) {
          thread_data->rounds[k].role = LOSER;
          int32_t opponent_id = i - (1 << (k - 1));
          thread_data_t *opponent = &barrier->threads[opponent_id];
          thread_data->rounds[k].opponent = &(opponent->rounds[k].flag);
        } else if ((i == 0) && ((1 << k) >= barrier->N)) {
          thread_data->rounds[k].role = CHAMPION;
          int32_t opponent_id = i + (1 << (k - 1));
          thread_data_t *opponent = &barrier->threads[opponent_id];
          thread_data->rounds[k].opponent = &(opponent->rounds[k].flag);
        }
      } else {
        thread_data->rounds[k].role = DROPOUT;
      }

    }
  }
}

void omp_tournament_barrier_destroy(omp_tournament_barrier_t *barrier) {
  for (int i = 0; i < barrier->N; i++) {
    thread_data_t *thread_data = &barrier->threads[i];
    free(thread_data->rounds);
  }
  free(barrier->threads);
}

void omp_tournament_barrier(omp_tournament_barrier_t *barrier) {
  thread_data_t *thread_data = &barrier->threads[omp_get_thread_num()];

  int round = 0;
  for (; ;) {
    int end_loop = false;
    switch (thread_data->rounds[round].role) {
      case LOSER:
        *(thread_data->rounds[round].opponent) = thread_data->sense;
        while (thread_data->rounds[round].flag != thread_data->sense);
        end_loop = true;
        break;
      case WINNER:
        while (thread_data->rounds[round].flag != thread_data->sense);
        break;
      case CHAMPION:
        while (thread_data->rounds[round].flag != thread_data->sense);
        *(thread_data->rounds[round].opponent) = thread_data->sense;
        end_loop = true;
        break;
      default:
        assert(false);
    }

    if (end_loop) {
      break;
    }

    round = round + 1;
  }

  for (; ;) {
    round = round - 1;
    int32_t end_loop = false;
    switch (thread_data->rounds[round].role) {
      case WINNER:
        *(thread_data->rounds[round].opponent) = thread_data->sense;
        break;
      case BYE:
        break;
      case DROPOUT:
        end_loop = true;
        break;
      default:
        assert(false);
    }

    if (end_loop) {
      break;
    }
  }

  thread_data->sense = !thread_data->sense;
}