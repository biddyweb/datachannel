#include <gtest/gtest.h>

#include "../src/rtcpeerconnection.h"
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable cv;
std::unique_lock<std::mutex> lk(m);

RTCPeerConnection* pc;
RTCPeerConnection* pc2;
bool closetest = false;

TEST(PeerConnection, shouldExist) {
  pc = new RTCPeerConnection();
  ASSERT_TRUE(pc != NULL);
  delete pc;
}

static void onOfferReady(RTCSessionDescription* sd) {
  ASSERT_TRUE(sd->getType() == "offer");
  printf("type: %s\n%s", sd->getType().c_str(), sd->getSdp().c_str());
  delete sd;
  delete pc;

  closetest = true;
  cv.notify_all();
}

TEST(PeerConnection, shouldCreateAnOffer) {
  pc = new RTCPeerConnection();

  pc->createOffer(onOfferReady);

  cv.wait(lk, []{ return closetest; });
  closetest = false;
};

static void onRemoteDescriptionSetted() {
  closetest = true;
  cv.notify_all();

  delete pc;
  delete pc2;
}

static void callSetRemoteDescription(RTCSessionDescription* sd) {
  pc2->setRemoteDescription(sd, onRemoteDescriptionSetted);

  cv.wait(lk, []{ return closetest; });
  delete sd;
}

TEST(PeerConnection, shouldSetRemoteDescription) {
  pc = new RTCPeerConnection();
  pc2 = new RTCPeerConnection();

  pc->createOffer(callSetRemoteDescription);

  cv.wait(lk, []{ return closetest; });
  closetest = false;
}
