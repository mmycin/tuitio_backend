#pragma once

#include "dtos/payment_dto.hpp"
#include "models/payments_model.hpp"
#include "time_utls.hpp"
#define _HAS_STD_BYTE 0

#include "controllers/controller.hpp"
#include "errors/api.hpp"
#include "errors/validation.hpp"
#include "services/payment_service.hpp"
#include <httplib.h>
#include <memory>
#include <string>

using namespace std;

class PaymentController : public IController {
  private:
    std::unique_ptr<PaymentService> service;

  public:
    explicit PaymentController(std::unique_ptr<PaymentService> service)
        : service(std::move(service)) {}

    void index(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void show(const httplib::Request &req, httplib::Response &res) override {
        string token = this->getTokenFromHeader(req);
        string id_str = req.matches[1];
        Validator::assertField(!id_str.empty(), "id", "Id is required");

        int itemId = 0;
        try {
            itemId = std::stoi(id_str);
        } catch (const std::exception &) {
            nlohmann::json details;
            details["id"] =
                nlohmann::json::array({ "Id must be a valid integer" });
            throw ValidationError(details, "Validation failed");
        }

        auto payment = this->service->getPaymentByID(token, itemId);
        if (payment.id == 0) {
            sendError(res, 404, "Can not find payment");
        } else {
            auto payment_dto = GetPaymentResponse(payment);
            sendJson(res, 200, payment_dto.toJson());
        }
    }

    void create(const httplib::Request &req, httplib::Response &res) override {
        string token = this->getTokenFromHeader(req);
        CreatePaymentRequest paymentReq = json::parse(req.body);

        Payment payment;
        payment.amount = paymentReq.amount;
        payment.student_id = paymentReq.student_id;
        payment.created_at =
            TimeConverter::sqliteToTimePoint(paymentReq.created_at);

        auto payment_created = this->service->createPayment(token, payment);
        if (payment_created.id == 0) {
            sendError(res, 404, "Can not find payment");
        } else {
            auto payment_dto = GetPaymentResponse(payment_created);
            sendJson(res, 200, payment_dto.toJson());
        }
    }

    void update(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void destroy(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }
};
