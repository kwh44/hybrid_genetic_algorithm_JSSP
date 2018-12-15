//
// Created by kwh44 on 12/7/18.
//

#ifndef HYBRID_ALGORITHM_POPULATION_H
#define HYBRID_ALGORITHM_POPULATION_H

#include "Chromosome.h"
#include <vector>
#include "Schedule.h"

class Population {
    std::vector<Chromosome> population_array;
    int population_size = 0;
    Schedule _schedule;

public:
    Population() = delete;

    Population(const Population &);

    std::vector<Chromosome> &population() { return population_array; }

    int size() const { return population_size; }

    explicit Population(size_t size, double prob_cross, std::vector<std::vector<int> > &data);

    Population &operator=(Population &&) noexcept;

    ~Population() {
        _schedule.~Schedule();
        for (size_t i = 0; i < population_array.size(); ++i) {
            population_array[i].~Chromosome();
        }
    }

    void new_generation();

    int solution();

    void display_population();

private:
    void sort();
};


#endif //HYBRID_ALGORITHM_POPULATION_H
