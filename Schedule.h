//
// Created by kwh44 on 12/7/18.
//

#ifndef HYBRID_ALGORITHM_SCHEDULE_H
#define HYBRID_ALGORITHM_SCHEDULE_H

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Schedule {
private:
    json data_set;

public:

    Schedule() = default;

    explicit Schedule(std::string dataset_filename);

    json &data() { return this->data_set; }
};


#endif //HYBRID_ALGORITHM_SCHEDULE_H
