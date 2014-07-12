#ifndef PEERCONNECTION_H
#define PEERCONNECTION_H

#include <agent.h>
#include "rtcsessiondescription.h"

class RTCPeerConnection {
  public:
    RTCPeerConnection();
    ~RTCPeerConnection();
    void createOffer(void onOfferReady(RTCSessionDescription*));
    void createAnswer(void onAnswerReady(RTCSessionDescription*));
    void setRemoteDescription(RTCSessionDescription* description, void onRemoteDescriptionSetted());
  private:
    int streamId = -1;
    /* Main Loop Stuff */
    static void* MainLoop(void* data);
    GThread* mainLoopThread;
    GMainLoop* gloop;
    NiceAgent* agent;
    /* ICE callbacks section */
    static void onCandidateGatheringDone(NiceAgent* agent, guint stream_id, gpointer data);
    static void onIceStateChanged(NiceAgent* agent, guint stream_id, guint component_id, guint state, gpointer data);
    static void onIceMessage(NiceAgent* agent, guint stream_id, guint component_id, guint len, gchar* buf, gpointer data);
    /* User callbacks section */
    void (*onOfferReady)(RTCSessionDescription*);
    void (*onAnswerReady)(RTCSessionDescription*);
    void (*onRemoteDescriptionSetted)();
};

#endif
