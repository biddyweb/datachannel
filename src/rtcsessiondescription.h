#ifndef RTCSESSIONDESCRIPTION_H
#define RTCSESSIONDESCRIPTION_H

#include <string>
#include <parson.h>

class RTCSessionDescription {
  public:
    static const std::string OFFER;
    static const std::string ANSWER;
    static RTCSessionDescription* parse(std::string descriptionAsString);

    std::string getType();
    std::string getSdp();
    void setType(std::string type);
    void setSdp(std::string sdp);
  private:
    std::string type;
    std::string sdp;
};

class InvalidJsonException : public std::exception {
};

class InvalidDescriptionException : public std::exception {
};

class JsonValueScopeGuard {
  public:
    JsonValueScopeGuard(JSON_Value* value) {
      this->pointer = value;
    }
    ~JsonValueScopeGuard() {
      json_value_free(this->pointer);
    }
  private:
    JSON_Value* pointer;
};

#endif
