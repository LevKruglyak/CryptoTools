#include "graph.h"

static int GLOBAL_ID = 0;

int cptl::GenerateID() { return GLOBAL_ID++; }

cptl::Buffer cptl::empty_buffer = cptl::Buffer();
cptl::Integer cptl::zero_integer = cptl::Integer();
