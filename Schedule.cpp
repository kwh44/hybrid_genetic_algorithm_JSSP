//
// Created by kwh44 on 12/7/18.
//

#include "Schedule.h"

int Schedule::cost_function(Chromosome &one) {
    return 1;
}

static void update_E(std::vector<int> &, int, Chromosome &);

void Schedule::construct_schedule(Chromosome &chromo) {

    int g = 0;
    int t = 0;
    std::vector<int> F = {0};
    std::vector<int> S = {0};
    std::vector<int> E;
    while (S.size() < number_of_jobs + 1) {
        update_E(E, t, chromo);
        while (!E.empty()) {
            // select operation with highest priority
            int j;
            // calculate the earliest finish time (in terms of precedence only)
            int EF;
            // calculate the earliest finish time (in terms of precedence and capacity)
            int FC;
            // Update S
            S.push_back(j);
            // Update F
            F.push_back(FC);
            // Iteration increment
            ++g;
        }
        // Determine the time associated with iteration g


    }
}

void Schedule::local_search() {

}

static void update_E(std::vector<int> &array, int t, Chromosome &chromo) {

}