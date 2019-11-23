//
// Created by kwh44 on 12/7/18.
//

#ifndef HYBRID_ALGORITHM_CHROMOSOME_H
#define HYBRID_ALGORITHM_CHROMOSOME_H

#include <vector>
#include <random>
#include <algorithm>

class Chromosome {
    size_t number_of_genes;
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

    Chromosome(size_t, int, double);

    Chromosome(const Chromosome &);

    Chromosome(Chromosome &);

    Chromosome(Chromosome &&other) noexcept;

    Chromosome &operator=(Chromosome &&) noexcept;

    Chromosome &operator=(const Chromosome &other);

    static Chromosome cross(Chromosome &, Chromosome &);

    inline size_t get_size() const { return number_of_genes; }

    inline std::vector<double> &get_genes() { return genes; }

    inline  int get_max_dur() const { return max_dur; }

    inline double get_prob_cross() const { return probability_of_crossing; }

    inline void set_number_of_genes(int value) { number_of_genes = value; }

    inline  void set_max_dur(int value) { max_dur = value; }

    inline  void set_prob_cross(double value) { probability_of_crossing = value; }

};

#endif //HYBRID_ALGORITHM_CHROMOSOME_H
