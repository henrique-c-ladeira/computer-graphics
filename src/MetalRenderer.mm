#import "MetalRenderer.h"
#include <vector>

// Vertex data matching the OpenGL cube
static const std::vector<float> cubeVertices = {
    // front face
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    // back face
    -0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    // left face
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    // right face
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
    // top face
    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
    // bottom face
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f
};

struct Uniforms {
    matrix_float4x4 modelMatrix;
    matrix_float4x4 viewMatrix;
    matrix_float4x4 projectionMatrix;
};

@interface MetalRendererDelegate : NSObject<MTKViewDelegate>
- (instancetype)initWithRenderer:(MetalRenderer*)renderer device:(id<MTLDevice>)device;
@end

@implementation MetalRendererDelegate {
    MetalRenderer* _renderer;
    id<MTLDevice> _device;
}

- (instancetype)initWithRenderer:(MetalRenderer*)renderer device:(id<MTLDevice>)device {
    self = [super init];
    if (self) {
        _renderer = renderer;
        _device = device;
    }
    return self;
}

- (void)drawInMTKView:(MTKView*)view {
    if (_renderer) {
        _renderer->drawInView(view);
    }
}

- (void)mtkView:(MTKView*)view drawableSizeWillChange:(CGSize)size {
    // Handle resize if needed
}
@end

void MetalRenderer::drawInView(MTKView* view) {
    id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
    MTLRenderPassDescriptor* rpd = view.currentRenderPassDescriptor;
    
    if (rpd != nil) {
        id<MTLRenderCommandEncoder> encoder = [commandBuffer renderCommandEncoderWithDescriptor:rpd];
        [encoder setRenderPipelineState:pipelineState];
        [encoder setVertexBuffer:vertexBuffer offset:0 atIndex:0];
        [encoder setVertexBuffer:uniformBuffer offset:0 atIndex:1];
        [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:36];
        [encoder endEncoding];
        
        [commandBuffer presentDrawable:view.currentDrawable];
    }
    
    [commandBuffer commit];
}

MetalRenderer::MetalRenderer() : device(nil), commandQueue(nil), pipelineState(nil),
    vertexBuffer(nil), uniformBuffer(nil), delegate(nil) {}

MetalRenderer::~MetalRenderer() {
    shutdown();
}

bool MetalRenderer::startup(MTKView* view) {
    device = MTLCreateSystemDefaultDevice();
    if (!device) return false;
    
    view.device = device;
    view.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
    view.depthStencilPixelFormat = MTLPixelFormatDepth32Float;
    view.clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);
    
    commandQueue = [device newCommandQueue];
    if (!commandQueue) return false;
    
    setupPipeline(view);
    setupVertexBuffer();
    setupUniformBuffer();
    
    delegate = [[MetalRendererDelegate alloc] initWithRenderer:this device:device];
    view.delegate = delegate;
    
    return true;
}

void MetalRenderer::shutdown() {
    if (delegate) {
        [delegate release];
        delegate = nil;
    }
    if (vertexBuffer) {
        [vertexBuffer release];
        vertexBuffer = nil;
    }
    if (uniformBuffer) {
        [uniformBuffer release];
        uniformBuffer = nil;
    }
    if (pipelineState) {
        [pipelineState release];
        pipelineState = nil;
    }
    if (commandQueue) {
        [commandQueue release];
        commandQueue = nil;
    }
    // device is owned by the system, don't release it
    device = nil;
}

void MetalRenderer::setupPipeline(MTKView* view) {
    NSError* error = nil;
    NSString* path = [[NSBundle mainBundle] pathForResource:@"shader" ofType:@"metallib" inDirectory:@"shaders"];
    if (!path) {
        // Try current directory if not found in bundle
        path = [NSString stringWithFormat:@"%@/shaders/shader.metallib", [[NSFileManager defaultManager] currentDirectoryPath]];
    }
    
    id<MTLLibrary> library = [device newLibraryWithFile:path error:&error];
    if (!library) {
        NSLog(@"Failed to load Metal shader library from path %@: %@", path, error);
        return;
    }
    
    id<MTLFunction> vertexFunc = [library newFunctionWithName:@"vertexShader"];
    id<MTLFunction> fragFunc = [library newFunctionWithName:@"fragmentShader"];
    
    MTLVertexDescriptor* vertexDesc = [MTLVertexDescriptor vertexDescriptor];
    vertexDesc.attributes[0].format = MTLVertexFormatFloat3;
    vertexDesc.attributes[0].offset = 0;
    vertexDesc.attributes[0].bufferIndex = 0;
    vertexDesc.layouts[0].stride = sizeof(float) * 3;
    
    MTLRenderPipelineDescriptor* pipelineDesc = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineDesc.vertexFunction = vertexFunc;
    pipelineDesc.fragmentFunction = fragFunc;
    pipelineDesc.vertexDescriptor = vertexDesc;
    pipelineDesc.colorAttachments[0].pixelFormat = view.colorPixelFormat;
    pipelineDesc.depthAttachmentPixelFormat = view.depthStencilPixelFormat;
    
    pipelineState = [device newRenderPipelineStateWithDescriptor:pipelineDesc error:&error];
    if (!pipelineState) {
        NSLog(@"Failed to create pipeline state: %@", error);
    }
    
    [pipelineDesc release];
    [library release];
}

void MetalRenderer::setupVertexBuffer() {
    vertexBuffer = [device newBufferWithBytes:cubeVertices.data()
                                     length:cubeVertices.size() * sizeof(float)
                                    options:MTLResourceStorageModeShared];
}

void MetalRenderer::setupUniformBuffer() {
    uniformBuffer = [device newBufferWithLength:sizeof(Uniforms)
                                      options:MTLResourceStorageModeShared];
}

void MetalRenderer::updateUniforms(const Camera& camera, const matrix_float4x4& model) {
    Uniforms* uniforms = (Uniforms*)uniformBuffer.contents;
    uniforms->modelMatrix = model;
    uniforms->viewMatrix = camera.getViewMatrix();
    uniforms->projectionMatrix = camera.getProjectionMatrix();
}