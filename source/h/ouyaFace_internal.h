/*
 * Internal header for the ouyaFace extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef OUYAFACE_INTERNAL_H
#define OUYAFACE_INTERNAL_H

#include "s3eTypes.h"
#include "ouyaFace.h"
#include "ouyaFace_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult ouyaFaceInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult ouyaFaceInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void ouyaFaceTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void ouyaFaceTerminate_platform();
void ouyaInit_platform(const char* pDeveloperId, const char* pApplicationKey);

void ouyaTerm_platform();

int32 ouyaFacadeIsInitialised_platform();

int32 ouyaFacadeIsRunningOnOUYAHardware_platform();

void ouyaFacadeGetGameData_platform(const char* pKey, char* pBuffer, int bufferSize);

void ouyaFacadePutGameData_platform(const char* pKey, const char* pValue);

void ouyaFacadeSetTestMode_platform();

s3eResult ouyaFacadeRequestGamerUUID_platform(s3eCallback pCallback, void* pUserData);

s3eResult ouyaFacadeRequestReceipts_platform(s3eCallback pCallback, void* pUserData);

s3eResult ouyaFacadeRequestProductList_platform(const char** parPurchasable, int numPurchasables, s3eCallback pCallback, void* pUserData);

s3eResult ouyaFacadeRequestPurchase_platform(const char* pPurchasable, s3eCallback pCallback, void* pUserData);

//s3eResult ouyaControllerRegister_platform(OuyaControllerEvent type, s3eCallback pCallback, void* pUserData);

//s3eResult ouyaControllerUnRegister_platform(OuyaControllerEvent type, s3eCallback pCallback);

int32 ouyaControllerGetButtonState_platform(uint32 controller, uint32 button);

float ouyaControllerGetAxis_platform(uint32 controller, uint32 axis);


#endif /* !OUYAFACE_INTERNAL_H */