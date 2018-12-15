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
        std::cout << "dataset.json successfully opened.\n";
    }
    file.close();
    std::string description = data_set[0]["description"];
    std::cout << description << std::endl;
    std::vector<std::vector<int> > test_case = data_set[0]["data"];
    int population_size = 10;
    Population population = Population(population_size, 0.7, test_case);
    for (int i = 0; i < 100; ++i) {
        population.new_generation();
        std::cout << "Generation " << i << " completed." << std::endl;
        population.display_population();
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
    for (int i = 0; i < 400; ++i) {
        population.new_generation();
    }
    auto x = population.solution();
    std::cout << "Best makespan is " << x << std::endl;
}
