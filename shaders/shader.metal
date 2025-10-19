#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float3 position [[attribute(0)]];
};

struct VertexOut {
    float4 position [[position]];
    float3 worldPos;
};

struct Uniforms {
    float4x4 modelMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
};

vertex VertexOut vertexShader(VertexIn in [[stage_in]],
                            constant Uniforms &uniforms [[buffer(1)]]) {
    VertexOut out;
    
    float4 worldPos = uniforms.modelMatrix * float4(in.position, 1.0);
    out.worldPos = worldPos.xyz;
    out.position = uniforms.projectionMatrix * uniforms.viewMatrix * worldPos;
    
    return out;
}

fragment float4 fragmentShader(VertexOut in [[stage_in]]) {
    // Normalize world position to get a color
    float3 normalizedPos = normalize(in.worldPos);
    return float4(normalizedPos * 0.5 + 0.5, 1.0);
}