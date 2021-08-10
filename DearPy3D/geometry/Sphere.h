#pragma once

#include <vector>

namespace DearPy3D {

    class Sphere
    {
    public:
    // ctor/dtor
    Sphere(float radius=1.0f, int sectorCount=36, int stackCount=18, bool smooth=true);

    void set(float radius, int sectorCount, int stackCount, bool smooth=true);

    // for vertex data
    uint16_t getVertexCount() const     { return (uint16_t)vertices.size() / 3; }
    uint16_t getNormalCount() const     { return (uint16_t)normals.size() / 3; }
    uint16_t getTexCoordCount() const   { return (uint16_t)texCoords.size() / 2; }
    uint16_t getIndexCount() const      { return (uint16_t)indices.size(); }
    uint16_t getLineIndexCount() const  { return (uint16_t)lineIndices.size(); }
    uint16_t getTriangleCount() const   { return getIndexCount() / 3; }
    uint16_t getVertexSize() const      { return (uint16_t)vertices.size() * sizeof(float); }
    uint16_t getNormalSize() const      { return (uint16_t)normals.size() * sizeof(float); }
    uint16_t getTexCoordSize() const    { return (uint16_t)texCoords.size() * sizeof(float); }
    uint16_t getIndexSize() const       { return (uint16_t)indices.size() * sizeof(uint16_t); }
    uint16_t getLineIndexSize() const   { return (uint16_t)lineIndices.size() * sizeof(uint16_t); }
    const float* getVertices() const        { return vertices.data(); }
    const float* getNormals() const         { return normals.data(); }
    const float* getTexCoords() const       { return texCoords.data(); }
    const uint16_t* getIndices() const  { return indices.data(); }
    const uint16_t* getLineIndices() const  { return lineIndices.data(); }

    // for interleaved vertices: V/N/T
    unsigned int getInterleavedVertexCount() const  { return getVertexCount(); }    // # of vertices
    unsigned int getInterleavedVertexSize() const   { return (uint16_t)interleavedVertices.size() * sizeof(float); }    // # of bytes
    int getInterleavedStride() const                { return interleavedStride; }   // should be 32 bytes
    const float* getInterleavedVertices() const     { return interleavedVertices.data(); }


    protected:

    public:
    // member functions
    void buildVerticesSmooth();
    void buildVerticesFlat();
    void buildInterleavedVertices();
    void clearArrays();
    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addTexCoord(float s, float t);
    void addIndices(uint16_t i1, uint16_t i2, uint16_t i3);
    std::vector<float> computeFaceNormal(float x1, float y1, float z1,
                                            float x2, float y2, float z2,
                                            float x3, float y3, float z3);

    // memeber vars
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    bool smooth;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<uint16_t> indices;
    std::vector<uint16_t> lineIndices;

    // interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)

    };

}
