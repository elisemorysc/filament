/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef TNT_FILAMENT_DETAILS_CAMERA_H
#define TNT_FILAMENT_DETAILS_CAMERA_H

#include <filament/Camera.h>

#include "upcast.h"

#include <filament/Frustum.h>

#include <utils/compiler.h>
#include <utils/Entity.h>

#include <math/mat4.h>
#include <math/scalar.h>

namespace filament {

class FEngine;

/*
 * FCamera is used to easily compute the projection and view matrices
 */
class FCamera : public Camera {
public:
    // a 35mm camera has a 36x24mm wide frame size
    static constexpr const float SENSOR_SIZE = 0.024f;    // 24mm

    FCamera(FEngine& engine, utils::Entity e);

    void terminate(FEngine& engine) noexcept { }

    // sets the projection matrix
    void setProjection(Projection projection,
                       double left, double right, double bottom, double top,
                       double near, double far) noexcept;

    // sets the projection matrix
    void setProjection(double fovInDegrees, double aspect, double near, double far,
                       Fov direction = Fov::VERTICAL) noexcept;

    // sets the projection matrix
    void setLensProjection(double focalLengthInMillimeters,
            double aspect, double near, double far) noexcept;

    // Sets a custom projection matrix (sets both the viewing and culling projections).
    void setCustomProjection(math::mat4 const& projection, double near, double far) noexcept;
    void setCustomProjection(math::mat4 const& projection,
            math::mat4 const& projectionForCulling, double near, double far) noexcept;

    void setScaling(math::double2 scaling) noexcept { mScaling = scaling; }

    math::double4 getScaling() const noexcept { return math::double4{ mScaling, 1.0, 1.0 }; }

    void setShift(math::double2 shift) noexcept { mShiftCS = shift * 2.0; }

    const math::double2 getShift() const noexcept { return mShiftCS * 0.5; }

    // returns the projection matrix
    math::mat4 getProjectionMatrix() const noexcept;

    math::mat4 getCullingProjectionMatrix() const noexcept;

    float getNear() const noexcept { return mNear; }

    float getCullingFar() const noexcept { return mFar; }

    // sets the camera's view matrix (must be a rigid transform)
    void setModelMatrix(const math::mat4f& modelMatrix) noexcept;

    // sets the camera's view matrix
    void lookAt(const math::float3& eye, const math::float3& center, const math::float3& up = { 0, 1, 0 })  noexcept;

    // returns the view matrix
    math::mat4f const& getModelMatrix() const noexcept;

    // returns the inverse of the view matrix
    math::mat4f getViewMatrix() const noexcept;

    template <typename T>
    static math::details::TMat44<T> rigidTransformInverse(math::details::TMat44<T> const& v) noexcept {
        // The inverse of a rigid transform can be computed from the transpose
        //  | R T |^-1    | Rt -Rt*T |
        //  | 0 1 |     = |  0   1   |

        const math::details::TMat33<T> rt(transpose(v.upperLeft()));
        const math::details::TVec3<T> t(rt * v[3].xyz);
        return math::details::TMat44<T>(rt, -t);
    }

    math::float3 const& getPosition() const noexcept {
        return getModelMatrix()[3].xyz;
    }

    math::float3 getLeftVector() const noexcept {
        return normalize(getModelMatrix()[0].xyz);
    }

    math::float3 getUpVector() const noexcept {
        return normalize(getModelMatrix()[1].xyz);
    }

    math::float3 getForwardVector() const noexcept {
        // the camera looks towards -z
        return normalize(-getModelMatrix()[2].xyz);
    }

    float getFieldOfView(Camera::Fov direction) const noexcept {
        // note: this is meaning less for an orthographic projection
        auto const& p = getProjectionMatrix();
        switch (direction) {
            case Fov::VERTICAL:
                return std::abs(2.0f * std::atan(1.0f / float(p[1][1])));
            case Fov::HORIZONTAL:
                return std::abs(2.0f * std::atan(1.0f / float(p[0][0])));
        }
    }

    float getFieldOfViewInDegrees(Camera::Fov direction) const noexcept {
        return getFieldOfView(direction) * math::f::RAD_TO_DEG;
    }

    // returns a Frustum object in world space
    Frustum getFrustum() const noexcept;

    // sets this camera's exposure (default is f/16, 1/125s, 100 ISO)
    void setExposure(float aperture, float shutterSpeed, float sensitivity) noexcept;

    // returns this camera's aperture in f-stops
    float getAperture() const noexcept {
        return mAperture;
    }

    // returns this camera's shutter speed in seconds
    float getShutterSpeed() const noexcept {
        return mShutterSpeed;
    }

    // returns this camera's sensitivity in ISO
    float getSensitivity() const noexcept {
        return mSensitivity;
    }

    void setFocusDistance(float distance) noexcept {
        mFocusDistance = distance;
    }

    float getFocusDistance() const noexcept {
        return mFocusDistance;
    }

    double getFocalLength() const noexcept;

    static double computeEffectiveFocalLength(double focalLength, double focusDistance) noexcept;

    static double computeEffectiveFov(double fovInDegrees, double focusDistance) noexcept;

    utils::Entity getEntity() const noexcept {
        return mEntity;
    }

    static math::mat4f getViewMatrix(math::mat4f const& model) noexcept;
    static Frustum getFrustum(math::mat4 const& projection, math::mat4f const& viewMatrix) noexcept;

private:
    FEngine& mEngine;
    utils::Entity mEntity;

    math::mat4 mProjection;                // projection matrix (infinite far)
    math::mat4 mProjectionForCulling;      // projection matrix (with far plane)
    math::double2 mScaling = { 1.0f };  // additional scaling applied to projection
    math::double2 mShiftCS = { 0.0f };  // additional translation applied to projection

    float mNear{};
    float mFar{};
    // exposure settings
    float mAperture = 16.0f;
    float mShutterSpeed = 1.0f / 125.0f;
    float mSensitivity = 100.0f;
    float mFocusDistance = 0.0f;
};

struct CameraInfo {
    CameraInfo() noexcept = default;
    explicit CameraInfo(FCamera const& camera) noexcept;
    CameraInfo(FCamera const& camera, const math::mat4f& worldOriginCamera) noexcept;

    math::mat4f projection;         // projection matrix for drawing (infinite zfar)
    math::mat4f cullingProjection;  // projection matrix for culling
    math::mat4f model;              // camera model matrix
    math::mat4f view;               // camera view matrix
    float zn{};                     // distance (positive) to the near plane
    float zf{};                     // distance (positive) to the far plane
    float ev100{};                  // exposure
    float f{};                      // focal length [m]
    float A{};                      // f-number or f / aperture diameter [m]
    float d{};                      // focus distance [m]
    math::float3 worldOffset{};     // world offset, API-level camera position
    math::float3 const& getPosition() const noexcept { return model[3].xyz; }
    math::float3 getForwardVector() const noexcept { return normalize(-model[2].xyz); }

    // for debugging:
    math::mat4f worldOrigin; // this is already applied to model and view
};

FILAMENT_UPCAST(Camera)

} // namespace filament

#endif // TNT_FILAMENT_DETAILS_CAMERA_H
