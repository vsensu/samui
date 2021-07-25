#ifndef SAMUI_TEXTURE_H_
#define SAMUI_TEXTURE_H_

#include "core/core.h"

namespace samui {
class SAMUI_API Texture {
 public:
  virtual ~Texture() = default;

  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;
  virtual void     Bind(uint8_t slot = 0) = 0;
};

class SAMUI_API Texture2D : public Texture {
 public:
  static Ref<Texture2D> Create(const char* path);
};
}  // namespace samui

#endif  // SAMUI_TEXTURE_H_