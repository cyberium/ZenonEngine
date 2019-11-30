#pragma once

//---------------------------------------------------------//
//--                      Types                          --//
//---------------------------------------------------------//
#include "Interfaces/Types/BufferBinding.h"
#include "Interfaces/Types/ClearFlags.h"
#include "Interfaces/Types/CPUAccess.h"
#include "Interfaces/Types/InputSemantic.h"
#include "Interfaces/Types/PrimitiveTopology.h"
#include "Interfaces/Types/Viewport.h"
#include "Interfaces/Types/CustomVertexElement.h"


//---------------------------------------------------------//
//--                   Interfaces                        --//
//---------------------------------------------------------//
#include "Interfaces/IShader.h"
#include "Interfaces/IShaderParameter.h"
#include "Interfaces/IShaderInputLayout.h"

#include "Interfaces/ITexture.h"
#include "Interfaces/ISamplerState.h"
#include "Interfaces/IBuffer.h"
#include "Interfaces/IQuery.h"
#include "Interfaces/IMaterial.h"
#include "Interfaces/IMesh.h"

#include "Interfaces/IRenderTarget.h"
#include "Interfaces/IRenderWindow.h"

#include "Interfaces/IBlendState.h"
#include "Interfaces/IDepthStencilState.h"
#include "Interfaces/IRasterizerState.h"
#include "Interfaces/IPipelineState.h"

#include "Interfaces/IRenderDevice.h"
