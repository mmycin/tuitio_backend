#pragma once

#include <nlohmann/json.hpp>

class IDTO {
  public:
    virtual void validate() = 0;
    virtual nlohmann::json toJson() = 0;

    virtual ~IDTO() = default;
};
