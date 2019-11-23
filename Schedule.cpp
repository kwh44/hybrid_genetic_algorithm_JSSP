//
// Created by kwh44 on 12/7/18.
//

#include "Schedule.h"
#include <algorithm>


Schedule::Schedule(std::vector<std::vector<int> > &data_set) {
    number_of_jobs = data_set.size();
    number_of_operations_in_one_job = data_set[0].size() / 2;
    int operation_counter = 0;
    for (size_t i = 0; i < data_set.size(); ++i) /**/ {
        for (size_t j = 0; j < data_set[0].size(); j += 2) {
            // operation_number, job_number, processing_time, machine
            operations.emplace_back(
                    Operation(operation_counter, i, data_set[i][j + 1], data_set[i][j]));
            ++operation_counter;
        }
    }
}

static int _max(std::vector<int> &);

int Schedule::cost_function(Chromosome &one, bool show) {
    construct_schedule(one);
    local_search(one);
    auto feedback_value = _max(array_of_finish_times);
    if (!show) {
        array_of_finish_times.clear();
        array_of_scheduled_operations.clear();
    }
    return feedback_value;
}

int _max(std::vector<int> &array) {
    int max_value = array[0];
    for (const auto &v: array) {
        if (v > max_value) {
            max_value = v;
        }
    }
    return max_value;
}


void Schedule::construct_schedule(Chromosome &chromo) {
    int g = 0, t = 0;
    std::vector<int> &F = array_of_finish_times;
    std::vector<int> &S = array_of_scheduled_operations;
    std::vector<int> E;
    int status = 1;
    while (S.size() < number_of_jobs * number_of_operations_in_one_job) {
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
            t = Schedule::time_of_g_iteration(F, t);
            update_E(E, S, F, t, g, chromo);
        }
    }
}

void Schedule::local_search(Chromosome &one) {
    bool current_solution_updated;
    do {
        current_solution_updated = false;
        // determine critical path
        auto critical_path = find_critical_path(array_of_finish_times, array_of_scheduled_operations);
        if (critical_path.empty()) return;
        // determine all critical blocks
        auto critical_blocks = get_critical_blocks(critical_path);
        size_t i = 0;
        while (i < critical_blocks.size() and not current_solution_updated) {
            if (critical_path[critical_blocks[i]] != critical_path[0] and not current_solution_updated) {
                // swap first two operations of block
                std::vector<int> cp_finish_time = array_of_finish_times;
                std::vector<int> cp_scheduled_operations = array_of_scheduled_operations;
                if (evaluate_swap(one, cp_finish_time, cp_scheduled_operations, critical_path[critical_blocks[i]],
                                  critical_path[critical_blocks[i] + 1])) {
                    array_of_finish_times = cp_finish_time;
                    array_of_scheduled_operations = cp_scheduled_operations;
                    current_solution_updated = true;
                }
            }
            if (critical_blocks[i] != critical_path[critical_path.size() - 1] and not current_solution_updated) {
                // swap last two operations of block
                std::vector<int> cp_finish_time = array_of_finish_times;
                std::vector<int> cp_scheduled_operations = array_of_scheduled_operations;
                if (evaluate_swap(one, cp_finish_time, cp_scheduled_operations,
                                  critical_path[critical_blocks[i + 1] - 1],
                                  critical_path[critical_blocks[i + 1]])) {
                    array_of_finish_times = cp_finish_time;
                    array_of_scheduled_operations = cp_scheduled_operations;
                    current_solution_updated = true;
                }
            }
            i += 2;
        }
    } while (current_solution_updated);
}

bool Schedule::evaluate_swap(Chromosome &one, std::vector<int> &F, std::vector<int> &S, int start, int end) {
    int old_make_span = 0, new_make_span = 0;
    for (const auto &v: F) if (old_make_span < v) old_make_span = v;
    F[start] = F[start] - operations[S[start]].get_processing_time() + operations[S[end]].get_processing_time();
    F[end] = F[start] + operations[S[start]].get_processing_time();
    std::swap(S[start], S[end]);
    for (size_t i = start + 1; i < S.size(); ++i) _new_ef(S, F, i, one);
    for (const auto &v : F) if (new_make_span < v) new_make_span = v;
    return old_make_span > new_make_span;
}

void Schedule::_new_ef(std::vector<int> &S, std::vector<int> &F, int i, Chromosome &one) {
    int time_in_line = 0;
    for (size_t j = 0; j < S.size(); ++j) {
        if (i == j) continue;
        if ((operations[S[i]].get_job_number() == operations[S[j]].get_job_number() &&
             one.get_genes()[operations[S[i]].get_operation_number()] >
             one.get_genes()[operations[S[j]].get_operation_number()]) ||
            (operations[S[i]].get_machine() == operations[S[j]].get_machine() && j < i)) {
            if (time_in_line < F[j]) time_in_line = F[j];
        }
    }
    F[i] = time_in_line + operations[S[i]].get_processing_time();
}

