#include <servers/text_server.hpp>

using namespace ism;

MEMBER_IMPL(TextServer::singleton) {};

EMBED_CLASS(TextServer, t) {}

TextServer::TextServer() {}

TextServer::~TextServer() {}