#include "dtos/dto.hpp"
#include "models/students_model.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

class StudentResponse : public IDTO {
  public:
    int id;
    string name;
    int fee;

    StudentResponse() = default;

    StudentResponse(Student student) {
        this->id = student.id;
        this->name = student.name;
        this->fee = student.fee;
    }

    void validate() override {}

    json toJson() override {
        return json{
            { "id", this->id },
            { "name", this->name },
            { "fee", this->fee },
        };
    }
};

class StudentsResponse : public IDTO {
  public:
    std::vector<StudentResponse> students;
    StudentsResponse() = default;

    StudentsResponse(std::vector<StudentResponse> students) {
        this->students = students;
    }

    void validate() override {}

    json toJson() override {
        json j;
        j["students"] = nlohmann::json::array();

        for (auto &student : this->students) {
            j["students"].push_back(student.toJson());
        }

        return j;
    }
};

class DeleteStudentResponse : IDTO {
  public:
    bool success;

    DeleteStudentResponse() = default;
    DeleteStudentResponse(bool success) {
        this->success = success;
    }

    void validate() override {}

    json toJson() override {
        json j;

        if(this->success) {
            j["sucess"] = true;
            j["message"] = "student deleted successfully";
        } else {
            j["sucess"] = false;
            j["error"] = "can not delete this student";
        }
        
        return j;
    }
};
