#include "geticon.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return geticon::Init(env, exports);
}

NODE_API_MODULE(geticon, InitAll)