//
// Created by kwh44 on 12/7/18.
//

#ifndef HYBRID_ALGORITHM_SCHEDULE_H
#define HYBRID_ALGORITHM_SCHEDULE_H

#include <nlohmann/json.hpp>
#include <string>
#include "Chromosome.h"

using json = nlohmann::json;

class Schedule {
private:
    size_t number_of_jobs;
    std::vector<std::vector<int>> test_data;
    std::vector<int> array_of_finish_times;
    std::vector<int> array_of_scheduled_operations;

public:

    Schedule() = default;

    explicit Schedule(std::vector<std::vector<int> > &data_set) : test_data{data_set},
                                                                  number_of_jobs{data_set.size()} {}

    std::vector<std::vector<int> > &data() { return this->test_data; }

    int cost_function(Chromosome &one);

private:
    void construct_schedule(Chromosome &chromo);

    void local_search();
};


#endif //HYBRID_ALGORITHM_SCHEDULE_H
