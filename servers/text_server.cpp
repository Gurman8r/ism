#include <servers/text_server.hpp>

using namespace ism;

MEMBER_IMPL(TextServer::singleton) {};

OBJECT_EMBED(TextServer, t) {}

TextServer::TextServer() { singleton = this; }

TextServer::~TextServer() {}