/*
 * Copyright (C) 2021 The Android Open Source Project
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

#ifndef GLTFIO_MORPHER_H
#define GLTFIO_MORPHER_H

#include <utils/Entity.h>

namespace gltfio {

class FilamentAsset;
class FilamentInstance;

class Morpher {
public:
    using Entity = utils::Entity;
    virtual ~Morpher() = default;
    virtual void applyWeights(Entity targetEntity, float const* weights, size_t count) noexcept = 0;
};

} // namespace gltfio

#endif // GLTFIO_MORPHER_H
