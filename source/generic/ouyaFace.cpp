/*
Generic implementation of the ouyaFace extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include <s3eEdk.h>
#include "ouyaFace_internal.h"
s3eResult ouyaFaceInit()
{
    //Add any generic initialisation code here
    return ouyaFaceInit_platform();
}

void ouyaFaceTerminate()
{
    //Add any generic termination code here
    ouyaFaceTerminate_platform();
}

void ouyaInit(const char* pDeveloperId, const char* pApplicationKey)
{
	ouyaInit_platform(pDeveloperId, pApplicationKey);
}

void ouyaTerm()
{
	ouyaTerm_platform();
}

int ouyaFacadeIsInitialised()
{
	return ouyaFacadeIsInitialised_platform();
}

int ouyaFacadeIsRunningOnOUYAHardware()
{
	return ouyaFacadeIsRunningOnOUYAHardware_platform();
}

void ouyaFacadeGetGameData(const char* pKey, char* pBuffer, int bufferSize)
{
	ouyaFacadeGetGameData_platform(pKey, pBuffer, bufferSize);
}

void ouyaFacadePutGameData(const char* pKey, const char* pValue)
{
	ouyaFacadePutGameData_platform(pKey, pValue);
}

void ouyaFacadeSetTestMode()
{
	ouyaFacadeSetTestMode_platform();
}

s3eResult ouyaFacadeRequestGamerUUID(s3eCallback pCallback, void* pUserData)
{
	return ouyaFacadeRequestGamerUUID_platform(pCallback, pUserData);
}

s3eResult ouyaFacadeRequestReceipts(s3eCallback pCallback, void* pUserData)
{
	return ouyaFacadeRequestReceipts_platform(pCallback, pUserData);
}

s3eResult ouyaFacadeRequestProductList(const char** parPurchasable, int numPurchasables, s3eCallback pCallback, void* pUserData)
{
	return ouyaFacadeRequestProductList_platform(parPurchasable, numPurchasables, pCallback, pUserData);
}

s3eResult ouyaFacadeRequestPurchase(const char* pPurchasable, s3eCallback pCallback, void* pUserData)
{
	return ouyaFacadeRequestPurchase_platform(pPurchasable, pCallback, pUserData);
}

s3eResult ouyaControllerRegister(OuyaControllerEvent type, s3eCallback pCallback, void* pUserData)
{
  s3eResult result = s3eEdkCallbacksRegister(S3E_EXT_OUYAFACE_HASH,
    kNumOuyaControllerEvents, type, pCallback, pUserData, true);

	return result;
}

s3eResult ouyaControllerUnRegister(OuyaControllerEvent type, s3eCallback pCallback)
{
  s3eResult result = s3eEdkCallbacksUnRegister(S3E_EXT_OUYAFACE_HASH,
    kNumOuyaControllerEvents, type, pCallback);

  return result;
}

int ouyaControllerGetButtonState(uint32 controller, uint32 button)
{
	return ouyaControllerGetButtonState_platform(controller, button);
}

float ouyaControllerGetAxis(uint32 controller, uint32 axis)
{
	return ouyaControllerGetAxis_platform(controller, axis);
}
