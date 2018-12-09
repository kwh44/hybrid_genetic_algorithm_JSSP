//
// Created by kwh44 on 12/7/18.
//

#include "Population.h"
#include <algorithm>
#include <random>
#include <ctime>

static int _max(std::vector<std::vector<int> > &data);


Population::Population(int size, double prob_cross, std::vector<std::vector<int> > & data) {
    this->_schedule = Schedule(data);
    this->population_size = size;
    this->population_array.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        population_array.emplace_back(Chromosome(data.size() * (data[0].size() / 2), _max(data), prob_cross));
    }
}

static int _max(std::vector<std::vector<int> > &data) {
    int duration = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 1; j < data[0].size(); j += 2) {
            if (duration < data[i][j]) {
                duration = data[i][j];
            }
        }
    }
    return duration;
}

void Population::new_generation() {
    std::vector<Chromosome> new_generation;
    new_generation.reserve(population_size);
    int n_percent = static_cast<int>(population_size * 0.1);
    std::sort(population_array.begin(), population_array.end(),
              [&](Chromosome& x, Chromosome& y) { return this->_schedule.cost_function(x) > this->_schedule.cost_function(y); });

    size_t i = 0;
    // elitist strategy
    for (; i < n_percent; ++i) {
        new_generation.push_back(population_array[i]);
    }
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
        new_generation.emplace_back(Chromosome::cross(this->population_array[parent_one_index], this->population_array[parent_two_index]));
    }

    // one or more new members of the population are randomly generated
    // from the same distribution as the original population

    for (; i < population_size; ++i) {
        new_generation.emplace_back(Chromosome(population_size, new_generation[0].get_max_dur(), new_generation[0].get_prob_cross()));
    }

    this->population_array.clear();
    this->population_array = new_generation;
}
