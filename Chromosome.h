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

    Chromosome(int size, int MaxDur, double prob_cross);

    static Chromosome cross(Chromosome &one, Chromosome &two);

    static int cost_function(Chromosome &item);

    double get_gene(size_t index) const { return genes[index]; }

    int get_size() const { return number_of_genes; }

    int get_max_dur() const { return max_dur; }

    double get_prob_cross() const { return probability_of_crossing; }

    void set_gene(int index, double value) { genes[index] = value; }
};


#endif //HYBRID_ALGORITHM_CHROMOSOME_H
