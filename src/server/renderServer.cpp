#include <server/renderServer.hpp>

// ideally we don't need renderServer to depend on OpenGL
// but we need this for input since we don't have our own key mapping
#include <utils/glfw.hpp>

namespace fcm {

RenderServer::RenderServer()
    : _viewer{}, _input{_viewer.input}, lastFrameTime{getTime()} {}

void RenderServer::render(Scene &scene) {
  auto now = getTime();
  auto dt = now - lastFrameTime;
  lastFrameTime = now;
  _viewer.renderBeginFrame();

  pollInput();
  selectObject(scene);
  updateCameraPos(dt);
  updateCameraDir(dt);

  _viewer.renderRigidBodies(scene.objects());
  _viewer.renderDirLights(scene.dirLights());
  _viewer.renderPointLights(scene.pointLights());

  _viewer.renderEndFrame();
}

void RenderServer::getInput() { /* NOT IMPLEMENTED */
}

void RenderServer::pollInput() {
  _input.pollInput();
  /* NOT IMPLEMENTED */
}

bool RenderServer::shouldClose() {
  if (_input.keyPressed(GLFW_KEY_ESCAPE)) {
    _viewer.window.setShouldClose(true);
  }
  return _viewer.window.shouldClose();
}

RenderMeshKey RenderServer::insertMesh(Mesh *mesh) {
  return _viewer.insertMesh(mesh);
}

TextureKey RenderServer::insertTexture(Sprite sprite) {
  return _viewer.insertTexture(std::move(sprite));
}

void RenderServer::updateCameraPos(double dt) {
  float speed;
  if (_input.keyPressed(GLFW_KEY_LEFT_SHIFT)) {
    speed = movementSpeed * 0.33;
  } else {
    speed = movementSpeed;
  }

  float multiplier = speed * (dt * 1000.f);
  glm::vec3 deltaPos = {0, 0, 0};

  bool WPressed = _input.keyPressed(GLFW_KEY_W);
  bool APressed = _input.keyPressed(GLFW_KEY_A);
  bool SPressed = _input.keyPressed(GLFW_KEY_S);
  bool DPressed = _input.keyPressed(GLFW_KEY_D); // I'm DPressed
  if (WPressed) {
    deltaPos += _viewer.camera().dir;
  }
  if (SPressed) {
    deltaPos -= _viewer.camera().dir;
  }
  if (APressed) {
    deltaPos -=
        glm::normalize(glm::cross(_viewer.camera().dir, _viewer.camera().up));
  }
  if (DPressed) {
    deltaPos +=
        glm::normalize(glm::cross(_viewer.camera().dir, _viewer.camera().up));
  }

  // don't move sqrt 2 times as fast diagonally
  bool wsMovement = WPressed || SPressed;
  bool adMovement = APressed || DPressed;
  if (wsMovement && adMovement) {
    multiplier *= INV_SQRT_2;
  }

  deltaPos *= multiplier;

  _viewer.cameraAddPos(deltaPos);
}

void RenderServer::updateCameraDir(double dt) {
  (void)dt;

  if (_input.mousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
    glm::vec2 pos = _input.cursorPos();
    if (!rbuttonDown) {
      lastMousePos = pos;
    }
    rbuttonDown = true;
    float xoffset = pos.x - lastMousePos.x;
    float yoffset = lastMousePos.y - pos.y;
    lastMousePos = pos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    glm::clamp(pitch, -89.f, 89.f);

    glm::vec3 newDir = {
        glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        glm::sin(glm::radians(pitch)),
        glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))};
    _viewer.cameraSetDir(glm::normalize(newDir));
  } else if (_input.mouseReleased(GLFW_MOUSE_BUTTON_RIGHT)) {
    rbuttonDown = false;
  }
}

void RenderServer::selectObject(Scene &scene) {
  RayCastResult result;
  if (_input.mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
    glm::vec2 pos = _input.cursorPos();
    glm::mat4 invCam = glm::inverse(_viewer.cameraVP());
    float hw = config.windowWidth / 2.f;
    float hh = config.windowHeight / 2.f;
    glm::vec4 near =
        glm::vec4((pos.x - hw) / hw, -1 * (pos.y - hh) / hh, -1, 1.0);
    glm::vec4 far =
        glm::vec4((pos.x - hw) / hw, -1 * (pos.y - hh) / hh, 1, 1.0);
    glm::vec4 nearResult = invCam * near;
    glm::vec4 farResult = invCam * far;
    nearResult /= nearResult.w;
    farResult /= farResult.w;
    glm::vec3 dir = glm::normalize(glm::vec3(farResult - nearResult));

    result = scene.castRay(_viewer.camera().pos, dir);
    if (result.hit) {
      result.object->ambientColour = glm::vec3{0.f, 0.1f, 0.f};
      result.object->diffuseColour = glm::vec3{0.f, 0.5f, 0.f};
    }
  }
}

} // namespace fcm
