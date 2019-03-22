#include "stdafx.h"

// General
#include "OpenGL.h"

#include <cstdlib>
#include <cstring>

#pragma region MyRegion

// GL 1.1
#if (GL_VERSION_NUM >= 11)
PFNGLGETTEXIMAGEPROC glGetTexImage = nullptr;
PFNGLPOLYGONMODEPROC glPolygonMode = nullptr;
PFNGLCLEARDEPTH glClearDepth = nullptr;
PFNGLDRAWBUFFERPROC glDrawBuffer = nullptr;
#endif

// GL 1.2
#if (GL_VERSION_NUM >= 12)
PFNGLBLENDCOLORPROC glBlendColor = nullptr;
PFNGLBLENDEQUATIONPROC glBlendEquation = nullptr;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = nullptr;
PFNGLTEXIMAGE3DPROC glTexImage3D = nullptr;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D = nullptr;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D = nullptr;
#endif

// GL 1.3
#if (GL_VERSION_NUM >= 13)
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage = nullptr;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D = nullptr;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = nullptr;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D = nullptr;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage = nullptr;
#endif

// GL 1.4
#if (GL_VERSION_NUM >= 14)
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = nullptr;
PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays = nullptr;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements = nullptr;
PFNGLPOINTPARAMETERFPROC glPointParameterf = nullptr;
PFNGLPOINTPARAMETERFVPROC glPointParameterfv = nullptr;
PFNGLPOINTPARAMETERIPROC glPointParameteri = nullptr;
PFNGLPOINTPARAMETERIVPROC glPointParameteriv = nullptr;
#endif

// GL 1.5
#if (GL_VERSION_NUM >= 15)
PFNGLGENQUERIESPROC glGenQueries = nullptr;
PFNGLDELETEQUERIESPROC glDeleteQueries = nullptr;
PFNGLISQUERYPROC glIsQuery = nullptr;
PFNGLBEGINQUERYPROC glBeginQuery = nullptr;
PFNGLENDQUERYPROC glEndQuery = nullptr;
PFNGLGETQUERYIVPROC glGetQueryiv = nullptr;
PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv = nullptr;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLISBUFFERPROC glIsBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData = nullptr;
PFNGLMAPBUFFERPROC glMapBuffer = nullptr;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = nullptr;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv = nullptr;
PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv = nullptr;
#endif

// GL 2.0
#if (GL_VERSION_NUM >= 20)
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = nullptr;
PFNGLDRAWBUFFERSPROC glDrawBuffers = nullptr;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate = nullptr;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate = nullptr;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLDETACHSHADERPROC glDetachShader = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = nullptr;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = nullptr;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = nullptr;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLGETSHADERSOURCEPROC glGetShaderSource = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLGETUNIFORMFVPROC glGetUniformfv = nullptr;
PFNGLGETUNIFORMIVPROC glGetUniformiv = nullptr;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv = nullptr;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv = nullptr;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv = nullptr;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv = nullptr;
PFNGLISPROGRAMPROC glIsProgram = nullptr;
PFNGLISSHADERPROC glIsShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM2FPROC glUniform2f = nullptr;
PFNGLUNIFORM3FPROC glUniform3f = nullptr;
PFNGLUNIFORM4FPROC glUniform4f = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLUNIFORM2IPROC glUniform2i = nullptr;
PFNGLUNIFORM3IPROC glUniform3i = nullptr;
PFNGLUNIFORM4IPROC glUniform4i = nullptr;
PFNGLUNIFORM1FVPROC glUniform1fv = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLUNIFORM1IVPROC glUniform1iv = nullptr;
PFNGLUNIFORM2IVPROC glUniform2iv = nullptr;
PFNGLUNIFORM3IVPROC glUniform3iv = nullptr;
PFNGLUNIFORM4IVPROC glUniform4iv = nullptr;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
#endif

// GL 2.1
#if (GL_VERSION_NUM >= 21)
PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv = nullptr;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv = nullptr;
PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv = nullptr;
PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv = nullptr;
PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv = nullptr;
PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv = nullptr;
#endif

// GL 3.0
#if (GL_VERSION_NUM >= 30)
PFNGLGETSTRINGIPROC glGetStringi = nullptr;
PFNGLCOLORMASKIPROC glColorMaski = 0;
PFNGLGETBOOLEANI_VPROC glGetBooleani_v = 0;
PFNGLGETINTEGERI_VPROC glGetIntegeri_v = 0;
PFNGLENABLEIPROC glEnablei = 0;
PFNGLDISABLEIPROC glDisablei = 0;
PFNGLISENABLEDIPROC glIsEnabledi = 0;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback = 0;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback = 0;
PFNGLBINDBUFFERRANGEPROC glBindBufferRange = 0;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = 0;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings = 0;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying = 0;
PFNGLCLAMPCOLORPROC glClampColor = 0;
PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender = 0;
PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender = 0;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = 0;
PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv = 0;
PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv = 0;
PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i = 0;
PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i = 0;
PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i = 0;
PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i = 0;
PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui = 0;
PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui = 0;
PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui = 0;
PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui = 0;
PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv = 0;
PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv = 0;
PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv = 0;
PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv = 0;
PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv = 0;
PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv = 0;
PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv = 0;
PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv = 0;
PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv = 0;
PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv = 0;
PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv = 0;
PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv = 0;
PFNGLGETUNIFORMUIVPROC glGetUniformuiv = 0;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation = 0;
PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation = 0;
PFNGLUNIFORM1UIPROC glUniform1ui = 0;
PFNGLUNIFORM2UIPROC glUniform2ui = 0;
PFNGLUNIFORM3UIPROC glUniform3ui = 0;
PFNGLUNIFORM4UIPROC glUniform4ui = 0;
PFNGLUNIFORM1UIVPROC glUniform1uiv = 0;
PFNGLUNIFORM2UIVPROC glUniform2uiv = 0;
PFNGLUNIFORM3UIVPROC glUniform3uiv = 0;
PFNGLUNIFORM4UIVPROC glUniform4uiv = 0;
PFNGLTEXPARAMETERIIVPROC glTexParameterIiv = 0;
PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv = 0;
PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv = 0;
PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv = 0;
PFNGLCLEARBUFFERIVPROC glClearBufferiv = 0;
PFNGLCLEARBUFFERUIVPROC glClearBufferuiv = 0;
PFNGLCLEARBUFFERFVPROC glClearBufferfv = 0;
PFNGLCLEARBUFFERFIPROC glClearBufferfi = 0;
PFNGLISRENDERBUFFERPROC glIsRenderbuffer = 0;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = 0;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = 0;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = 0;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = 0;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv = 0;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer = 0;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = 0;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = 0;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = 0;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = 0;
PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D = 0;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = 0;
PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D = 0;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = 0;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv = 0;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = 0;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = 0;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample = 0;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer = 0;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange = 0;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange = 0;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
PFNGLISVERTEXARRAYPROC glIsVertexArray = 0;
#endif

// GL 3.1
#if (GL_VERSION_NUM >= 31)
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = 0;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced = 0;
PFNGLTEXBUFFERPROC glTexBuffer = 0;
PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex = 0;
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData = 0;
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices = 0;
PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv = 0;
PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName = 0;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = 0;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv = 0;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName = 0;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = 0;
#endif

// GL 3.2
#if (GL_VERSION_NUM >= 32)
PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex = 0;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex = 0;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex = 0;
PFNGLPROVOKINGVERTEXPROC glProvokingVertex = 0;
PFNGLFENCESYNCPROC glFenceSync = 0;
PFNGLISSYNCPROC glIsSync = 0;
PFNGLDELETESYNCPROC glDeleteSync = 0;
PFNGLCLIENTWAITSYNCPROC glClientWaitSync = 0;
PFNGLWAITSYNCPROC glWaitSync = 0;
PFNGLGETINTEGER64VPROC glGetInteger64v = 0;
PFNGLGETSYNCIVPROC glGetSynciv = 0;
PFNGLGETINTEGER64I_VPROC glGetInteger64i_v = 0;
PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v = 0;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture = 0;
PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample = 0;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample = 0;
PFNGLGETMULTISAMPLEFVPROC glGetMultisamplefv = 0;
PFNGLSAMPLEMASKIPROC glSampleMaski = 0;
#endif

// GL 3.3
#if (GL_VERSION_NUM >= 33)
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed = 0;
PFNGLGETFRAGDATAINDEXPROC glGetFragDataIndex = 0;
PFNGLGENSAMPLERSPROC glGenSamplers = 0;
PFNGLDELETESAMPLERSPROC glDeleteSamplers = 0;
PFNGLISSAMPLERPROC glIsSampler = 0;
PFNGLBINDSAMPLERPROC glBindSampler = 0;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = 0;
PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv = 0;
PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf = 0;
PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv = 0;
PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv = 0;
PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv = 0;
PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv = 0;
PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv = 0;
PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv = 0;
PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv = 0;
PFNGLQUERYCOUNTERPROC glQueryCounter = 0;
PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64v = 0;
PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v = 0;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor = 0;
PFNGLVERTEXATTRIBP1UIPROC glVertexAttribP1ui = 0;
PFNGLVERTEXATTRIBP1UIVPROC glVertexAttribP1uiv = 0;
PFNGLVERTEXATTRIBP2UIPROC glVertexAttribP2ui = 0;
PFNGLVERTEXATTRIBP2UIVPROC glVertexAttribP2uiv = 0;
PFNGLVERTEXATTRIBP3UIPROC glVertexAttribP3ui = 0;
PFNGLVERTEXATTRIBP3UIVPROC glVertexAttribP3uiv = 0;
PFNGLVERTEXATTRIBP4UIPROC glVertexAttribP4ui = 0;
PFNGLVERTEXATTRIBP4UIVPROC glVertexAttribP4uiv = 0;
#endif

