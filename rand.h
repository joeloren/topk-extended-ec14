#ifndef RAND_H
#define RAND_H

// everything here uses the random()/srandom() system

// make a random permutation, optionally initializing the array
void make_perm(int n, int *a, int init);

// make a random permutation corresponding to a Mallows distribution
void mallows(int n, int *a, double phi);

#endif
