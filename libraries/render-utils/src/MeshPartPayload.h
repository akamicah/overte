//
//  MeshPartPayload.h
//  interface/src/renderer
//
//  Created by Sam Gateau on 10/3/15.
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_MeshPartPayload_h
#define hifi_MeshPartPayload_h

#include <Interpolate.h>

#include <gpu/Batch.h>

#include <render/Scene.h>
#include <render/ShapePipeline.h>

#include <model/Geometry.h>

#include "Model.h"

class Model;

class MeshPartPayload {
public:
    MeshPartPayload() {}
    MeshPartPayload(const std::shared_ptr<const model::Mesh>& mesh, int partIndex, model::MaterialPointer material);

    typedef render::Payload<MeshPartPayload> Payload;
    typedef Payload::DataPointer Pointer;

    virtual void updateMeshPart(const std::shared_ptr<const model::Mesh>& drawMesh, int partIndex);

    virtual void notifyLocationChanged() {}
    void updateTransform(const Transform& transform, const Transform& offsetTransform);

    virtual void updateMaterial(model::MaterialPointer drawMaterial);

    // Render Item interface
    virtual render::ItemKey getKey() const;
    virtual render::Item::Bound getBound() const;
    virtual render::ShapeKey getShapeKey() const; // shape interface
    virtual void render(RenderArgs* args);

    // ModelMeshPartPayload functions to perform render
    void drawCall(gpu::Batch& batch) const;
    virtual void bindMesh(gpu::Batch& batch);
    virtual void bindMaterial(gpu::Batch& batch, const render::ShapePipeline::LocationsPointer locations, bool enableTextures) const;
    virtual void bindTransform(gpu::Batch& batch, const render::ShapePipeline::LocationsPointer locations, RenderArgs::RenderMode renderMode) const;

    // Payload resource cached values
    Transform _drawTransform;
    Transform _transform;
    int _partIndex = 0;
    bool _hasColorAttrib { false };

    model::Box _localBound;
    model::Box _adjustedLocalBound;
    mutable model::Box _worldBound;
    std::shared_ptr<const model::Mesh> _drawMesh;

    std::shared_ptr<const model::Material> _drawMaterial;
    model::Mesh::Part _drawPart;

    size_t getVerticesCount() const { return _drawMesh ? _drawMesh->getNumVertices() : 0; }
    size_t getMaterialTextureSize() { return _drawMaterial ? _drawMaterial->getTextureSize() : 0; }
    int getMaterialTextureCount() { return _drawMaterial ? _drawMaterial->getTextureCount() : 0; }
    bool hasTextureInfo() const { return _drawMaterial ? _drawMaterial->hasTextureInfo() : false; }
};

namespace render {
    template <> const ItemKey payloadGetKey(const MeshPartPayload::Pointer& payload);
    template <> const Item::Bound payloadGetBound(const MeshPartPayload::Pointer& payload);
    template <> const ShapeKey shapeGetShapeKey(const MeshPartPayload::Pointer& payload);
    template <> void payloadRender(const MeshPartPayload::Pointer& payload, RenderArgs* args);
}

class ModelMeshPartPayload : public MeshPartPayload {
public:
    ModelMeshPartPayload(ModelPointer model, int meshIndex, int partIndex, int shapeIndex, const Transform& transform, const Transform& offsetTransform);

    typedef render::Payload<ModelMeshPartPayload> Payload;
    typedef Payload::DataPointer Pointer;

    void notifyLocationChanged() override;
    void updateClusterBuffer(const std::vector<glm::mat4>& clusterMatrices);
    void updateTransformForSkinnedMesh(const Transform& renderTransform, const Transform& boundTransform);

    // Render Item interface
    render::ItemKey getKey() const override;
    int getLayer() const;
    render::ShapeKey getShapeKey() const override; // shape interface
    void render(RenderArgs* args) override;

    void setKey(bool isVisible, bool isLayered);
    void setLayer(bool isLayeredInFront, bool isLayeredInHUD);
    void setShapeKey(bool invalidateShapeKey, bool isWireframe);

    // ModelMeshPartPayload functions to perform render
    void bindMesh(gpu::Batch& batch) override;
    void bindTransform(gpu::Batch& batch, const render::ShapePipeline::LocationsPointer locations, RenderArgs::RenderMode renderMode) const override;

    void computeAdjustedLocalBound(const std::vector<glm::mat4>& clusterMatrices);

    gpu::BufferPointer _clusterBuffer;

    int _meshIndex;
    int _shapeID;

    bool _isSkinned{ false };
    bool _isBlendShaped { false };
    bool _hasTangents { false };

private:
    void initCache(const ModelPointer& model);

    gpu::BufferPointer _blendedVertexBuffer;
    render::ItemKey _itemKey { render::ItemKey::Builder::opaqueShape().build() };
    render::ShapeKey _shapeKey { render::ShapeKey::Builder::invalid() };
    int _layer { render::Item::LAYER_3D };
};

namespace render {
    template <> const ItemKey payloadGetKey(const ModelMeshPartPayload::Pointer& payload);
    template <> const Item::Bound payloadGetBound(const ModelMeshPartPayload::Pointer& payload);
    template <> int payloadGetLayer(const ModelMeshPartPayload::Pointer& payload);
    template <> const ShapeKey shapeGetShapeKey(const ModelMeshPartPayload::Pointer& payload);
    template <> void payloadRender(const ModelMeshPartPayload::Pointer& payload, RenderArgs* args);
}

#endif // hifi_MeshPartPayload_h
