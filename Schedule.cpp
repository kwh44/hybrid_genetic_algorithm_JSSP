//
// Created by kwh44 on 12/7/18.
//

#include "Schedule.h"

int Schedule::cost_function(Chromosome &one) {
    return 1;
}

static int get_highest_priority_operation(std::vector<int> &, Chromosome &);

void Schedule::construct_schedule(Chromosome &chromo) {
    int g = 0;
    int t = 0;
    std::vector<int> F = {0};
    std::vector<int> S = {0};
    std::vector<int> E;
    while (S.size() < number_of_jobs + 1) {
        update_E(E, S, t, g, chromo);
        while (!E.empty()) {
            // select operation with highest priority
            int j = get_highest_priority_operation(E, chromo);
            // calculate the earliest finish time (in terms of precedence only)
            int EF = precedence_earliest_finish_time(j, chromo);
            // calculate the earliest finish time (in terms of precedence and capacity)
            int FC;
            // Update S
            S.push_back(j);
            // Update F
            F.push_back(FC);
            // Iteration increment
            ++g;
        }
        // Determine the time associated with iteration g while E.empty() returns True



    }
}

void Schedule::local_search() {

}


void Schedule::update_E(std::vector<int> &E, std::vector<int> &S, int t, int g, Chromosome &chromo) {
    double delay_of_g_th_iteration = chromo.genes[number_of_operations_in_one_job * number_of_jobs + g];
    for (int i = 0; i < test_data.size(); ++i) {
        for (int j = 0; j < test_data[i].size(); ++j) {
            int status = 0;
            for (const auto &q: S) {
                if (q == test_data[i][j]) {
                    status = 1;
                    break;
                }
            }
            if (status) continue;
            // data_set[i][j] is the j-th operation of the i-th job
            double priority_of_current_operation = chromo.genes[i * number_of_operations_in_one_job + j];

            std::vector<int> predecessor_index_list;
            for (int r = 0; r < test_data[i].size(); ++r) {
                status = 0;
                for (const auto &u: S) {
                    if (u == test_data[i][r] || test_data[i][r] == test_data[i][j]) {
                        status = 1;
                        break;
                    }
                }
                if (status) continue;

                double priority_of_r_th_operation = chromo.genes[i * number_of_operations_in_one_job + r];
                if (priority_of_current_operation > priority_of_r_th_operation) {
                    predecessor_index_list.push_back(r);
                }
            }
            status = 1;
            for (int z = 0; z < predecessor_index_list.size(); ++z) {
                int execution_time_of_zth_procedure = test_data[i][z * 2 + 1];
                if (execution_time_of_zth_procedure > t + delay_of_g_th_iteration) {
                    status = 0;
                    break;
                }
            }
            if (status) {
                E.push_back(i * number_of_operations_in_one_job + j);
            }
        }
    }
}

int get_highest_priority_operation(std::vector<int> &array, Chromosome &chromo) {
    int highest = array[0];
    for (const auto &v: array) {
        if (chromo.genes[v] > highest) {
            highest = v;
        }
    }
    return highest;
}

int Schedule::precedence_earliest_finish_time(int operation_number, Chromosome &chromo) {
    int job = operation_number / number_of_operations_in_one_job;
    int max_duration_of_predecessor_operation;
    for (int i = 0; i < test_data[job].size(); ++i) {
        if (i == (operation_number % number_of_jobs)) continue;
        int execution_time_of_ith_procedure = test_data[job][i * 2 + 1];
        if (max_duration_of_predecessor_operation < execution_time_of_ith_procedure) {
            max_duration_of_predecessor_operation = execution_time_of_ith_procedure;
        }
    }
    return max_duration_of_predecessor_operation + test_data[job][(operation_number % number_of_jobs) * 2 + 1];
}
