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

    Chromosome(const Chromosome & other);

    static Chromosome cross(Chromosome &one, Chromosome &two);

    int get_size() const { return number_of_genes; }

    int get_max_dur() const { return max_dur; }

    double get_prob_cross() const { return probability_of_crossing; }

};


#endif //HYBRID_ALGORITHM_CHROMOSOME_H
