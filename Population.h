//
// Created by kwh44 on 12/7/18.
//

#ifndef HYBRID_ALGORITHM_POPULATION_H
#define HYBRID_ALGORITHM_POPULATION_H

#include "Chromosome.h"
#include "Schedule.h"

class Population {
    std::vector<Chromosome> population_array;
    size_t population_size = 0;
    Schedule _schedule;
    // random number engine based on Mersenne Twister algorithm
    static std::mt19937_64 gen;
public:
    Population() = delete;

    Population(const Population &);

    std::vector<Chromosome> &population() { return population_array; }

    size_t size() const { return population_size; }

    explicit Population(size_t size, double prob_cross, std::vector<std::vector<int> > &data);

    Population &operator=(Population &&) noexcept;

    Schedule &get_schedule() { return _schedule; }

    void new_generation();

    int solution(bool);

private:
    void sort();

#ifdef DDEBUG
    void display_population();
#endif
};

#endif //HYBRID_ALGORITHM_POPULATION_H
