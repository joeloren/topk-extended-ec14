#include "rules.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void describe_rules() {
    fprintf(stderr, "rule: borda harmonic geometric-%%f\n");
}

void parse_rule(const char *arg, int m,
		char *name, double *scores) {
  int i;

  if (strcmp(arg, "borda") == 0) {
    strcpy(name, "Borda");
    for (i = 0; i < m; i++)
      scores[i] = m - i;
  } else if (strcmp(arg, "harmonic") == 0) {
    strcpy(name, "Harmonic");
    for (i = 0; i < m; i++)
      scores[i] = 1./(i+1);
  } else if (strncmp(arg, "geometric-", strlen("geometric-")) == 0) {
    double p = atof(arg + strlen("geometric-"));
    sprintf(name, "Geometric-%f", p);
    scores[0] = 1;
    for (i = 1; i < m; i++)
      scores[i] = scores[i-1] * p;
  } else {
    fprintf(stderr, "error: unknown rule %s\n", arg);
    exit(2);
  }
}
