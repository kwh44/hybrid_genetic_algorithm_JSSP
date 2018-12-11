#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Population.h"
#include "Schedule.h"

using json = nlohmann::json;

void test();

int main() {
    test();
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

    std::string description = data_set[0]["description"];
    std::vector<std::vector<int> > test_case = data_set[0]["data"];

    int population_size = 1;
    std::cout << description << std::endl;
    Population population = Population(population_size, 0.7, test_case);
    population.new_generation();
};