// GL 4.0
#if (GL_VERSION_NUM >= 40)
PFNGLMINSAMPLESHADINGPROC glMinSampleShading = 0;
PFNGLBLENDEQUATIONIPROC glBlendEquationi = 0;
PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei = 0;
PFNGLBLENDFUNCIPROC glBlendFunci = 0;
PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei = 0;
PFNGLDRAWARRAYSINDIRECTPROC glDrawArraysIndirect = 0;
PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect = 0;
PFNGLUNIFORM1DPROC glUniform1d = 0;
PFNGLUNIFORM2DPROC glUniform2d = 0;
PFNGLUNIFORM3DPROC glUniform3d = 0;
PFNGLUNIFORM4DPROC glUniform4d = 0;
PFNGLUNIFORM1DVPROC glUniform1dv = 0;
PFNGLUNIFORM2DVPROC glUniform2dv = 0;
PFNGLUNIFORM3DVPROC glUniform3dv = 0;
PFNGLUNIFORM4DVPROC glUniform4dv = 0;
PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv = 0;
PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv = 0;
PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv = 0;
PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv = 0;
PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv = 0;
PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv = 0;
PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv = 0;
PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv = 0;
PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv = 0;
PFNGLGETUNIFORMDVPROC glGetUniformdv = 0;
PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation = 0;
PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex = 0;
PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glGetActiveSubroutineUniformiv = 0;
PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName = 0;
PFNGLGETACTIVESUBROUTINENAMEPROC glGetActiveSubroutineName = 0;
PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv = 0;
PFNGLGETUNIFORMSUBROUTINEUIVPROC glGetUniformSubroutineuiv = 0;
PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv = 0;
PFNGLPATCHPARAMETERIPROC glPatchParameteri = 0;
PFNGLPATCHPARAMETERFVPROC glPatchParameterfv = 0;
PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback = 0;
PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks = 0;
PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks = 0;
PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback = 0;
PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback = 0;
PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback = 0;
PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback = 0;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream = 0;
PFNGLBEGINQUERYINDEXEDPROC glBeginQueryIndexed = 0;
PFNGLENDQUERYINDEXEDPROC glEndQueryIndexed = 0;
PFNGLGETQUERYINDEXEDIVPROC glGetQueryIndexediv = 0;
#endif

// GL 4.1
#if (GL_VERSION_NUM >= 41)
PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler = 0;
PFNGLSHADERBINARYPROC glShaderBinary = 0;
PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat = 0;
PFNGLDEPTHRANGEFPROC glDepthRangef = 0;
PFNGLCLEARDEPTHFPROC glClearDepthf = 0;
PFNGLGETPROGRAMBINARYPROC glGetProgramBinary = 0;
PFNGLPROGRAMBINARYPROC glProgramBinary = 0;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri = 0;
PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages = 0;
PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram = 0;
PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv = 0;
PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline = 0;
PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines = 0;
PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines = 0;
PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline = 0;
PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv = 0;
PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i = 0;
PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv = 0;
PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f = 0;
PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv = 0;
PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d = 0;
PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv = 0;
PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui = 0;
PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv = 0;
PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i = 0;
PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv = 0;
PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f = 0;
PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv = 0;
PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d = 0;
PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv = 0;
PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui = 0;
PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv = 0;
PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i = 0;
PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv = 0;
PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f = 0;
PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv = 0;
PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d = 0;
PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv = 0;
PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui = 0;
PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv = 0;
PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i = 0;
PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv = 0;
PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f = 0;
PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv = 0;
PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d = 0;
PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv = 0;
PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui = 0;
PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv = 0;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv = 0;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv = 0;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv = 0;
PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv = 0;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv = 0;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv = 0;
PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline = 0;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog = 0;
PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d = 0;
PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d = 0;
PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d = 0;
PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d = 0;
PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv = 0;
PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv = 0;
PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv = 0;
PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv = 0;
PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer = 0;
PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv = 0;
PFNGLVIEWPORTARRAYVPROC glViewportArrayv = 0;
PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf = 0;
PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv = 0;
PFNGLSCISSORARRAYVPROC glScissorArrayv = 0;
PFNGLSCISSORINDEXEDPROC glScissorIndexed = 0;
PFNGLSCISSORINDEXEDVPROC glScissorIndexedv = 0;
PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv = 0;
PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed = 0;
PFNGLGETFLOATI_VPROC glGetFloati_v = 0;
PFNGLGETDOUBLEI_VPROC glGetDoublei_v = 0;
#endif

// GL 4.2
#if (GL_VERSION_NUM >= 42)
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glDrawElementsInstancedBaseInstance = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance = 0;
PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ = 0;
PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glGetActiveAtomicCounterBufferiv = 0;
PFNGLBINDIMAGETEXTUREPROC glBindImageTexture = 0;
PFNGLMEMORYBARRIERPROC glMemoryBarrier = 0;
PFNGLTEXSTORAGE1DPROC glTexStorage1D = 0;
PFNGLTEXSTORAGE2DPROC glTexStorage2D = 0;
PFNGLTEXSTORAGE3DPROC glTexStorage3D = 0;
PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glDrawTransformFeedbackInstanced = 0;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glDrawTransformFeedbackStreamInstanced = 0;
#endif

// GL 4.3
#if (GL_VERSION_NUM >= 43)
PFNGLCLEARBUFFERDATAPROC glClearBufferData = 0;
PFNGLCLEARBUFFERSUBDATAPROC glClearBufferSubData = 0;
PFNGLDISPATCHCOMPUTEPROC glDispatchCompute = 0;
PFNGLDISPATCHCOMPUTEINDIRECTPROC glDispatchComputeIndirect = 0;
PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData = 0;
PFNGLFRAMEBUFFERPARAMETERIPROC glFramebufferParameteri = 0;
PFNGLGETFRAMEBUFFERPARAMETERIVPROC glGetFramebufferParameteriv = 0;
PFNGLGETINTERNALFORMATI64VPROC glGetInternalformati64v = 0;
PFNGLINVALIDATETEXSUBIMAGEPROC glInvalidateTexSubImage = 0;
PFNGLINVALIDATETEXIMAGEPROC glInvalidateTexImage = 0;
PFNGLINVALIDATEBUFFERSUBDATAPROC glInvalidateBufferSubData = 0;
PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData = 0;
PFNGLINVALIDATEFRAMEBUFFERPROC glInvalidateFramebuffer = 0;
PFNGLINVALIDATESUBFRAMEBUFFERPROC glInvalidateSubFramebuffer = 0;
PFNGLMULTIDRAWARRAYSINDIRECTPROC glMultiDrawArraysIndirect = 0;
PFNGLMULTIDRAWELEMENTSINDIRECTPROC glMultiDrawElementsIndirect = 0;
PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv = 0;
PFNGLGETPROGRAMRESOURCEINDEXPROC glGetProgramResourceIndex = 0;
PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName = 0;
PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv = 0;
PFNGLGETPROGRAMRESOURCELOCATIONPROC glGetProgramResourceLocation = 0;
PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex = 0;
PFNGLSHADERSTORAGEBLOCKBINDINGPROC glShaderStorageBlockBinding = 0;
PFNGLTEXBUFFERRANGEPROC glTexBufferRange = 0;
PFNGLTEXSTORAGE2DMULTISAMPLEPROC glTexStorage2DMultisample = 0;
PFNGLTEXSTORAGE3DMULTISAMPLEPROC glTexStorage3DMultisample = 0;
PFNGLTEXTUREVIEWPROC glTextureView = 0;
PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer = 0;
PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat = 0;
PFNGLVERTEXATTRIBIFORMATPROC glVertexAttribIFormat = 0;
PFNGLVERTEXATTRIBLFORMATPROC glVertexAttribLFormat = 0;
PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding = 0;
PFNGLVERTEXBINDINGDIVISORPROC glVertexBindingDivisor = 0;
PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl = 0;
PFNGLDEBUGMESSAGEINSERTPROC glDebugMessageInsert = 0;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = 0;
PFNGLGETDEBUGMESSAGELOGPROC glGetDebugMessageLog = 0;
PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup = 0;
PFNGLPOPDEBUGGROUPPROC glPopDebugGroup = 0;
PFNGLOBJECTLABELPROC glObjectLabel = 0;
PFNGLGETOBJECTLABELPROC glGetObjectLabel = 0;
PFNGLOBJECTPTRLABELPROC glObjectPtrLabel = 0;
PFNGLGETOBJECTPTRLABELPROC glGetObjectPtrLabel = 0;
#endif

// GL 4.4
#if (GL_VERSION_NUM >= 44)
PFNGLBUFFERSTORAGEPROC glBufferStorage = 0;
PFNGLCLEARTEXIMAGEPROC glClearTexImage = 0;
PFNGLCLEARTEXSUBIMAGEPROC glClearTexSubImage = 0;
PFNGLBINDBUFFERSBASEPROC glBindBuffersBase = 0;
PFNGLBINDBUFFERSRANGEPROC glBindBuffersRange = 0;
PFNGLBINDTEXTURESPROC glBindTextures = 0;
PFNGLBINDSAMPLERSPROC glBindSamplers = 0;
PFNGLBINDIMAGETEXTURESPROC glBindImageTextures = 0;
PFNGLBINDVERTEXBUFFERSPROC glBindVertexBuffers = 0;
#endif

// Extensions

// GL_ARB_timer_query
PFNGLQUERYCOUNTERPROC glQueryCounterARB = nullptr;
PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64vARB = nullptr;
PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64vARB = nullptr;

// GL_OES_EGL_image
PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES = nullptr;

#pragma endregion


typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int *attribList);
wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;

#define GL_FALSE                          0
#define GL_TRUE                           1

// See https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt for all values
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;

// See https://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt for all values
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B


void* OGLGetProcAddress(const char* _procname)
{
	void* p = (void*)wglGetProcAddress(_procname);
	if (p == (void*)0x0 ||
		p == (void*)0x1 ||
		p == (void*)0x2 ||
		p == (void*)0x3 ||
		p == (void*)-1
		)
	{
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void*)::GetProcAddress(module, _procname);
	}

	return p;
}

bool OGLIsExtensionSupported(const char *extName)
{
	GLint numExts;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExts);
	for (int i = 0; i < numExts; ++i)
	{
		if (strcmp(extName, (char *)glGetStringi(GL_EXTENSIONS, i)) == 0)
		{
			return true;
		}
	}
	return false;
}

void OGLCheckError()
{
	GLenum error = glGetError();
	assert1(error != GL_INVALID_ENUM);
	assert1(error != GL_INVALID_VALUE);
	assert1(error != GL_INVALID_OPERATION);
	assert1(error != GL_OUT_OF_MEMORY);
	assert1(error != GL_STACK_OVERFLOW && error != GL_STACK_UNDERFLOW);
}

void initWGLExtensions()
{
	WNDCLASS renderWindowClass;
	ZeroMemory(&renderWindowClass, sizeof(WNDCLASS));
	renderWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	renderWindowClass.lpfnWndProc = &DefWindowProcA;
	renderWindowClass.hInstance = GetModuleHandle(0);
	renderWindowClass.lpszMenuName = NULL;
	renderWindowClass.lpszClassName = "TestRenderClass";

	if (!RegisterClassA(&renderWindowClass)) 
	{
		fail1();
	}

	HWND dummy_window = CreateWindowExA(
		0,
		renderWindowClass.lpszClassName,
		"Dummy OpenGL Window",
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		renderWindowClass.hInstance,
		0
	);

	if (!dummy_window) 
	{
		fail2("Failed to create dummy OpenGL window.");
	}

	HDC dummy_dc = GetDC(dummy_window);

	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
	if (!pixel_format) 
	{
		fail2("Failed to find a suitable pixel format.");
	}

	if (!SetPixelFormat(dummy_dc, pixel_format, &pfd)) 
	{
		fail2("Failed to set the pixel format.");
	}

	HGLRC dummy_context = wglCreateContext(dummy_dc);
	if (!dummy_context)
	{
		fail2("Failed to create a dummy OpenGL rendering context.");
	}

	if (!wglMakeCurrent(dummy_dc, dummy_context))
	{
		fail2("Failed to activate dummy OpenGL rendering context.");
	}

	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");

	wglMakeCurrent(dummy_dc, 0);
	wglDeleteContext(dummy_context);
	ReleaseDC(dummy_window, dummy_dc);
	DestroyWindow(dummy_window);
}

HGLRC initOpenGL(HDC real_dc)
{
	initWGLExtensions();

	// Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
	int pixel_format_attribs[] =
	{
		WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
		WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,         32,
		WGL_DEPTH_BITS_ARB,         24,
		WGL_STENCIL_BITS_ARB,       8,
		0
	};

	int pixel_format;
	UINT num_formats;
	wglChoosePixelFormatARB(real_dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
	if (!num_formats)
	{
		fail1("Failed to set the OpenGL 3.3 pixel format.");
	}

	PIXELFORMATDESCRIPTOR pfd;
	DescribePixelFormat(real_dc, pixel_format, sizeof(pfd), &pfd);
	if (!SetPixelFormat(real_dc, pixel_format, &pfd))
	{
		fail1("Failed to set the OpenGL 3.3 pixel format.");
	}

	// Specify that we want to create an OpenGL 3.3 core profile context
	int gl33_attribs[] = 
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 4,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0,
	};

	HGLRC gl33_context = wglCreateContextAttribsARB(real_dc, 0, gl33_attribs);
	if (!gl33_context)
	{
		fail1("Failed to create OpenGL 3.3 context.");
	}

	if (!wglMakeCurrent(real_dc, gl33_context))
	{
		fail1("Failed to activate OpenGL 3.3 rendering context.");
	}

	return gl33_context;
}

