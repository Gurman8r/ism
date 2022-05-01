#include <servers/audio_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(AudioServer::singleton) {};

OBJECT_EMBED(AudioServer, t) {}

AudioServer::AudioServer() { singleton = this; }

AudioServer::~AudioServer() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */