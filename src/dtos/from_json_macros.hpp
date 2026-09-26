#pragma once

#include <nlohmann/json.hpp>

// --- Internal dispatch macro ---
// Shift parameters so NAME maps to the correct macro based on argument count
#define _JSON_GET_MACRO(_1, _2, _3, _4, _5, NAME, ...) NAME

// --- Required-field variants ---
#define FROM_JSON_1(cls, f1) \
inline void from_json(const nlohmann::json &j, cls &obj) { \
    j.at(#f1).get_to(obj.f1); \
    obj.validate(); \
}

#define FROM_JSON_2(cls, f1, f2) \
inline void from_json(const nlohmann::json &j, cls &obj) { \
    j.at(#f1).get_to(obj.f1); \
    j.at(#f2).get_to(obj.f2); \
    obj.validate(); \
}

#define FROM_JSON_3(cls, f1, f2, f3) \
inline void from_json(const nlohmann::json &j, cls &obj) { \
    j.at(#f1).get_to(obj.f1); \
    j.at(#f2).get_to(obj.f2); \
    j.at(#f3).get_to(obj.f3); \
    obj.validate(); \
}

#define FROM_JSON_4(cls, f1, f2, f3, f4) \
inline void from_json(const nlohmann::json &j, cls &obj) { \
    j.at(#f1).get_to(obj.f1); \
    j.at(#f2).get_to(obj.f2); \
    j.at(#f3).get_to(obj.f3); \
    j.at(#f4).get_to(obj.f4); \
    obj.validate(); \
}

#define FROM_JSON_5(cls, f1, f2, f3, f4, f5) \
inline void from_json(const nlohmann::json &j, cls &obj) { \
    j.at(#f1).get_to(obj.f1); \
    j.at(#f2).get_to(obj.f2); \
    j.at(#f3).get_to(obj.f3); \
    j.at(#f4).get_to(obj.f4); \
    j.at(#f5).get_to(obj.f5); \
    obj.validate(); \
}

// Auto-dispatch: FROM_JSON(CyclesRequest, student_id, ids) -> FROM_JSON_2(...)
#define FROM_JSON(cls, ...) \
    _JSON_GET_MACRO(__VA_ARGS__, FROM_JSON_5, FROM_JSON_4, FROM_JSON_3, FROM_JSON_2, FROM_JSON_1, unused)(cls, __VA_ARGS__)

// --- Optional-field variants ---
#define FROM_JSON_OPTIONAL_1(cls, f1) \
inline void from_json(const nlohmann::json &j, cls &obj) { \
    if (j.contains(#f1)) j.at(#f1).get_to(obj.f1); \
    obj.validate(); \
}

#define FROM_JSON_OPTIONAL_2(cls, f1, f2) \
inline void from_json(const nlohmann::json &j, cls &obj) { \
    if (j.contains(#f1)) j.at(#f1).get_to(obj.f1); \
    if (j.contains(#f2)) j.at(#f2).get_to(obj.f2); \
    obj.validate(); \
}

#define FROM_JSON_OPTIONAL_3(cls, f1, f2, f3) \
inline void from_json(const nlohmann::json &j, cls &obj) { \
    if (j.contains(#f1)) j.at(#f1).get_to(obj.f1); \
    if (j.contains(#f2)) j.at(#f2).get_to(obj.f2); \
    if (j.contains(#f3)) j.at(#f3).get_to(obj.f3); \
    obj.validate(); \
}

#define FROM_JSON_OPTIONAL_4(cls, f1, f2, f3, f4) \
inline void from_json(const nlohmann::json &j, cls &obj) { \
    if (j.contains(#f1)) j.at(#f1).get_to(obj.f1); \
    if (j.contains(#f2)) j.at(#f2).get_to(obj.f2); \
    if (j.contains(#f3)) j.at(#f3).get_to(obj.f3); \
    if (j.contains(#f4)) j.at(#f4).get_to(obj.f4); \
    obj.validate(); \
}

#define FROM_JSON_OPTIONAL_5(cls, f1, f2, f3, f4, f5) \
inline void from_json(const nlohmann::json &j, cls &obj) { \
    if (j.contains(#f1)) j.at(#f1).get_to(obj.f1); \
    if (j.contains(#f2)) j.at(#f2).get_to(obj.f2); \
    if (j.contains(#f3)) j.at(#f3).get_to(obj.f3); \
    if (j.contains(#f4)) j.at(#f4).get_to(obj.f4); \
    if (j.contains(#f5)) j.at(#f5).get_to(obj.f5); \
    obj.validate(); \
}

// Auto-dispatch: FROM_JSON_OPTIONAL(UpdateUserRequest, name, email) -> FROM_JSON_OPTIONAL_2(...)
#define FROM_JSON_OPTIONAL(cls, ...) \
    _JSON_GET_MACRO(__VA_ARGS__, FROM_JSON_OPTIONAL_5, FROM_JSON_OPTIONAL_4, FROM_JSON_OPTIONAL_3, FROM_JSON_OPTIONAL_2, FROM_JSON_OPTIONAL_1, unused)(cls, __VA_ARGS__)