bool initOGLExtensions()
{
	CGroupOpenGL& openglSettings = GetSettingsGroup<CGroupOpenGL>();

    bool r = true;

    // GL 1.1
    r &= (glPolygonMode = (PFNGLPOLYGONMODEPROC)OGLGetProcAddress("glPolygonMode")) != nullptr;
    r &= (glGetTexImage = (PFNGLGETTEXIMAGEPROC)OGLGetProcAddress("glGetTexImage")) != nullptr;
    r &= (glClearDepth = (PFNGLCLEARDEPTH)OGLGetProcAddress("glClearDepth")) != nullptr;
    r &= (glDrawBuffer = (PFNGLDRAWBUFFERPROC)OGLGetProcAddress("glDrawBuffer")) != nullptr;

    // GL 1.2
    r &= (glBlendColor = (PFNGLBLENDCOLORPROC)OGLGetProcAddress("glBlendColor")) != nullptr;
    r &= (glBlendEquation = (PFNGLBLENDEQUATIONPROC)OGLGetProcAddress("glBlendEquation")) != nullptr;
    r &= (glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)OGLGetProcAddress("glDrawRangeElements")) != nullptr;
    r &= (glTexImage3D = (PFNGLTEXIMAGE3DPROC)OGLGetProcAddress("glTexImage3D")) != nullptr;
    r &= (glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)OGLGetProcAddress("glTexSubImage3D")) != nullptr;
    r &= (glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)OGLGetProcAddress("glCopyTexSubImage3D")) != nullptr;

    // GL 1.3
    r &= (glActiveTexture = (PFNGLACTIVETEXTUREPROC)OGLGetProcAddress("glActiveTexture")) != nullptr;
    r &= (glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)OGLGetProcAddress("glSampleCoverage")) != nullptr;
    r &= (glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)OGLGetProcAddress("glCompressedTexImage3D")) != nullptr;
    r &= (glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)OGLGetProcAddress("glCompressedTexImage2D")) != nullptr;
    r &= (glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)OGLGetProcAddress("glCompressedTexImage1D")) != nullptr;
    r &= (glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)OGLGetProcAddress("glCompressedTexSubImage3D")) != nullptr;
    r &= (glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)OGLGetProcAddress("glCompressedTexSubImage2D")) != nullptr;
    r &= (glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)OGLGetProcAddress("glCompressedTexSubImage1D")) != nullptr;
    r &= (glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)OGLGetProcAddress("glGetCompressedTexImage")) != nullptr;

    // GL 1.4
    r &= (glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)OGLGetProcAddress("glBlendFuncSeparate")) != nullptr;
    r &= (glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)OGLGetProcAddress("glMultiDrawArrays")) != nullptr;
    r &= (glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)OGLGetProcAddress("glMultiDrawElements")) != nullptr;
    r &= (glPointParameterf = (PFNGLPOINTPARAMETERFPROC)OGLGetProcAddress("glPointParameterf")) != nullptr;
    r &= (glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)OGLGetProcAddress("glPointParameterfv")) != nullptr;
    r &= (glPointParameteri = (PFNGLPOINTPARAMETERIPROC)OGLGetProcAddress("glPointParameteri")) != nullptr;
    r &= (glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC)OGLGetProcAddress("glPointParameteriv")) != nullptr;

    // GL 1.5
    r &= (glGenQueries = (PFNGLGENQUERIESPROC)OGLGetProcAddress("glGenQueries")) != nullptr;
    r &= (glDeleteQueries = (PFNGLDELETEQUERIESPROC)OGLGetProcAddress("glDeleteQueries")) != nullptr;
    r &= (glIsQuery = (PFNGLISQUERYPROC)OGLGetProcAddress("glIsQuery")) != nullptr;
    r &= (glBeginQuery = (PFNGLBEGINQUERYPROC)OGLGetProcAddress("glBeginQuery")) != nullptr;
    r &= (glEndQuery = (PFNGLENDQUERYPROC)OGLGetProcAddress("glEndQuery")) != nullptr;
    r &= (glGetQueryiv = (PFNGLGETQUERYIVPROC)OGLGetProcAddress("glGetQueryiv")) != nullptr;
    r &= (glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)OGLGetProcAddress("glGetQueryObjectiv")) != nullptr;
    r &= (glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)OGLGetProcAddress("glGetQueryObjectuiv")) != nullptr;
    r &= (glBindBuffer = (PFNGLBINDBUFFERPROC)OGLGetProcAddress("glBindBuffer")) != nullptr;
    r &= (glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)OGLGetProcAddress("glDeleteBuffers")) != nullptr;
    r &= (glGenBuffers = (PFNGLGENBUFFERSPROC)OGLGetProcAddress("glGenBuffers")) != nullptr;
    r &= (glIsBuffer = (PFNGLISBUFFERPROC)OGLGetProcAddress("glIsBuffer")) != nullptr;
    r &= (glBufferData = (PFNGLBUFFERDATAPROC)OGLGetProcAddress("glBufferData")) != nullptr;
    r &= (glBufferSubData = (PFNGLBUFFERSUBDATAPROC)OGLGetProcAddress("glBufferSubData")) != nullptr;
    r &= (glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)OGLGetProcAddress("glGetBufferSubData")) != nullptr;
    r &= (glMapBuffer = (PFNGLMAPBUFFERPROC)OGLGetProcAddress("glMapBuffer")) != nullptr;
    r &= (glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)OGLGetProcAddress("glUnmapBuffer")) != nullptr;
    r &= (glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)OGLGetProcAddress("glGetBufferParameteriv")) != nullptr;
    r &= (glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)OGLGetProcAddress("glGetBufferPointerv")) != nullptr;

    // GL 2.0
    r &= (glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)OGLGetProcAddress("glBlendEquationSeparate")) != nullptr;
    r &= (glDrawBuffers = (PFNGLDRAWBUFFERSPROC)OGLGetProcAddress("glDrawBuffers")) != nullptr;
    r &= (glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)OGLGetProcAddress("glStencilOpSeparate")) != nullptr;
    r &= (glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)OGLGetProcAddress("glStencilFuncSeparate")) != nullptr;
    r &= (glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)OGLGetProcAddress("glStencilMaskSeparate")) != nullptr;
    r &= (glAttachShader = (PFNGLATTACHSHADERPROC)OGLGetProcAddress("glAttachShader")) != nullptr;
    r &= (glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)OGLGetProcAddress("glBindAttribLocation")) != nullptr;
    r &= (glCompileShader = (PFNGLCOMPILESHADERPROC)OGLGetProcAddress("glCompileShader")) != nullptr;
    r &= (glCreateProgram = (PFNGLCREATEPROGRAMPROC)OGLGetProcAddress("glCreateProgram")) != nullptr;
    r &= (glCreateShader = (PFNGLCREATESHADERPROC)OGLGetProcAddress("glCreateShader")) != nullptr;
    r &= (glDeleteProgram = (PFNGLDELETEPROGRAMPROC)OGLGetProcAddress("glDeleteProgram")) != nullptr;
    r &= (glDeleteShader = (PFNGLDELETESHADERPROC)OGLGetProcAddress("glDeleteShader")) != nullptr;
    r &= (glDetachShader = (PFNGLDETACHSHADERPROC)OGLGetProcAddress("glDetachShader")) != nullptr;
    r &= (glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)OGLGetProcAddress("glDisableVertexAttribArray")) != nullptr;
    r &= (glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)OGLGetProcAddress("glEnableVertexAttribArray")) != nullptr;
    r &= (glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)OGLGetProcAddress("glGetActiveAttrib")) != nullptr;
    r &= (glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)OGLGetProcAddress("glGetActiveUniform")) != nullptr;
    r &= (glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)OGLGetProcAddress("glGetAttachedShaders")) != nullptr;
    r &= (glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)OGLGetProcAddress("glGetAttribLocation")) != nullptr;
    r &= (glGetProgramiv = (PFNGLGETPROGRAMIVPROC)OGLGetProcAddress("glGetProgramiv")) != nullptr;
    r &= (glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)OGLGetProcAddress("glGetProgramInfoLog")) != nullptr;
    r &= (glGetShaderiv = (PFNGLGETSHADERIVPROC)OGLGetProcAddress("glGetShaderiv")) != nullptr;
    r &= (glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)OGLGetProcAddress("glGetShaderInfoLog")) != nullptr;
    r &= (glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)OGLGetProcAddress("glGetShaderSource")) != nullptr;
    r &= (glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)OGLGetProcAddress("glGetUniformLocation")) != nullptr;
    r &= (glGetUniformfv = (PFNGLGETUNIFORMFVPROC)OGLGetProcAddress("glGetUniformfv")) != nullptr;
    r &= (glGetUniformiv = (PFNGLGETUNIFORMIVPROC)OGLGetProcAddress("glGetUniformiv")) != nullptr;
    r &= (glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)OGLGetProcAddress("glGetVertexAttribdv")) != nullptr;
    r &= (glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)OGLGetProcAddress("glGetVertexAttribfv")) != nullptr;
    r &= (glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)OGLGetProcAddress("glGetVertexAttribiv")) != nullptr;
    r &= (glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)OGLGetProcAddress("glGetVertexAttribPointerv")) != nullptr;
    r &= (glIsProgram = (PFNGLISPROGRAMPROC)OGLGetProcAddress("glIsProgram")) != nullptr;
    r &= (glIsShader = (PFNGLISSHADERPROC)OGLGetProcAddress("glIsShader")) != nullptr;
    r &= (glLinkProgram = (PFNGLLINKPROGRAMPROC)OGLGetProcAddress("glLinkProgram")) != nullptr;
    r &= (glShaderSource = (PFNGLSHADERSOURCEPROC)OGLGetProcAddress("glShaderSource")) != nullptr;
    r &= (glUseProgram = (PFNGLUSEPROGRAMPROC)OGLGetProcAddress("glUseProgram")) != nullptr;
    r &= (glUniform1f = (PFNGLUNIFORM1FPROC)OGLGetProcAddress("glUniform1f")) != nullptr;
    r &= (glUniform2f = (PFNGLUNIFORM2FPROC)OGLGetProcAddress("glUniform2f")) != nullptr;
    r &= (glUniform3f = (PFNGLUNIFORM3FPROC)OGLGetProcAddress("glUniform3f")) != nullptr;
    r &= (glUniform4f = (PFNGLUNIFORM4FPROC)OGLGetProcAddress("glUniform4f")) != nullptr;
    r &= (glUniform1i = (PFNGLUNIFORM1IPROC)OGLGetProcAddress("glUniform1i")) != nullptr;
    r &= (glUniform2i = (PFNGLUNIFORM2IPROC)OGLGetProcAddress("glUniform2i")) != nullptr;
    r &= (glUniform3i = (PFNGLUNIFORM3IPROC)OGLGetProcAddress("glUniform3i")) != nullptr;
    r &= (glUniform4i = (PFNGLUNIFORM4IPROC)OGLGetProcAddress("glUniform4i")) != nullptr;
    r &= (glUniform1fv = (PFNGLUNIFORM1FVPROC)OGLGetProcAddress("glUniform1fv")) != nullptr;
    r &= (glUniform2fv = (PFNGLUNIFORM2FVPROC)OGLGetProcAddress("glUniform2fv")) != nullptr;
    r &= (glUniform3fv = (PFNGLUNIFORM3FVPROC)OGLGetProcAddress("glUniform3fv")) != nullptr;
    r &= (glUniform4fv = (PFNGLUNIFORM4FVPROC)OGLGetProcAddress("glUniform4fv")) != nullptr;
    r &= (glUniform1iv = (PFNGLUNIFORM1IVPROC)OGLGetProcAddress("glUniform1iv")) != nullptr;
    r &= (glUniform2iv = (PFNGLUNIFORM2IVPROC)OGLGetProcAddress("glUniform2iv")) != nullptr;
    r &= (glUniform3iv = (PFNGLUNIFORM3IVPROC)OGLGetProcAddress("glUniform3iv")) != nullptr;
    r &= (glUniform4iv = (PFNGLUNIFORM4IVPROC)OGLGetProcAddress("glUniform4iv")) != nullptr;
    r &= (glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)OGLGetProcAddress("glUniformMatrix2fv")) != nullptr;
    r &= (glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)OGLGetProcAddress("glUniformMatrix3fv")) != nullptr;
    r &= (glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)OGLGetProcAddress("glUniformMatrix4fv")) != nullptr;
    r &= (glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)OGLGetProcAddress("glValidateProgram")) != nullptr;
    r &= (glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)OGLGetProcAddress("glVertexAttribPointer")) != nullptr;

    // GL 2.1
    r &= (glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)OGLGetProcAddress("glUniformMatrix2x3fv")) != nullptr;
    r &= (glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)OGLGetProcAddress("glUniformMatrix3x2fv")) != nullptr;
    r &= (glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)OGLGetProcAddress("glUniformMatrix2x4fv")) != nullptr;
    r &= (glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)OGLGetProcAddress("glUniformMatrix4x2fv")) != nullptr;
    r &= (glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)OGLGetProcAddress("glUniformMatrix3x4fv")) != nullptr;
    r &= (glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)OGLGetProcAddress("glUniformMatrix4x3fv")) != nullptr;

    // GL 3.0 - GL 3.3 
    {
        // GL 3.0
        r &= (glGetStringi = (PFNGLGETSTRINGIPROC)OGLGetProcAddress("glGetStringi")) != nullptr;
        r &= (glColorMaski = (PFNGLCOLORMASKIPROC)OGLGetProcAddress("glColorMaski")) != nullptr;
        r &= (glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC)OGLGetProcAddress("glGetBooleani_v")) != nullptr;
        r &= (glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)OGLGetProcAddress("glGetIntegeri_v")) != nullptr;
        r &= (glEnablei = (PFNGLENABLEIPROC)OGLGetProcAddress("glEnablei")) != nullptr;
        r &= (glDisablei = (PFNGLDISABLEIPROC)OGLGetProcAddress("glDisablei")) != nullptr;
        r &= (glIsEnabledi = (PFNGLISENABLEDIPROC)OGLGetProcAddress("glIsEnabledi")) != nullptr;
        r &= (glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)OGLGetProcAddress("glBeginTransformFeedback")) != nullptr;
        r &= (glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)OGLGetProcAddress("glEndTransformFeedback")) != nullptr;
        r &= (glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)OGLGetProcAddress("glBindBufferRange")) != nullptr;
        r &= (glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)OGLGetProcAddress("glBindBufferBase")) != nullptr;
        r &= (glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)OGLGetProcAddress("glTransformFeedbackVaryings")) != nullptr;
        r &= (glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)OGLGetProcAddress("glGetTransformFeedbackVarying")) != nullptr;
        r &= (glClampColor = (PFNGLCLAMPCOLORPROC)OGLGetProcAddress("glClampColor")) != nullptr;
        r &= (glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)OGLGetProcAddress("glBeginConditionalRender")) != nullptr;
        r &= (glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC)OGLGetProcAddress("glEndConditionalRender")) != nullptr;
        r &= (glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)OGLGetProcAddress("glVertexAttribIPointer")) != nullptr;
        r &= (glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)OGLGetProcAddress("glGetVertexAttribIiv")) != nullptr;
        r &= (glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)OGLGetProcAddress("glGetVertexAttribIuiv")) != nullptr;
        r &= (glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC)OGLGetProcAddress("glVertexAttribI1i")) != nullptr;
        r &= (glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC)OGLGetProcAddress("glVertexAttribI2i")) != nullptr;
        r &= (glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC)OGLGetProcAddress("glVertexAttribI3i")) != nullptr;
        r &= (glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)OGLGetProcAddress("glVertexAttribI4i")) != nullptr;
        r &= (glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC)OGLGetProcAddress("glVertexAttribI1ui")) != nullptr;
        r &= (glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC)OGLGetProcAddress("glVertexAttribI2ui")) != nullptr;
        r &= (glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC)OGLGetProcAddress("glVertexAttribI3ui")) != nullptr;
        r &= (glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)OGLGetProcAddress("glVertexAttribI4ui")) != nullptr;
        r &= (glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC)OGLGetProcAddress("glVertexAttribI1iv")) != nullptr;
        r &= (glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC)OGLGetProcAddress("glVertexAttribI2iv")) != nullptr;
        r &= (glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC)OGLGetProcAddress("glVertexAttribI3iv")) != nullptr;
        r &= (glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)OGLGetProcAddress("glVertexAttribI4iv")) != nullptr;
        r &= (glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC)OGLGetProcAddress("glVertexAttribI1uiv")) != nullptr;
        r &= (glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC)OGLGetProcAddress("glVertexAttribI2uiv")) != nullptr;
        r &= (glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC)OGLGetProcAddress("glVertexAttribI3uiv")) != nullptr;
        r &= (glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)OGLGetProcAddress("glVertexAttribI4uiv")) != nullptr;
        r &= (glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC)OGLGetProcAddress("glVertexAttribI4bv")) != nullptr;
        r &= (glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC)OGLGetProcAddress("glVertexAttribI4sv")) != nullptr;
        r &= (glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC)OGLGetProcAddress("glVertexAttribI4ubv")) != nullptr;
        r &= (glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC)OGLGetProcAddress("glVertexAttribI4usv")) != nullptr;
        r &= (glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC)OGLGetProcAddress("glGetUniformuiv")) != nullptr;
        r &= (glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)OGLGetProcAddress("glBindFragDataLocation")) != nullptr;
        r &= (glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)OGLGetProcAddress("glGetFragDataLocation")) != nullptr;
        r &= (glUniform1ui = (PFNGLUNIFORM1UIPROC)OGLGetProcAddress("glUniform1ui")) != nullptr;
        r &= (glUniform2ui = (PFNGLUNIFORM2UIPROC)OGLGetProcAddress("glUniform2ui")) != nullptr;
        r &= (glUniform3ui = (PFNGLUNIFORM3UIPROC)OGLGetProcAddress("glUniform3ui")) != nullptr;
        r &= (glUniform4ui = (PFNGLUNIFORM4UIPROC)OGLGetProcAddress("glUniform4ui")) != nullptr;
        r &= (glUniform1uiv = (PFNGLUNIFORM1UIVPROC)OGLGetProcAddress("glUniform1uiv")) != nullptr;
        r &= (glUniform2uiv = (PFNGLUNIFORM2UIVPROC)OGLGetProcAddress("glUniform2uiv")) != nullptr;
        r &= (glUniform3uiv = (PFNGLUNIFORM3UIVPROC)OGLGetProcAddress("glUniform3uiv")) != nullptr;
        r &= (glUniform4uiv = (PFNGLUNIFORM4UIVPROC)OGLGetProcAddress("glUniform4uiv")) != nullptr;
        r &= (glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC)OGLGetProcAddress("glTexParameterIiv")) != nullptr;
        r &= (glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC)OGLGetProcAddress("glTexParameterIuiv")) != nullptr;
        r &= (glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC)OGLGetProcAddress("glGetTexParameterIiv")) != nullptr;
        r &= (glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC)OGLGetProcAddress("glGetTexParameterIuiv")) != nullptr;
        r &= (glClearBufferiv = (PFNGLCLEARBUFFERIVPROC)OGLGetProcAddress("glClearBufferiv")) != nullptr;
        r &= (glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)OGLGetProcAddress("glClearBufferuiv")) != nullptr;
        r &= (glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)OGLGetProcAddress("glClearBufferfv")) != nullptr;
        r &= (glClearBufferfi = (PFNGLCLEARBUFFERFIPROC)OGLGetProcAddress("glClearBufferfi")) != nullptr;
        r &= (glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)OGLGetProcAddress("glIsRenderbuffer")) != nullptr;
        r &= (glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)OGLGetProcAddress("glBindRenderbuffer")) != nullptr;
        r &= (glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)OGLGetProcAddress("glDeleteRenderbuffers")) != nullptr;
        r &= (glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)OGLGetProcAddress("glGenRenderbuffers")) != nullptr;
        r &= (glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)OGLGetProcAddress("glRenderbufferStorage")) != nullptr;
        r &= (glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)OGLGetProcAddress("glGetRenderbufferParameteriv")) != nullptr;
        r &= (glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)OGLGetProcAddress("glIsFramebuffer")) != nullptr;
        r &= (glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)OGLGetProcAddress("glBindFramebuffer")) != nullptr;
        r &= (glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)OGLGetProcAddress("glDeleteFramebuffers")) != nullptr;
        r &= (glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)OGLGetProcAddress("glGenFramebuffers")) != nullptr;
        r &= (glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)OGLGetProcAddress("glCheckFramebufferStatus")) != nullptr;
        r &= (glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)OGLGetProcAddress("glFramebufferTexture1D")) != nullptr;
        r &= (glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)OGLGetProcAddress("glFramebufferTexture2D")) != nullptr;
        r &= (glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)OGLGetProcAddress("glFramebufferTexture3D")) != nullptr;
        r &= (glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)OGLGetProcAddress("glFramebufferRenderbuffer")) != nullptr;
        r &= (glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)OGLGetProcAddress("glGetFramebufferAttachmentParameteriv")) != nullptr;
        r &= (glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)OGLGetProcAddress("glGenerateMipmap")) != nullptr;
        r &= (glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)OGLGetProcAddress("glBlitFramebuffer")) != nullptr;
        r &= (glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)OGLGetProcAddress("glRenderbufferStorageMultisample")) != nullptr;
        r &= (glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)OGLGetProcAddress("glFramebufferTextureLayer")) != nullptr;
        r &= (glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)OGLGetProcAddress("glMapBufferRange")) != nullptr;
        r &= (glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)OGLGetProcAddress("glFlushMappedBufferRange")) != nullptr;
        r &= (glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)OGLGetProcAddress("glBindVertexArray")) != nullptr;
        r &= (glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)OGLGetProcAddress("glDeleteVertexArrays")) != nullptr;
        r &= (glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)OGLGetProcAddress("glGenVertexArrays")) != nullptr;
        r &= (glIsVertexArray = (PFNGLISVERTEXARRAYPROC)OGLGetProcAddress("glIsVertexArray")) != nullptr;

        // GL 3.1
        if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 31)
        {
            r &= (glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)OGLGetProcAddress("glDrawArraysInstanced")) != nullptr;
            r &= (glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)OGLGetProcAddress("glDrawElementsInstanced")) != nullptr;
            r &= (glTexBuffer = (PFNGLTEXBUFFERPROC)OGLGetProcAddress("glTexBuffer")) != nullptr;
            r &= (glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)OGLGetProcAddress("glPrimitiveRestartIndex")) != nullptr;
            r &= (glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)OGLGetProcAddress("glCopyBufferSubData")) != nullptr;
            r &= (glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)OGLGetProcAddress("glGetUniformIndices")) != nullptr;
            r &= (glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)OGLGetProcAddress("glGetActiveUniformsiv")) != nullptr;
            r &= (glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)OGLGetProcAddress("glGetActiveUniformName")) != nullptr;
            r &= (glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)OGLGetProcAddress("glGetUniformBlockIndex")) != nullptr;
            r &= (glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)OGLGetProcAddress("glGetActiveUniformBlockiv")) != nullptr;
            r &= (glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)OGLGetProcAddress("glGetActiveUniformBlockName")) != nullptr;
            r &= (glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)OGLGetProcAddress("glUniformBlockBinding")) != nullptr;
        }

        // GL 3.2
        if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 32)
        {
            r &= (glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)OGLGetProcAddress("glDrawElementsBaseVertex")) != nullptr;
            r &= (glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)OGLGetProcAddress("glDrawRangeElementsBaseVertex")) != nullptr;
            r &= (glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)OGLGetProcAddress("glDrawElementsInstancedBaseVertex")) != nullptr;
            r &= (glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)OGLGetProcAddress("glMultiDrawElementsBaseVertex")) != nullptr;
            r &= (glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)OGLGetProcAddress("glProvokingVertex")) != nullptr;
            r &= (glFenceSync = (PFNGLFENCESYNCPROC)OGLGetProcAddress("glFenceSync")) != nullptr;
            r &= (glIsSync = (PFNGLISSYNCPROC)OGLGetProcAddress("glIsSync")) != nullptr;
            r &= (glDeleteSync = (PFNGLDELETESYNCPROC)OGLGetProcAddress("glDeleteSync")) != nullptr;
            r &= (glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)OGLGetProcAddress("glClientWaitSync")) != nullptr;
            r &= (glWaitSync = (PFNGLWAITSYNCPROC)OGLGetProcAddress("glWaitSync")) != nullptr;
            r &= (glGetInteger64v = (PFNGLGETINTEGER64VPROC)OGLGetProcAddress("glGetInteger64v")) != nullptr;
            r &= (glGetSynciv = (PFNGLGETSYNCIVPROC)OGLGetProcAddress("glGetSynciv")) != nullptr;
            r &= (glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)OGLGetProcAddress("glGetInteger64i_v")) != nullptr;
            r &= (glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)OGLGetProcAddress("glGetBufferParameteri64v")) != nullptr;
            r &= (glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)OGLGetProcAddress("glFramebufferTexture")) != nullptr;
            r &= (glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)OGLGetProcAddress("glTexImage2DMultisample")) != nullptr;
            r &= (glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)OGLGetProcAddress("glTexImage3DMultisample")) != nullptr;
            r &= (glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC)OGLGetProcAddress("glGetMultisamplefv")) != nullptr;
            r &= (glSampleMaski = (PFNGLSAMPLEMASKIPROC)OGLGetProcAddress("glSampleMaski")) != nullptr;
        }

        // GL 3.3
        if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 33)
        {
            r &= (glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)OGLGetProcAddress("glBindFragDataLocationIndexed")) != nullptr;
            r &= (glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC)OGLGetProcAddress("glGetFragDataIndex")) != nullptr;
            r &= (glGenSamplers = (PFNGLGENSAMPLERSPROC)OGLGetProcAddress("glGenSamplers")) != nullptr;
            r &= (glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)OGLGetProcAddress("glDeleteSamplers")) != nullptr;
            r &= (glIsSampler = (PFNGLISSAMPLERPROC)OGLGetProcAddress("glIsSampler")) != nullptr;
            r &= (glBindSampler = (PFNGLBINDSAMPLERPROC)OGLGetProcAddress("glBindSampler")) != nullptr;
            r &= (glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)OGLGetProcAddress("glSamplerParameteri")) != nullptr;
            r &= (glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)OGLGetProcAddress("glSamplerParameteriv")) != nullptr;
            r &= (glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)OGLGetProcAddress("glSamplerParameterf")) != nullptr;
            r &= (glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)OGLGetProcAddress("glSamplerParameterfv")) != nullptr;
            r &= (glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)OGLGetProcAddress("glSamplerParameterIiv")) != nullptr;
            r &= (glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)OGLGetProcAddress("glSamplerParameterIuiv")) != nullptr;
            r &= (glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)OGLGetProcAddress("glGetSamplerParameteriv")) != nullptr;
            r &= (glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)OGLGetProcAddress("glGetSamplerParameterIiv")) != nullptr;
            r &= (glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)OGLGetProcAddress("glGetSamplerParameterfv")) != nullptr;
            r &= (glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)OGLGetProcAddress("glGetSamplerParameterIuiv")) != nullptr;
            r &= (glQueryCounter = (PFNGLQUERYCOUNTERPROC)OGLGetProcAddress("glQueryCounter")) != nullptr;
            r &= (glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC)OGLGetProcAddress("glGetQueryObjecti64v")) != nullptr;
            r &= (glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC)OGLGetProcAddress("glGetQueryObjectui64v")) != nullptr;
            r &= (glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)OGLGetProcAddress("glVertexAttribDivisor")) != nullptr;
            r &= (glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC)OGLGetProcAddress("glVertexAttribP1ui")) != nullptr;
            r &= (glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC)OGLGetProcAddress("glVertexAttribP1uiv")) != nullptr;
            r &= (glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC)OGLGetProcAddress("glVertexAttribP2ui")) != nullptr;
            r &= (glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC)OGLGetProcAddress("glVertexAttribP2uiv")) != nullptr;
            r &= (glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC)OGLGetProcAddress("glVertexAttribP3ui")) != nullptr;
            r &= (glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC)OGLGetProcAddress("glVertexAttribP3uiv")) != nullptr;
            r &= (glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC)OGLGetProcAddress("glVertexAttribP4ui")) != nullptr;
            r &= (glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC)OGLGetProcAddress("glVertexAttribP4uiv")) != nullptr;
        }
    }

    // GL 4.0 - GL 4.4
#if (GL_VERSION_NUM >= 40)
    if (openglSettings.majorVersion >= 4)
    {
        // GL 4.0
        r &= (glMinSampleShading = (PFNGLMINSAMPLESHADINGPROC)OGLGetProcAddress("glMinSampleShading")) != nullptr;
        r &= (glBlendEquationi = (PFNGLBLENDEQUATIONIPROC)OGLGetProcAddress("glBlendEquationi")) != nullptr;
        r &= (glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)OGLGetProcAddress("glBlendEquationSeparatei")) != nullptr;
        r &= (glBlendFunci = (PFNGLBLENDFUNCIPROC)OGLGetProcAddress("glBlendFunci")) != nullptr;
        r &= (glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)OGLGetProcAddress("glBlendFuncSeparatei")) != nullptr;
        r &= (glDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC)OGLGetProcAddress("glDrawArraysIndirect")) != nullptr;
        r &= (glDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC)OGLGetProcAddress("glDrawElementsIndirect")) != nullptr;
        r &= (glUniform1d = (PFNGLUNIFORM1DPROC)OGLGetProcAddress("glUniform1d")) != nullptr;
        r &= (glUniform2d = (PFNGLUNIFORM2DPROC)OGLGetProcAddress("glUniform2d")) != nullptr;
        r &= (glUniform3d = (PFNGLUNIFORM3DPROC)OGLGetProcAddress("glUniform3d")) != nullptr;
        r &= (glUniform4d = (PFNGLUNIFORM4DPROC)OGLGetProcAddress("glUniform4d")) != nullptr;
        r &= (glUniform1dv = (PFNGLUNIFORM1DVPROC)OGLGetProcAddress("glUniform1dv")) != nullptr;
        r &= (glUniform2dv = (PFNGLUNIFORM2DVPROC)OGLGetProcAddress("glUniform2dv")) != nullptr;
        r &= (glUniform3dv = (PFNGLUNIFORM3DVPROC)OGLGetProcAddress("glUniform3dv")) != nullptr;
        r &= (glUniform4dv = (PFNGLUNIFORM4DVPROC)OGLGetProcAddress("glUniform4dv")) != nullptr;
        r &= (glUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC)OGLGetProcAddress("glUniformMatrix2dv")) != nullptr;
        r &= (glUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC)OGLGetProcAddress("glUniformMatrix3dv")) != nullptr;
        r &= (glUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC)OGLGetProcAddress("glUniformMatrix4dv")) != nullptr;
        r &= (glUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC)OGLGetProcAddress("glUniformMatrix2x3dv")) != nullptr;
        r &= (glUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC)OGLGetProcAddress("glUniformMatrix2x4dv")) != nullptr;
        r &= (glUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC)OGLGetProcAddress("glUniformMatrix3x2dv")) != nullptr;
        r &= (glUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC)OGLGetProcAddress("glUniformMatrix3x4dv")) != nullptr;
        r &= (glUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC)OGLGetProcAddress("glUniformMatrix4x2dv")) != nullptr;
        r &= (glUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC)OGLGetProcAddress("glUniformMatrix4x3dv")) != nullptr;
        r &= (glGetUniformdv = (PFNGLGETUNIFORMDVPROC)OGLGetProcAddress("glGetUniformdv")) != nullptr;
        r &= (glGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC)OGLGetProcAddress("glGetSubroutineUniformLocation")) != nullptr;
        r &= (glGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC)OGLGetProcAddress("glGetSubroutineIndex")) != nullptr;
        r &= (glGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC)OGLGetProcAddress("glGetActiveSubroutineUniformiv")) != nullptr;
        r &= (glGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC)OGLGetProcAddress("glGetActiveSubroutineUniformName")) != nullptr;
        r &= (glGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC)OGLGetProcAddress("glGetActiveSubroutineName")) != nullptr;
        r &= (glUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC)OGLGetProcAddress("glUniformSubroutinesuiv")) != nullptr;
        r &= (glGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC)OGLGetProcAddress("glGetUniformSubroutineuiv")) != nullptr;
        r &= (glGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC)OGLGetProcAddress("glGetProgramStageiv")) != nullptr;
        r &= (glPatchParameteri = (PFNGLPATCHPARAMETERIPROC)OGLGetProcAddress("glPatchParameteri")) != nullptr;
        r &= (glPatchParameterfv = (PFNGLPATCHPARAMETERFVPROC)OGLGetProcAddress("glPatchParameterfv")) != nullptr;
        r &= (glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)OGLGetProcAddress("glBindTransformFeedback")) != nullptr;
        r &= (glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC)OGLGetProcAddress("glDeleteTransformFeedbacks")) != nullptr;
        r &= (glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)OGLGetProcAddress("glGenTransformFeedbacks")) != nullptr;
        r &= (glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC)OGLGetProcAddress("glIsTransformFeedback")) != nullptr;
        r &= (glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC)OGLGetProcAddress("glPauseTransformFeedback")) != nullptr;
        r &= (glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC)OGLGetProcAddress("glResumeTransformFeedback")) != nullptr;
        r &= (glDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC)OGLGetProcAddress("glDrawTransformFeedback")) != nullptr;
        r &= (glDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC)OGLGetProcAddress("glDrawTransformFeedbackStream")) != nullptr;
        r &= (glBeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC)OGLGetProcAddress("glBeginQueryIndexed")) != nullptr;
        r &= (glEndQueryIndexed = (PFNGLENDQUERYINDEXEDPROC)OGLGetProcAddress("glEndQueryIndexed")) != nullptr;
        r &= (glGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIVPROC)OGLGetProcAddress("glGetQueryIndexediv")) != nullptr;
    }
