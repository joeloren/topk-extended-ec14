#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rand.h"

int n; // number of voters
int m; // number of alternatives
int *votes; // votes

// randomize votes from a given starting point
void randomize_votes(int st);

// calculate winner
// K1,K2 = m ==> true winner
// K1,K2 = k ==> naive algorithm
// K1 = k, K2 = m ==> Algorithm 1
// needs an array of length m^2
int winner(int *s, int K1, int K2);

// maximum index in array
int max(int n, double *a);

int main(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr, "usage: %s num-voters num-alternatives num-samples [num-samples-prob [num-samples-optimal]]\n", *argv);
    fprintf(stderr, "num-samples-prob is the number of samples used to estimate probabilities in optimal algorithm\n");
    fprintf(stderr, "num-samples-optimal is the number of samples used to estimate success probability of optimal algorithm\n");
    exit(1);
  }

  srandom(9876356);
  n = atoi(*++argv);
  m = atoi(*++argv);
  assert(votes = (int *)malloc(n * m * sizeof(int)));
  int samples = atoi(*++argv);
  int samples_p = 0, samples_opt = 0;
  if (*++argv) {
    samples_p = atoi(*argv);
    samples_opt = *++argv ? atoi(*argv) : samples;
  }

  // expected success probability
  int i, j, k, r, *temp, alg, real_winner, *cnt_naive, *cnt_better;
  assert(temp = (int *)malloc(m * m * sizeof(int)));
  assert(cnt_naive = (int *)calloc(m, sizeof(int)));
  assert(cnt_better = (int *)calloc(m, sizeof(int)));

  for (i = 0; i < samples; i++) {
    randomize_votes(0);
    real_winner = winner(temp, m, m);
    for (k = 1; k <= m-1; k++) {
      if (winner(temp, k, k) == real_winner)
	cnt_naive[k]++;
      if (winner(temp, k, m) == real_winner)
	cnt_better[k]++;
    }
  }

  printf("n %d m %d rule Copeland samples %d prob-samples %d opt-samples %d\n",
	 n, m, samples, samples_p, samples_opt);
  printf("naive  ");
  for (k = 0; k < m-1; k++)
    printf(" %f", (double)(cnt_naive[k])/samples);
  printf("\n");
  printf("better ");
  for (k = 0; k < m-1; k++)
    printf(" %f", (double)(cnt_better[k])/samples);
  printf("\n");

  if (samples_opt == 0)
    return;

  double E, *sc;
  int *cnt = cnt_naive;
  assert(sc = (double *)malloc(m * sizeof(double)));

  printf("optimal");
  for (k = 1; k <= m-1; k++) {
    E = 0;
    for (i = 0; i < samples_opt; i++) {
      for (j = 0; j < m; j++)
	cnt[j] = 0;
      randomize_votes(0);
      for (r = 0; r < samples_p; r++) {
	randomize_votes(k);
	cnt[winner(temp, m, m)]++;
      }
      for (j = 0; j < m; j++)
	sc[j] = (double)(cnt[j])/samples_p;
      E += sc[max(m, sc)];
    }
    printf(" %f", E/samples_opt);
  }
  printf("\n");
}

void randomize_votes(int st) {
  int i;
  int *vote = votes;
  
  for (i = 0; i < n; i++) {
    make_perm(m-st, vote+st, st == 0);
    vote += m;
  }
}

int winner(int *a, int K1, int K2) {
  int i, j, k;
  int *vote = votes;
  for (j = 0; j < m*m; j++)
    a[j] = 0;
  for (i = 0; i < n; i++) {
    for (j = 0; j < K1; j++)
      for (k = j + 1; k < K2; k++) {
	a[m*vote[j] + vote[k]]++;
	a[m*vote[k] + vote[j]]--;
      }
    vote += m;
  }
  for (j = 0; j < m; j++)
    for (k = 0; k < m; k++)
      if (j != k) {
	if (a[m*j + k] > 0)
	  a[(m+1)*j]++;
	if (a[m*j + k] < 0)
	  a[(m+1)*j]--;
      }
  int res = 0;
  for (j = 1; j < m; j++)
    if (a[(m+1)*j] > a[(m+1)*res])
      res = j;
  return res;
}

int max(int n, double *a) {
  int res = 0, i;
  for (i = 1; i < n; i++)
    if (a[i] > a[res])
      res = i;
  return res;
}
