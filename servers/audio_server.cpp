#include <servers/audio_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(AudioServer::g_singleton) {};

OBJECT_EMBED(AudioServer, t) {}

AudioServer::AudioServer() { g_singleton = this; }

AudioServer::~AudioServer() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */