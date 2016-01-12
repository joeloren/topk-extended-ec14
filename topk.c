#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rules.h"
#include "rand.h"

int n; // number of voters
int m; // number of alternatives
double *scores; // rule scores
int *votes; // votes

// randomize votes from a given starting point
void randomize_votes(int st);

// calculate winner
// needs an array of length m
int winner(double *s);

// maximum index in array
int max(int n, double *a);

int main(int argc, char *argv[]) {
  if (argc < 5) {
    fprintf(stderr, "usage: %s num-voters num-alternatives rule num-samples [num-samples-prob [num-samples-optimal]]\n", *argv);
    fprintf(stderr, "num-samples-prob is the number of samples used to estimate probabilities in optimal algorithm\n");
    fprintf(stderr, "num-samples-optimal is the number of samples used to estimate success probability of optimal algorithm\n");
    describe_rules();
    exit(1);
  }

  srandom(9876356);
  n = atoi(*++argv);
  m = atoi(*++argv);
  assert(scores = (double *)malloc(m * sizeof(double)));
  assert(votes = (int *)malloc(n * m * sizeof(int)));
  char rule_name[100];
  parse_rule(*++argv, m, rule_name, scores);
  int samples = atoi(*++argv);
  int samples_p = 0, samples_opt = 0;
  if (*++argv) {
    samples_p = atoi(*argv);
    samples_opt = *++argv ? atoi(*argv) : samples;
  }

  int i, j, k, l, r, true_winner;
  double *sc, tail_score;
  int *missing, *cnt_naive, *cnt_better, *vote;
  assert(sc = (double *)malloc(m * sizeof(double)));
  assert(missing = (int *)malloc(m * sizeof(int)));
  assert(cnt_naive = (int *)calloc(m, sizeof(int)));
  assert(cnt_better = (int *)calloc(m, sizeof(int)));

  for (i = 0; i < samples; i++) {
    randomize_votes(0);
    true_winner = winner(sc);
    tail_score = 0;
    for (j = 0; j < m; j++)
      tail_score += scores[j];
    for (j = 0; j < m; j++) {
      sc[j] = 0;
      missing[j] = n;
    }
    for (k = 0; k < m-1; k++) {
      tail_score -= scores[k];
      vote = votes;
      for (j = 0; j < n; j++) {
	sc[vote[k]] += scores[k];
	missing[vote[k]]--;
	vote += m;
      }
      if (max(m, sc) == true_winner)
	cnt_naive[k]++;
      for (j = 0; j < m; j++)
	sc[j] += missing[j] * tail_score/(m-k-1);
      if (max(m, sc) == true_winner)
	cnt_better[k]++;
      for (j = 0; j < m; j++)
	sc[j] -= missing[j] * tail_score/(m-k-1);
    }
  }

  printf("n %d m %d rule %s samples %d prob-samples %d opt-samples %d\n",
	 n, m, rule_name, samples, samples_p, samples_opt);
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

  // expected success probability
  double E;
  int *cnt = cnt_naive;

  printf("optimal");
  for (k = 1; k <= m-1; k++) {
    E = 0;
    for (i = 0; i < samples_opt; i++) {
      for (j = 0; j < m; j++)
	cnt[j] = 0;
      randomize_votes(0);
      for (r = 0; r < samples_p; r++) {
	randomize_votes(k);
	cnt[winner(sc)]++;
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

int winner(double *a) {
  int i, j;
  int *vote = votes;
  for (j = 0; j < m; j++)
    a[j] = 0;
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++)
      a[vote[j]] += scores[j];
    vote += m;
  }
  return max(m, a);
}

int max(int n, double *a) {
  int res = 0, i;
  for (i = 1; i < n; i++)
    if (a[i] > a[res])
      res = i;
  return res;
}
