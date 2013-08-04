/*
 * WARNING: this is an autogenerated file and will be overwritten by
 * the extension interface script.
 */
/*
 * This file contains the automatically generated loader-side
 * functions that form part of the extension.
 *
 * This file is awlays compiled into all loaders but compiles
 * to nothing if this extension is not enabled in the loader
 * at build time.
 */
#include "IwDebug.h"
#include "ouyaFace_autodefs.h"
#include "s3eEdk.h"
#include "ouyaFace.h"
//Declarations of Init and Term functions
extern s3eResult ouyaFaceInit();
extern void ouyaFaceTerminate();


// On platforms that use a seperate UI/OS thread we can autowrap functions
// here.   Note that we can't use the S3E_USE_OS_THREAD define since this
// code is oftern build standalone, outside the main loader build.
#if defined I3D_OS_IPHONE || defined I3D_OS_OSX || defined I3D_OS_LINUX || defined I3D_OS_WINDOWS

static void ouyaInit_wrap(const char* pDeveloperId, const char* pApplicationKey)
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaInit"));
    s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaInit, 2, pDeveloperId, pApplicationKey);
}

static void ouyaTerm_wrap()
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaTerm"));
    s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaTerm, 0);
}

static int ouyaFacadeIsInitialised_wrap()
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaFacadeIsInitialised"));
    return (int)(intptr_t)s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaFacadeIsInitialised, 0);
}

static int ouyaFacadeIsRunningOnOUYAHardware_wrap()
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaFacadeIsRunningOnOUYAHardware"));
    return (int)(intptr_t)s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaFacadeIsRunningOnOUYAHardware, 0);
}

static void ouyaFacadeGetGameData_wrap(const char* pKey, char* pBuffer, int bufferSize)
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaFacadeGetGameData"));
    s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaFacadeGetGameData, 3, pKey, pBuffer, bufferSize);
}

static void ouyaFacadePutGameData_wrap(const char* pKey, const char* pValue)
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaFacadePutGameData"));
    s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaFacadePutGameData, 2, pKey, pValue);
}

static void ouyaFacadeSetTestMode_wrap()
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaFacadeSetTestMode"));
    s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaFacadeSetTestMode, 0);
}

static s3eResult ouyaFacadeRequestGamerUUID_wrap(s3eCallback pCallback, void* pUserData)
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaFacadeRequestGamerUUID"));
    return (s3eResult)(intptr_t)s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaFacadeRequestGamerUUID, 2, pCallback, pUserData);
}

static s3eResult ouyaFacadeRequestReceipts_wrap(s3eCallback pCallback, void* pUserData)
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaFacadeRequestReceipts"));
    return (s3eResult)(intptr_t)s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaFacadeRequestReceipts, 2, pCallback, pUserData);
}

static s3eResult ouyaFacadeRequestProductList_wrap(const char** parPurchasable, int numPurchasables, s3eCallback pCallback, void* pUserData)
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaFacadeRequestProductList"));
    return (s3eResult)(intptr_t)s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaFacadeRequestProductList, 4, parPurchasable, numPurchasables, pCallback, pUserData);
}

static s3eResult ouyaFacadeRequestPurchase_wrap(const char* pPurchasable, s3eCallback pCallback, void* pUserData)
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaFacadeRequestPurchase"));
    return (s3eResult)(intptr_t)s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaFacadeRequestPurchase, 3, pPurchasable, pCallback, pUserData);
}

static s3eResult ouyaControllerRegister_wrap(OuyaControllerEvent type, s3eCallback pCallback, void* pUserData)
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaControllerRegister"));
    return (s3eResult)(intptr_t)s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaControllerRegister, 3, type, pCallback, pUserData);
}

static s3eResult ouyaControllerUnRegister_wrap(OuyaControllerEvent type, s3eCallback pCallback)
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaControllerUnRegister"));
    return (s3eResult)(intptr_t)s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaControllerUnRegister, 2, type, pCallback);
}

