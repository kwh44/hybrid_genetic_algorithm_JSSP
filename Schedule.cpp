//
// Created by kwh44 on 12/7/18.
//

#include "Schedule.h"
#include <iostream>

int Schedule::cost_function(Chromosome &one) {
    std::cout << "COST FUNCTION CALL.\n";
    std::cout << "The chromosome is: ";
    one.display_genes();
    construct_schedule(one);
    auto feedback_value = array_of_finish_times[array_of_finish_times.size() - 1];
    std::cout << "Feedback value is " << feedback_value << std::endl;
    array_of_finish_times.clear();
    array_of_scheduled_operations.clear();
    return feedback_value;
}

static int get_highest_priority_operation(std::vector<int> &, Chromosome &);

static int time_of_g_iteration(std::vector<int> &, int);

void Schedule::construct_schedule(Chromosome &chromo) {
    std::cout << "Process of schedule construction initiated.\n";
    int g = 0;
    int t = 0;
    std::vector<int> &F = array_of_finish_times;
    F.push_back(0);
    std::vector<int> &S = array_of_scheduled_operations;
    S.push_back(0);
    std::vector<int> E;
    int status = 1;
    while (S.size() < number_of_jobs * number_of_operations_in_one_job + 1) {
        if (g == 4) return;
        if (status) {
            E.clear();
            update_E(E, S, t, g, chromo);
        }

        do {
            // select operation with highest priority
            // for (int index = 0; index < E.size(); ++index) {}
            int j = get_highest_priority_operation(E, chromo);
            std::cout << "The highest priority operation is " << j << std::endl;

            // calculate the earliest finish time (in terms of precedence only)
            int EF = precedence_earliest_finish_time(j, chromo, F, S);
            std::cout << "The earliest finish time (in terms of precedence only) of " << j << " operation is " << EF
                      << std::endl;

            // calculate the earliest finish time (in terms of precedence and capacity)
            int FC = precedence_capacity_earliest_finish_time(j, S, F, chromo);
            std::cout << "The earliest finish time (in terms of precedence and capacity) of " << j << " operation is "
                      << FC
                      << std::endl;
            // Update S
            S.push_back(j);
            std::cout << "The S set is : ";
            for (int i = 0; i < S.size(); ++i) std::cout << S[i] << ' ';
            std::cout << std::endl;
            // Update F

            F.push_back(FC);
            // Iteration increment
            std::cout << "The F set is : ";
            for (int i = 0; i < F.size(); ++i) std::cout << F[i] << ' ';
            std::cout << std::endl;
            ++g;
        } while (0);

        // Determine the time associated with iteration g while E.empty()
        if (E.empty()) status = 0;
        else status = 1;
        std::cout << "Status is " << status << std::endl;
        while (E.empty()) {
            t = time_of_g_iteration(F, t);   // check now time_of_g_iteration
            std::cout << "The updated time of g iteration is " << t << std::endl;
            update_E(E, S, t, g, chromo);
        }
    }
    std::cout << "Process of schedule construction completed.\n";
}

void Schedule::local_search() {

}


void Schedule::update_E(std::vector<int> &E, std::vector<int> &S, int t, int g, Chromosome &chromo) {
    std::cout << "Routine of updating set E initiated.\n";
    double delay_of_g_th_iteration = chromo.genes[number_of_operations_in_one_job * number_of_jobs + g];
    std::cout << "The delay of " << g << " iteration is " << delay_of_g_th_iteration << std::endl;
    std::cout << "test data size is " << test_data.size() << std::endl;
    std::cout << "The size of test_data[0].size() is " << test_data[0].size() << std::endl;
    std::cout << "The number of jobs is " << number_of_jobs << std::endl;
    std::cout << "The number of operations in one job is " << number_of_operations_in_one_job << std::endl;
    for (int i = 0; i < number_of_jobs; ++i) { // traversing each job
        for (int j = 0; j < number_of_operations_in_one_job; ++j) { // traversing each operation in job
            int status = 0;
            for (const auto &q: S) { // traversing already scheduled operations
                // if operation was already scheduled for previous iterations
                if (q == i * number_of_operations_in_one_job + j + 1) {
                    status = 1;
                    break;
                }
            }
            if (status) continue;

            double priority_of_current_operation = chromo.genes[i * number_of_operations_in_one_job + j];
            std::cout << "The priority of operation " << i * number_of_operations_in_one_job + j + 1 << " is "
                      << priority_of_current_operation << std::endl;
            std::vector<int> predecessor_index_list;
            for (int r = 0; r < number_of_operations_in_one_job; ++r) {
                status = 0;
                for (const auto &u: S) {
                    if (u == i * number_of_operations_in_one_job + r + 1 || r == j) {
                        status = 1;
                        break;
                    }
                }
                if (status) continue;

                double priority_of_r_th_operation = chromo.genes[i * number_of_operations_in_one_job + r];
                std::cout << "The priority of predecessor operation " << i * number_of_operations_in_one_job + r + 1
                          << " is " << priority_of_r_th_operation << std::endl;
                if (priority_of_current_operation > priority_of_r_th_operation) {
                    predecessor_index_list.push_back(r);
                }
            }
            std::cout << "Operation " << i * number_of_operations_in_one_job + j + 1 << " has "
                      << predecessor_index_list.size() << " predecessors\n";
            for (int z = 0; z < predecessor_index_list.size(); ++z) {
                status = 1;
                int execution_time_of_zth_procedure = test_data[i][z * 2 + 1];
                if (execution_time_of_zth_procedure > t + delay_of_g_th_iteration) {
                    status = 0;
                }

                if (status) {
                    E.push_back(i * number_of_operations_in_one_job + j + 1);
                }
            }
            if (predecessor_index_list.size() == 0) /**/{
                E.push_back(i * number_of_operations_in_one_job + j + 1);
            }
        }
    }

    std::cout << "The E is :" << std::endl;
    for (int i = 0; i < E.size(); ++i) {
        std::cout << E[i] << ' ';
    }
    std::cout << std::endl;
    std::cout << "Routine of updating set E completed.\n";
}

