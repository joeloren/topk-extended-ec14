#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int n; // number of voters
int m; // number of alternatives
double disp; // dispersion
double *scores; // rule scores
int *votes; // votes
double *res; // double [m]

// randomize votes
void randomize_votes();
// return winner
int winner();
// return top k winner, via either new or old algorithm
int topk_winner(int k, int old);

int main(int argc, char *argv[]) {
  int i, j, k;

  if (argc != 7) {
    fprintf(stderr, "usage: %s num-voters num-alternatives dispersion samples rule algorithm\n", *argv);
    fprintf(stderr, "rule could be: borda harmonic geometric-{p}\n");
    fprintf(stderr, "algorithm could be: new old optimal-{samples}\n");
    exit(1);
  }

  srandom(9876356);
  n = atoi(*++argv);
  m = atoi(*++argv);
  disp = atof(*++argv);
  assert(scores = (double *)malloc(m * sizeof(double)));
  assert(res = (double *)malloc(m * sizeof(double)));
  assert(votes = (int *)malloc(n * m * sizeof(int)));

  int samples = atoi(*++argv);
  enum { rBorda, rHarmonic, rGeometric } rule;
  char rule_name[100];
  double geometric_p;
  ++argv;
  if (strcmp(*argv, "borda") == 0) {
    rule = rBorda;
    strcpy(rule_name, "Borda");
    for (i = 0; i < m; i++)
      scores[i] = m - i;
  } else if (strcmp(*argv, "harmonic") == 0) {
    rule = rHarmonic;
    strcpy(rule_name, "Harmonic");
    for (i = 0; i < m; i++)
      scores[i] = 1./(i+1);
  } else if (strncmp(*argv, "geometric-", strlen("geometric-")) == 0) {
    rule = rGeometric;
    geometric_p = atof(*argv + strlen("geometric-"));
    sprintf(rule_name, "Geometric-%f\n", geometric_p);
    scores[0] = 1;
    for (i = 1; i < m; i++)
      scores[i] = scores[i-1] * geometric_p;
  } else {
    fprintf(stderr, "error: unknown rule %s\n", *argv);
    exit(2);
  }

  enum { aNew, aOld, aOptimal } algorithm;
  char algorithm_name[100];
  int optimal_samples;
  ++argv;
  if (strcmp(*argv, "new") == 0) {
    algorithm = aNew;
    strcpy(algorithm_name, "New");
  } else if (strcmp(*argv, "old") == 0) {
    algorithm = aOld;
    strcpy(algorithm_name, "Old");
  } else if (strncmp(*argv, "optimal-", strlen("optimal-")) == 0) {
    algorithm = aOptimal;
    optimal_samples = atoi(*argv + strlen("optimal-"));
    sprintf(algorithm_name, "Optimal-%d", optimal_samples);
  } else {
    fprintf(stderr, "error: unknown algorithm %s\n");
    exit(3);
  }

  int results, winner_true, winner_alg;
  printf("n %d m %d disp %f samples %d rule %s alg %s results",
	 n, m, disp, samples, rule_name, algorithm_name);
  for (k = 1; k <= m-1; k++) {
    results = 0;
    for (i = 0; i < samples; i++) {
      randomize_votes();
      winner_true = winner();
      switch (algorithm) {
      case aNew: case aOld:
	winner_alg = topk_winner(k, algorithm == aOld); break;
      case aOptimal:
	assert(0); // unsupported currently
      }
      if (winner_alg == winner_true)
	results++;
    }
    printf(" %f", (double)(results)/samples);
  }
  printf("\n");
}

// randomly permute array
void make_perm(int s, int *a) {
  int i, j, t;
  for (i = 0; i < s-1; i++) {
    j = i + random() % (s - i);
    t = a[i];
    a[i] = a[j];
    a[j] = t;
  }
}

// randomize votes
void randomize_votes() {
  assert(disp == 1); // only supported value currently
  int i, j;
  int *vote;

  vote = votes;
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++)
      vote[j] = j;
    make_perm(m, vote);
    vote += m;
  }
}

// winner
int winner() {
  return topk_winner(m, 1);
}

// top-k winner
int topk_winner(int k, int old) {
  double def = 0;
  int i, j;
  int *vote;

  if (!old && (k < m)) {
    for (i = k; i < m; i++)
      def += scores[i];
    def /= m - k;
  }
  for (i = 0; i < m; i++)
    res[i] = 0;
  vote = votes;
  for (i = 0; i < n; i++) {
    assert(k <= m);
    for (j = 0; j < k; j++)
      res[vote[j]] += scores[j];
    for (; j < m; j++)
      res[vote[j]] += def;
    vote += m;
  }

  int ans;
  double max;
  ans = 0;
  max = res[0];
  for (i = 1; i < m; i++)
    if (res[i] > max) {
      ans = i;
      max = res[i];
    }
  return ans;
}
