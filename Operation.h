//
// Created by kwh44 on 12/17/18.
//

#ifndef HYBRID_ALGORITHM_OPERATION_H
#define HYBRID_ALGORITHM_OPERATION_H


class Operation {
    int operation_number;
    int job_number;
    int processing_time;
    int machine;
public:
    Operation(int, int, int, int);

    Operation();

    Operation(const Operation &);

    Operation(Operation &);

    Operation(Operation &&other) noexcept;

    Operation &operator=(Operation &&) noexcept;

    Operation &operator=(const Operation &other) = default;

    int get_operation_number() const { return operation_number; }

    int get_job_number() const { return job_number; }

    int get_processing_time() const { return processing_time; }

    int get_machine() const { return machine; }
};


#endif //HYBRID_ALGORITHM_OPERATION_H
