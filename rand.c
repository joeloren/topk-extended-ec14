#include "rand.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>

void make_perm(int n, int *a, int init) {
  int i, j, t;

  if (init)
    for (i = 0; i < n; i++)
      a[i] = i;
  for (i = 0; i < n-1; i++) {
    j = i + random() % (n - i);
    t = a[i];
    a[i] = a[j];
    a[j] = t;
  }
}

double uniform() {
  return random()/(double)(RAND_MAX);
}

void mallows(int n, int *a, double phi) {
  int i, j, k;
  double U, phi0, phi1;

  // we use the repeated insertion model
  a[0] = 0;
  phi0 = phi; // phi**(i+1)
  for (i = 1; i < n; i++) {
    phi0 *= phi;
    U = uniform() * (1 - phi0)/(1 - phi);
    phi1 = 1;
    for (j = 0; j < i; j++) {
      if (U < phi1)
	break;
      U -= phi1;
      phi1 *= phi;
    }
    j = i-j;
    // insert i at position j
    for (k = i-1; k >= j; k--)
      a[k+1] = a[k];
    a[j] = i;
  }
}
