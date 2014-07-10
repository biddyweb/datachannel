#include "rtcsessiondescription.h"

const std::string RTCSessionDescription::OFFER = "offer";
const std::string RTCSessionDescription::ANSWER = "answer";

std::string RTCSessionDescription::getType() {
  return this->type;
}

std::string RTCSessionDescription::getSdp() {
  return this->sdp;
}

RTCSessionDescription* RTCSessionDescription::parse(std::string descriptionAsString) {
  JSON_Value* root = json_parse_string(descriptionAsString.c_str());
  JsonValueScopeGuard guard(root);

  if(json_value_get_type(root) != JSONObject)
    throw InvalidJsonException();

  JSON_Object* descriptionAsJson = json_value_get_object(root);

  RTCSessionDescription* description = new RTCSessionDescription();

  JSON_Value* typeValue = json_object_get_value(descriptionAsJson, "type");
  JSON_Value* sdpValue = json_object_get_value(descriptionAsJson, "sdp");

  if(json_value_get_type(typeValue) != JSONString || json_value_get_type(sdpValue) != JSONString)
    throw InvalidDescriptionException();

  description->type = json_value_get_string(typeValue);
  description->sdp = json_value_get_string(sdpValue);

  return description;
}