#endif

#if (GL_VERSION_NUM >= 41)
    if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 41)
    {
        // GL 4.1
        r &= (glReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC)OGLGetProcAddress("glReleaseShaderCompiler")) != nullptr;
        r &= (glShaderBinary = (PFNGLSHADERBINARYPROC)OGLGetProcAddress("glShaderBinary")) != nullptr;
        r &= (glGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC)OGLGetProcAddress("glGetShaderPrecisionFormat")) != nullptr;
        r &= (glDepthRangef = (PFNGLDEPTHRANGEFPROC)OGLGetProcAddress("glDepthRangef")) != nullptr;
        r &= (glClearDepthf = (PFNGLCLEARDEPTHFPROC)OGLGetProcAddress("glClearDepthf")) != nullptr;
        r &= (glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)OGLGetProcAddress("glGetProgramBinary")) != nullptr;
        r &= (glProgramBinary = (PFNGLPROGRAMBINARYPROC)OGLGetProcAddress("glProgramBinary")) != nullptr;
        r &= (glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)OGLGetProcAddress("glProgramParameteri")) != nullptr;
        r &= (glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)OGLGetProcAddress("glUseProgramStages")) != nullptr;
        r &= (glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)OGLGetProcAddress("glActiveShaderProgram")) != nullptr;
        r &= (glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)OGLGetProcAddress("glCreateShaderProgramv")) != nullptr;
        r &= (glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)OGLGetProcAddress("glBindProgramPipeline")) != nullptr;
        r &= (glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)OGLGetProcAddress("glDeleteProgramPipelines")) != nullptr;
        r &= (glGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC)OGLGetProcAddress("glGenProgramPipelines")) != nullptr;
        r &= (glIsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC)OGLGetProcAddress("glIsProgramPipeline")) != nullptr;
        r &= (glGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC)OGLGetProcAddress("glGetProgramPipelineiv")) != nullptr;
        r &= (glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)OGLGetProcAddress("glProgramUniform1i")) != nullptr;
        r &= (glProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC)OGLGetProcAddress("glProgramUniform1iv")) != nullptr;
        r &= (glProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC)OGLGetProcAddress("glProgramUniform1f")) != nullptr;
        r &= (glProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC)OGLGetProcAddress("glProgramUniform1fv")) != nullptr;
        r &= (glProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC)OGLGetProcAddress("glProgramUniform1d")) != nullptr;
        r &= (glProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC)OGLGetProcAddress("glProgramUniform1dv")) != nullptr;
        r &= (glProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC)OGLGetProcAddress("glProgramUniform1ui")) != nullptr;
        r &= (glProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC)OGLGetProcAddress("glProgramUniform1uiv")) != nullptr;
        r &= (glProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC)OGLGetProcAddress("glProgramUniform2i")) != nullptr;
        r &= (glProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC)OGLGetProcAddress("glProgramUniform2iv")) != nullptr;
        r &= (glProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC)OGLGetProcAddress("glProgramUniform2f")) != nullptr;
        r &= (glProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC)OGLGetProcAddress("glProgramUniform2fv")) != nullptr;
        r &= (glProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC)OGLGetProcAddress("glProgramUniform2d")) != nullptr;
        r &= (glProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC)OGLGetProcAddress("glProgramUniform2dv")) != nullptr;
        r &= (glProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC)OGLGetProcAddress("glProgramUniform2ui")) != nullptr;
        r &= (glProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC)OGLGetProcAddress("glProgramUniform2uiv")) != nullptr;
        r &= (glProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC)OGLGetProcAddress("glProgramUniform3i")) != nullptr;
        r &= (glProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC)OGLGetProcAddress("glProgramUniform3iv")) != nullptr;
        r &= (glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)OGLGetProcAddress("glProgramUniform3f")) != nullptr;
        r &= (glProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC)OGLGetProcAddress("glProgramUniform3fv")) != nullptr;
        r &= (glProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC)OGLGetProcAddress("glProgramUniform3d")) != nullptr;
        r &= (glProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC)OGLGetProcAddress("glProgramUniform3dv")) != nullptr;
        r &= (glProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC)OGLGetProcAddress("glProgramUniform3ui")) != nullptr;
        r &= (glProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC)OGLGetProcAddress("glProgramUniform3uiv")) != nullptr;
        r &= (glProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC)OGLGetProcAddress("glProgramUniform4i")) != nullptr;
        r &= (glProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC)OGLGetProcAddress("glProgramUniform4iv")) != nullptr;
        r &= (glProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC)OGLGetProcAddress("glProgramUniform4f")) != nullptr;
        r &= (glProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC)OGLGetProcAddress("glProgramUniform4fv")) != nullptr;
        r &= (glProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC)OGLGetProcAddress("glProgramUniform4d")) != nullptr;
        r &= (glProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC)OGLGetProcAddress("glProgramUniform4dv")) != nullptr;
        r &= (glProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC)OGLGetProcAddress("glProgramUniform4ui")) != nullptr;
        r &= (glProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC)OGLGetProcAddress("glProgramUniform4uiv")) != nullptr;
        r &= (glProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)OGLGetProcAddress("glProgramUniformMatrix2fv")) != nullptr;
        r &= (glProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)OGLGetProcAddress("glProgramUniformMatrix3fv")) != nullptr;
        r &= (glProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)OGLGetProcAddress("glProgramUniformMatrix4fv")) != nullptr;
        r &= (glProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)OGLGetProcAddress("glProgramUniformMatrix2dv")) != nullptr;
        r &= (glProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)OGLGetProcAddress("glProgramUniformMatrix3dv")) != nullptr;
        r &= (glProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)OGLGetProcAddress("glProgramUniformMatrix4dv")) != nullptr;
        r &= (glProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)OGLGetProcAddress("glProgramUniformMatrix2x3fv")) != nullptr;
        r &= (glProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)OGLGetProcAddress("glProgramUniformMatrix3x2fv")) != nullptr;
        r &= (glProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)OGLGetProcAddress("glProgramUniformMatrix2x4fv")) != nullptr;
        r &= (glProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)OGLGetProcAddress("glProgramUniformMatrix4x2fv")) != nullptr;
        r &= (glProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)OGLGetProcAddress("glProgramUniformMatrix3x4fv")) != nullptr;
        r &= (glProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)OGLGetProcAddress("glProgramUniformMatrix4x3fv")) != nullptr;
        r &= (glProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC)OGLGetProcAddress("glProgramUniformMatrix2x3dv")) != nullptr;
        r &= (glProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC)OGLGetProcAddress("glProgramUniformMatrix3x2dv")) != nullptr;
        r &= (glProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC)OGLGetProcAddress("glProgramUniformMatrix2x4dv")) != nullptr;
        r &= (glProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC)OGLGetProcAddress("glProgramUniformMatrix4x2dv")) != nullptr;
        r &= (glProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC)OGLGetProcAddress("glProgramUniformMatrix3x4dv")) != nullptr;
        r &= (glProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC)OGLGetProcAddress("glProgramUniformMatrix4x3dv")) != nullptr;
        r &= (glValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC)OGLGetProcAddress("glValidateProgramPipeline")) != nullptr;
        r &= (glGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)OGLGetProcAddress("glGetProgramPipelineInfoLog")) != nullptr;
        r &= (glVertexAttribL1d = (PFNGLVERTEXATTRIBL1DPROC)OGLGetProcAddress("glVertexAttribL1d")) != nullptr;
        r &= (glVertexAttribL2d = (PFNGLVERTEXATTRIBL2DPROC)OGLGetProcAddress("glVertexAttribL2d")) != nullptr;
        r &= (glVertexAttribL3d = (PFNGLVERTEXATTRIBL3DPROC)OGLGetProcAddress("glVertexAttribL3d")) != nullptr;
        r &= (glVertexAttribL4d = (PFNGLVERTEXATTRIBL4DPROC)OGLGetProcAddress("glVertexAttribL4d")) != nullptr;
        r &= (glVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DVPROC)OGLGetProcAddress("glVertexAttribL1dv")) != nullptr;
        r &= (glVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DVPROC)OGLGetProcAddress("glVertexAttribL2dv")) != nullptr;
        r &= (glVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DVPROC)OGLGetProcAddress("glVertexAttribL3dv")) != nullptr;
        r &= (glVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DVPROC)OGLGetProcAddress("glVertexAttribL4dv")) != nullptr;
        r &= (glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC)OGLGetProcAddress("glVertexAttribLPointer")) != nullptr;
        r &= (glGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDVPROC)OGLGetProcAddress("glGetVertexAttribLdv")) != nullptr;
        r &= (glViewportArrayv = (PFNGLVIEWPORTARRAYVPROC)OGLGetProcAddress("glViewportArrayv")) != nullptr;
        r &= (glViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC)OGLGetProcAddress("glViewportIndexedf")) != nullptr;
        r &= (glViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC)OGLGetProcAddress("glViewportIndexedfv")) != nullptr;
        r &= (glScissorArrayv = (PFNGLSCISSORARRAYVPROC)OGLGetProcAddress("glScissorArrayv")) != nullptr;
        r &= (glScissorIndexed = (PFNGLSCISSORINDEXEDPROC)OGLGetProcAddress("glScissorIndexed")) != nullptr;
        r &= (glScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC)OGLGetProcAddress("glScissorIndexedv")) != nullptr;
        r &= (glDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC)OGLGetProcAddress("glDepthRangeArrayv")) != nullptr;
        r &= (glDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC)OGLGetProcAddress("glDepthRangeIndexed")) != nullptr;
        r &= (glGetFloati_v = (PFNGLGETFLOATI_VPROC)OGLGetProcAddress("glGetFloati_v")) != nullptr;
        r &= (glGetDoublei_v = (PFNGLGETDOUBLEI_VPROC)OGLGetProcAddress("glGetDoublei_v")) != nullptr;
    }
