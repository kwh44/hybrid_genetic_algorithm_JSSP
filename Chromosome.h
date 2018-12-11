//
// Created by kwh44 on 12/7/18.
//

#ifndef HYBRID_ALGORITHM_CHROMOSOME_H
#define HYBRID_ALGORITHM_CHROMOSOME_H

#include <vector>

class Chromosome {
private:
    int number_of_genes = 0;
    int max_dur = 0;
    double probability_of_crossing = 0.0;
public:

    std::vector<double> genes;

    Chromosome() = default;

    Chromosome(int, int, int, double);

    Chromosome(const Chromosome &);

    static Chromosome cross(Chromosome &, Chromosome &);

    int get_size() const { return number_of_genes; }

    int get_max_dur() const { return max_dur; }

    double get_prob_cross() const { return probability_of_crossing; }

    void set_prob_cross(double value) { this->probability_of_crossing = value; }

    void set_num_of_genes(int value) { this->number_of_genes = value; }

    void display_genes();
};


#endif //HYBRID_ALGORITHM_CHROMOSOME_H
