//
// Created by kwh44 on 12/7/18.
//

#include "Schedule.h"
#include <iostream>
#include <algorithm>


Schedule::Schedule() {
    number_of_jobs = 0;
    number_of_operations_in_one_job = 0;
    test_data = std::vector<std::vector<int>>{{0}};

}

Schedule &Schedule::operator=(const Schedule &other) {
    number_of_jobs = other.number_of_jobs;
    number_of_operations_in_one_job = other.number_of_jobs;
    test_data = other.test_data;
    return *this;
}


Schedule &Schedule::operator=(Schedule &&other) noexcept {
    number_of_jobs = other.number_of_jobs;
    number_of_operations_in_one_job = other.number_of_jobs;
    test_data = other.test_data;
    return *this;
}

static int _max(std::vector<int> &);

int Schedule::cost_function(Chromosome &one) {
    construct_schedule(one);
    local_search();
    auto feedback_value = _max(array_of_finish_times);
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


void Schedule::construct_schedule(Chromosome &chromo) {
    int g = 0;
    int t = 0;
    std::vector<int> &F = array_of_finish_times;
    F.push_back(0);
    std::vector<int> &S = array_of_scheduled_operations;
    S.push_back(0);
    std::vector<int> E;
    int status = 1;
    while (S.size() < number_of_jobs * number_of_operations_in_one_job + 1) {
        if (status) {
            E.clear();
            update_E(E, S, F, t, g, chromo);
        }
        if (!E.empty()) {
            // select operation with highest priority
            int j = get_highest_priority_operation(E, chromo);
            // calculate the earliest finish time (in terms of precedence and capacity)
            int FC = precedence_capacity_earliest_finish_time(j, S, F, chromo);
            // Update S
            S.push_back(j);
            // Update F
            F.push_back(FC);
            // Iteration increment
            ++g;
            // Determine the time associated with iteration g while E.empty()
            status = 1;
        } else status = 0;
        while (E.empty()) {
            t = time_of_g_iteration(F, t);
            update_E(E, S, F, t, g, chromo);
        }
    }
}

void Schedule::local_search() {
    int current_solution_updated = 0;
    std::vector<int> cp_of_finish_time = array_of_finish_times;
    std::vector<int> cp_of_scheduled_operations = array_of_scheduled_operations;
    // determine critical path
    auto critical_path = find_critical_path(cp_of_finish_time, cp_of_scheduled_operations);
    // determine all critical blocks
    int start = 0, end = 0;
    while (start != critical_path.size() - 1) /* */ {
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
        if (end - start + 1 == 2) {
            auto one_cp_of_finish_time = cp_of_finish_time;
            auto one_cp_of_scheduled_operations = cp_of_scheduled_operations;
            bool improved = evaluate_swap(one_cp_of_finish_time, one_cp_of_scheduled_operations, critical_path[end],
                                          critical_path[start]);
            if (improved) {
                ++current_solution_updated;
                cp_of_finish_time = one_cp_of_finish_time;
                cp_of_scheduled_operations = one_cp_of_scheduled_operations;
            }
        }
        if (end - start + 1 >= 3) {
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
    int old_makespan = 0, new_makespan = 0;
    for (int i = 0; i < F.size(); ++i) {
        if (old_makespan < F[i]) old_makespan = F[i];
    }
    int start_operation = S[start], start_job = (start_operation - 1) / number_of_operations_in_one_job;
    int end_operation = S[end], end_job = (end_operation - 1) / number_of_operations_in_one_job;
    std::swap(S[start], S[end]);
    _new_ef(S, F, start);
    _new_ef(S, F, end);
    for (int i = end + 1; i < S.size(); ++i) {
        int operation_number = S[i];
        int job = (operation_number - 1) / number_of_operations_in_one_job;
        int machine_required_by_current_operation = test_data[job][((operation_number - 1) % number_of_jobs) * 2];
        int time_in_line = 0, operation, operation_job, machine_required_by_operation;
        for (int j = 1; j < i; ++j) {
            operation = S[j];
            operation_job = (operation - 1) / number_of_operations_in_one_job;
            machine_required_by_operation = test_data[operation_job][((operation - 1) % number_of_jobs) * 2];
            if (operation_job == job || machine_required_by_current_operation == machine_required_by_operation) {
                if (time_in_line < F[j]) time_in_line = F[j];
            }
        }
        F[i] = time_in_line + test_data[job][((operation_number - 1) % number_of_jobs) * 2 + 1];

    }
    for (int i = 0; i < F.size(); ++i) {
        if (new_makespan < F[i]) new_makespan = F[i];
    }
    return old_makespan > new_makespan;
}

void Schedule::_new_ef(std::vector<int> &S, std::vector<int> &F, int i) {
    int operation_number = S[i];
    int job = (operation_number - 1) / number_of_operations_in_one_job;
    int machine_required_by_current_operation = test_data[job][((operation_number - 1) % number_of_jobs) * 2];
    int time_in_line = 0, operation, operation_job, machine_required_by_operation;
    for (int j = 1; j < i; ++j) {
        operation = S[j];
        operation_job = (operation - 1) / number_of_operations_in_one_job;
        machine_required_by_operation = test_data[operation_job][((operation - 1) % number_of_jobs) * 2];
        if (operation_job == job || machine_required_by_current_operation == machine_required_by_operation) {
            if (time_in_line < F[j]) time_in_line = F[j];
        }
    }
    F[i] = time_in_line + test_data[job][((operation_number - 1) % number_of_jobs) * 2 + 1];
}


void
Schedule::update_E(std::vector<int> &E, std::vector<int> &S, std::vector<int> &F, int t, int g, Chromosome &chromo) {
    double delay_of_g_th_iteration = chromo.get_genes()[number_of_operations_in_one_job * number_of_jobs + g];

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
            double priority_of_current_operation = chromo.get_genes()[i * number_of_operations_in_one_job + j];
            std::vector<int> predecessor_index_list;
            for (int r = 0; r < number_of_operations_in_one_job; ++r) {
                double priority_of_r_th_operation = chromo.get_genes()[i * number_of_operations_in_one_job + r];
                if (priority_of_current_operation > priority_of_r_th_operation) {
                    predecessor_index_list.push_back(r);
                }
            }
            if (predecessor_index_list.size() == 0) /**/{
                E.push_back(i * number_of_operations_in_one_job + j + 1);
            } else {
                status = 1;
                for (int z = 0; z < predecessor_index_list.size(); ++z) {
                    int index = -1, execution_time_of_zth_procedure;
                    for (int y = 0; y < S.size(); ++y) {
                        if (S[y] == i * number_of_operations_in_one_job +
                                    predecessor_index_list[z] + 1) {
                            index = y;
                        }
                    }
                    if (index != -1) execution_time_of_zth_procedure = F[index];
                    else execution_time_of_zth_procedure = test_data[i][predecessor_index_list[z] * 2 + 1];
                    if (execution_time_of_zth_procedure > t + delay_of_g_th_iteration) {
                        status = 0;
                        break;
                    }
                }
                if (status) {
                    E.push_back(i * number_of_operations_in_one_job + j + 1);
                }
            }
        }
    }
}

int Schedule::get_highest_priority_operation(std::vector<int> &array, Chromosome &chromo) {
    int highest_priority_operation = array[0];
    double highest = chromo.get_genes()[array[0] - 1];
    for (int i = 1; i < array.size(); ++i) {
        if (chromo.get_genes()[array[i] - 1] > highest) {
            highest = chromo.get_genes()[array[i] - 1];
            highest_priority_operation = array[i];
        }
    }
    return highest_priority_operation;
}

int Schedule::precedence_capacity_earliest_finish_time(int operation_number, std::vector<int> &S, std::vector<int> &F,
                                                       Chromosome &chromo) {
    int job = (operation_number - 1) / number_of_operations_in_one_job;
    int machine_required_by_current_operation = test_data[job][((operation_number - 1) % number_of_jobs) * 2];
    int time_in_line = 0, operation_job, machine_required_by_operation;
    for (int i = 1; i < S.size(); ++i) {
        int operation = S[i];
        // where operation is the index of operation priority in a chromosome
        operation_job = (operation - 1) / number_of_operations_in_one_job;
        machine_required_by_operation = test_data[operation_job][((operation - 1) % number_of_jobs) * 2];
        if (operation_job == job || machine_required_by_current_operation == machine_required_by_operation) {
            if (time_in_line < F[i]) time_in_line = F[i];
        }
    }
    return time_in_line + test_data[job][((operation_number - 1) % number_of_jobs) * 2 + 1];
}

int Schedule::time_of_g_iteration(std::vector<int> &F, int t) {
    int minimal_time = F[1];
    for (int i = 2; i < F.size(); ++i) {
        if (minimal_time <= t && F[i] > t) {
            minimal_time = F[i];
        }
    }
    return minimal_time;
}

std::vector<int> Schedule::find_critical_path(std::vector<int> &F, std::vector<int> &S) {
    std::vector<int> critical_path;
    // find operation that completes the last
    int index_of_last_operation = 0, finish_time = 0;
    for (int i = 1; i < S.size(); ++i) {
        if (F[i] > finish_time) {
            finish_time = F[i];
            index_of_last_operation = i;
        }
    }
    int last_operation = S[index_of_last_operation], job_of_last_operation =
            (last_operation - 1) / number_of_operations_in_one_job;
    critical_path.push_back(index_of_last_operation);
    finish_time -= test_data[job_of_last_operation][((last_operation - 1) % number_of_jobs) * 2 + 1];
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

            if (i != index_of_last_operation && machine_required_by_last_operation == machine_required_by_operation &&
                F[i] == finish_time) {
                index_of_last_operation = i;
                finish_time -= processing_time_of_operation;
                critical_path.push_back(i);
                end_of_block = false;
                break;
            }
        }
        if (!end_of_block) continue;
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
                break;
            }
        }
    }
    return critical_path;
}
