#pragma once

#include "models/students_model.hpp"
#include "repositories/repository.hpp"
#include <SQLiteCpp/Statement.h>
#include <vector>

using namespace std;

class StudentRepository : public IRepository {
  public:
    std::vector<Student> getStudents(int id) {
        std::vector<Student> students;

        SQLite::Statement query(this->db, R"SQL(
                SELECT * FROM students
                WHERE user_id = ?;
            )SQL");
        query.bind(1, id);

        while (query.executeStep()) {
            int id = query.getColumn(0);
            string name = query.getColumn(2).getString();
            int fee = query.getColumn(3);

            Student student(id);
            student.name = name;
            student.fee = fee;

            students.push_back(student);
        }

        return students;
    }

    Student getStudenById(int user_id, int id) {
        Student student;

        SQLite::Statement query(this->db, R"SQL(
                SELECT * FROM students
                WHERE user_id = ? AND id = ?;
            )SQL");
        query.bind(1, user_id);
        query.bind(2, id);

        while (query.executeStep()) {
            student.id = query.getColumn(0);
            student.name = query.getColumn(2).getString();
            student.fee = query.getColumn(3);
        }

        return student;
    }

    Student updateStudent(Student student) {
        Student student_old = this->getStudenById(student.user.id, student.id);

        if (student.name.empty())
            student.name = student_old.name;
        if (student.fee == 0)
            student.fee = student_old.fee;

        SQLite::Statement query(this->db, R"SQL(
                UPDATE students
                SET name = ?, fee = ?
                WHERE id = ?;
            )SQL");
        query.bind(1, student.name);
        query.bind(2, student.fee);
        query.bind(3, student.id);

        while(query.executeStep()) {
            student.id = query.getColumn(0);
            student.user.id = query.getColumn(1);
            student.name = query.getColumn(2).getString();
            student.fee = query.getColumn(3);
        }

        return student;
    }

    bool deleteStudent(int id, int user_id) {
        SQLite::Statement query(this->db, R"SQL(
                DELETE FROM students
                WHERE id = ? AND
                user_id = ?
                RETURNING *;
            )SQL");
        query.bind(1, id);
        query.bind(2, user_id);

        while (query.executeStep()) {
            int id = query.getColumn(0);

            if (id == 0)
                return false;
            else
                return true;
        }

        return false;
    }
};
