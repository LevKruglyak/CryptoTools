#include "graph.h"

static int GLOBAL_ID = 0;

int cptl::GenerateID() { return GLOBAL_ID++; }