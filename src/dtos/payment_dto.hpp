#pragma once

#include "dtos/dto.hpp"
#include "dtos/from_json_macros.hpp"
#include "errors/validation.hpp"
#include "models/payments_model.hpp"
#include "time_utls.hpp"
#include <chrono>

using json = nlohmann::json;

class GetPaymentResponse : public IDTO {
  public:
    Payment payment;

    GetPaymentResponse() = default;

    GetPaymentResponse(Payment payment) {
        this->payment = payment;
    }

    void validate() {}

    json toJson() {
        return json{
            { "id", this->payment.id },
            { "created_at",
              TimeConverter::timePointToString(payment.created_at) },
            { "amount", this->payment.amount },
        };
    }
};

class CreatePaymentRequest : public IDTO {
  public:
    int student_id = 0;
    string created_at;
    int amount = 0;

    CreatePaymentRequest() = default;

    CreatePaymentRequest(int student_id, string created_at, int amount) {
        this->student_id = student_id;
        this->created_at = created_at;
        this->amount = amount;
    }

    void validate() override {
        Validator()
            .check(!created_at.empty(), "created_at",
                   "created_at can not be null")
            .check(student_id != 0, "student_id", "student_id can not be null")
            .check(amount != 0, "amount", "amount can not be null")
            .validate();
    }

    json toJson() override {
        return json{};
    }
};
FROM_JSON(CreatePaymentRequest, student_id, created_at, amount);
