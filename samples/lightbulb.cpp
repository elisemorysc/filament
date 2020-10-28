/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <iostream>
#include <string>
#include <map>
#include <vector>

#if defined (_MSC_VER) || !defined (FILAMENT_USE_HUNTER)
#include <getopt/getopt.h>
#else
#include <getopt.h>
#endif

#include <utils/EntityManager.h>
#include <utils/Path.h>

#include <filament/Engine.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/Scene.h>
#include <filament/LightManager.h>
#include <filament/RenderableManager.h>
#include <filament/TransformManager.h>
#include <filament/VertexBuffer.h>

#include <math/norm.h>
#include <utils/Log.h>
#include <filament/View.h>

#include <filamentapp/Config.h>
#include <filamentapp/FilamentApp.h>
#include <filamentapp/MeshAssimp.h>
#include <filamentapp/Cube.h>
#include <filamentapp/IcoSphere.h>
#include <filamentapp/Sphere.h>

#include "generated/resources/resources.h"

using namespace filament::math;
using namespace filament;
using namespace filamat;
using namespace utils;

static std::vector<Path> g_filenames;

std::map<std::string, MaterialInstance*> g_materialLibrary;
std::unique_ptr<MeshAssimp> g_meshSet;
std::unique_ptr<Cube> g_meshAabb;

static std::vector<Sphere> g_spheres;
static std::vector<Entity> g_lights;

static Entity g_discoBallEntity;
static float g_discoAngle = 0;
static float g_discoAngularSpeed = 0.25f * float(M_PI);  // rad/s

static Config g_config;
static bool g_moreLights = false;
static bool g_shadowPlane = false;
static bool g_discoBall = false;


static void printUsage(char* name) {
    std::string exec_name(Path(name).getName());
    std::string usage(
            "LIGHTBULB is a command-line tool for testing the filament engine\n"
            "Usage:\n"
            "    LIGHTBULB [options] <mesh files>\n"
            "Options:\n"
            "   --help, -h\n"
            "       Prints this message\n\n"
            "   --api, -a\n"
            "       Specify the backend API: opengl (default), vulkan, or metal\n\n"
            "   --ibl=<path to cmgen IBL>, -i <path>\n"
            "       Applies an IBL generated by cmgen's deploy option\n\n"
            "   --split-view, -v\n"
            "       Splits the window into 4 views\n\n"
            "   --scale=[number], -s [number]\n"
            "       Applies uniform scale\n\n"
            "   --more-lights, -m\n"
            "       Adds a few more lights\n\n"
            "   --disco, -d\n"
            "       Add a disco ball\n\n"
            "   --shadow-plane, -p\n"
            "       Enable shadow plane\n\n"
    );
    const std::string from("LIGHTBULB");
    for (size_t pos = usage.find(from); pos != std::string::npos; pos = usage.find(from, pos)) {
        usage.replace(pos, from.length(), exec_name);
    }
    std::cout << usage;
}

static int handleCommandLineArgments(int argc, char* argv[], Config* config) {
    static constexpr const char* OPTSTR = "hi:vs:mdcpa:";
    static const struct option OPTIONS[] = {
            { "help",         no_argument,       nullptr, 'h' },
            { "api",          required_argument, nullptr, 'a' },
            { "ibl",          required_argument, nullptr, 'i' },
            { "split-view",   no_argument,       nullptr, 'v' },
            { "scale",        required_argument, nullptr, 's' },
            { "more-lights",  no_argument,       nullptr, 'm' },
            { "disco",        no_argument,       nullptr, 'd' },
            { "clear-coat",   no_argument,       nullptr, 'c' },
            { "shadow-plane", no_argument,       nullptr, 'p' },
            { nullptr, 0, nullptr, 0 }  // termination of the option list
    };
    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, OPTSTR, OPTIONS, &option_index)) >= 0) {
        std::string arg(optarg != nullptr ? optarg : "");
        switch (opt) {
            default:
            case 'h':
                printUsage(argv[0]);
                exit(0);
            case 'a':
                if (arg == "opengl") {
                    config->backend = Engine::Backend::OPENGL;
                } else if (arg == "vulkan") {
                    config->backend = Engine::Backend::VULKAN;
                } else if (arg == "metal") {
                    config->backend = Engine::Backend::METAL;
                } else {
                    std::cerr << "Unrecognized backend. Must be 'opengl'|'vulkan'|'metal'." << std::endl;
                }
                break;
            case 'i':
                config->iblDirectory = arg;
                break;
            case 's':
                try {
                    config->scale = std::stof(arg);
                } catch (std::invalid_argument& e) {
                    // keep scale of 1.0
                } catch (std::out_of_range& e) {
                    // keep scale of 1.0
                }
                break;
            case 'v':
                config->splitView = true;
                break;
            case 'm':
                g_moreLights = true;
                break;
            case 'd':
                g_discoBall = true;
                break;
            case 'p':
                g_shadowPlane = true;
                break;
        }
    }

    return optind;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
