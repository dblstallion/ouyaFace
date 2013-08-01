/*
 * android-specific implementation of the ouyaFace extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "ouyaFace_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"

enum  OuyaFacadeEvent
{
  OUYA_FACADE_GAMER_UUID_EVENT,
  OUYA_FACADE_RECEIPTS_EVENT,
  OUYA_FACADE_PRODUCT_LIST_EVENT,
  OUYA_FACADE_PURCHASE_EVENT,
  kNumOuyaFacadeEvents
};

static jobject g_Obj;
static jmethodID g_ouyaInit;
static jmethodID g_ouyaTerm;
static jmethodID g_ouyaFacadeIsInitialised;
static jmethodID g_ouyaFacadeIsRunningOnOUYAHardware;
static jmethodID g_ouyaFacadeGetGameData;
static jmethodID g_ouyaFacadePutGameData;
static jmethodID g_ouyaFacadeSetTestMode;
static jmethodID g_ouyaFacadeRequestGamerUUID;
static jmethodID g_ouyaFacadeRequestReceipts;
static jmethodID g_ouyaFacadeRequestProductList;
static jmethodID g_ouyaFacadeRequestPurchase;
static jmethodID g_ouyaControllerRegister;
static jmethodID g_ouyaControllerUnRegister;
static jmethodID g_ouyaControllerGetButtonState;
static jmethodID g_ouyaControllerGetAxis;

void  ouyaFaceOnGenericMotionEvent(JNIEnv* env, jobject obj, jint deviceId, jint playerId, jfloat lx, jfloat ly, jfloat rx, jfloat ry, jfloat lTrig, jfloat rTrig);
void  ouyaFaceOnKeyEvent(JNIEnv* env, jobject obj, jint deviceId, jint playerId, jint keyCode, jint action);
    
void	ouyaFaceOnGamerUUIDResult(JNIEnv* env, jobject obj, jint error, jstring uuid);
void	ouyaFaceOnReceiptsResult(JNIEnv* env, jobject obj, jint error, jobjectArray receipts);
void	ouyaFaceOnProductListResult(JNIEnv* env, jobject obj, jint error, jobjectArray products);
void	ouyaFaceOnPurchaseResult(JNIEnv* env, jobject obj, jint error, jobject product);

//void(* s3eEdkCallbackCompleteFn)(uint32 extID, int32 notification, void *systemData, void *instance, int32 returnCode, void *completeData)  

static const JNINativeMethod karOuyaFaceNative[] =
{
  { "ouyaFaceOnGenericMotionEvent", "(IIFFFFFF)V", (void*)&ouyaFaceOnGenericMotionEvent },
  { "ouyaFaceOnKeyEvent", "(IIII)V", (void*)&ouyaFaceOnKeyEvent },
  { "ouyaFaceOnGamerUUIDResult", "(ILjava/lang/String;)V", (void*)&ouyaFaceOnGamerUUIDResult },
  { "ouyaFaceOnReceiptsResult", "(I[Ltv/ouya/console/api/Receipt;)V", (void*)&ouyaFaceOnReceiptsResult },
  { "ouyaFaceOnProductListResult", "(I[Ltv/ouya/console/api/Product;)V", (void*)&ouyaFaceOnProductListResult },
  { "ouyaFaceOnPurchaseResult", "(ILtv/ouya/console/api/Product;)V", (void*)&ouyaFaceOnPurchaseResult },
};

static void  ouyaFacadeCompleteCallback(uint32 extID, int32 notification,
  void *pSystem, void* pInstance, int32 result, void* pCompleteData)
{
  IwAssert(ouyaFace, extID == S3E_EXT_OUYAFACE_HASH);
  s3eEdkFreeOS(pCompleteData);
}

static void  ouyaFacadeReceiptsCompleteCallback(uint32 extID, int32 notification,
  void *pSystem, void* pInstance, int32 result, void* pCompleteData)
{
  IwAssert(ouyaFace, extID == S3E_EXT_OUYAFACE_HASH);

  OuyaFacadeReceiptsResult* pResult(static_cast<OuyaFacadeReceiptsResult*>(pSystem));
  
  for(int i = 0; i < pResult->numReceipts; ++i)
  {
    s3eEdkFreeOS(const_cast<char*>(pResult->parReceipt[i].pProductId));
  }

  s3eEdkFreeOS(const_cast<OuyaFacadeReceipt*>(pResult->parReceipt));
}

static void  ouyaFacadeProductListCompleteCallback(uint32 extID, int32 notification,
  void *pSystem, void* pInstance, int32 result, void* pCompleteData)
{
  IwAssert(ouyaFace, extID == S3E_EXT_OUYAFACE_HASH);

  OuyaFacadeProductListResult* pResult(static_cast<OuyaFacadeProductListResult*>(pSystem));
  
  for(int i = 0; i < pResult->numProducts; ++i)
  {
    s3eEdkFreeOS(const_cast<char*>(pResult->parProduct[i].pName));
    s3eEdkFreeOS(const_cast<char*>(pResult->parProduct[i].pProductId));
  }

  s3eEdkFreeOS(const_cast<OuyaFacadeProduct*>(pResult->parProduct));
}

static void  ouyaFacadePurchaseCompleteCallback(uint32 extID, int32 notification,
  void *pSystem, void* pInstance, int32 result, void* pCompleteData)
{
  IwAssert(ouyaFace, extID == S3E_EXT_OUYAFACE_HASH);

  OuyaFacadePurchaseResult* pResult(static_cast<OuyaFacadePurchaseResult*>(pSystem));
  
  s3eEdkFreeOS(const_cast<char*>(pResult->productPurchased.pName));
  s3eEdkFreeOS(const_cast<char*>(pResult->productPurchased.pProductId));
}

void ouyaFaceOnGenericMotionEvent(JNIEnv* env, jobject obj, jint deviceId,
  jint playerId, jfloat lx, jfloat ly, jfloat rx, jfloat ry, jfloat lTrig, jfloat rTrig)
{
  OuyaControllerMotionEvent e =
  {
    deviceId,
    playerId,
    lx,
    ly,
    rx,
    ry,
    lTrig,
    rTrig
  };

  s3eEdkCallbacksEnqueue(S3E_EXT_OUYAFACE_HASH, OUYA_CONTROLLER_MOTION_EVENT,
    &e, sizeof(e), 0, false, 0, 0);
}

void ouyaFaceOnKeyEvent(JNIEnv* env, jobject obj, jint deviceId, jint playerId, jint keyCode, jint action)
{
  OuyaControllerButtonEvent e =
  {
    deviceId,
    playerId,
    keyCode,
    action
  };

  s3eEdkCallbacksEnqueue(S3E_EXT_OUYAFACE_HASH, OUYA_CONTROLLER_BUTTON_EVENT,
    &e, sizeof(e), 0, false, 0, 0);
}

void	ouyaFaceOnGamerUUIDResult(JNIEnv* env, jobject obj, jint error, jstring uuid)
{
  const char* pUUIDChars(env->GetStringUTFChars(uuid, false));
  
  jsize uuidSize(env->GetStringUTFLength(uuid) + 1);
  void* pMem(s3eEdkMallocOS(uuidSize));
  memcpy(pMem, pUUIDChars, uuidSize);
  
  env->ReleaseStringUTFChars(uuid, pUUIDChars);
  
  OuyaFacadeGamerUUIDResult r =
  {
    error,
    static_cast<char*>(pMem)
  };

  s3eEdkCallbacksEnqueue(S3E_EXT_OUYAFACE_HASH, OUYA_FACADE_GAMER_UUID_EVENT,
    &r, sizeof(r), 0, true, ouyaFacadeCompleteCallback, pMem);
}

void	ouyaFaceOnReceiptsResult(JNIEnv* env, jobject obj, jint error, jobjectArray receipts)
{
  jsize numReceipts(env->GetArrayLength(receipts));

  void* pMem(s3eEdkMallocOS(sizeof(OuyaFacadeReceipt) * numReceipts));
  OuyaFacadeReceipt*  parReceipt = static_cast<OuyaFacadeReceipt*>(pMem);

  OuyaFacadeReceiptsResult  r;
  r.error = error;
  r.numReceipts = numReceipts;
  r.parReceipt = parReceipt;

  jclass    cReceipt = env->FindClass("tv/ouya/console/api/Receipt");
  jmethodID mReceiptGetIdentifier = env->GetMethodID(cReceipt, "getIdentifier",
    "()Ljava/lang/string");
  jmethodID mReceiptGetPriceInCents = env->GetMethodID(cReceipt, "getPriceInCents",
    "()I");
  jmethodID mReceiptGetPurchaseDate = env->GetMethodID(cReceipt, "getPurchaseDate",
    "()Ljava/util/Date");
  jmethodID mReceiptGetGeneratedDate = env->GetMethodID(cReceipt, "getGeneratedDate",
    "()Ljava/util/Date");

  jclass    cDate(env->FindClass("java/util/Date"));
  jmethodID mDateGetTime(env->GetMethodID(cDate, "getTime", "()J"));
  jobject   jDate;

  for(int i = 0; i < numReceipts; ++i)
  {
    jobject jReceipt = env->GetObjectArrayElement(receipts, i);

    jstring jProductId = (jstring)env->CallObjectMethod(jReceipt,
      mReceiptGetIdentifier);

    const char* pProductId(env->GetStringUTFChars(jProductId, false));
    jsize productIdSize(env->GetStringUTFLength(jProductId) + 1);
    pMem = s3eEdkMallocOS(productIdSize);
    memcpy(pMem, pProductId, productIdSize);
    
    env->ReleaseStringUTFChars(jProductId, pProductId);
    env->DeleteLocalRef(jProductId);
    
    parReceipt[i].pProductId = static_cast<char*>(pMem);

    parReceipt[i].priceInCents = env->CallIntMethod(jReceipt,
      mReceiptGetPriceInCents);

    jDate = env->CallObjectMethod(jReceipt, mReceiptGetPurchaseDate);
    parReceipt[i].purchaseDate = env->CallLongMethod(jDate, mDateGetTime);
    env->DeleteLocalRef(jDate);

    jDate = env->CallObjectMethod(jReceipt, mReceiptGetGeneratedDate);
    parReceipt[i].generatedDate = env->CallLongMethod(jDate, mDateGetTime);
    env->DeleteLocalRef(jDate);
  }
  
  env->DeleteLocalRef(cDate);
  env->DeleteLocalRef(cReceipt);

  s3eEdkCallbacksEnqueue(S3E_EXT_OUYAFACE_HASH, OUYA_FACADE_RECEIPTS_EVENT,
    &r, sizeof(r), 0, true, ouyaFacadeReceiptsCompleteCallback, 0);
}

void	ouyaFaceOnProductListResult(JNIEnv* env, jobject obj, jint error, jobjectArray products)
{
  jsize arraySize = env->GetArrayLength(products);
  
  void* pMem(s3eEdkMallocOS(sizeof(OuyaFacadeProduct) * arraySize));
  OuyaFacadeProduct*  parProduct = static_cast<OuyaFacadeProduct*>(pMem);

  OuyaFacadeProductListResult r;
  r.error = error;
  r.numProducts = arraySize;
  r.parProduct = parProduct;
  
  jclass    cProduct = env->FindClass("tv/ouya/console/api/Product");
  jmethodID mProductGetName = env->GetMethodID(cProduct, "getName",
    "()Ljava/lang/string");
  jmethodID mProductGetIdentifier = env->GetMethodID(cProduct, "getIdentifier",
    "()Ljava/lang/string");
  jmethodID mProductGetPriceInCents = env->GetMethodID(cProduct, "getPriceInCents",
    "()I");

  for(int i = 0; i < arraySize; ++i)
  {
    jobject jProduct = env->GetObjectArrayElement(products, i);
 
    jstring jName = (jstring)env->CallObjectMethod(jProduct, mProductGetName);
    const char* pName(env->GetStringUTFChars(jName, false));
    jsize nameSize(env->GetStringUTFLength(jName) + 1);
    pMem = s3eEdkMallocOS(nameSize);
    memcpy(pMem, pName, nameSize);
    
    parProduct[i].pName = static_cast<char*>(pMem);

    env->ReleaseStringUTFChars(jName, pName);
    env->DeleteLocalRef(jName);

    jstring jProductId = (jstring)env->CallObjectMethod(jProduct, mProductGetIdentifier);
    const char* pProductId(env->GetStringUTFChars(jProductId, false));
    jsize productIdSize(env->GetStringUTFLength(jProductId) + 1);
    pMem = s3eEdkMallocOS(productIdSize);
    memcpy(pMem, pProductId, productIdSize);

    parProduct[i].pProductId = static_cast<char*>(pMem);

    env->ReleaseStringUTFChars(jProductId, pProductId);
    env->DeleteLocalRef(jProductId);
    
    parProduct[i].priceInCents = env->CallIntMethod(jProduct,
      mProductGetPriceInCents);
  }

  env->DeleteLocalRef(cProduct);

  s3eEdkCallbacksEnqueue(S3E_EXT_OUYAFACE_HASH, OUYA_FACADE_PRODUCT_LIST_EVENT,
    &r, sizeof(r), 0, true, ouyaFacadeProductListCompleteCallback, 0);
}

void	ouyaFaceOnPurchaseResult(JNIEnv* env, jobject obj, jint error, jobject product)
{
  OuyaFacadePurchaseResult  r;
  r.error = error;

  if(error == OUYA_FACADE_NO_ERROR)
  {
    jclass    cProduct = env->FindClass("tv/ouya/console/api/Product");

    jmethodID mProductGetName = env->GetMethodID(cProduct, "getName",
      "()Ljava/lang/string");
    jmethodID mProductGetIdentifier = env->GetMethodID(cProduct, "getIdentifier",
      "()Ljava/lang/string");
    jmethodID mProductGetPriceInCents = env->GetMethodID(cProduct, "getPriceInCents",
      "()I");

    void* pMem(0);

    jstring jName = (jstring)env->CallObjectMethod(product, mProductGetName);
    const char* pName(env->GetStringUTFChars(jName, false));
    jsize nameSize(env->GetStringUTFLength(jName) + 1);
    pMem = s3eEdkMallocOS(nameSize);
    memcpy(pMem, pName, nameSize);
    
    r.productPurchased.pName = static_cast<char*>(pMem);

    env->ReleaseStringUTFChars(jName, pName);
    env->DeleteLocalRef(jName);

    jstring jProductId = (jstring)env->CallObjectMethod(product, mProductGetIdentifier);
    const char* pProductId(env->GetStringUTFChars(jProductId, false));
    jsize productIdSize(env->GetStringUTFLength(jProductId) + 1);
    pMem = s3eEdkMallocOS(productIdSize);
    memcpy(pMem, pProductId, productIdSize);

    r.productPurchased.pProductId = static_cast<char*>(pMem);

    env->ReleaseStringUTFChars(jProductId, pProductId);
    env->DeleteLocalRef(jProductId);
    
    r.productPurchased.priceInCents = env->CallIntMethod(product,
      mProductGetPriceInCents);

    env->DeleteLocalRef(cProduct);
  }

  s3eEdkCallbacksEnqueue(S3E_EXT_OUYAFACE_HASH, OUYA_FACADE_PURCHASE_EVENT,
    &r, sizeof(r), 0, true, 0, 0);
}


s3eResult ouyaFaceInit_platform()
{
    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;

    // Get the extension class
    jclass cls = s3eEdkAndroidFindClass("ouyaFace");
    if (!cls)
        goto fail;

    // Get its constructor
    cons = env->GetMethodID(cls, "<init>", "()V");
    if (!cons)
        goto fail;

    // Construct the java class
    obj = env->NewObject(cls, cons);
    if (!obj)
        goto fail;

    // Get all the extension methods
    g_ouyaInit = env->GetMethodID(cls, "ouyaInit", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_ouyaInit)
        goto fail;

    g_ouyaTerm = env->GetMethodID(cls, "ouyaTerm", "()V");
    if (!g_ouyaTerm)
        goto fail;

    g_ouyaFacadeIsInitialised = env->GetMethodID(cls, "ouyaFacadeIsInitialised", "()Z");
    if (!g_ouyaFacadeIsInitialised)
        goto fail;

    g_ouyaFacadeIsRunningOnOUYAHardware = env->GetMethodID(cls, "ouyaFacadeIsRunningOnOUYAHardware", "()Z");
    if (!g_ouyaFacadeIsRunningOnOUYAHardware)
        goto fail;

    g_ouyaFacadeGetGameData = env->GetMethodID(cls, "ouyaFacadeGetGameData", "(Ljava/lang/String;)Ljava/lang/String;");
    if (!g_ouyaFacadeGetGameData)
        goto fail;

    g_ouyaFacadePutGameData = env->GetMethodID(cls, "ouyaFacadePutGameData", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_ouyaFacadePutGameData)
        goto fail;

    g_ouyaFacadeSetTestMode = env->GetMethodID(cls, "ouyaFacadeSetTestMode", "()V");
    if (!g_ouyaFacadeSetTestMode)
        goto fail;

    g_ouyaFacadeRequestGamerUUID = env->GetMethodID(cls, "ouyaFacadeRequestGamerUUID", "()V");
    if (!g_ouyaFacadeRequestGamerUUID)
        goto fail;

    g_ouyaFacadeRequestReceipts = env->GetMethodID(cls, "ouyaFacadeRequestReceipts", "()V");
    if (!g_ouyaFacadeRequestReceipts)
        goto fail;

    g_ouyaFacadeRequestProductList = env->GetMethodID(cls, "ouyaFacadeRequestProductList", "([Ljava/lang/String;)V");
    if (!g_ouyaFacadeRequestProductList)
        goto fail;

    g_ouyaFacadeRequestPurchase = env->GetMethodID(cls, "ouyaFacadeRequestPurchase", "(Ljava/lang/String;)V");
    if (!g_ouyaFacadeRequestPurchase)
        goto fail;

    g_ouyaControllerRegister = env->GetMethodID(cls, "ouyaControllerRegister", "()I");
    if (!g_ouyaControllerRegister)
        goto fail;

    g_ouyaControllerUnRegister = env->GetMethodID(cls, "ouyaControllerUnRegister", "()I");
    if (!g_ouyaControllerUnRegister)
        goto fail;

    g_ouyaControllerGetButtonState = env->GetMethodID(cls, "ouyaControllerGetButtonState", "(II)Z");
    if (!g_ouyaControllerGetButtonState)
        goto fail;

    g_ouyaControllerGetAxis = env->GetMethodID(cls, "ouyaControllerGetAxis", "(II)F");
    if (!g_ouyaControllerGetAxis)
        goto fail;

    // register native callbacks
    if (env->RegisterNatives(cls, karOuyaFaceNative, 6))
    {
        goto fail;
    }

    IwTrace(OUYAFACE, ("OUYAFACE init success"));
    g_Obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);

    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        IwTrace(ouyaFace, ("One or more java methods could not be found"));
    }
    return S3E_RESULT_ERROR;

}

void ouyaFaceTerminate_platform()
{
    // Add any platform-specific termination code here
}

void ouyaInit_platform(const char* pDeveloperId, const char* pApplicationKey)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring pDeveloperId_jstr = env->NewStringUTF(pDeveloperId);
    jstring pApplicationKey_jstr = env->NewStringUTF(pApplicationKey);
    env->CallVoidMethod(g_Obj, g_ouyaInit, pDeveloperId_jstr, pApplicationKey_jstr);
}

void ouyaTerm_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_ouyaTerm);
}

bool ouyaFacadeIsInitialised_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (bool)env->CallBooleanMethod(g_Obj, g_ouyaFacadeIsInitialised);
}

bool ouyaFacadeIsRunningOnOUYAHardware_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (bool)env->CallBooleanMethod(g_Obj, g_ouyaFacadeIsRunningOnOUYAHardware);
}

void ouyaFacadeGetGameData_platform(const char* pKey, char* pBuffer, int bufferSize)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring pKey_jstr = env->NewStringUTF(pKey);
    jstring	data_jstr = (jstring)env->CallObjectMethod(g_Obj, g_ouyaFacadeGetGameData, pKey_jstr, bufferSize);
    jsize   data_len = env->GetStringUTFLength(data_jstr);

    const char* pDataChars = env->GetStringUTFChars(data_jstr, false);
    if(bufferSize > data_len)
    {
      bufferSize = data_len;
    }

    memcpy(pBuffer, pDataChars, bufferSize);

    env->ReleaseStringUTFChars(data_jstr, pDataChars);
    env->DeleteLocalRef(data_jstr);
    env->DeleteLocalRef(pKey_jstr);
}

void ouyaFacadePutGameData_platform(const char* pKey, const char* pValue)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring pKey_jstr = env->NewStringUTF(pKey);
    jstring pValue_jstr = env->NewStringUTF(pValue);
    env->CallVoidMethod(g_Obj, g_ouyaFacadePutGameData, pKey_jstr, pValue_jstr);

    env->DeleteLocalRef(pKey_jstr);
    env->DeleteLocalRef(pValue_jstr);
}

void ouyaFacadeSetTestMode_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_ouyaFacadeSetTestMode);
}

s3eResult ouyaFacadeRequestGamerUUID_platform(s3eCallback pCallback, void* pUserData)
{
  s3eResult result = s3eEdkCallbacksRegister(S3E_EXT_OUYAFACE_HASH,
    kNumOuyaFacadeEvents, OUYA_FACADE_GAMER_UUID_EVENT, pCallback, pUserData,
    true);

  if(result == S3E_RESULT_SUCCESS)
  {
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_ouyaFacadeRequestGamerUUID);
  }
  return result;
}

s3eResult ouyaFacadeRequestReceipts_platform(s3eCallback pCallback, void* pUserData)
{
  s3eResult result = s3eEdkCallbacksRegister(S3E_EXT_OUYAFACE_HASH,
    kNumOuyaFacadeEvents, OUYA_FACADE_RECEIPTS_EVENT, pCallback, pUserData,
    true);

  if(result == S3E_RESULT_SUCCESS)
  {
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_ouyaFacadeRequestReceipts);
  }
  return result;
}

s3eResult ouyaFacadeRequestProductList_platform(const char** parPurchasable, int numPurchasables, s3eCallback pCallback, void* pUserData)
{
  s3eResult result = s3eEdkCallbacksRegister(S3E_EXT_OUYAFACE_HASH,
    kNumOuyaFacadeEvents, OUYA_FACADE_RECEIPTS_EVENT, pCallback, pUserData,
    true);

  if(result == S3E_RESULT_SUCCESS)
  {
    JNIEnv* env = s3eEdkJNIGetEnv();

    jclass        cString = env->FindClass("Ljava/lang/String;");
    jobjectArray  jarPurchasables = env->NewObjectArray(numPurchasables,
      cString, 0);

    for(int i = 0; i < numPurchasables; ++i)
    {
      jstring jPurchasable = env->NewStringUTF(parPurchasable[0]);
      env->SetObjectArrayElement(jarPurchasables, i, jPurchasable);

      env->DeleteLocalRef(jPurchasable);
    }

    env->CallVoidMethod(g_Obj, g_ouyaFacadeRequestProductList, jarPurchasables);

    env->DeleteLocalRef(jarPurchasables);
    env->DeleteLocalRef(cString);
  }
  return result;
}

s3eResult ouyaFacadeRequestPurchase_platform(const char* pPurchasable, s3eCallback pCallback, void* pUserData)
{
  s3eResult result = s3eEdkCallbacksRegister(S3E_EXT_OUYAFACE_HASH,
    kNumOuyaFacadeEvents, OUYA_FACADE_RECEIPTS_EVENT, pCallback, pUserData,
    true);

  if(result == S3E_RESULT_SUCCESS)
  {
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring pPurchasable_jstr = env->NewStringUTF(pPurchasable);
    env->CallVoidMethod(g_Obj, g_ouyaFacadeRequestPurchase, pPurchasable_jstr);

    env->DeleteLocalRef(pPurchasable_jstr);
  }
  return result;
}

s3eResult ouyaControllerRegister_platform(OuyaControllerEvent type, s3eCallback pCallback, void* pUserData)
{
  s3eResult result = s3eEdkCallbacksRegister(S3E_EXT_OUYAFACE_HASH,
    kNumOuyaControllerEvents, type, pCallback, pUserData,
    true);

  return result;
}

s3eResult ouyaControllerUnRegister_platform(OuyaControllerEvent type, s3eCallback pCallback)
{
  s3eResult result = s3eEdkCallbacksUnRegister(S3E_EXT_OUYAFACE_HASH,
    kNumOuyaControllerEvents, type, pCallback);

  return result;
}

bool ouyaControllerGetButtonState_platform(uint32 controller, uint32 button)
{
  JNIEnv* env = s3eEdkJNIGetEnv();
  return (bool)env->CallBooleanMethod(g_Obj, g_ouyaControllerGetButtonState, controller, button);
}

float ouyaControllerGetAxis_platform(uint32 controller, uint32 axis)
{
  JNIEnv* env = s3eEdkJNIGetEnv();
  return (float)env->CallFloatMethod(g_Obj, g_ouyaControllerGetAxis, controller, axis);
}
