//
// Created by kwh44 on 12/7/18.
//

#include "Schedule.h"
#include <iostream>
#include <algorithm>

static int _max(std::vector<int> &);

int Schedule::cost_function(Chromosome &one) {
    std::cout << "COST FUNCTION CALL.\n";
    std::cout << "The chromosome is: ";
    one.display_genes();
    construct_schedule(one);
    local_search();
    auto feedback_value = _max(array_of_finish_times);
    std::cout << "Feedback value is " << feedback_value << std::endl;
    array_of_finish_times.clear();
    array_of_scheduled_operations.clear();
    return feedback_value;
}

int _max(std::vector<int> &array) {
    int max_value = array[0];
    for (int i = 0; i < array.size(); ++i) {
        if (array[i] > max_value) {
            max_value = array[i];
        }
    }
    return max_value;
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
    std::cout << "Local search procedure started." << std::endl;
    int current_solution_updated = 0;
    auto cp_of_finish_time = array_of_finish_times;
    auto cp_of_scheduled_operations = array_of_scheduled_operations;

    // determine critical path
    auto critical_path = find_critical_path(cp_of_finish_time, cp_of_scheduled_operations);
    std::cout << "Critical path is: ";
    for (int i = 0; i < critical_path.size(); ++i) {
        std::cout << critical_path[i] << ' ';
    }
    std::cout << std::endl;

    // determine all critical blocks
    int start = 0, end = 0;

    while (start != critical_path.size() - 1) /* while (true) change to*/ {
        for (int i = start + 1; i < critical_path.size(); ++i) {
            int operation = cp_of_scheduled_operations[critical_path[i]];
            int operation_job = (operation - 1) / number_of_operations_in_one_job;
            int machine_required_by_operation = test_data[operation_job][((operation - 1) % number_of_jobs) * 2];

            int start_operation = cp_of_scheduled_operations[critical_path[start]];
            int start_job = (start_operation - 1) / number_of_operations_in_one_job;
            int machine_required_by_start_job = test_data[start_job][((start_operation - 1) % number_of_jobs) * 2];

            if (machine_required_by_operation == machine_required_by_start_job) ++end;
            else {
                if (start == end) {
                    ++start;
                    ++end;
                } else {
                    break;
                }
            }
        }
        if (start == critical_path.size() - 1) break;
        std::cout << "start = " << start << std::endl;
        std::cout << "end = " << end << std::endl;
        if (end - start + 1 == 2) {
            auto one_cp_of_finish_time = cp_of_finish_time;
            auto one_cp_of_scheduled_operations = cp_of_scheduled_operations;
            bool improved = evaluate_swap(one_cp_of_finish_time, one_cp_of_scheduled_operations, critical_path[end], critical_path[start]);
            if (improved) {
                ++current_solution_updated;
                cp_of_finish_time = one_cp_of_finish_time;
                cp_of_scheduled_operations = one_cp_of_scheduled_operations;
            }
        }
        if (end - start + 1 >= 4) {
            auto one_cp_of_finish_time = cp_of_finish_time;
            auto one_cp_of_scheduled_operations = cp_of_scheduled_operations;
            bool improved = evaluate_swap(one_cp_of_finish_time, one_cp_of_scheduled_operations,
                                          critical_path[start + 1], critical_path[start]);
            if (improved) {
                ++current_solution_updated;
                cp_of_finish_time = one_cp_of_finish_time;
                cp_of_scheduled_operations = one_cp_of_scheduled_operations;
            }
            one_cp_of_finish_time = cp_of_finish_time;
            one_cp_of_scheduled_operations = cp_of_scheduled_operations;
            improved = evaluate_swap(one_cp_of_finish_time, one_cp_of_scheduled_operations, critical_path[end],
                                     critical_path[end - 1]);
            if (improved) {
                ++current_solution_updated;
                cp_of_finish_time = one_cp_of_finish_time;
                cp_of_scheduled_operations = one_cp_of_scheduled_operations;
            }
        }
        start = end;
    }
    if (current_solution_updated) {
        array_of_finish_times = cp_of_finish_time;
        array_of_scheduled_operations = cp_of_scheduled_operations;
    }
}