static void cleanup(Engine* engine, View* view, Scene* scene) {
    for (auto& item : g_materialLibrary) {
        auto materialInstance = item.second;
        engine->destroy(materialInstance);
    }
    g_meshSet.reset(nullptr);
    g_meshAabb.reset(nullptr);

    auto& em = EntityManager::get();

    g_spheres.clear();

    for (Entity e : g_lights) {
        engine->destroy(e);
        em.destroy(e);
    }
    if (g_discoBallEntity) {
        em.destroy(g_discoBallEntity);
    }
}
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
static void setup(Engine* engine, View* view, Scene* scene) {
    g_meshSet.reset(new MeshAssimp(*engine));
    for (auto& filename : g_filenames) {
        g_meshSet->addFromFile(filename, g_materialLibrary);
    }

    auto& lcm = engine->getLightManager();

    // set the transform on the root node only
    auto& tcm = engine->getTransformManager();
    auto ti = tcm.getInstance(g_meshSet->getRenderables()[0]);
    tcm.setTransform(ti, mat4f{ mat3f(g_config.scale), float3(0.0f, 0.0f, -4.0f) } *
                         tcm.getWorldTransform(ti));

    auto& rcm = engine->getRenderableManager();
    for (auto renderable : g_meshSet->getRenderables()) {
        if (rcm.hasComponent(renderable)) {
            auto ri = rcm.getInstance(renderable);
            auto blendingMode = rcm.getMaterialInstanceAt(ri, 0)->getMaterial()->getBlendingMode();
            rcm.setCastShadows(ri,
                    blendingMode == BlendingMode::OPAQUE || blendingMode == BlendingMode::MASKED);
            scene->addEntity(renderable);
        }
    }

    auto& em = EntityManager::get();
    g_lights.push_back(em.create());

    LightManager::Builder(LightManager::Type::SUN)
            .color(Color::toLinear<ACCURATE>(sRGBColor(0.98f, 0.92f, 0.89f)))
            .intensity(110000)
            .direction({ 0.7, -1, -0.8 })
            .sunAngularRadius(1.9f)
            .castShadows(true)
            .build(*engine, g_lights.back());
    scene->addEntity(g_lights.back());

    lcm.setShadowOptions(lcm.getInstance(g_lights[0]), {
            .screenSpaceContactShadows = true });

    view->setAmbientOcclusionOptions({ .enabled = true });
    view->setBloomOptions({ .enabled = true });
    view->setFogOptions({ .density = 0.2f, .enabled = true });


    if (g_moreLights) {
    g_lights.push_back(em.create());
        LightManager::Builder(LightManager::Type::POINT)
                .color(Color::toLinear<ACCURATE>(sRGBColor(0.98f, 0.92f, 0.89f)))
                .intensity(1000.0f, LightManager::EFFICIENCY_LED)
                .position({ 0.0f, -0.2f, -3.0f })
                .falloff(4.0f)
                .build(*engine, g_lights.back());

        g_lights.push_back(em.create());
        LightManager::Builder(LightManager::Type::FOCUSED_SPOT)
                .color(Color::toLinear<ACCURATE>(sRGBColor(0.98f, 0.12f, 0.19f)))
                .intensity(2000.0f, LightManager::EFFICIENCY_LED)
                .position({ 0.6f, 0.6f, -3.2f })
                .direction({ -1.0f, 0.0f, 0.0f })
                .spotLightCone(static_cast<float>(M_PI / 8), static_cast<float>((M_PI / 8) * 1.1))
                .falloff(4.0f)
                .castShadows(true)
                .build(*engine, g_lights.back());

        g_lights.push_back(em.create());
        LightManager::Builder(LightManager::Type::POINT)
                .color(Color::toLinear<ACCURATE>(sRGBColor(0.18f, 0.12f, 0.89f)))
                .intensity(1000.0f, LightManager::EFFICIENCY_LED)
                .position({ -0.6f, 0.3f, -3.2f })
                .falloff(2.0f)
                .build(*engine, g_lights.back());

        g_lights.push_back(em.create());
        LightManager::Builder(LightManager::Type::POINT)
                .color(Color::toLinear<ACCURATE>(sRGBColor(0.88f, 0.82f, 0.29f)))
                .intensity(1000.0f, LightManager::EFFICIENCY_LED)
                .position({ 0.0f, 1.5f, -3.5f })
                .falloff(2.0f)
                .build(*engine, g_lights.back());

        g_lights.push_back(em.create());
        LightManager::Builder(LightManager::Type::FOCUSED_SPOT)
                .color(Color::toLinear<ACCURATE>(sRGBColor(0.12f, 0.98f, 0.19f)))
                .intensity(2000.0f, LightManager::EFFICIENCY_LED)
                .position({ 0.0f, 0.6f, -3.2f })
                .direction({  1.0f, 0.0f, 0.0f })
                .spotLightCone(static_cast<float>(M_PI / 8), static_cast<float>((M_PI / 8) * 1.1))
                .falloff(4.0f)
                .castShadows(true)
                .build(*engine, g_lights.back());

        for (const auto& light : g_lights) {
            scene->addEntity(light);
            const LightManager::Instance& instance = lcm.getInstance(light);
            if (!lcm.isDirectional(instance)) {
                g_spheres.emplace_back(*engine, FilamentApp::get().getDefaultMaterial());
                g_spheres.back()
                         .setRadius(0.025f)
                         .setPosition(lcm.getPosition(instance));

                auto mi = g_spheres.back().getMaterialInstance();
                mi->setParameter("baseColor", RgbaType::LINEAR, LinearColorA{ lcm.getColor(instance), 100.0f });
                mi->setParameter("roughness", 0.2f);
                mi->setParameter("metallic", 0.0f);
            }
        }

        for (const auto& sphere : g_spheres) {
            scene->addEntity(sphere.getSolidRenderable());
        }
    }

    if (g_discoBall) {
        IcoSphere sphere(2);
        auto const& vertices = sphere.getVertices();
        auto n = vertices.size();
        g_lights.resize(g_lights.size() + n);
        slog.d << "light count = " << n << io::endl;

        Entity discoBall = em.create();
        tcm.create(discoBall, {}, mat4f::translation(float3{ 0, 3, -14 }));
        auto parent = tcm.getInstance(discoBall);

        for (size_t i = 0, c = n; i < c; i++) {
            g_lights.push_back(em.create());
            LightManager::Builder(LightManager::Type::FOCUSED_SPOT)
                    .color(abs(vertices[i]))
                    .intensity(1000.0f, LightManager::EFFICIENCY_HALOGEN)
                    .direction(vertices[i])
                    .spotLightCone(0.0174f * 0.5f, 0.0174f * 2.0f)
                    .falloff(20.0f)
                    .build(*engine, g_lights.back());

            tcm.create(g_lights.back(), parent, {});

            scene->addEntity(g_lights.back());
        }
        g_discoBallEntity = discoBall;

        g_spheres.emplace_back(*engine, FilamentApp::get().getDefaultMaterial());
        g_spheres.back().setRadius(0.2f);
        auto mi = g_spheres.back().getMaterialInstance();
        mi->setParameter("baseColor", RgbaType::LINEAR, LinearColorA{ 1, 1, 1, 1 });
        mi->setParameter("roughness", 0.01f);
        mi->setParameter("metallic", 1.0f);
        scene->addEntity(g_spheres.back().getSolidRenderable());
        // parent the sphere representing the disco ball to the discoball transform -- w00t.
        tcm.setParent(tcm.getInstance(g_spheres.back().getSolidRenderable()), tcm.getInstance(discoBall));
    }

    g_meshAabb.reset(new Cube(*engine, FilamentApp::get().getTransparentMaterial(), {0,0,1}));

    // First object in the scene
    Entity object = g_meshSet->getRenderables()[1];

    // Get its AABB and add it to the scene
    RenderableManager::Instance ri = rcm.getInstance(object);
    if (ri) {
        g_meshAabb->mapAabb(*engine, rcm.getAxisAlignedBoundingBox(ri));
        scene->addEntity(g_meshAabb->getWireFrameRenderable());
        scene->addEntity(g_meshAabb->getSolidRenderable());
    }

    // now transform the aabb's renderable by the object's transform
    tcm.setParent(tcm.getInstance(g_meshAabb->getSolidRenderable()),  tcm.getInstance(object));
    tcm.setParent(tcm.getInstance(g_meshAabb->getWireFrameRenderable()),  tcm.getInstance(object));
    rcm.setLayerMask(rcm.getInstance(g_meshAabb->getSolidRenderable()), 0x3, 0x2);
    rcm.setLayerMask(rcm.getInstance(g_meshAabb->getWireFrameRenderable()), 0x3, 0x2);

    if (g_shadowPlane) {
        Material* shadowMaterial = Material::Builder()
                .package(RESOURCES_GROUNDSHADOW_DATA, RESOURCES_GROUNDSHADOW_SIZE)
                .build(*engine);
        shadowMaterial->setDefaultParameter("strength", 0.7f);

        const static uint32_t indices[] = {
                0, 1, 2, 2, 3, 0
        };

        const static filament::math::float3 vertices[] = {
                { -10, 0, -10 },
                { -10, 0,  10 },
                {  10, 0,  10 },
                {  10, 0, -10 },
        };

        short4 tbn = filament::math::packSnorm16(
                mat3f::packTangentFrame(
                        filament::math::mat3f{
                                float3{ 1.0f, 0.0f, 0.0f },
                                float3{ 0.0f, 0.0f, 1.0f },
                                float3{ 0.0f, 1.0f, 0.0f }
                        }
                ).xyzw);

        const static filament::math::short4 normals[] { tbn, tbn, tbn, tbn };

        VertexBuffer* vertexBuffer = VertexBuffer::Builder()
                .vertexCount(4)
                .bufferCount(2)
                .attribute(VertexAttribute::POSITION, 0, VertexBuffer::AttributeType::FLOAT3)
                .attribute(VertexAttribute::TANGENTS, 1, VertexBuffer::AttributeType::SHORT4)
                .normalized(VertexAttribute::TANGENTS)
                .build(*engine);

        vertexBuffer->setBufferAt(*engine, 0, VertexBuffer::BufferDescriptor(
                vertices, vertexBuffer->getVertexCount() * sizeof(vertices[0])));
        vertexBuffer->setBufferAt(*engine, 1, VertexBuffer::BufferDescriptor(
                normals, vertexBuffer->getVertexCount() * sizeof(normals[0])));

        IndexBuffer* indexBuffer = IndexBuffer::Builder()
                .indexCount(6)
                .build(*engine);

        indexBuffer->setBuffer(*engine, IndexBuffer::BufferDescriptor(
                indices, indexBuffer->getIndexCount() * sizeof(uint32_t)));

        Entity planeRenderable = em.create();
        RenderableManager::Builder(1)
                .boundingBox({{ 0, 0, 0 },
                              { 10, 1e-4f, 10 }})
                .material(0, shadowMaterial->getDefaultInstance())
                .geometry(0, RenderableManager::PrimitiveType::TRIANGLES,
                        vertexBuffer, indexBuffer, 0, 6)
                .culling(false)
                .receiveShadows(true)
                .castShadows(false)
                .build(*engine, planeRenderable);

        scene->addEntity(planeRenderable);

        tcm.setTransform(tcm.getInstance(planeRenderable),
                filament::math::mat4f::translation(float3{ 0, -1, -4 }));
    }
}
#pragma clang diagnostic pop

