//
// Created by kwh44 on 12/17/18.
//
#include "Operation.h"


Operation::Operation() {
    operation_number = 0;
    job_number = 0;
    processing_time = 0;
    machine = 0;
}

Operation::Operation(int operation, int job, int time, int machine_required) {
    operation_number = operation;
    job_number = job;
    processing_time = time;
    machine = machine_required;
}

Operation::Operation(const Operation &other) {
    operation_number = other.operation_number;
    job_number = other.job_number;
    processing_time = other.processing_time;
    machine = other.machine;
}

Operation::Operation(Operation &other) {
    operation_number = other.operation_number;
    job_number = other.job_number;
    processing_time = other.processing_time;
    machine = other.machine;
}

Operation::Operation(Operation &&other) noexcept {
    operation_number = other.operation_number;
    job_number = other.job_number;
    processing_time = other.processing_time;
    machine = other.machine;
}

Operation &Operation::operator=(Operation &&other) noexcept {
    operation_number = other.operation_number;
    job_number = other.job_number;
    processing_time = other.processing_time;
    machine = other.machine;
    return *this;
}