int get_highest_priority_operation(std::vector<int> &array, Chromosome &chromo) {
    std::cout << "Routine of getting highest priority operation initiated.\n";
    int highest_priority_operation = array[0];
    double highest = chromo.genes[array[0] - 1];
    for (int i = 0; i < array.size(); ++i) {
        if (chromo.genes[array[i] - 1] > highest) {
            highest = chromo.genes[array[i] - 1];
            highest_priority_operation = array[i];
        }
    }
    std::cout << "Routine of getting highest priority operation completed.\n";
    return highest_priority_operation;
}

int Schedule::precedence_earliest_finish_time(int operation_number, Chromosome &chromo, std::vector<int> &F,
                                              std::vector<int> &S) {
    std::cout << "Routine of getting precedence earliest finish time initiated.\n";

    int job = (operation_number - 1) / number_of_operations_in_one_job;
    std::cout << "The job number of " << operation_number << " operation is " << job << std::endl;

    int max_duration_of_predecessor_operation = 0;

    for (int i = 0; i < number_of_operations_in_one_job; ++i) {
        // if it is the same operation continue
        if (operation_number - 1 == job * number_of_operations_in_one_job + i) continue;
        std::cout << "the operation is " << job * number_of_operations_in_one_job + i << std::endl;
        // if i-th operation has lower priority
        if (chromo.genes[operation_number - 1] <= chromo.genes[job * number_of_operations_in_one_job + i]) continue;
        std::cout << "the predecessor operation is " << job * number_of_operations_in_one_job + i << std::endl;
        int predecessor_finish_index = 0;
        for (int j = 0; j < S.size(); ++j)
            if (S[j] == job * number_of_operations_in_one_job + i + 1) predecessor_finish_index = j;
        int execution_time_of_ith_procedure = F[predecessor_finish_index];
        if (max_duration_of_predecessor_operation < execution_time_of_ith_procedure) {
            max_duration_of_predecessor_operation = execution_time_of_ith_procedure;
        }
    }

    std::cout << "Routine of getting precedence earliest finish time completed.\n";
    return max_duration_of_predecessor_operation + test_data[job][((operation_number - 1) % number_of_jobs) * 2 + 1];
}


int Schedule::precedence_capacity_earliest_finish_time(int operation_number, std::vector<int> &S, std::vector<int> &F,
                                                       Chromosome &chromo) {
    std::cout << "Routine of getting precedence capacity earliest finish time initiated.\n";
    int job = (operation_number - 1) / number_of_operations_in_one_job;
    int machine_required_by_current_operation = test_data[job][((operation_number - 1) % number_of_jobs) * 2];
    std::cout << "Machine required by " << operation_number << " operation is " << machine_required_by_current_operation
              << std::endl;
    int time_in_line = 0;
    int operation_job;
    int machine_required_by_operation;
    for (int i = 0; i < S.size(); ++i) {
        int operation = S[i];
        if (operation == 0) continue; // 0 operation is dumb
        // where operation is the number of operation in the chromosome

        operation_job = (operation - 1) / number_of_operations_in_one_job;

        machine_required_by_operation = test_data[operation_job][((operation - 1) % number_of_jobs) * 2];

        if (operation_job == job || machine_required_by_current_operation == machine_required_by_operation) {
            if (time_in_line < F[i]) time_in_line = F[i];
        }
    }
    std::cout << "Routine of getting precedence capacity earliest finish time completed.\n";
    return time_in_line + test_data[job][((operation_number - 1) % number_of_jobs) * 2 + 1];
}

int _min(std::vector<int> &);

int time_of_g_iteration(std::vector<int> &F, int t) {
    std::cout << "Routine of getting time of g iteration initiated.\n";
    int minimal_time = 1000000;
    while (minimal_time <= t) {
        for (int i = 1; i < F.size(); ++i) {
            if (minimal_time > F[i] && F[i] > t) {
                minimal_time = F[i];
            }
        }
    }
    std::cout << "Routine of getting time of g iteration completed.\n";
    return minimal_time;
}