void
Schedule::update_E(std::vector<int> &E, std::vector<int> &S, std::vector<int> &F, int t, int g, Chromosome &chromo) {
    double delay_of_g_th_iteration = chromo.get_genes()[number_of_operations_in_one_job * number_of_jobs + g];
    for (size_t i = 0; i < operations.size(); ++i) { // traversing each operation
        bool status = false;
        for (const auto &q: S) { // traversing already scheduled operations
            // if operation was already scheduled in previous iterations
            if (q == i) {
                status = true;
                break;
            }
        }
        if (status) continue;
        double priority_of_current_operation = chromo.get_genes()[operations[i].get_operation_number()];
        std::vector<int> predecessor_index_list;
        for (size_t r = 0; r < operations.size(); ++r) {
            if (r == i || operations[r].get_job_number() != operations[i].get_job_number()) continue;
            double priority_of_r_th_operation = chromo.get_genes()[operations[r].get_operation_number()];
            if (priority_of_current_operation > priority_of_r_th_operation) predecessor_index_list.emplace_back(r);
        }
        if (predecessor_index_list.empty()) E.emplace_back(i);
        else {
            status = true;
            for (const auto &v :predecessor_index_list) {
                int index = -1, execution_time_of_zth_procedure;
                for (size_t y = 0; y < S.size(); ++y) {
                    // S contains indexes of operations in operations vector
                    if (S[y] == v) {
                        index = y;
                        break;
                    }
                }
                if (index != -1) execution_time_of_zth_procedure = F[index];
                else {
                    status = false;
                    break;
                }
                if (execution_time_of_zth_procedure > t + delay_of_g_th_iteration) {
                    status = false;
                    break;
                }
            }
            if (status) E.emplace_back(i);
        }
    }
}

int Schedule::get_highest_priority_operation(std::vector<int> &array, Chromosome &chromo) {
    int index = -1;
    double highest = -1;
    for (const auto &v: array) {
        if (chromo.get_genes()[operations[v].get_operation_number()] > highest) {
            highest = chromo.get_genes()[operations[v].get_operation_number()];
            index = v;
        }
    }
    return index;
}

int Schedule::precedence_capacity_earliest_finish_time(int index, std::vector<int> &S, std::vector<int> &F,
                                                       Chromosome &chromo) {
    int time_in_line = 0;
    for (size_t i = 0; i < operations.size(); ++i) {
        if (i == index) continue;
        if (operations[i].get_job_number() != operations[index].get_job_number()) continue;
        if (chromo.get_genes()[operations[i].get_operation_number()] >
            chromo.get_genes()[operations[index].get_operation_number()])
            continue;
        int finish_time = -1;
        for (size_t j = 0; j < S.size(); ++j) {
            if (S[j] == i) {
                finish_time = F[j];
            }
        }
        if (time_in_line < finish_time) time_in_line = finish_time;
    }
    for (size_t i = 0; i < S.size(); ++i) {
        if (operations[S[i]].get_machine() == operations[index].get_machine()) {
            if (time_in_line < F[i]) time_in_line = F[i];
        }
    }
    return time_in_line + operations[index].get_processing_time();
}

int Schedule::time_of_g_iteration(std::vector<int> &F, int t) {
    int minimal_time = F[1];
    for (size_t i = 2; i < F.size(); ++i) {
        if (minimal_time <= t && F[i] > t) minimal_time = F[i];
    }
    return minimal_time;
}


std::vector<int> Schedule::find_critical_path(std::vector<int> &F, std::vector<int> &S) {
    std::vector<int> critical_path;
    size_t index_of_last_operation = 0, finish_time = 0;
    for (size_t i = 0; i < S.size(); ++i) {
        if (F[i] > finish_time) {
            finish_time = F[i];
            index_of_last_operation = i;
        }
    }
    critical_path.push_back(index_of_last_operation);
    finish_time -= operations[S[index_of_last_operation]].get_processing_time();
    while (finish_time > 0) {
        bool end_of_block = true;
        // searching on the same machine
        for (size_t i = 0; i < S.size(); ++i) {
            if (S[i] == S[index_of_last_operation]) continue;
            if (operations[S[i]].get_machine() == operations[S[index_of_last_operation]].get_machine() &&
                F[i] == finish_time) {
                index_of_last_operation = i;
                finish_time -= operations[S[index_of_last_operation]].get_processing_time();
                critical_path.push_back(index_of_last_operation);
                end_of_block = false;
                break;
            }
        }
        if (!end_of_block) continue;
        // switching to another machine
        int status = true;
        for (size_t j = 0; j < S.size(); ++j) {
            if (S[j] == S[index_of_last_operation]) continue;
            if (operations[S[j]].get_job_number() == operations[S[index_of_last_operation]].get_job_number() &&
                F[j] == finish_time) {
                index_of_last_operation = j;
                finish_time -= operations[S[index_of_last_operation]].get_processing_time();
                critical_path.push_back(index_of_last_operation);
                status = false;
                break;
            }
        }
        if (status) {
            critical_path.clear();
            return critical_path;
        }
    }
    for (size_t i = 0; i < critical_path.size() / 2; ++i) {
        auto temp = critical_path[i];
        critical_path[i] = critical_path[critical_path.size() - i - 1];
        critical_path[critical_path.size() - i - 1] = temp;
    }
    return critical_path;
}


std::vector<int> Schedule::get_critical_blocks(std::vector<int> &critical_path) {
    std::vector<int> blocks;
    int start_of_block = 0, end_of_block = 0;
    while (end_of_block < critical_path.size() - 1) {
        for (size_t i = end_of_block + 1; i < critical_path.size(); ++i) {
            if (operations[array_of_scheduled_operations[critical_path[start_of_block]]].get_machine() ==
                operations[array_of_scheduled_operations[critical_path[i]]].get_machine()) {
                ++end_of_block;
            } else {
                if (start_of_block == end_of_block) {
                    ++start_of_block;
                    ++end_of_block;
                } else {
                    blocks.push_back(start_of_block);
                    blocks.push_back(end_of_block);
                    ++end_of_block;
                    start_of_block = end_of_block;
                }
            }
        }
    }
    return blocks;
}