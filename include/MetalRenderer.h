#pragma once

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <simd/simd.h>
@class MetalRendererDelegate;
@class MTKView;
#else
#include <simd/simd.h>
class MTKView;
#endif

#include "Camera.h"

class MetalRenderer {
 public:
  MetalRenderer();
  ~MetalRenderer();

  bool startup(MTKView* view);
  void shutdown();
  void updateUniforms(const Camera& camera, const matrix_float4x4& model);
  void drawInView(MTKView* view);  // New public method for delegate to call

 private:
  id<MTLDevice> device;
  id<MTLCommandQueue> commandQueue;
  id<MTLRenderPipelineState> pipelineState;
  id<MTLBuffer> vertexBuffer;
  id<MTLBuffer> uniformBuffer;
  MetalRendererDelegate* delegate;

  void setupPipeline(MTKView* view);
  void setupVertexBuffer();
  void setupUniformBuffer();
};