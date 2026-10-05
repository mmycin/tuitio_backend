#pragma once

#include "errors/api.hpp"
#include <memory>
#include "models/payments_model.hpp"
#include "repositories/payment_repository.hpp"
#include "repositories/student_repository.hpp"
#include "services/service.hpp"
#include "utils/bearer_token.hpp"

class PaymentService : public IService {
public:
    explicit PaymentService(std::unique_ptr<PaymentRepository> repo,
                           std::unique_ptr<AuthManager> auth_manager)
        : repo(std::move(repo)), auth_manager(std::move(auth_manager)) {}

    Payment getPaymentByID(const std::string &token, int id) {
        auto user_id = auth_manager->get_id(token);
        if (!user_id) {
            throw UnauthorizedError("Invalid or expired token");
        }
        Payment payment = this->repo->getPaymentByID(id);
        if (payment.id != 0) {
            StudentRepository student_repo;
            auto student = student_repo.getStudentById(*user_id, payment.student_id);
            if (student.id == 0) {
                throw UnauthorizedError("You are not authorized to view this payment");
            }
        }
        return payment;
    }

    Payment createPayment(const std::string &token, Payment payment) {
        auto user_id = auth_manager->get_id(token);
        if (!user_id) {
            throw UnauthorizedError("Invalid or expired token");
        }
        StudentRepository student_repo;
        auto student = student_repo.getStudentById(*user_id, payment.student_id);
        if (student.id == 0) {
            throw UnauthorizedError("You are not authorized to create payments for this student");
        }
        return this->repo->createPayment(payment);
    }

private:
    std::unique_ptr<PaymentRepository> repo;
    std::unique_ptr<AuthManager> auth_manager;
};

