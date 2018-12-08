#include <iostream>
#include "Population.h"
#include "Schedule.h"

int main() {
    std::cout << "__ init __" << std::endl;
    auto test_population_object = Population(1000, 2, 0.7);
    auto test_schedule_object = Schedule("../dataset.json");
    return 0;
}
