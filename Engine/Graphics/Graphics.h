//
// Urho3D Engine
// Copyright (c) 2008-2011 Lasse ��rni
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "Color.h"
#include "Object.h"
#include "Rect.h"
#include "GraphicsDefs.h"

class Image;
class IndexBuffer;
class Matrix3;
class Matrix4;
class Matrix4x3;
class PixelShader;
class GPUObject;
class GraphicsImpl;
class RenderSurface;
class Texture;
class Texture2D;
class TextureCube;
class Vector3;
class Vector4;
class VertexBuffer;
class VertexDeclaration;
class VertexShader;

static const int IMMEDIATE_BUFFER_DEFAULT_SIZE = 1024;
static const int NUM_SCREEN_BUFFERS = 2;

/// Graphics subsystem. Manages the Direct3D9 device, application window, rendering state and GPU resources
class Graphics : public Object
{
    OBJECT(Graphics);
    
public:
    /// Construct
    Graphics(Context* context);
    /// Destruct. Close the window and release the Direct3D9 device 
    virtual ~Graphics();
    
    /// Pump operating system messages
    void MessagePump();
    /// Set window title
    void SetWindowTitle(const String& windowTitle);
    /// Set screen mode. In deferred rendering modes multisampling means edge filtering instead of MSAA
    bool SetMode(RenderMode mode, int width, int height, bool fullscreen, bool vsync, int multiSample);
    /// Set screen resolution only
    bool SetMode(int width, int height);
    /// Set rendering mode only
    bool SetMode(RenderMode mode);
    /// Toggle between full screen and windowed mode
    bool ToggleFullscreen();
    /// Close the window
    void Close();
    /// Take a screenshot
    bool TakeScreenShot(Image& destImage);
    /// Set whether to flush GPU command queue at the end of each frame. Default true
    void SetFlushGPU(bool enable);
    
