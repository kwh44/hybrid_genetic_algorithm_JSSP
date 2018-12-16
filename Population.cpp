//
// Created by kwh44 on 12/7/18.
//

#include "Population.h"
#include <iostream>

static int _max(std::vector<std::vector<int> > &data);

std::mt19937_64 Population::gen = std::mt19937_64(20);

Population::Population(const Population &other) {
    this->_schedule = other._schedule;
    population_array.clear();
    for (int i = 0; i < other.population_size; ++i) population_array.push_back(other.population_array[i]);
    this->population_size = other.population_size;
}

Population::Population(size_t size, double prob_cross, std::vector<std::vector<int> > &data) {
    this->_schedule = Schedule(data);
    this->population_size = size;
    this->population_array.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        population_array.emplace_back(Chromosome(data.size() * (data[0].size() / 2), _max(data), prob_cross));
    }
}

Population &Population::operator=(Population &&other) noexcept {
    this->_schedule = other._schedule;
    population_array.clear();
    for (int i = 0; i < other.population_size; ++i) population_array.push_back(other.population_array[i]);
    this->population_size = other.population_size;
    return *this;
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
    int n_percent = population_size * 0.1;
    sort();
    size_t i = 0;
    // elitist strategy
    for (; i < n_percent; ++i) {
        new_generation.push_back(population_array[i]);
    }
    // parameterized uniform crossover
    n_percent = population_size * 0.8;

    std::uniform_int_distribution<> dist(0, population_size - 1);

    for (; i < n_percent; ++i) {
        int parent_one_index = dist(Population::gen);
        int parent_two_index = dist(Population::gen);
        while (parent_one_index == parent_two_index) {
                parent_two_index = dist(Population::gen);
        }
        new_generation.emplace_back(
                Chromosome::cross(this->population_array[parent_one_index], this->population_array[parent_two_index]));
    }
    // one or more new members of the population are randomly generated
    // from the same distribution as the original population
    for (; i < population_size; ++i) {
        auto new_size = population_array[i].get_size() / 2;
        auto max_dur = population_array[i].get_max_dur();
        auto prob_of_cross = population_array[i].get_prob_cross();
        new_generation.push_back(Chromosome(new_size, max_dur, prob_of_cross));
    }
    population_array.clear();
    for (const auto &v:new_generation) population_array.push_back(v);
}

int Population::solution() {
    auto return_value = this->_schedule.cost_function(population_array[0]);
    return return_value;
}

void Population::display_population() {
    for (int i = 0; i < size(); ++i) {
        for (int j = 0; j < population_array[i].get_size(); ++j) {
            std::cout << population_array[i].get_genes()[j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Population::sort() {
    std::vector<int> index_list;
    index_list.reserve(population_size);
    for (int i = 0; i < population_size; ++i) index_list.push_back(i);
    std::sort(index_list.begin(), index_list.end(),
              [&](int x, int y) {
                  return this->_schedule.cost_function(population_array[x]) >
                         this->_schedule.cost_function(population_array[y]);
              });

    for (size_t i = 0; i < index_list.size(); ++i) {
        auto temp = population_array[i];
        population_array[i] = population_array[index_list[i]];
        population_array[index_list[i]] = temp;
    }
}
