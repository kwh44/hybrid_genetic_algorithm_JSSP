//
// Created by kwh44 on 12/7/18.
//

#include "Chromosome.h"
#include <random>
#include <iostream>
#include <ctime>
#include <algorithm>

Chromosome::Chromosome(int number_of_operations, int MaxDur, double prob_cross) {
    this->number_of_genes = number_of_operations * 2;
    this->max_dur = MaxDur;
    this->probability_of_crossing = prob_cross;

    genes.reserve(number_of_operations * 2);
    std::mt19937_64 rng(number_of_operations);
    std::uniform_real_distribution<double> dist(0, 1);

    for (size_t i = 0; i < number_of_operations; ++i) {
        genes[i] = dist(rng);
        genes[i + number_of_operations] = genes[i] * 1.5 * max_dur;
    }
}

Chromosome::Chromosome(const Chromosome &other) {
    this->number_of_genes = other.get_size();
    this->max_dur = other.get_max_dur();
    this->probability_of_crossing = other.get_prob_cross();

    genes.reserve(number_of_genes);
    std::copy(other.genes.begin(), other.genes.end(), genes.begin());

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