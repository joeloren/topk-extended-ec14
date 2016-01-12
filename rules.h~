#ifndef RULES_H
#define RULES_H

// describe (on stderr) available rules
void describe_rules();

// input:
//   arg - rule: borda, harmonic or geometric-%f
//   m - number of candidates
// output:
//   if rule is unknown, displays error message and exits with return value 2
//   name - rule's name (preallocated)
//   scores - the scores

void parse_rule(const char *arg, int m,
		char *name, double *scores);

#endif
