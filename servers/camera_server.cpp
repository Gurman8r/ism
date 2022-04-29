#include <servers/camera_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_OBJECT_CLASS(Camera, t) {}

Camera::~Camera() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(CameraServer::singleton) {};

EMBED_OBJECT_CLASS(CameraServer, t) {}

CameraServer::CameraServer() { singleton = this; }

CameraServer::~CameraServer() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */