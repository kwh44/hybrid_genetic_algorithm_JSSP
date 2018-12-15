//
// Created by kwh44 on 12/7/18.
//

#ifndef HYBRID_ALGORITHM_SCHEDULE_H
#define HYBRID_ALGORITHM_SCHEDULE_H

#include <string>
#include "Chromosome.h"

class Schedule {
private:
    size_t number_of_jobs;
    size_t number_of_operations_in_one_job;
    std::vector<std::vector<int>> test_data;
    std::vector<int> array_of_finish_times;
    std::vector<int> array_of_scheduled_operations;

public:

    Schedule();

    explicit Schedule(std::vector<std::vector<int> > &data_set) : test_data{data_set},
                                                                  number_of_jobs{data_set.size()},
                                                                  number_of_operations_in_one_job{
                                                                          data_set[0].size() / 2} {}

    Schedule &operator=(const Schedule &);

    Schedule &operator=(Schedule &&) noexcept;

    ~Schedule() {
        number_of_jobs = 0;
        number_of_operations_in_one_job = 0;
        array_of_finish_times.clear();
        array_of_scheduled_operations.clear();
        test_data.clear();
    }

    std::vector<std::vector<int> > &get_data() { return this->test_data; }

    int cost_function(Chromosome &);

private:
    void construct_schedule(Chromosome &);

    void update_E(std::vector<int> &, std::vector<int> &, std::vector<int> &, int, int, Chromosome &);

    int get_highest_priority_operation(std::vector<int> &, Chromosome &);

    int time_of_g_iteration(std::vector<int> &, int);

    void local_search();

    int precedence_capacity_earliest_finish_time(int, std::vector<int> &, std::vector<int> &, Chromosome &);

    std::vector<int> find_critical_path(std::vector<int> &, std::vector<int> &);

    bool evaluate_swap(std::vector<int> &, std::vector<int> &, int, int);

    void _new_ef(std::vector<int> &, std::vector<int> &, int);
};


#endif //HYBRID_ALGORITHM_SCHEDULE_H
