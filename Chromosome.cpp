//
// Created by kwh44 on 12/7/18.
//

#include "Chromosome.h"
#include <random>
#include <iostream>
#include <ctime>


Chromosome::Chromosome(int size, int MaxDur, double prob_cross) {
    this->number_of_genes = size * 2;
    this->max_dur = MaxDur;
    this->probability_of_crossing = prob_cross;

    genes.reserve(size * 2);
    std::mt19937_64 rng(size);
    std::uniform_real_distribution<double> dist(0, 1);

    for (size_t i = 0; i < size; ++i) {
        genes[i] = dist(rng);
        genes[i + size] = genes[i] * 1.5 * max_dur;
    }
}


Chromosome Chromosome::cross(Chromosome &one, Chromosome &two) {
    Chromosome return_chromosome = Chromosome(one.get_size() / 2, one.get_max_dur(), one.get_prob_cross());

    auto seed = static_cast<int>(time(nullptr));
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> dist(0, 1);

    for (int i = 0; i < return_chromosome.get_size(); ++i) {
        if (dist(rng) <= return_chromosome.get_prob_cross()) {
            return_chromosome.genes[i] = one.genes[i];
        } else {
            return_chromosome.genes[i] = two.genes[i];
        }
    }
    return return_chromosome;
}


int Chromosome::cost_function(Chromosome &item) {
    // it takes as the input the chromosome
    // the return is the makespan of the schedule
    // obtained by using this chromosome

    // I need to construct the schedule
    // apply to the obtained schedule local search
    // procedure, and return the makespan of the schedule
    return 1;
}
