//
// Created by kwh44 on 12/7/18.
//

#include "Chromosome.h"
#include <iostream>

std::mt19937_64 Chromosome::gen = std::mt19937_64(20);

Chromosome::Chromosome(size_t number_of_operations, int MaxDur, double prob_cross) {
    number_of_genes = number_of_operations * 2;
    max_dur = MaxDur;
    probability_of_crossing = prob_cross;
    std::uniform_real_distribution<double> dist(0, 1);
    for (size_t i = 0; i < number_of_operations; ++i) genes.emplace_back(dist(Chromosome::gen));
    for (size_t i = 0; i < number_of_operations; ++i) genes.emplace_back(genes[i] * 1.5 * max_dur);
}


Chromosome::Chromosome(Chromosome &other) {
    number_of_genes = other.get_size();
    max_dur = other.get_max_dur();
    probability_of_crossing = other.get_prob_cross();
    for (const auto &v: other.genes) genes.push_back(v);
}

Chromosome::Chromosome(const Chromosome &other) {
    number_of_genes = other.number_of_genes;
    max_dur = other.max_dur;
    probability_of_crossing = other.probability_of_crossing;
    for (const auto &v: other.genes) genes.emplace_back(v);
}


Chromosome::Chromosome(Chromosome &&other) noexcept {
    number_of_genes = other.number_of_genes;
    max_dur = other.max_dur;
    probability_of_crossing = other.probability_of_crossing;
    for (const auto &v: other.genes) genes.emplace_back(v);
}

Chromosome &Chromosome::operator=(const Chromosome &other) {
    number_of_genes = other.number_of_genes;
    max_dur = other.max_dur;
    probability_of_crossing = other.probability_of_crossing;
    genes.clear();
    for (const auto &v: other.genes) genes.emplace_back(v);
    return *this;
}

Chromosome &Chromosome::operator=(Chromosome &&other) noexcept {
    number_of_genes = other.number_of_genes;
    max_dur = other.max_dur;
    probability_of_crossing = other.probability_of_crossing;
    genes.clear();
    for (const auto &v: other.genes) genes.emplace_back(v);
    return *this;
}

Chromosome Chromosome::cross(Chromosome &one, Chromosome &two) {
    auto return_chromosome = Chromosome();
    return_chromosome.set_max_dur(one.get_max_dur());
    return_chromosome.set_prob_cross(one.get_prob_cross());
    return_chromosome.set_number_of_genes(one.get_size());
    std::uniform_real_distribution<double> dist(0, 1);
    for (size_t i = 0; i < return_chromosome.get_size(); ++i) {
        if (dist(Chromosome::gen) <= return_chromosome.get_prob_cross()) {
            return_chromosome.get_genes().emplace_back(one.genes[i]);
        } else return_chromosome.get_genes().emplace_back(two.genes[i]);
    }
    return return_chromosome;
}