int main(int argc, char* argv[]) {
    int option_index = handleCommandLineArgments(argc, argv, &g_config);
    int num_args = argc - option_index;
    if (num_args < 1) {
        printUsage(argv[0]);
        return 1;
    }

    for (int i = option_index; i < argc; i++) {
        utils::Path filename = argv[i];
        if (!filename.exists()) {
            std::cerr << "file " << argv[i] << " not found!" << std::endl;
            return 1;
        }
        g_filenames.push_back(filename);
    }

    g_config.title = "Lightbulb";
    FilamentApp& filamentApp = FilamentApp::get();

    double lastTime = 0;
    if (g_discoBall) {
        filamentApp.animate([lastTime](filament::Engine* engine, filament::View*, double now) mutable {
            auto& tcm = engine->getTransformManager();
            tcm.setTransform(tcm.getInstance(g_discoBallEntity),
                    mat4f::translation(float3{ 0, 2, -4, }) *
                                                            mat4f::rotation(g_discoAngle,
                                                                    float3{ 0, 1, 0 })
            );
            if (lastTime != 0) {
                double dT = now - lastTime;
                g_discoAngle += g_discoAngularSpeed * dT;
            }
            lastTime = now;
        });
    }

    filamentApp.run(g_config, setup, cleanup);

    return 0;
}
