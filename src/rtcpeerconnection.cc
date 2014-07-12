#include "rtcpeerconnection.h"

void* RTCPeerConnection::MainLoop(void* data) {
  RTCPeerConnection* pc = (RTCPeerConnection*)data;

  g_main_loop_run(pc->gloop);

  return NULL;
}

RTCPeerConnection::RTCPeerConnection() {
  this->streamId = -1;

  this->gloop = g_main_loop_new(NULL, FALSE);

#if !GLIB_CHECK_VERSION(2, 36, 0)
  g_type_init();
#endif

  this->agent = nice_agent_new(g_main_loop_get_context(this->gloop), NICE_COMPATIBILITY_RFC5245);
  g_object_set(G_OBJECT(this->agent), "stun-server", "173.194.70.127", NULL);
  g_object_set(G_OBJECT(this->agent), "stun-server-port", 19302, NULL);
  
  g_signal_connect(G_OBJECT(this->agent), "candidate-gathering-done", G_CALLBACK(RTCPeerConnection::onCandidateGatheringDone), this);

  this->mainLoopThread = g_thread_new("MainLoop", &RTCPeerConnection::MainLoop, this);
}

RTCPeerConnection::~RTCPeerConnection() {
  if(this->gloop)
    g_main_loop_unref(this->gloop);
}

void RTCPeerConnection::createOffer(void onOfferReady(RTCSessionDescription*)) {
  this->onOfferReady = onOfferReady;

  this->streamId = nice_agent_add_stream(this->agent, 1);
  nice_agent_set_stream_name(this->agent, this->streamId, "datachannel");

  nice_agent_gather_candidates(this->agent, this->streamId);
}

void RTCPeerConnection::setRemoteDescription(RTCSessionDescription* description, void onRemoteDescriptionSetted()) {
  std::string sdp = description->getSdp();

  if(this->streamId == -1) {
    this->streamId = nice_agent_add_stream(this->agent, 1);
    nice_agent_set_stream_name(this->agent, this->streamId, "datachannel");
  }

  guint nc = nice_agent_parse_remote_sdp(this->agent, sdp.c_str());
  g_print("decoded %d candidates!\n", nc);

  onRemoteDescriptionSetted();
}

/** 
 * onCandidateGatheringDone:
 * 
 * This callback is fired when all the ice candidates are found.
 * Here the ice section of the sdp is generated and 
 **/
void RTCPeerConnection::onCandidateGatheringDone(NiceAgent* agent, guint streamId, gpointer data) {
  RTCPeerConnection* pc = (RTCPeerConnection*) data;

  gchar* sdp = nice_agent_generate_local_sdp(agent);

  RTCSessionDescription* description = new RTCSessionDescription();
  description->setType(RTCSessionDescription::OFFER);
  description->setSdp(sdp);
  pc->onOfferReady(description);

  //g_free(sdp);
}
