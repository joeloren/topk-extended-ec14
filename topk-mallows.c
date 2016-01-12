#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rules.h"
#include "rand.h"

int n; // number of voters
int m; // number of alternatives
double *scores; // rule scores

int main(int argc, char *argv[]) {
  if (argc < 6) {
    fprintf(stderr, "usage: %s num-voters num-alternatives rule num-samples phi | phi-from phi-to phi-step\n", *argv);
    describe_rules();
    fprintf(stderr, "copeland is also supported as a rule (but is O(m^2) instead of O(m))\n");
    exit(1);
  }

  srandom(9876356);
  n = atoi(*++argv);
  m = atoi(*++argv);
  assert(scores = (double *)malloc(m * sizeof(double)));
  char rule_name[100];
  int copeland = 0;
  if (strcmp(*++argv, "copeland") == 0) {
    copeland = 1;
    strcpy(rule_name, "Copeland");
  } else
    parse_rule(*argv, m, rule_name, scores);
  int samples = atoi(*++argv);
  double phi_from, phi_to, phi_step, phi;
  phi_from = atof(*++argv);
  if (*++argv) {
    phi_to = atof(*argv++);
    assert(*argv);
    phi_step = atof(*argv++);
  } else {
    phi_to = phi_from;
    phi_step = 1;
  }

  int cnt_winner, cnt_first;
  int samp, i, j, k;
  int *vote, *first;
  double *sc;
  int *sc2;
  assert(vote = (int *)malloc(m * sizeof(int)));
  assert(first = (int *)malloc(m * sizeof(int)));
  assert(sc = (double *)malloc(m * sizeof(double)));
  assert(sc2 = (int *)malloc(m * m * sizeof(int)));

  printf("n %d m %d rule %s samples %d\n", n, m, rule_name, samples);
  for (phi = phi_from; phi <= phi_to; phi += phi_step) {
    cnt_winner = cnt_first = 0;
    for (samp = 0; samp < samples; samp++) {
      for (i = 0; i < m; i++) {
	first[i] = 0;
	sc[i] = 0;
      }
      for (i = 0; i < m*m; i++)
	sc2[i] = 0;
      for (i = 0; i < n; i++) {
	mallows(m, vote, phi);
	first[vote[0]]++;
	if (copeland) {
	  for (j = 0; j < m; j++)
	    for (k = j + 1; k < m; k++) {
	      sc2[m*vote[j] + vote[k]]++;
	      sc2[m*vote[k] + vote[j]]--;
	    }
	} else {
	  for (j = 0; j < m; j++)
	    sc[vote[j]] += scores[j];
	}
      }
      for (i = 1; i < m; i++)
	if (first[i] > first[0])
	  break;
      cnt_first += i == m;
      if (copeland) {
	for (j = 0; j < m; j++)
	  for (k = 0; k < m; k++)
	    if (j != k) {
	      if (sc2[m*j + k] > 0)
		sc[j]++;
	      if (sc2[m*j + k] < 0)
		sc[j]--;
	    }
      }
      for (i = 1; i < m; i++)
	if (sc[i] > sc[0])
	  break;
      cnt_winner += i == m;
    }
    printf("%f %f %f\n", phi, (double)cnt_winner/samples, (double)cnt_first/samples);
  }
}