#endif

#if (GL_VERSION_NUM >= 42)
    if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 42)
    {
        // GL 4.2
        r &= (glDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)OGLGetProcAddress("glDrawArraysInstancedBaseInstance")) != nullptr;
        r &= (glDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC)OGLGetProcAddress("glDrawElementsInstancedBaseInstance")) != nullptr;
        r &= (glDrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC)OGLGetProcAddress("glDrawElementsInstancedBaseVertexBaseInstance")) != nullptr;
        r &= (glGetInternalformativ = (PFNGLGETINTERNALFORMATIVPROC)OGLGetProcAddress("glGetInternalformativ")) != nullptr;
        r &= (glGetActiveAtomicCounterBufferiv = (PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC)OGLGetProcAddress("glGetActiveAtomicCounterBufferiv")) != nullptr;
        r &= (glBindImageTexture = (PFNGLBINDIMAGETEXTUREPROC)OGLGetProcAddress("glBindImageTexture")) != nullptr;
        r &= (glMemoryBarrier = (PFNGLMEMORYBARRIERPROC)OGLGetProcAddress("glMemoryBarrier")) != nullptr;
        r &= (glTexStorage1D = (PFNGLTEXSTORAGE1DPROC)OGLGetProcAddress("glTexStorage1D")) != nullptr;
        r &= (glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)OGLGetProcAddress("glTexStorage2D")) != nullptr;
        r &= (glTexStorage3D = (PFNGLTEXSTORAGE3DPROC)OGLGetProcAddress("glTexStorage3D")) != nullptr;
        r &= (glDrawTransformFeedbackInstanced = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC)OGLGetProcAddress("glDrawTransformFeedbackInstanced")) != nullptr;
        r &= (glDrawTransformFeedbackStreamInstanced = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC)OGLGetProcAddress("glDrawTransformFeedbackStreamInstanced")) != nullptr;
    }
