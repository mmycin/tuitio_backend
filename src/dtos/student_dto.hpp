#include "dtos/dto.hpp"
#include "dtos/from_json_macros.hpp"
#include "dtos/user_dto.hpp"
#include "errors/validation.hpp"
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

class DeleteStudentResponse : public IDTO {
  public:
    bool success;

    DeleteStudentResponse() = default;
    DeleteStudentResponse(bool success) {
        this->success = success;
    }

    void validate() override {}

    json toJson() override {
        json j;

        if (this->success) {
            j["sucess"] = true;
            j["message"] = "student deleted successfully";
        } else {
            j["sucess"] = false;
            j["error"] = "can not delete this student";
        }

        return j;
    }
};

class UpdateStudentRequest : public IDTO {
  public:
    int id = 0;
    string name;
    int fee;

    UpdateStudentRequest() = default;

    UpdateStudentRequest(int id, string name, int fee) {
        this->id = id;
        this->name = name;
        this->fee = fee;
    }

    void validate() override {}

    json toJson() override {
        return json{};
    }
};

FROM_JSON_OPTIONAL(UpdateStudentRequest, id, name, fee);

class UpdateStudentResponse : public IDTO {
  public:
    Student student;

    UpdateStudentResponse() = default;

    UpdateStudentResponse(Student student) {
        this->student = student;
    }

    void validate() override {}

    json toJson() override {
        auto student_dto = StudentResponse(this->student);
        return json{
            { "success", true },
            { "student", student_dto.toJson() },
        };
    }
};

class CreateStudentRequest : public IDTO {
    public:
        string name;
        int fee;

        CreateStudentRequest() = default;

        CreateStudentRequest(string name, int fee) {
            this->name = name;
            this->fee = fee;
            validate();
        }

        void validate() override {
            Validator()
                .check(!this->name.empty(), "name", "Name is required")
                .check(this->fee != 0, "fee", "fee can not be null")
                .validate();
        }

        json toJson() override {
            return json{};
        }

};
FROM_JSON(CreateStudentRequest,name ,fee);

class CreateStudentResponse : public IDTO {
    public:
      Student student;
  
      CreateStudentResponse() = default;
  
      CreateStudentResponse(Student student) {
          this->student = student;
      }
  
      void validate() override {}
  
      json toJson() override {
          auto student_dto = StudentResponse(this->student);
          return json{
              { "success", true },
              { "student", student_dto.toJson() },
          };
      }
};

