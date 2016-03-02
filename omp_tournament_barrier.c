#include <stdio.h>
#include <assert.h>

#include "omp_tournament_barrier.h"


void omp_tournament_barrier_init(omp_tournament_barrier_t* barrier, int num_threads) {
  barrier->N = num_threads;
  barrier->threads = (tournament_barrier_thread_data_t*) malloc(sizeof(tournament_barrier_thread_data_t) * num_threads);
  barrier->num_rounds = log2ceil(barrier->N) + 1;

  if (barrier->threads == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < barrier->N; i++) {
    tournament_barrier_thread_data_t* thread_data = &barrier->threads[i];
    thread_data->sense = true;
    thread_data->rounds = (round_t*) malloc(sizeof(round_t) * barrier->num_rounds);

    if (thread_data->rounds == NULL) {
      perror("malloc failed");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < barrier->N; i++) {
    tournament_barrier_thread_data_t* thread_data = &barrier->threads[i];
    for (int k = 0; k < barrier->num_rounds; k++) {
      thread_data->rounds[k].flag = false;
      thread_data->rounds[k].role = UNKNOWN;

      if (k > 0) {
        if ((i == 0) && ((1 << k) >= barrier->N)) {
          thread_data->rounds[k].role = CHAMPION;
          int32_t opponent_id = i + (1 << (k - 1));
          tournament_barrier_thread_data_t* opponent = &barrier->threads[opponent_id];
          thread_data->rounds[k].opponent = &(opponent->rounds[k].flag);
        } else if ((i % (1 << k)) == 0) {
          if ((i + (1 << (k - 1))) < barrier->N) {
            if ((1 << k) < barrier->N) {
              thread_data->rounds[k].role = WINNER;
              int32_t opponent_id = i + (1 << (k - 1));
              tournament_barrier_thread_data_t* opponent = &barrier->threads[opponent_id];
              thread_data->rounds[k].opponent = &(opponent->rounds[k].flag);
            }
          } else {
            thread_data->rounds[k].role = BYE;
          }
        } else if ((i % (1 << k)) == (1 << (k - 1))) {
          thread_data->rounds[k].role = LOSER;
          int32_t opponent_id = i - (1 << (k - 1));
          tournament_barrier_thread_data_t* opponent = &barrier->threads[opponent_id];
          thread_data->rounds[k].opponent = &(opponent->rounds[k].flag);
        }
      } else {
        thread_data->rounds[k].role = DROPOUT;
      }
    }
  }
}

void omp_tournament_barrier_destroy(omp_tournament_barrier_t* barrier) {
  for (int i = 0; i < barrier->N; i++) {
    tournament_barrier_thread_data_t* thread_data = &barrier->threads[i];
    free(thread_data->rounds);
  }
  free(barrier->threads);
}

void omp_tournament_barrier(omp_tournament_barrier_t* barrier) {
  tournament_barrier_thread_data_t* thread_data = &barrier->threads[omp_get_thread_num()];

  int round = 1;
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
      case BYE:
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