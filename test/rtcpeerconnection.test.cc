#include <gtest/gtest.h>

#include "../src/rtcpeerconnection.h"
#include <mutex>
#include <condition_variable>

RTCPeerConnection* pc;

std::mutex m;
std::condition_variable cv;
std::unique_lock<std::mutex> lk(m);
bool closetest = false;

TEST(PeerConnection, shouldExist) {
  pc = new RTCPeerConnection();
  ASSERT_TRUE(pc != NULL);
  delete pc;
}

TEST(PeerConnection, shouldCreateAnOffer) {
  pc = new RTCPeerConnection();

  pc->createOffer([](RTCSessionDescription* sd) {
    ASSERT_TRUE(sd->getType() == "offer");
    printf("type: %s\n%s", sd->getType().c_str(), sd->getSdp().c_str());
    delete sd;
    delete pc;

    closetest = true;
    cv.notify_all();
  });

  cv.wait(lk, []{ return closetest; });
  closetest = false;
};
