/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TNT_FILAMENT_DRIVER_OPENGL_PLATFORM_EGL_OPENGL_H
#define TNT_FILAMENT_DRIVER_OPENGL_PLATFORM_EGL_OPENGL_H

#include "PlatformEGL.h"

namespace filament {

class PlatformEGLOpenGL final : public PlatformEGL {
public:

    PlatformEGLOpenGL() noexcept;

    backend::Driver* createDriver(void* sharedContext) noexcept final;

    int getOSVersion() const noexcept final { return 0; }

    void setPresentationTime(int64_t presentationTimeInNanosecond) noexcept final {}

    Stream* createStream(void* nativeStream) noexcept final { return nullptr; }
    void destroyStream(Stream* stream) noexcept final {}
    void attach(Stream* stream, intptr_t tname) noexcept final {}
    void detach(Stream* stream) noexcept final {}
    void updateTexImage(Stream* stream, int64_t* timestamp) noexcept final {}

    ExternalTexture* createExternalTextureStorage() noexcept final { return nullptr; }
    void reallocateExternalStorage(ExternalTexture* ets,
            uint32_t w, uint32_t h, backend::TextureFormat format) noexcept final {}
    void destroyExternalTextureStorage(ExternalTexture* ets) noexcept final {}
};

} // namespace filament

#endif // TNT_FILAMENT_DRIVER_OPENGL_PLATFORM_EGL_OPENGL_H
