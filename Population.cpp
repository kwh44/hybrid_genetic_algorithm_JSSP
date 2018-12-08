//
// Created by kwh44 on 12/7/18.
//

#include "Population.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <ctime>

Population::Population(int size, int MaxDur, double prob_cross) {
    this->population_size = size;
    this->population_array.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        population_array.emplace_back(Chromosome(size, MaxDur, prob_cross));
    }
}


void Population::new_generation() {
    std::vector<Chromosome> new_generation{static_cast<size_t>(population_size)};

    int n_percent = population_size * 0.1;
    std::sort(population_array.begin(), population_array.end(),
              [](Chromosome x, Chromosome y) { return Chromosome::cost_function(x) > Chromosome::cost_function(y); });

    size_t i = 0;
    // elitist strategy
    for (; i < n_percent; ++i) {
        new_generation[i] = population_array[i];
    }
    std::cout << "The best chromosome were copied into new generation.\n";

    // parameterized uniform crossover
    n_percent = population_size * 0.8;

    auto seed = static_cast<int>(time(nullptr));
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<> dist(0, population_size - 1);

    for (; i < n_percent; ++i) {
        int parent_one_index = dist(rng);
        int parent_two_index = dist(rng);
        while (parent_one_index == parent_two_index) {
            parent_two_index = dist(rng);
        }
        new_generation[i] = Chromosome::cross(this->population_array[parent_one_index],
                                              this->population_array[parent_two_index]);
    }
    std::cout << "Crossover process completed.\n";

    std::cout << "The rest of new population is randomly generated to prevent early convergence.\n";
    // one or more new members of the population are randomly generated
    // from the same distribution as the original population
    for (; i < population_size; ++i) {
        new_generation.emplace_back(
                Chromosome(population_size, new_generation[0].get_max_dur(), new_generation[0].get_prob_cross()));
    }
    this->population_array.clear();
    this->population_array = new_generation;
}
