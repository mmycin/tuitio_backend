#pragma once

#include "errors/api.hpp"
#include <memory>
#include "models/payments_model.hpp"
#include "repositories/payment_repository.hpp"
#include "services/service.hpp"

class PaymentService : public IService {
public:
    explicit PaymentService(std::unique_ptr<PaymentRepository> repo)
        : repo(std::move(repo)) {}

    Payment getPaymentByID(int id) {
        return this->repo->getPaymentByID(id);
    }

    Payment createPayment(Payment payment) {
        return this->repo->createPayment(payment);
    }


private:
    std::unique_ptr<PaymentRepository> repo;
};