    /// Begin frame rendering
    bool BeginFrame();
    /// End frame rendering and swap buffers
    void EndFrame();
    /// Clear any or all of render target, depth buffer and stencil buffer
    void Clear(unsigned flags, const Color& color = Color(0.0f, 0.0f, 0.0f, 0.0f), float depth = 1.0f, unsigned stencil = 0);
    /// Draw non-indexed geometry
    void Draw(PrimitiveType type, unsigned vertexStart, unsigned vertexCount);
    /// Draw indexed geometry
    void Draw(PrimitiveType type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount);
    /// Draw indexed, instanced geometry. An instancing vertex buffer must be set
    void DrawInstanced(PrimitiveType type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount, unsigned instanceCount);
    /// Set vertex buffer
    void SetVertexBuffer(VertexBuffer* buffer);
    /// Set multiple vertex buffers
    bool SetVertexBuffers(const Vector<VertexBuffer*>& buffers, const PODVector<unsigned>& elementMasks, unsigned instanceOffset = 0);
    /// Set multiple vertex buffers
    bool SetVertexBuffers(const Vector<SharedPtr<VertexBuffer> >& buffers, const PODVector<unsigned>& elementMasks, unsigned instanceOffset = 0);
    /// Set index buffer
    void SetIndexBuffer(IndexBuffer* buffer);
    /// Set shaders
    void SetShaders(VertexShader* vs, PixelShader* ps);
    /// Set vertex shader bool parameter
    void SetVertexShaderParameter(VSParameter param, const bool* data, unsigned count);
    /// Set vertex shader float constants
    void SetVertexShaderParameter(VSParameter param, const float* data, unsigned count);
    /// Set vertex shader int constants
    void SetVertexShaderParameter(VSParameter param, const int* data, unsigned count);
    /// Set vertex shader float constant
    void SetVertexShaderParameter(VSParameter param, float value);
    /// Set vertex shader color constant
    void SetVertexShaderParameter(VSParameter param, const Color& color);
    /// Set vertex shader 3x3 matrix constant
    void SetVertexShaderParameter(VSParameter param, const Matrix3& matrix);
    /// Set vertex shader 3D vector constant
    void SetVertexShaderParameter(VSParameter param, const Vector3& vector);
    /// Set vertex shader 4x4 matrix constant
    void SetVertexShaderParameter(VSParameter param, const Matrix4& matrix);
    /// Set vertex shader 4D vector constant
    void SetVertexShaderParameter(VSParameter param, const Vector4& vector);
    /// Set vertex shader 4x3 matrix constant
    void SetVertexShaderParameter(VSParameter param, const Matrix4x3& matrix);
    /// Set pixel shader bool constants
    void SetPixelShaderParameter(PSParameter param, const bool* data, unsigned count);
    /// Set pixel shader float constants
    void SetPixelShaderParameter(PSParameter param, const float* data, unsigned count);
    /// Set pixel shader int constants
    void SetPixelShaderParameter(PSParameter param, const int* data, unsigned count);
    /// Set pixel shader float constant
    void SetPixelShaderParameter(PSParameter param, float value);
    /// Set pixel shader color constant
    void SetPixelShaderParameter(PSParameter param, const Color& color);
    /// Set pixel shader 3x3 matrix constant
    void SetPixelShaderParameter(PSParameter param, const Matrix3& matrix);
    /// Set pixel shader 3D vector constant
    void SetPixelShaderParameter(PSParameter param, const Vector3& vector);
     /// Set pixel shader 4x4 matrix constant
    void SetPixelShaderParameter(PSParameter param, const Matrix4& matrix);
    /// Set pixel shader 3D vector constant
    void SetPixelShaderParameter(PSParameter param, const Vector4& vector);
    /// Set pixel shader 4x3 matrix constant
    void SetPixelShaderParameter(PSParameter param, const Matrix4x3& matrix);
    /// Set vertex shader parameter source. Called by VertexShader
    void SetVSParameterSource(VSParameter param, const void* source) { lastVSParameterSources_[param] = source; }
    /// Set pixel shader parameter source. Called by PixelShader
    void SetPSParameterSource(PSParameter param, const void* source) { lastPSParameterSources_[param] = source; }
    /// Map vertex shader parameter to a constant Register. Called by VertexShader
    void SetVSRegister(VSParameter param, unsigned index) { vsRegisters_[param] = index; }
    /// Map pixel shader parameter to a constant Register. Called by PixelShader
    void SetPSRegister(PSParameter param, unsigned index) { psRegisters_[param] = index; }
    /// Clear remembered shader parameter sources
    void ClearLastParameterSources();
    /// Clear remembered transform shader parameter sources
    void ClearTransformSources();
    /// Set texture
    void SetTexture(unsigned index, Texture* texture);
    /// Set default texture filtering mode
    void SetDefaultTextureFilterMode(TextureFilterMode mode);
    /// Reset all render targets and depth buffer (render to back buffer and back buffer depth stencil)
    void ResetRenderTargets();
    /// Reset specific render target
    void ResetRenderTarget(unsigned index);
    /// Reset depth stencil
    void ResetDepthStencil();
    /// Set render target
    void SetRenderTarget(unsigned index, RenderSurface* renderTarget);
    /// Set render target
    void SetRenderTarget(unsigned index, Texture2D* renderTexture);
    /// Set depth stencil buffer
    void SetDepthStencil(RenderSurface* depthStencil);
    /// Set depth stencil buffer
    void SetDepthStencil(Texture2D* depthTexture);
    /// Set viewport
    void SetViewport(const IntRect& rect);
    /// Set "view texture" to prevent sampling from the destination render target
    void SetViewTexture(Texture* texture);
    /// Set alpha test
    void SetAlphaTest(bool enable, CompareMode mode = CMP_ALWAYS, float alphaRef = 0.5f);
    /// Set texture anisotropy
    void SetTextureAnisotropy(unsigned level);
    /// Set blending mode
    void SetBlendMode(BlendMode mode);
    /// Set color write on/off
    void SetColorWrite(bool enable);
    /// Set hardware culling mode
    void SetCullMode(CullMode mode);
    /// Set depth bias
    void SetDepthBias(float constantBias, float slopeScaledBias);
    /// Set depth compare
    void SetDepthTest(CompareMode mode);
    /// Set depth write on/off
    void SetDepthWrite(bool enable);
    /// Set polygon fill mode
    void SetFillMode(FillMode mode);
    /// Set scissor test
    void SetScissorTest(bool enable, const Rect& rect = Rect::FULL, bool borderInclusive = true);
    /// Set scissor test
    void SetScissorTest(bool enable, const IntRect& rect);
    /// Set stencil test
    void SetStencilTest(bool enable, CompareMode mode = CMP_ALWAYS, StencilOp pass = OP_KEEP, StencilOp fail = OP_KEEP, StencilOp zFail = OP_KEEP, unsigned stencilRef = 0, unsigned stencilMask = M_MAX_UNSIGNED);
    /// Set vertex buffer stream frequency
    void SetStreamFrequency(unsigned index, unsigned frequency);
    /// Reset stream frequencies
    void ResetStreamFrequencies();
    /// Begin immediate rendering command
    bool BeginImmediate(PrimitiveType type, unsigned vertexCount, unsigned elementMask);
    /// Define immediate vertex
    bool DefineVertex(const Vector3& vertex);
    /// Define immediate normal
    bool DefineNormal(const Vector3& normal);
    /// Define immediate texture coordinate
    bool DefineTexCoord(const Vector2& texCoord);
    /// Define immediate color
    bool DefineColor(const Color& color);
    /// Define immediate color
    bool DefineColor(unsigned color);
    /// End immediate rendering command and render
    void EndImmediate();
    /// Set force Shader Model 2 flag. Needs to be set before setting initial screen mode to have effect.
    void SetForceSM2(bool enable);
    
    /// Return whether rendering initialized
    bool IsInitialized() const;
    /// Return graphics implementation, which holds the actual Direct3D resources
    GraphicsImpl* GetImpl() const { return impl_; }
    /// Return window title
    const String& GetWindowTitle() const { return windowTitle_; }
    /// Return rendering mode
    RenderMode GetRenderMode() const { return mode_; }
    /// Return window width
    int GetWidth() const { return width_; }
    /// Return window height
    int GetHeight() const { return height_; }
    /// Return multisample mode (0 = no multisampling)
    int GetMultiSample() const { return multiSample_; }
    /// Return whether window is fullscreen
    bool GetFullscreen() const { return fullscreen_; }
    /// Return whether vertical sync is on
    bool GetVSync() const { return vsync_; }
    /// Return whether GPU command queue is flushed at the end of each frame
    bool GetFlushGPU() const { return flushGPU_; }
    /// Return whether Direct3D device is lost, and can not yet render. This happens during fullscreen resolution switching
    bool IsDeviceLost() const { return deviceLost_; }
    /// Return immediate rendering data pointer
    unsigned char* GetImmediateDataPtr() const;
    /// Return window handle
    unsigned GetWindowHandle() const;
    /// Return number of primitives drawn this frame
    unsigned GetNumPrimitives() const { return numPrimitives_; }
    /// Return number of batches drawn this frame
    unsigned GetNumBatches() const { return numBatches_; }
    /// Return dummy color texture format for shadow maps. Is "NULL" (consume no video memory) if supported
    unsigned GetDummyColorFormat() const { return dummyColorFormat_; }
    /// Return shadow map depth texture format, or D3DFMT_UNKNOWN if not supported
    unsigned GetShadowMapFormat() const { return shadowMapFormat_; }
    /// Return 24-bit shadow map depth texture format, or D3DFMT_UNKNOWN if not supported
    unsigned GetHiresShadowMapFormat() const { return hiresShadowMapFormat_; }
    /// Return whether deferred rendering is supported
    bool GetDeferredSupport() const { return deferredSupport_; }
    /// Return whether light prepass rendering is supported
    bool GetPrepassSupport() const { return prepassSupport_; }
    /// Return whether Shader Model 3 is supported
    bool GetSM3Support() const { return hasSM3_; }
    /// Return whether shadow map depth compare is done in hardware
    bool GetHardwareShadowSupport() const { return hardwareShadowSupport_; }
    /// Return whether 24-bit shadow maps are supported
    bool GetHiresShadowSupport() const { return hiresShadowSupport_; }
    /// Return whether stream offset is supported
    bool GetStreamOffsetSupport() const { return streamOffsetSupport_; }
    /// Return supported fullscreen resolutions
    PODVector<IntVector2> GetResolutions() const;
    /// Return supported multisampling levels
    PODVector<int> GetMultiSampleLevels() const;
    /// Return vertex buffer by index
    VertexBuffer* GetVertexBuffer(unsigned index) const;
    /// Return index buffer
    IndexBuffer* GetIndexBuffer() const { return indexBuffer_; }
    /// Return vertex declaration
    VertexDeclaration* GetVertexDeclaration() const { return vertexDeclaration_; }
    /// Return vertex shader
    VertexShader* GetVertexShader() const { return vertexShader_; }
    /// Return pixel shader
    PixelShader* GetPixelShader() const { return pixelShader_; }
    /// Return vertex shader parameter index by name
    VSParameter GetVSParameter(const String& name);
    /// Return pixel shader parameter index by name
    PSParameter GetPSParameter(const String& name);
    /// Return texture unit index by name
    TextureUnit GetTextureUnit(const String& name);
    /// Return vertex shader parameter name by index
    const String& GetVSParameterName(VSParameter parameter);
    /// Return vertex pixel parameter name by index
    const String& GetPSParameterName(PSParameter parameter);
    /// Return texture unit name by index
    const String& GetTextureUnitName(TextureUnit unit);
    /// Return vertex shader constant register by parameter index
    unsigned GetVSRegister(VSParameter param) { return vsRegisters_[param]; }
    /// Return pixel shader constant register by parameter index
    unsigned GetPSRegister(PSParameter param) { return psRegisters_[param]; }
    /// Return last vertex shader parameter source
    const void* GetVSParameterSource(VSParameter param) { return lastVSParameterSources_[param]; }
    /// Return last pixel shader parameter source
    const void* GetPSParameterSource(PSParameter param) { return lastPSParameterSources_[param]; }
    /// Return texture by texture unit index
    Texture* GetTexture(unsigned index) const;
    /// Return the "view texture"
    Texture* GetViewTexture() const { return viewTexture_; }
    /// Return default texture filtering mode
    TextureFilterMode GetDefaultTextureFilterMode() const { return defaultTextureFilterMode_; }
    /// Return render target by index
    RenderSurface* GetRenderTarget(unsigned index) const;
    /// Return depth stencil buffer
    RenderSurface* GetDepthStencil() const { return depthStencil_; }
    /// Return the viewport coordinates
    IntRect GetViewport() const { return viewport_; }
    /// Return whether alpha testing is enabled
    bool GetAlphaTest() const { return alphaTest_; }
    /// Return alpha test compare mode
    CompareMode GetAlphaTestMode() const { return alphaTestMode_; }
    /// Return texture anisotropy
    unsigned GetTextureAnisotropy() const { return textureAnisotropy_; }
    /// Return alpha test reference value
    float GetAlphaRef() const { return alphaRef_; }
    /// Return blending mode
    BlendMode GetBlendMode() const { return blendMode_; }
    /// Return whether color write is enabled
    bool GetColorWrite() const { return colorWrite_; }
    /// Return hardware culling mode
    CullMode GetCullMode() const { return cullMode_; }
    /// Return depth constant bias
    float GetDepthConstantBias() const { return constantDepthBias_; }
    /// Return depth slope scaled bias
    float GetDepthSlopeScaledBias() const { return slopeScaledDepthBias_; }
    /// Return depth compare mode
    CompareMode GetDepthTest() const { return depthTestMode_; }
    /// Return whether depth write is enabled
    bool GetDepthWrite() const { return depthWrite_; }
    /// Return polygon fill mode
    FillMode GetFillMode() const { return fillMode_; }
    /// Return whether stencil test is enabled
    bool GetStencilTest() const { return stencilTest_; }
    /// Return whether scissor test is enabled
    bool GetScissorTest() const { return scissorTest_; }
    /// Return scissor rectangle coordinates
    const IntRect& GetScissorRect() const { return scissorRect_; }
    /// Return stencil compare mode
    CompareMode GetStencilTestMode() const { return stencilTestMode_; }
    /// Return stencil operation to do if stencil test passes
    StencilOp GetStencilPass() const { return stencilPass_; }
    /// Return stencil operation to do if stencil test fails
    StencilOp GetStencilFail() const { return stencilFail_; }
    /// Return stencil operation to do if depth compare fails
    StencilOp GetStencilZFail() const { return stencilZFail_; }
    /// Return stencil reference value
    unsigned GetStencilRef() const { return stencilRef_; }
    /// Return stencil compare bitmask
    unsigned GetStencilMask() const { return stencilMask_; }
    /// Return stream frequency by vertex buffer index
    unsigned GetStreamFrequency(unsigned index) const;
    /// Return render target width and height
    IntVector2 GetRenderTargetDimensions() const;
    /// Return diffuse buffer for deferred rendering
    Texture2D* GetDiffBuffer() const { return diffBuffer_; }
    /// Return normal buffer for deferred rendering
    Texture2D* GetNormalBuffer() const { return normalBuffer_; }
    /// Return depth buffer for deferred rendering. If reading hardware depth is supported, return a depth texture
    Texture2D* GetDepthBuffer() const { return depthBuffer_; }
    /// Return screen buffer for post-processing
    Texture2D* GetScreenBuffer(unsigned index) const { return screenBuffer_[index & (NUM_SCREEN_BUFFERS - 1)]; }
    
    /// Add a GPU object to keep track of. Called by GPUObject.
    void AddGPUObject(GPUObject* object);
    /// Remove a GPU object. Called by GPUObject
    void RemoveGPUObject(GPUObject* object);
    
private:
    /// Create the application window
    bool OpenWindow(int width, int height);
    /// Create the Direct3D interface
    bool CreateInterface();
    /// Create the Direct3D device
    bool CreateDevice(unsigned adapter, unsigned deviceType);
    /// Create deferred rendering render targets
    void CreateRenderTargets();
    /// Reset the Direct3D device
    void ResetDevice();
    /// Notify all GPU resources so they can release themselves as needed
    void OnDeviceLost();
    /// Notify all GPU resources so they can recreate themselves as needed
    void OnDeviceReset();
    /// Reset cached rendering state
    void ResetCachedState();
    /// Initialize shader parameter and texture unit mappings
    void InitializeShaderParameters();
    /// Handle operating system window message
    void HandleWindowMessage(StringHash eventType, VariantMap& eventData);
    
    /// Implementation
    GraphicsImpl* impl_;
    /// Window title
    String windowTitle_;
    /// Rendering mode
    RenderMode mode_;
    /// Window width
    int width_;
    /// Window height
    int height_;
    /// Multisampling mode
    int multiSample_;
    /// Stored window X-position
    int windowPosX_;
    /// Stored window Y-position
    int windowPosY_;
    /// Fullscreen flag
    bool fullscreen_;
    /// Vertical sync flag
    bool vsync_;
    /// Flush GPU command queue flag
    bool flushGPU_;
    /// Direct3D device lost flag
    bool deviceLost_;
    /// Query issued (used to flush the GPU command queue) flag
    bool queryIssued_;
    /// Deferred rendering support flag
    bool deferredSupport_;
    /// Light prepass support flag
    bool prepassSupport_;
    /// Hardware shadow map depth compare support flag
    bool hardwareShadowSupport_;
    /// 24-bit shadow map support flag
    bool hiresShadowSupport_;
    /// Stream offset support flag
    bool streamOffsetSupport_;
    /// Shader Model 3 flag
    bool hasSM3_;
    /// Force Shader Model 2 flag
    bool forceSM2_;
    /// Number of primitives this frame
    unsigned numPrimitives_;
    /// Number of batches this frame
    unsigned numBatches_;
    /// Immediate rendering primitive type
    PrimitiveType immediateType_;
    /// Immediate vertex buffer start position
    unsigned immediateStartPos_;
    /// Immediate rendering vertex buffer size
    unsigned immediateVertexCount_;
    /// Immediate rendering vertex number
    unsigned immediateCurrentVertex_;
    /// Immediate rendering vertex buffer in use
    VertexBuffer* immediateBuffer_;
    /// Immediate rendering data pointer
    unsigned char* immediateDataPtr_;
    /// GPU objects
    Vector<GPUObject*> gpuObjects_;
    /// Vertex declarations
    Map<unsigned long long, SharedPtr<VertexDeclaration> > vertexDeclarations_;
    /// Immediate rendering vertex buffers by vertex declaration
    Map<unsigned, SharedPtr<VertexBuffer> > immediatevertexBuffer_;
    /// Immediate rendering vertex buffer start positions
    Map<unsigned, unsigned> immediateVertexBufferPos_;
    /// Deferred rendering diffuse buffer
    SharedPtr<Texture2D> diffBuffer_;
    /// Deferred rendering normal buffer
    SharedPtr<Texture2D> normalBuffer_;
    /// Deferred rendering depth buffer
    SharedPtr<Texture2D> depthBuffer_;
    /// Screen buffers for post processing
    SharedPtr<Texture2D> screenBuffer_[NUM_SCREEN_BUFFERS];
    /// Shadow map dummy color texture format
    unsigned dummyColorFormat_;
    /// Shadow map depth texture format
    unsigned shadowMapFormat_;
    /// Shadow map 24-bit depth texture format
    unsigned hiresShadowMapFormat_;
    /// Vertex buffers in use
    VertexBuffer* vertexBuffer_[MAX_VERTEX_STREAMS];
    /// Stream frequencies by vertex buffer
    unsigned streamFrequency_[MAX_VERTEX_STREAMS];
    /// Stream offsets by vertex buffer
    unsigned streamOffset_[MAX_VERTEX_STREAMS];
    /// Index buffer in use
    IndexBuffer* indexBuffer_;
    /// Vertex declaration in use
    VertexDeclaration* vertexDeclaration_;
    /// Vertex shader in use
    VertexShader* vertexShader_;
    /// Pixel shader in use
    PixelShader* pixelShader_;
    /// Vertex shader parameter mappings
    Map<String, VSParameter> vsParameters_;
    /// Pixel shader parameter mappings
    Map<String, PSParameter> psParameters_;
    /// Vertex shader constant register mappings
    unsigned vsRegisters_[MAX_VS_PARAMETERS];
    /// Pixel shader constant register mappings
    unsigned psRegisters_[MAX_PS_PARAMETERS];
    /// Last vertex shader parameter sources per parameter
    const void* lastVSParameterSources_[MAX_VS_PARAMETERS];
    /// Last pixel shader parameter sources per parameter
    const void* lastPSParameterSources_[MAX_PS_PARAMETERS];
    /// Textures in use
    Texture* texture_[MAX_TEXTURE_UNITS];
    /// "View texture" to prevent sampling the destination render target
    Texture* viewTexture_;
    /// Texture unit mappings
    Map<String, TextureUnit> textureUnits_;
    /// Render targets in use
    RenderSurface* renderTarget_[MAX_RENDERTARGETS];
    /// Depth stencil buffer in use
    RenderSurface* depthStencil_;
    /// Viewport coordinates
    IntRect viewport_;
    /// Alpha test enable flag
    bool alphaTest_;
    /// Alpha test compare mode
    CompareMode alphaTestMode_;
    /// Alpha test reference value
    float alphaRef_;
    /// Texture anisotropy level
    unsigned textureAnisotropy_;
    /// Blending mode
    BlendMode blendMode_;
    /// Color write enable
    bool colorWrite_;
    /// Hardware culling mode
    CullMode cullMode_;
    /// Depth constant bias
    float constantDepthBias_;
    /// Depth slope scaled bias
    float slopeScaledDepthBias_;
    /// Depth compare mode
    CompareMode depthTestMode_;
    /// Depth write enable flag
    bool depthWrite_;
    /// Polygon fill mode
    FillMode fillMode_;
    /// Scissor test rectangle
    IntRect scissorRect_;
    /// Scissor test enable flag
    bool scissorTest_;
    /// Stencil test compare mode
    CompareMode stencilTestMode_;
    /// Stencil operation on pass
    StencilOp stencilPass_;
    /// Stencil operation on fail
    StencilOp stencilFail_;
    /// Stencil operation on depth fail
    StencilOp stencilZFail_;
    /// Stencil test enable flag
    bool stencilTest_;
    /// Stencil test reference value
    unsigned stencilRef_;
    /// Stencil compare bitmask
    unsigned stencilMask_;
    /// Default texture filtering mode
    TextureFilterMode defaultTextureFilterMode_;
};

/// Register Graphics library objects
void RegisterGraphicsLibrary(Context* context);
