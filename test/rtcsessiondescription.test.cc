#include <gtest/gtest.h>

#include "../src/rtcsessiondescription.h"

TEST(SessionDescription, shouldExist) {
  RTCSessionDescription* sd = new RTCSessionDescription();
  ASSERT_TRUE(sd != NULL);
  delete sd;
}

TEST(SessionDescription, shouldSetAndGetType) {
  RTCSessionDescription* sd = new RTCSessionDescription();
  sd->setType("testType");
  ASSERT_TRUE(sd->getType() == "testType");
  delete sd;
}

TEST(SessionDescription, shouldSetAndGetSdp) {
  RTCSessionDescription* sd = new RTCSessionDescription();
  sd->setSdp("testSDP");
  ASSERT_TRUE(sd->getSdp() == "testSDP");
  delete sd;
}

TEST(SessionDescription, shouldDefineOfferAndAnswerConstants) {
  ASSERT_TRUE(RTCSessionDescription::OFFER == "offer");
  ASSERT_TRUE(RTCSessionDescription::ANSWER == "answer");
}

TEST(SessionDescription, shouldParseAStringifiedDescription) {
  std::string testString = "{ \"type\": \"offer\", \"sdp\": \"this sdp\" }";
  RTCSessionDescription* sd = RTCSessionDescription::parse(testString);
  ASSERT_TRUE(sd->getType() == "offer");
  ASSERT_TRUE(sd->getSdp() == "this sdp");
  delete sd;
}

TEST(SessionDescription, shouldExceptOnWrongJson) {
  std::string testString = "wrong json";
  ASSERT_THROW(RTCSessionDescription::parse(testString), InvalidJsonException);
}

TEST(SessionDescription, shouldExceptOnWrongDescription) {
  std::string testString = "{ \"wrong\": \"sdp\" }";
  ASSERT_THROW(RTCSessionDescription::parse(testString), InvalidDescriptionException);
}
