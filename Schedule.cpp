//
// Created by kwh44 on 12/7/18.
//

#include "Schedule.h"
#include <fstream>

Schedule::Schedule(std::string dataset_filename) {

    std::ifstream file(dataset_filename);

    if (file.is_open()) {
        this->data_set = json::parse(file);
    } else {

    }
}