#endif

#if (GL_VERSION_NUM >= 43)
    if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 43)
    {
        // GL 4.3
        r &= (glClearBufferData = (PFNGLCLEARBUFFERDATAPROC)OGLGetProcAddress("glClearBufferData")) != nullptr;
        r &= (glClearBufferSubData = (PFNGLCLEARBUFFERSUBDATAPROC)OGLGetProcAddress("glClearBufferSubData")) != nullptr;
        r &= (glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)OGLGetProcAddress("glDispatchCompute")) != nullptr;
        r &= (glDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECTPROC)OGLGetProcAddress("glDispatchComputeIndirect")) != nullptr;
        r &= (glCopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC)OGLGetProcAddress("glCopyImageSubData")) != nullptr;
        r &= (glFramebufferParameteri = (PFNGLFRAMEBUFFERPARAMETERIPROC)OGLGetProcAddress("glFramebufferParameteri")) != nullptr;
        r &= (glGetFramebufferParameteriv = (PFNGLGETFRAMEBUFFERPARAMETERIVPROC)OGLGetProcAddress("glGetFramebufferParameteriv")) != nullptr;
        r &= (glGetInternalformati64v = (PFNGLGETINTERNALFORMATI64VPROC)OGLGetProcAddress("glGetInternalformati64v")) != nullptr;
        r &= (glInvalidateTexSubImage = (PFNGLINVALIDATETEXSUBIMAGEPROC)OGLGetProcAddress("glInvalidateTexSubImage")) != nullptr;
        r &= (glInvalidateTexImage = (PFNGLINVALIDATETEXIMAGEPROC)OGLGetProcAddress("glInvalidateTexImage")) != nullptr;
        r &= (glInvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATAPROC)OGLGetProcAddress("glInvalidateBufferSubData")) != nullptr;
        r &= (glInvalidateBufferData = (PFNGLINVALIDATEBUFFERDATAPROC)OGLGetProcAddress("glInvalidateBufferData")) != nullptr;
        r &= (glInvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFERPROC)OGLGetProcAddress("glInvalidateFramebuffer")) != nullptr;
        r &= (glInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFERPROC)OGLGetProcAddress("glInvalidateSubFramebuffer")) != nullptr;
        r &= (glMultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECTPROC)OGLGetProcAddress("glMultiDrawArraysIndirect")) != nullptr;
        r &= (glMultiDrawElementsIndirect = (PFNGLMULTIDRAWELEMENTSINDIRECTPROC)OGLGetProcAddress("glMultiDrawElementsIndirect")) != nullptr;
        r &= (glGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC)OGLGetProcAddress("glGetProgramInterfaceiv")) != nullptr;
        r &= (glGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEXPROC)OGLGetProcAddress("glGetProgramResourceIndex")) != nullptr;
        r &= (glGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC)OGLGetProcAddress("glGetProgramResourceName")) != nullptr;
        r &= (glGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC)OGLGetProcAddress("glGetProgramResourceiv")) != nullptr;
        r &= (glGetProgramResourceLocation = (PFNGLGETPROGRAMRESOURCELOCATIONPROC)OGLGetProcAddress("glGetProgramResourceLocation")) != nullptr;
        r &= (glGetProgramResourceLocationIndex = (PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC)OGLGetProcAddress("glGetProgramResourceLocationIndex")) != nullptr;
        r &= (glShaderStorageBlockBinding = (PFNGLSHADERSTORAGEBLOCKBINDINGPROC)OGLGetProcAddress("glShaderStorageBlockBinding")) != nullptr;
        r &= (glTexBufferRange = (PFNGLTEXBUFFERRANGEPROC)OGLGetProcAddress("glTexBufferRange")) != nullptr;
        r &= (glTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLEPROC)OGLGetProcAddress("glTexStorage2DMultisample")) != nullptr;
        r &= (glTexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLEPROC)OGLGetProcAddress("glTexStorage3DMultisample")) != nullptr;
        r &= (glTextureView = (PFNGLTEXTUREVIEWPROC)OGLGetProcAddress("glTextureView")) != nullptr;
        r &= (glBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC)OGLGetProcAddress("glBindVertexBuffer")) != nullptr;
        r &= (glVertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC)OGLGetProcAddress("glVertexAttribFormat")) != nullptr;
        r &= (glVertexAttribIFormat = (PFNGLVERTEXATTRIBIFORMATPROC)OGLGetProcAddress("glVertexAttribIFormat")) != nullptr;
        r &= (glVertexAttribLFormat = (PFNGLVERTEXATTRIBLFORMATPROC)OGLGetProcAddress("glVertexAttribLFormat")) != nullptr;
        r &= (glVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC)OGLGetProcAddress("glVertexAttribBinding")) != nullptr;
        r &= (glVertexBindingDivisor = (PFNGLVERTEXBINDINGDIVISORPROC)OGLGetProcAddress("glVertexBindingDivisor")) != nullptr;
        r &= (glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)OGLGetProcAddress("glDebugMessageControl")) != nullptr;
        r &= (glDebugMessageInsert = (PFNGLDEBUGMESSAGEINSERTPROC)OGLGetProcAddress("glDebugMessageInsert")) != nullptr;
        r &= (glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)OGLGetProcAddress("glDebugMessageCallback")) != nullptr;
        r &= (glGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGPROC)OGLGetProcAddress("glGetDebugMessageLog")) != nullptr;
        r &= (glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC)OGLGetProcAddress("glPushDebugGroup")) != nullptr;
        r &= (glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)OGLGetProcAddress("glPopDebugGroup")) != nullptr;
        r &= (glObjectLabel = (PFNGLOBJECTLABELPROC)OGLGetProcAddress("glObjectLabel")) != nullptr;
        r &= (glGetObjectLabel = (PFNGLGETOBJECTLABELPROC)OGLGetProcAddress("glGetObjectLabel")) != nullptr;
        r &= (glObjectPtrLabel = (PFNGLOBJECTPTRLABELPROC)OGLGetProcAddress("glObjectPtrLabel")) != nullptr;
        r &= (glGetObjectPtrLabel = (PFNGLGETOBJECTPTRLABELPROC)OGLGetProcAddress("glGetObjectPtrLabel")) != nullptr;
    }
#endif

#if (GL_VERSION_NUM >= 44)
    if (openglSettings.majorVersion * 10 + openglSettings.minorVersion >= 44)
    {
        // GL 4.4
        r &= (glBufferStorage = (PFNGLBUFFERSTORAGEPROC)OGLGetProcAddress("glBufferStorage")) != nullptr;
        r &= (glClearTexImage = (PFNGLCLEARTEXIMAGEPROC)OGLGetProcAddress("glClearTexImage")) != nullptr;
        r &= (glClearTexSubImage = (PFNGLCLEARTEXSUBIMAGEPROC)OGLGetProcAddress("glClearTexSubImage")) != nullptr;
        r &= (glBindBuffersBase = (PFNGLBINDBUFFERSBASEPROC)OGLGetProcAddress("glBindBuffersBase")) != nullptr;
        r &= (glBindBuffersRange = (PFNGLBINDBUFFERSRANGEPROC)OGLGetProcAddress("glBindBuffersRange")) != nullptr;
        r &= (glBindTextures = (PFNGLBINDTEXTURESPROC)OGLGetProcAddress("glBindTextures")) != nullptr;
        r &= (glBindSamplers = (PFNGLBINDSAMPLERSPROC)OGLGetProcAddress("glBindSamplers")) != nullptr;
        r &= (glBindImageTextures = (PFNGLBINDIMAGETEXTURESPROC)OGLGetProcAddress("glBindImageTextures")) != nullptr;
        r &= (glBindVertexBuffers = (PFNGLBINDVERTEXBUFFERSPROC)OGLGetProcAddress("glBindVertexBuffers")) != nullptr;
    }
#endif

    // OES image
    openglSettings.OES_EGL_image = OGLIsExtensionSupported("GL_OES_EGL_image");
    if (openglSettings.OES_EGL_image)
    {
        r &= (glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)OGLGetProcAddress("glEGLImageTargetTexture2DOES")) != nullptr;
    }

    // Default extensions, suitable for any OpenGL version
    openglSettings.EXT_texture_filter_anisotropic = OGLIsExtensionSupported("GL_EXT_texture_filter_anisotropic");
    openglSettings.EXT_texture_compression_s3tc = OGLIsExtensionSupported("GL_EXT_texture_compression_s3tc") || OGLIsExtensionSupported("GL_S3_s3tc");
    openglSettings.EXT_texture_sRGB = OGLIsExtensionSupported("GL_EXT_texture_sRGB");

    return r;
}

