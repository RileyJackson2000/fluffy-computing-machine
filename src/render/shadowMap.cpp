#include <render/shadowMap.hpp>
#include <utils/sprite.hpp>

namespace fcm {

ShadowMap::ShadowMap(int width, int height)
    : _depthMap{NullSprite(width, height, 3)}, _width{width}, _height{height} {
  glGenFramebuffers(1, &_depthMapFBO);

  _depthMap.setMinFilter(GL_NEAREST);
  _depthMap.setMagFilter(GL_NEAREST);
  _depthMap.setWrap(GL_CLAMP_TO_BORDER);
  _depthMap.setDataFormat(GL_DEPTH_COMPONENT);
  _depthMap.setGenMipMaps(false);
}

void ShadowMap::bind() const noexcept {
  bindFrameBuffer();
  bindTexture();
}

void ShadowMap::bindFrameBuffer() const noexcept {
  glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
}

void ShadowMap::bindTexture() const noexcept { _depthMap.bind(); }

void ShadowMap::to_gpu() noexcept {
  _depthMap.to_gpu();

  // bind depth buffer
  glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);

  // attach texture to frame buffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         _depthMap.handle(), 0);
  // explicitly tell opengl we won't be rendering colours
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  // unbind depth buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::setTextureUnit(int textureUnit) noexcept {
  _depthMap.setTextureUnit(textureUnit);
}

int ShadowMap::width() const noexcept { return _width; }

int ShadowMap::height() const noexcept { return _height; }

GLHandle ShadowMap::depthMap() const noexcept { return _depthMap.handle(); }

GLHandle ShadowMap::depthMapFBO() const noexcept { return _depthMapFBO; }

} // namespace fcm
