//
// Created by kwh44 on 12/7/18.
//

#include "Chromosome.h"
#include <random>
#include <iostream>
#include <ctime>
#include <algorithm>


Chromosome::Chromosome(int index, int number_of_operations, int MaxDur, double prob_cross) {
    this->number_of_genes = number_of_operations * 2;
    this->max_dur = MaxDur;
    this->probability_of_crossing = prob_cross;
    auto seed = index * static_cast<int>(time(nullptr));
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> dist(0, 1);

    for (size_t i = 0; i < number_of_operations; ++i)
        this->genes.push_back(dist(rng));
    for (size_t i = 0; i < number_of_operations; ++i)
        this->genes.push_back(genes[i] * 1.5 * max_dur);

}


Chromosome::Chromosome(Chromosome &other) {
    this->number_of_genes = other.get_size();
    this->max_dur = other.get_max_dur();
    this->probability_of_crossing = other.get_prob_cross();
    for (size_t i = 0; i < other.genes.size(); ++i) genes.push_back(other.genes[i]);
}

Chromosome::Chromosome(const Chromosome &other) {
    this->number_of_genes = other.number_of_genes;
    this->max_dur = other.max_dur;
    this->probability_of_crossing = other.probability_of_crossing;
    for (size_t i = 0; i < other.number_of_genes; ++i) genes.push_back(other.genes[i]);
}


Chromosome::Chromosome(Chromosome &&other) noexcept {
    this->number_of_genes = other.number_of_genes;
    this->max_dur = other.max_dur;
    this->probability_of_crossing = other.probability_of_crossing;
    for (size_t i = 0; i < other.number_of_genes; ++i) genes.push_back(other.genes[i]);
}

Chromosome &Chromosome::operator=(const Chromosome &other) {
    this->number_of_genes = other.number_of_genes;
    this->max_dur = other.max_dur;
    this->probability_of_crossing = other.probability_of_crossing;
    this->genes.clear();
    for (size_t i = 0; i < other.number_of_genes; ++i) genes.push_back(other.genes[i]);
    return *this;
}

Chromosome &Chromosome::operator=(Chromosome &&other) noexcept {
    this->number_of_genes = other.number_of_genes;
    this->max_dur = other.max_dur;
    this->probability_of_crossing = other.probability_of_crossing;
    this->genes.clear();
    for (size_t i = 0; i < other.number_of_genes; ++i) genes.push_back(other.genes[i]);
    return *this;
}


Chromosome Chromosome::cross(Chromosome &one, Chromosome &two) {
    auto return_chromosome = Chromosome();
    return_chromosome.set_max_dur(one.get_max_dur());
    return_chromosome.set_prob_cross(one.get_prob_cross());
    return_chromosome.set_number_of_genes(one.get_size());
    auto seed = static_cast<int>(time(nullptr));
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> dist(0, 1);
    for (int i = 0; i < return_chromosome.get_size(); ++i) {
        if (dist(rng) <= return_chromosome.get_prob_cross()) {
            return_chromosome.get_genes().push_back(one.genes[i]);
        } else {
            return_chromosome.get_genes().push_back(two.genes[i]);
        }
    }
    return return_chromosome;
}

void Chromosome::display_genes() const {
    for (int i = 0; i < get_size(); ++i)
        std::cout << this->genes[i] << ' ';
    std::cout << std::endl;
}