#include <servers/text_server.hpp>

using namespace ism;

MEMBER_IMPL(TextServer::singleton) {};

EMBED_OBJECT_CLASS(TextServer, t) {}

TextServer::TextServer() { singleton = this; }

TextServer::~TextServer() {}