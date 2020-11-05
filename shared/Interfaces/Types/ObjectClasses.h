#pragma once

//
// Core
//
const ObjectClass cManager                      = UINT16_MAX - 0u;
//const ObjectClass cBaseManager                  = UINT16_MAX - 1u;
const ObjectClass cFontsManager                 = UINT16_MAX - 2u;
const ObjectClass cImagesFactory                = UINT16_MAX - 3u;
const ObjectClass cModelsFactory                = UINT16_MAX - 4u;
const ObjectClass cTexturesFactory              = UINT16_MAX - 5u;
const ObjectClass cMaterialsFactory             = UINT16_MAX - 6u;
const ObjectClass cRenderDeviceFactory          = UINT16_MAX - 7u;
const ObjectClass cRenderPassFactory            = UINT16_MAX - 8u;
const ObjectClass cObjectClassFactory           = UINT16_MAX - 9u;
const ObjectClass cObjectsFactory               = UINT16_MAX - 10u;
const ObjectClass cConsole                      = UINT16_MAX - 11u;
const ObjectClass cFilesManager                 = UINT16_MAX - 12u;
const ObjectClass cLoader                       = UINT16_MAX - 13u;
const ObjectClass cLog                          = UINT16_MAX - 14u;
const ObjectClass cPluginsManager               = UINT16_MAX - 15u;
const ObjectClass cSettingsManager              = UINT16_MAX - 16u;

//
// 3D
//
ZN_MAKE_OBJECTCLASS(cSceneNode3D, 100);
ZN_MAKE_OBJECTCLASS(cCRTSSceneNodeGround, 101);
ZN_MAKE_OBJECTCLASS(cSceneNodePoint, 102);
ZN_MAKE_OBJECTCLASS(cSceneNodePath, 103);



//
// UI
//
ZN_MAKE_OBJECTCLASS(cSceneNodeUI, 200);
ZN_MAKE_OBJECTCLASS(cSceneNodeUI_Text, 201);
ZN_MAKE_OBJECTCLASS(cSceneNodeUI_Color, 202);



//
// 3D components
//
const ObjectClass cSceneNodeComponent           = UINT16_MAX - 500u;
const ObjectClass cSceneNodeColliderComponent   = UINT16_MAX - 501u;
const ObjectClass cSceneNodeModelsComponent     = UINT16_MAX - 502u;
const ObjectClass cSceneNodeSkeletonComponent   = UINT16_MAX - 503u;
const ObjectClass cSceneNodeParticleComponent   = UINT16_MAX - 504u;
const ObjectClass cSceneNodePhysicsComponent    = UINT16_MAX - 505u;
const ObjectClass cSceneNodePortalsComponent    = UINT16_MAX - 506u;
const ObjectClass cSceneNodeLightComponent      = UINT16_MAX - 507u;
const ObjectClass cSceneNodeCameraComponent     = UINT16_MAX - 508u;
const ObjectClass cSceneNodeAnimationComponent  = UINT16_MAX - 509u;