static int ouyaControllerGetButtonState_wrap(uint32 controller, uint32 button)
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaControllerGetButtonState"));
    return (int)(intptr_t)s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaControllerGetButtonState, 2, controller, button);
}

static float ouyaControllerGetAxis_wrap(uint32 controller, uint32 axis)
{
    IwTrace(OUYAFACE_VERBOSE, ("calling ouyaFace func on main thread: ouyaControllerGetAxis"));
    return (float)(intptr_t)s3eEdkThreadRunOnOS((s3eEdkThreadFunc)ouyaControllerGetAxis, 2, controller, axis);
}

#define ouyaInit ouyaInit_wrap
#define ouyaTerm ouyaTerm_wrap
#define ouyaFacadeIsInitialised ouyaFacadeIsInitialised_wrap
#define ouyaFacadeIsRunningOnOUYAHardware ouyaFacadeIsRunningOnOUYAHardware_wrap
#define ouyaFacadeGetGameData ouyaFacadeGetGameData_wrap
#define ouyaFacadePutGameData ouyaFacadePutGameData_wrap
#define ouyaFacadeSetTestMode ouyaFacadeSetTestMode_wrap
#define ouyaFacadeRequestGamerUUID ouyaFacadeRequestGamerUUID_wrap
#define ouyaFacadeRequestReceipts ouyaFacadeRequestReceipts_wrap
#define ouyaFacadeRequestProductList ouyaFacadeRequestProductList_wrap
#define ouyaFacadeRequestPurchase ouyaFacadeRequestPurchase_wrap
#define ouyaControllerRegister ouyaControllerRegister_wrap
#define ouyaControllerUnRegister ouyaControllerUnRegister_wrap
#define ouyaControllerGetButtonState ouyaControllerGetButtonState_wrap
#define ouyaControllerGetAxis ouyaControllerGetAxis_wrap

#endif

void ouyaFaceRegisterExt()
{
    /* fill in the function pointer struct for this extension */
    void* funcPtrs[15];
    funcPtrs[0] = (void*)ouyaInit;
    funcPtrs[1] = (void*)ouyaTerm;
    funcPtrs[2] = (void*)ouyaFacadeIsInitialised;
    funcPtrs[3] = (void*)ouyaFacadeIsRunningOnOUYAHardware;
    funcPtrs[4] = (void*)ouyaFacadeGetGameData;
    funcPtrs[5] = (void*)ouyaFacadePutGameData;
    funcPtrs[6] = (void*)ouyaFacadeSetTestMode;
    funcPtrs[7] = (void*)ouyaFacadeRequestGamerUUID;
    funcPtrs[8] = (void*)ouyaFacadeRequestReceipts;
    funcPtrs[9] = (void*)ouyaFacadeRequestProductList;
    funcPtrs[10] = (void*)ouyaFacadeRequestPurchase;
    funcPtrs[11] = (void*)ouyaControllerRegister;
    funcPtrs[12] = (void*)ouyaControllerUnRegister;
    funcPtrs[13] = (void*)ouyaControllerGetButtonState;
    funcPtrs[14] = (void*)ouyaControllerGetAxis;

    /*
     * Flags that specify the extension's use of locking and stackswitching
     */
    int flags[15] = { 0 };

    /*
     * Register the extension
     */
    s3eEdkRegister("ouyaFace", funcPtrs, sizeof(funcPtrs), flags, ouyaFaceInit, ouyaFaceTerminate, 0);
}

#if !defined S3E_BUILD_S3ELOADER

#if defined S3E_EDK_USE_STATIC_INIT_ARRAY
int ouyaFaceStaticInit()
{
    void** p = g_StaticInitArray;
    void* end = p + g_StaticArrayLen;
    while (*p) p++;
    if (p < end)
        *p = (void*)&ouyaFaceRegisterExt;
    return 0;
}

int g_ouyaFaceVal = ouyaFaceStaticInit();

#elif defined S3E_EDK_USE_DLLS
S3E_EXTERN_C S3E_DLL_EXPORT void RegisterExt()
{
    ouyaFaceRegisterExt();
}
#endif

#endif
