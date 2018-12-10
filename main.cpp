#include <iostream>
#include <fstream>
#include "Population.h"
#include "Schedule.h"

int main() {
    std::cout << "__ Genetic Algorithm started __" << std::endl;

    std::ifstream file("../dataset.json");

    json data_set;

    if (file.is_open()) {
        data_set = json::parse(file);
        std::cout << "dataset.json successfully opened.\n";
    }

    for (const auto &v: data_set) {

        std::string description = v["description"];
        std::vector<std::vector<int> > test_case = v["data"];

        int population_size = 5;

        Population population = Population(population_size, 0.7, test_case);

        break;
    }

    return 0;
}
