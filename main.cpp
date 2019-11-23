#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include "Population.h"
#include "Schedule.h"

using json = nlohmann::json;

void run_test_case(int &number, int &number_of_generations);

int main(int argc, char *argv[]) {
    // there are 44 test cases
    // test_case(30) -- means that algorithm will work
    // on 31-th instance of JSP problem from dataset.json
    int number = stoi(std::string(argv[1])), number_of_generations = stoi(std::string(argv[2]));
    run_test_case(number, number_of_generations);
    return 0;
}

void run_test_case(int &number, int &number_of_generations) {
    std::cout << "__ Genetic Algorithm started __" << std::endl;
    std::ifstream file("../dataset.json");
    json data_set;
    if (file.is_open()) {
        data_set = json::parse(file);
        std::cout << "dataset.json successfully opened.\n";
    }
    file.close();
    std::string description = data_set[number]["description"];
    std::cout << description << std::endl;
    std::vector<std::vector<int> > test_case = data_set[number]["data"];
    size_t population_size = test_case.size() * test_case[0].size();
    Population population = Population(population_size, 0.7, test_case);
    int best_make_span = 88888888;
    Chromosome best_chromosome;
    int best_solution_generation_number = 0;
    for (int i = 0; i < number_of_generations; ++i) {
        population.new_generation();
        std::cout << "[ OK ] ___ generation ___ " << i + 1 << ";\n";
        auto current_solution = population.solution(false);
        if (best_make_span > current_solution) {
            best_make_span = current_solution;
            best_chromosome = population.population()[0];
            best_solution_generation_number = i + 1;
        }
    }
    std::cout << "Best make-span is " << best_make_span << std::endl;
    std::cout << "Number of generations taken to obtain best make-span is " << best_solution_generation_number
              << std::endl;
    std::cout << "Chromosome that generated best make-span is: " << std::endl;
    for (int i = 0; i < best_chromosome.get_size(); ++i)
        std::cout << best_chromosome.get_genes()[i] << ' ';
    std::cout << std::endl;
    population.get_schedule().cost_function(best_chromosome, true);
    std::cout << "Scheduled operations: \n";
    for (const auto &v : population.get_schedule().get_array_of_scheduled_operations()) std::cout << v + 1 << ' ';
    std::cout << std::endl;
    std::cout << "their finish time, respectively: \n";
    for (const auto &v : population.get_schedule().get_array_of_finish_times()) std::cout << v << ' ';
    std::cout << std::endl;
}
