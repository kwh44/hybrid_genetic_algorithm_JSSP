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
    Operation(int operation, int job, int time, int machine_required) : operation_number(operation),
                                                                        job_number(job), processing_time(time),
                                                                        machine(machine_required) {}

    Operation()  = default;

    Operation(const Operation &) = default;

    Operation(Operation &)  = default;

    Operation(Operation &&other) noexcept  = default;

    Operation &operator=(Operation &&) noexcept  = default;

    Operation &operator=(const Operation &other) = default;

    inline int get_operation_number() const { return operation_number; }

    inline int get_job_number() const { return job_number; }

    inline int get_processing_time() const { return processing_time; }

    inline int get_machine() const { return machine; }
};


#endif //HYBRID_ALGORITHM_OPERATION_H
