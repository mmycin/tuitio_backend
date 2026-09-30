#pragma once

#include "errors/api.hpp"
#include "spdlog/spdlog.h"
#include <SQLiteCpp/Statement.h>
#include "models/payments_model.hpp"
#include "repositories/repository.hpp"
#include "time_utls.hpp"

using namespace std;

class PaymentRepository : public IRepository {
    public:
        Payment createPayment(Payment payment) {
            SQLite::Statement query(this->db, R"SQL(
                INSERT INTO payments
                (student_id, created_at, amount)
                VALUES
                (?, ?, ?)
                RETURNING *;
            )SQL");
            query.bind(1, payment.student_id);
            query.bind(2, TimeConverter::timePointToString(payment.created_at));
            query.bind(3, payment.amount);

            while (query.executeStep()) {
                payment.id = query.getColumn(0);
            }
            
            return payment;
        }

        Payment getPaymentByID(int id) {
            Payment payment;

            SQLite::Statement query(this->db, R"SQL(
                SELECT * FROM payments
                WHERE id = ?;
            )SQL");
            query.bind(1, id);

            while(query.executeStep()) {
                payment.id = query.getColumn(0);
                payment.student_id = query.getColumn(1);
                payment.created_at = TimeConverter::sqliteToTimePoint(query.getColumn(2).getString());
                payment.amount = query.getColumn(3);
            }
            
            return payment;
        }
};
