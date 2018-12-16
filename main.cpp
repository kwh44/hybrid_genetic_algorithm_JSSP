#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Population.h"
#include "Schedule.h"

using json = nlohmann::json;

void test();

void test_mt06();


int main() {
    test_mt06();
    return 0;
}


void test() {
    std::cout << "__ Genetic Algorithm started __" << std::endl;
    std::ifstream file("../test_dataset.json");
    json data_set;
    if (file.is_open()) {
        data_set = json::parse(file);
        std::cout << "test_dataset.json successfully opened.\n";
    }
    file.close();
    std::string description = data_set[0]["description"];
    std::cout << description << std::endl;
    std::vector<std::vector<int> > test_case = data_set[0]["data"];
    size_t population_size = test_case.size() * test_case[0].size();;
    Population population = Population(population_size, 0.7, test_case);
    std::cout << "___ initial population ___" << std::endl;
    population.display_population();
    std::cout << "___ end ____\n" << std::endl;
    for (int i = 0; i < 100; ++i) {
        population.new_generation();
    }
    auto x = population.solution();
    std::cout << "Best makespan is " << x << std::endl;
}

void test_mt06() {
    std::cout << "__ Genetic Algorithm started __" << std::endl;
    std::ifstream file("../dataset.json");
    json data_set;
    if (file.is_open()) {
        data_set = json::parse(file);
        std::cout << "dataset.json successfully opened.\n";
    }
    file.close();
    std::string description = data_set[0]["description"];
    std::cout << description << std::endl;
    std::vector<std::vector<int> > test_case = data_set[0]["data"];
    size_t population_size = test_case.size() * test_case[0].size();
    Population population = Population(population_size, 0.7, test_case);
    std::vector<int> number_of_generations = {50, 100, 200, 300, 400};
    std::vector<int> results;
    for (const auto &v: number_of_generations) {
        for (int i = 0; i < v; ++i) {
            population.new_generation();
        }
        results.push_back(population.solution());
    }
    int generation_num;
    int best_makespan = 100000000;
    for (int i = 0; i < results.size(); ++i) {
        if (results[i] < best_makespan) {
            best_makespan = results[i];
            generation_num = i;
        }
    }
    std::cout << "Best makespan is " << best_makespan << " after " << number_of_generations[generation_num] << " generations." << std::endl;
}
