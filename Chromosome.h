//
// Created by kwh44 on 12/7/18.
//

#ifndef HYBRID_ALGORITHM_CHROMOSOME_H
#define HYBRID_ALGORITHM_CHROMOSOME_H

#include <vector>
#include <random>
#include <algorithm>

class Chromosome {
    int number_of_genes;
    int max_dur;
    double probability_of_crossing = 0;
    std::vector<double> genes;
    static std::mt19937_64 gen;
public:
    Chromosome() {
        number_of_genes = 0;
        max_dur = 0;
        probability_of_crossing = 0;
        genes.reserve(number_of_genes);
    }

    Chromosome(int, int, double);

    Chromosome(const Chromosome &);

    Chromosome(Chromosome &);

    Chromosome(Chromosome &&other) noexcept;

    Chromosome &operator=(Chromosome &&) noexcept;

    Chromosome &operator=(const Chromosome &other);

    ~Chromosome() { genes.clear(); }

    static Chromosome cross(Chromosome &, Chromosome &);

    int get_size() const { return number_of_genes; }

    std::vector<double> &get_genes() { return this->genes; }

    int get_max_dur() const { return max_dur; }

    double get_prob_cross() const { return this->probability_of_crossing; }

    void set_number_of_genes(int value) { number_of_genes = value; }

    void set_max_dur(int value) { this->max_dur = value; }

    void set_prob_cross(double value) { this->probability_of_crossing = value; }

    void display_genes() const;
};

#endif //HYBRID_ALGORITHM_CHROMOSOME_H