bool Schedule::evaluate_swap(std::vector<int> &F, std::vector<int> &S, int start, int end) {
    std::cout << "Evaluate swap routine initiated" << std::endl;
    std::cout << "In here start is " << start << " end is " << end << std::endl;
    int old_makespan = 0;
    for (int i = 0; i < F.size(); ++i) {
        if (old_makespan < F[i]) old_makespan = F[i];
    }

    int start_operation = S[start];
    std::cout << "Start operation is " << start_operation << std::endl;
    int start_job = (start_operation - 1) / number_of_operations_in_one_job;
    int processing_time_of_start_operation = test_data[start_job][((start_operation - 1) % number_of_jobs) * 2 + 1];

    int end_operation = S[end]; // = 1
    std::cout << "End operation is " << end_operation << std::endl;
    int end_job = (end_operation - 1) / number_of_operations_in_one_job;
    int processing_time_of_end_operation = test_data[end_job][((end_operation - 1) % number_of_jobs) * 2 + 1];
    std::cout << "Processing time of end operation " << processing_time_of_end_operation << std::endl;
    std::swap(S[start], S[end]);
    std::cout << "F[start] = " << F[start] << "  F[end] = " << F[end] << std::endl;
    F[start] = F[start - 1] + processing_time_of_end_operation;
    F[end] = F[end - 1] + processing_time_of_start_operation;
    std::cout << "F[start] = " << F[start] << "  F[end] = " << F[end] << std::endl;
    for (int i = end + 1; i < S.size(); ++i) {
        std::cout << "Operation is " << S[i] << std::endl;
        std::cout << "old F[i] = " << F[i] << std::endl;

        int operation_number = S[i];
        int job = (operation_number - 1) / number_of_operations_in_one_job;
        int machine_required_by_current_operation = test_data[job][((operation_number - 1) % number_of_jobs) * 2];

        int time_in_line = 0;
        int operation;
        int operation_job;
        int machine_required_by_operation;
        for (int j = 1; j < i; ++j) {
            operation = S[j];
            operation_job = (operation - 1) / number_of_operations_in_one_job;
            machine_required_by_operation = test_data[operation_job][((operation - 1) % number_of_jobs) * 2];
            if (operation_job == job || machine_required_by_current_operation == machine_required_by_operation) {
                if (time_in_line < F[j]) time_in_line = F[j];
            }
        }
        F[i] = time_in_line + test_data[job][((operation_number - 1) % number_of_jobs) * 2 + 1];
        std::cout << "New F[i] is " << F[i] << std::endl;
    }
    int new_makespan = 0;
    for (int i = 0; i < F.size(); ++i) {
        if (new_makespan < F[i]) new_makespan = F[i];
    }
    std::cout << "Old makespan = " << old_makespan << " New makespan = " << new_makespan << std::endl;
    return old_makespan > new_makespan;
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

std::vector<int> Schedule::find_critical_path(std::vector<int> &F, std::vector<int> &S) {
    std::cout << "Routine of finding critical path initiated." << std::endl;
    // indexes of operations that comprise the critical path
    std::vector<int> critical_path;
    // find operation that completes the last
    int index_of_last_operation = 0;
    int finish_time = 0;

    for (int i = 1; i < S.size(); ++i) {
        if (F[i] > finish_time) {
            finish_time = F[i];
            index_of_last_operation = i;
        }
    }

    std::cout << "The index of last operation is " << index_of_last_operation << std::endl;
    std::cout << "The last operation is " << S[index_of_last_operation] << std::endl;

    int last_operation = S[index_of_last_operation];
    int job_of_last_operation = (last_operation - 1) / number_of_operations_in_one_job;

    critical_path.push_back(index_of_last_operation);

    finish_time -= test_data[job_of_last_operation][((last_operation - 1) % number_of_jobs) * 2 + 1];

    std::cout << "Finish time is " << finish_time << std::endl;

    while (finish_time > 0) {
        bool end_of_block;
        // searching on the same machine
        for (int i = 1; i < S.size(); ++i) {
            end_of_block = true;
            int operation = S[i];
            int operation_job = (operation - 1) / number_of_operations_in_one_job;
            int processing_time_of_operation = test_data[operation_job][((operation - 1) % number_of_jobs) * 2 + 1];
            int machine_required_by_operation = test_data[operation_job][((operation - 1) % number_of_jobs) * 2];

            last_operation = S[index_of_last_operation];
            job_of_last_operation = (last_operation - 1) / number_of_operations_in_one_job;
            int machine_required_by_last_operation = test_data[job_of_last_operation][
                    ((last_operation - 1) % number_of_jobs) * 2];

            std::cout << "Machine required by last operation is " << machine_required_by_last_operation << std::endl;
            if (i != index_of_last_operation && machine_required_by_last_operation == machine_required_by_operation &&
                F[i] == finish_time) {
                //if (S[index_of_last_operation] == 1 && S[i] == 4){
                //    std::cout << "Operation machine " << machine_required_by_operation << std::endl;
                //    std::cout << "Last operation machine " << machine_required_by_last_operation << std::endl;
                //    std::cout << "Finish time of operation " << F[i] << std::endl;
                //    std::cout << "Global finish time is " << finish_time << std::endl;
                //    std::cout << "i = " << i << " index_of_last_operation = " << index_of_last_operation << std::endl;
                //}
                index_of_last_operation = i;
                finish_time -= processing_time_of_operation;
                critical_path.push_back(i);
                end_of_block = false;
                //std::cout << "New finish time " << finish_time << std::endl;
                //exit(1);
                break;
            }
        }
        if (!end_of_block) continue;
        std::cout << "Didn't find any more critical operation on the same machine" << std::endl;

        // switching to another machine
        for (int j = 1; j < S.size(); ++j) {
            int operation = S[j];
            int operation_job = (operation - 1) / number_of_operations_in_one_job;
            int processing_time_of_operation = test_data[operation_job][((operation - 1) % number_of_jobs) * 2 + 1];
            last_operation = S[index_of_last_operation];
            job_of_last_operation = (last_operation - 1) / number_of_operations_in_one_job;
            if (operation_job == job_of_last_operation && F[j] == finish_time) {
                index_of_last_operation = j;
                finish_time -= processing_time_of_operation;
                critical_path.push_back(j);
                std::cout << "The next critical operation is " << S[j] << std::endl;
                std::cout << "Finish time now is " << finish_time << std::endl;
                break;
            }
        }
        // if (finish_time < 4) exit(1);
    }
    std::cout << "Routine of finding critical path completed." << std::endl;
    return critical_path;
}
