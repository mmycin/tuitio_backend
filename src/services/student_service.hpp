#pragma once

#include "errors/api.hpp"
#include "models/students_model.hpp"
#include "repositories/student_repository.hpp"
#include "services/service.hpp"
#include "utils/bearer_token.hpp"
#include <memory>
#include <vector>

class StudentService : public IService {
  public:
    explicit StudentService(std::unique_ptr<StudentRepository> repo,
                            std::unique_ptr<AuthManager> auth_manager)
        : repo(std::move(repo)), auth_manager(std::move(auth_manager)) {}

    std::vector<Student> getUsers(string token) {
        auto id_opt = this->auth_manager->get_id(token);
        if (!id_opt) {
            throw ValidationError("Token not found or expired");
        }

        int id = *id_opt;

        return this->repo->getStudents(id);
    }

    Student getStudentById(string token, int id) {
        auto user_id_opt = this->auth_manager->get_id(token);
        if (!user_id_opt) {
            throw ValidationError("Token not found or expired");
        }

        int user_id = *user_id_opt;

        return this->repo->getStudenById(user_id, id);
    }

    Student updateStudent(string token, Student student) {
        auto user_id_opt = this->auth_manager->get_id(token);
        if (!user_id_opt) {
            throw ValidationError("Token not found or expired");
        }

        int user_id = *user_id_opt;

        student.user.id = user_id;

        return this->repo->updateStudent(student);
    }

    bool deleteStudent(string token, int id) {
        auto user_id_opt = this->auth_manager->get_id(token);
        if (!user_id_opt) {
            throw ValidationError("Token not found or expired");
        }

        int user_id = *user_id_opt;

        return this->repo->deleteStudent(id, user_id);
    }

    Student createStudent(string token, Student student) {
        auto user_id_opt = this->auth_manager->get_id(token);
        if (!user_id_opt) {
            throw ValidationError("Token not found or expired");
        }

        int user_id = *user_id_opt;
        student.user.id = user_id;

        return this->repo->createStudent(student);
    }

  private:
    std::unique_ptr<StudentRepository> repo;
    std::unique_ptr<AuthManager> auth_manager;
};
