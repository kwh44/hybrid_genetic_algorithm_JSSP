//
// Created by kwh44 on 12/7/18.
//

#ifndef HYBRID_ALGORITHM_POPULATION_H
#define HYBRID_ALGORITHM_POPULATION_H

#include "Chromosome.h"
#include <vector>

class Population {
    std::vector<Chromosome> population_array;
    int population_size = 0;

public:
    std::vector<Chromosome> &population() { return population_array; }

    int size() const { return population_size; }

    explicit Population(int size, int MaxDur, double prob_cross);

    void new_generation();

};


#endif //HYBRID_ALGORITHM_POPULATION_H
