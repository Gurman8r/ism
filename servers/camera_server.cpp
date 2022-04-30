#include <servers/camera_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Camera, t) {}

Camera::~Camera() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(CameraServer::singleton) {};

OBJECT_EMBED(CameraServer, t) {}

CameraServer::CameraServer() { singleton = this; }

CameraServer::~CameraServer() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */