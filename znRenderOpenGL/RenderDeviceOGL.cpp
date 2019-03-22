#include <stdafx.h>

// Include
#include "BufferOGL.h"
#include "ConstantBufferOGL.h"
#include "StructuredBufferOGL.h"
#include "RenderTargetOGL.h"
#include "MaterialOGL.h"
#include "MeshOGL.h"
#include "ShaderOGL.h"
#include "TextureOGL.h"
#include "SamplerStateOGL.h"
#include "PipelineStateOGL.h"
#include "QueryOGL.h"

// General
#include "RenderDeviceOGL.h"

#ifdef _DEBUG
void _stdcall glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	//if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	//  Dynamic buff	pixel sync
	if (id == 131185 || id == 131154) return;


	Log::Error("-----------------------------------------------------");
	Log::Error("OpenGL Debug message (%d): [%s]", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             Log::Error("Source: OpenGL API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Log::Error("Source: Window System API"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: Log::Error("Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     Log::Error("Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     Log::Error("Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           Log::Error("Source: Other"); break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               Log::Error("Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: Log::Error("Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  Log::Error("Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         Log::Error("Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         Log::Error("Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:              Log::Error("Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          Log::Error("Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           Log::Error("Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:               Log::Error("Type: Other"); break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         Log::Error("Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       Log::Error("Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:          Log::Error("Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: Log::Error("Severity: notification"); break;
	}

	Log::Error("");

	assert1(false);
}
#endif

RenderDeviceOGL::RenderDeviceOGL() :
	m_DeviceCapsSettings(GetSettingsGroup<CGroupRenderCaps>()),
	m_OpenGLSettings(GetSettingsGroup<CGroupOpenGL>())
{
	//CreateDevice();
}

RenderDeviceOGL::~RenderDeviceOGL()
{
	m_Meshes.clear();
	m_Buffers.clear();
	m_Shaders.clear();
	m_Textures.clear();
	m_Samplers.clear();
	m_Pipelines.clear();
	m_Queries.clear();
}

void RenderDeviceOGL::CreateDevice(HDC _hdc)
{
	bool failed = false;

	HGLRC glrc = initOpenGL(_hdc);

	Log::Info("Initializing GL4 backend.");
	if (!initOGLExtensions())
	{
		Log::Error("Could not find all required OpenGL function entry points");
		failed = true;
	}

	char* vendor = (char*)glGetString(GL_VENDOR);
	char* renderer = (char*)glGetString(GL_RENDERER);
	char* version = (char*)glGetString(GL_VERSION);

	if (!version || !renderer || !vendor)
	{
		Log::Error("OpenGL not initialized. Make sure you have a valid OpenGL context");
		return;
	}

	Log::Info("OpenGL driver[%s] by[%s] on[%s]", version, vendor, renderer);

	// Check that OpenGL 3.3 is available
	if (m_OpenGLSettings.majorVersion * 10 + m_OpenGLSettings.minorVersion < 33)
	{
		Log::Error("OpenGL 3.3 not available");
		failed = true;
	}

	// Check that required extensions are supported
	if (!m_OpenGLSettings.EXT_texture_filter_anisotropic)
	{
		Log::Error("Extension EXT_texture_filter_anisotropic not supported");
		failed = true;
	}
	if (!m_OpenGLSettings.EXT_texture_compression_s3tc)
	{
		Log::Error("Extension EXT_texture_compression_s3tc not supported");
		failed = true;
	}
	if (!m_OpenGLSettings.EXT_texture_sRGB)
	{
		Log::Error("Extension EXT_texture_sRGB not supported");
		failed = true;
	}

	if (failed)
	{
		Log::Error("Failed to init renderer backend (OpenGL %d.%d), retrying with legacy OpenGL 2.1 backend", m_OpenGLSettings.majorVersion, m_OpenGLSettings.minorVersion);
		//return false;
	}

	// Debug output
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
#endif

	// Set capabilities
	m_DeviceCapsSettings.texFloat = true;
	m_DeviceCapsSettings.texNPOT = true;
	m_DeviceCapsSettings.rtMultisampling = true;
	m_DeviceCapsSettings.geometryShaders = true;
	m_DeviceCapsSettings.tesselation = m_OpenGLSettings.majorVersion >= 4 && m_OpenGLSettings.minorVersion >= 1;
	m_DeviceCapsSettings.computeShaders = m_OpenGLSettings.majorVersion >= 4 && m_OpenGLSettings.minorVersion >= 3;

	// Find maximum number of storage buffers in compute shader
	glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, (GLint *)&_maxComputeBufferAttachments);

	// Init states before creating test render buffer, to ensure binding the current FBO again
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		GLint value;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &value);
		m_IsDefaultFBOMultisampled = value > 0;

		GLboolean doubleBuffered;
		glGetBooleanv(GL_DOUBLEBUFFER, &doubleBuffered);
		_doubleBuffered = doubleBuffered != 0;

		// Get the currently bound frame buffer object to avoid reset to invalid FBO
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_RBDefault);
	}

	// Find supported depth format (some old ATI cards only support 16 bit depth for FBOs)
	{
		m_DepthFormat = GL_DEPTH_COMPONENT24;
		/*Log::Info("Render target depth precision limited to 32 bit");
		std::shared_ptr<R_RenderBuffer> testBuf32 = createRenderBuffer(32, 32, R_TextureFormats::RGBA8, true, 1, 0);
		if (testBuf32 == nullptr)
		{
			m_DepthFormat = GL_DEPTH_COMPONENT24;
			Log::Warn("Render target depth precision limited to 24 bit");
			std::shared_ptr<R_RenderBuffer> testBuf24 = createRenderBuffer(32, 32, R_TextureFormats::RGBA8, true, 1, 0);
			if (testBuf24 == nullptr)
			{
				m_DepthFormat = GL_DEPTH_COMPONENT16;
				Log::Error("Render target depth precision limited to 16 bit");
			}
		}*/
	}

	m_DeviceName = std::string(vendor) + std::string(renderer) + std::string(version);
}

cstring RenderDeviceOGL::GetDeviceName() const
{
	return m_DeviceName;
}

std::shared_ptr<IBuffer> RenderDeviceOGL::CreateVoidVertexBuffer(const void * data, uint32 count, uint32 offset, uint32 stride)
{
	std::shared_ptr<IBuffer> buffer = std::make_shared<BufferOGL>(GL_ARRAY_BUFFER, data, count, stride);
	m_Buffers.push_back(buffer);

	return buffer;
}

std::shared_ptr<IBuffer> RenderDeviceOGL::CreateUInt16IndexBuffer(const uint16* data, uint32 count)
{
	std::shared_ptr <IBuffer> buffer = std::make_shared<BufferOGL>(GL_ELEMENT_ARRAY_BUFFER, data, count, (UINT)sizeof(uint16));
	m_Buffers.push_back(buffer);

	return buffer;
}

std::shared_ptr<IBuffer> RenderDeviceOGL::CreateUInt32IndexBuffer(const uint32* data, uint32 count)
{
	std::shared_ptr <IBuffer> buffer = std::make_shared<BufferOGL>(GL_ELEMENT_ARRAY_BUFFER, data, count, (UINT)sizeof(uint32));
	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceOGL::DestroyBuffer(std::shared_ptr<IBuffer> buffer)
{
	BufferList::iterator iter = std::find(m_Buffers.begin(), m_Buffers.end(), buffer);
	if (iter != m_Buffers.end())
	{
		m_Buffers.erase(iter);
	}
}

void RenderDeviceOGL::DestroyVertexBuffer(std::shared_ptr<IBuffer> buffer)
{
	DestroyBuffer(buffer);
}

void RenderDeviceOGL::DestroyIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
	DestroyBuffer(buffer);
}


std::shared_ptr<ConstantBuffer> RenderDeviceOGL::CreateConstantBuffer(const void* data, size_t size)
{
	std::shared_ptr<ConstantBuffer> buffer = std::make_shared<ConstantBufferOGL>(size);

	if (data)
	{
		buffer->Set(data, size);
	}

	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceOGL::DestroyConstantBuffer(std::shared_ptr<ConstantBuffer> buffer)
{
	DestroyBuffer(buffer);
}


std::shared_ptr<StructuredBuffer> RenderDeviceOGL::CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<StructuredBuffer> buffer = std::make_shared<StructuredBufferOGL>(GL_SHADER_STORAGE_BUFFER, data, count, stride, cpuAccess, gpuWrite);
	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceOGL::DestroyStructuredBuffer(std::shared_ptr<StructuredBuffer> buffer)
{
	DestroyBuffer(buffer);
}

void RenderDeviceOGL::Lock()
{
}

void RenderDeviceOGL::Unlock()
{
}


std::shared_ptr<IMesh> RenderDeviceOGL::CreateMesh()
{
	std::shared_ptr<IMesh> mesh = std::make_shared<MeshOGL>();
	m_Meshes.push_back(mesh);

	return mesh;
}

void RenderDeviceOGL::DestroyMesh(std::shared_ptr<IMesh> mesh)
{
	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter != m_Meshes.end())
	{
		m_Meshes.erase(iter);
	}
}


std::shared_ptr<Shader> RenderDeviceOGL::CreateShader(Shader::ShaderType type, cstring fileName, const Shader::ShaderMacros& shaderMacros, cstring entryPoint, cstring profile)
{
	std::string fullName = fileName + entryPoint + profile;

	ShaderMap::iterator iter = m_ShadersByName.find(fullName);
	if (iter != m_ShadersByName.end())
		return iter->second;

	std::shared_ptr<Shader> pShader = std::make_shared<ShaderOGL>();
	pShader->LoadShaderFromFile(type, fileName, shaderMacros, entryPoint, profile);

	m_Shaders.push_back(pShader);
	m_ShadersByName.insert(ShaderMap::value_type(fullName, pShader));

	return pShader;
}

void RenderDeviceOGL::DestroyShader(std::shared_ptr<Shader> shader)
{
	ShaderList::iterator iter = std::find(m_Shaders.begin(), m_Shaders.end(), shader);
	if (iter != m_Shaders.end())
	{
		m_Shaders.erase(iter);
	}
}


std::shared_ptr<Texture> RenderDeviceOGL::CreateTexture2D(cstring fileName)
{
	TextureMap::iterator iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second;
	}

	std::shared_ptr<Texture> texture = std::make_shared<TextureOGL>(this);
	texture->LoadTexture2D(fileName);

	m_Textures.push_back(texture);
	m_TexturesByName.insert(TextureMap::value_type(fileName, texture));

	return texture;
}

std::shared_ptr<Texture> RenderDeviceOGL::CreateTextureCube(cstring fileName)
{
	TextureMap::iterator iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second;
	}

	std::shared_ptr<Texture> texture = std::make_shared<TextureOGL>(this);
	texture->LoadTextureCube(fileName);

	m_Textures.push_back(texture);
	m_TexturesByName.insert(TextureMap::value_type(fileName, texture));

	return texture;

}

std::shared_ptr<Texture> RenderDeviceOGL::CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices, const Texture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<Texture> texture = std::make_shared<TextureOGL>(this, width, height, slices, format, cpuAccess);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceOGL::CreateTextureCube(uint16_t size, uint16_t numCubes, const Texture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<Texture> texture = std::make_shared<TextureOGL>(this, size, numCubes, format, cpuAccess);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceOGL::CreateTexture()
{
	std::shared_ptr<Texture> texture = std::make_shared<TextureOGL>(this);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceOGL::GetDefaultTexture() const
{
	return m_pDefaultTexture;
}

void RenderDeviceOGL::DestroyTexture(std::shared_ptr<Texture> texture)
{
	TextureList::iterator iter = std::find(m_Textures.begin(), m_Textures.end(), texture);
	if (iter != m_Textures.end())
	{
		m_Textures.erase(iter);
	}

	TextureMap::iterator iter2 = std::find_if(m_TexturesByName.begin(), m_TexturesByName.end(), [=](TextureMap::value_type val) { return (val.second == texture); });
	if (iter2 != m_TexturesByName.end())
	{
		m_TexturesByName.erase(iter2);
	}
}

std::shared_ptr<Material> RenderDeviceOGL::CreateMaterial()
{
	std::shared_ptr<Material> pMaterial = std::make_shared<MaterialOGL>(this);
	m_Materials.push_back(pMaterial);
	return pMaterial;
}

void RenderDeviceOGL::DestroyMaterial(std::shared_ptr<Material> material)
{
	MaterialList::iterator iter = std::find(m_Materials.begin(), m_Materials.end(), material);
	if (iter != m_Materials.end())
	{
		m_Materials.erase(iter);
	}
}


std::shared_ptr<IRenderTarget> RenderDeviceOGL::CreateRenderTarget()
{
	std::shared_ptr<RenderTargetOGL> renderTarget = std::make_shared<RenderTargetOGL>(this);
	m_RenderTargets.push_back(renderTarget);

	return renderTarget;
}

void RenderDeviceOGL::DestroyRenderTarget(std::shared_ptr<IRenderTarget> renderTarget)
{
	RenderTargetList::iterator iter = std::find(m_RenderTargets.begin(), m_RenderTargets.end(), renderTarget);
	if (iter != m_RenderTargets.end())
	{
		m_RenderTargets.erase(iter);
	}
}


std::shared_ptr<SamplerState> RenderDeviceOGL::CreateSamplerState()
{
	std::shared_ptr<SamplerState> sampler = std::make_shared<SamplerStateOGL>();
	m_Samplers.push_back(sampler);

	return sampler;
}

void RenderDeviceOGL::DestroySampler(std::shared_ptr<SamplerState> sampler)
{
	SamplerList::iterator iter = std::find(m_Samplers.begin(), m_Samplers.end(), sampler);
	if (iter != m_Samplers.end())
	{
		m_Samplers.erase(iter);
	}
}


std::shared_ptr<PipelineState> RenderDeviceOGL::CreatePipelineState()
{
	std::shared_ptr<PipelineState> pPipeline = std::make_shared<PipelineStateOGL>();
	m_Pipelines.push_back(pPipeline);

	return pPipeline;
}

void RenderDeviceOGL::DestoryPipelineState(std::shared_ptr<PipelineState> pipeline)
{
	PipelineList::iterator iter = std::find(m_Pipelines.begin(), m_Pipelines.end(), pipeline);
	if (iter != m_Pipelines.end())
	{
		m_Pipelines.erase(iter);
	}
}


bool RenderDeviceOGL::IsDoubleBuffered()
{
	return _doubleBuffered;
}

uint32 RenderDeviceOGL::GetDefaultRB()
{
	return m_RBDefault;
}

void RenderDeviceOGL::SetDefaultRB(uint32 _obj)
{
	m_RBDefault = _obj;
}


std::shared_ptr<Query> RenderDeviceOGL::CreateQuery(Query::QueryType queryType, uint8_t numBuffers)
{
	std::shared_ptr<Query> query = std::make_shared<QueryOGL>(queryType, numBuffers);
	m_Queries.push_back(query);

	return query;
}

void RenderDeviceOGL::DestoryQuery(std::shared_ptr<Query> query)
{
	QueryList::iterator iter = std::find(m_Queries.begin(), m_Queries.end(), query);
	if (iter != m_Queries.end())
	{
		m_Queries.erase(iter);
	}
}


void RenderDeviceOGL::LoadDefaultResources()
{
	// Create a magenta texture if a texture defined in the shader is not bound.
	m_pDefaultTexture = CreateTexture2D("Textures\\ShaneCube.blp");
	//m_pDefaultTexture = CreateTexture2D(1, 1, 1, Texture::TextureFormat());
	//m_pDefaultTexture->Clear(ClearFlags::Color, vec4(1, 0, 1, 1));
}