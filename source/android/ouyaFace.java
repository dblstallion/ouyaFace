/*
java implementation of the ouyaFace extension.

Add android-specific functionality here.

These functions are called via JNI from native code.
*/
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.GeneralSecurityException;
import java.security.KeyFactory;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.spec.X509EncodedKeySpec;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import org.json.JSONException;
import org.json.JSONObject;

import tv.ouya.console.api.OuyaController;
import tv.ouya.console.api.OuyaEncryptionHelper;
import tv.ouya.console.api.OuyaErrorCodes;
import tv.ouya.console.api.OuyaFacade;
import tv.ouya.console.api.OuyaResponseListener;
import tv.ouya.console.api.Product;
import tv.ouya.console.api.Purchasable;
import tv.ouya.console.api.Receipt;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnGenericMotionListener;
import android.view.View.OnKeyListener;

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
import com.ideaworks3d.marmalade.LoaderAPI;

class ouyaFace
{
	private static final int	ERROR_NONE      = 0;
	private static final int	ERROR_CANCELLED = -1;
	
	private final String LOG_TAG = "ouyaFace";

	private	OuyaFacade	facade;
	private	PublicKey	publicKey;
	
  public void	ouyaInit(String developerId, String applicationKey)
  {
	Log.v(LOG_TAG, "in ouyaInit()...");
    Activity	activity = LoaderAPI.getActivity();
    Context		ctx = activity.getApplicationContext();
    
    OuyaController.init(ctx);
	Log.v(LOG_TAG, "controller initialised...");
    
    View	view = LoaderAPI.getMainView();
    view.setOnGenericMotionListener(new OnGenericMotionListener() {
      
      @Override
      public boolean onGenericMotion(View v, MotionEvent event) {
        if((event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0)
        {
          int deviceId = event.getDeviceId();
          onGenericMotionEvent(deviceId,
            OuyaController.getPlayerNumByDeviceId(deviceId),
            event.getAxisValue(OuyaController.AXIS_LS_X),
            event.getAxisValue(OuyaController.AXIS_LS_Y),
            event.getAxisValue(OuyaController.AXIS_RS_X),
            event.getAxisValue(OuyaController.AXIS_RS_Y),
            event.getAxisValue(OuyaController.AXIS_L2),
            event.getAxisValue(OuyaController.AXIS_R2));
          return true;
        }
        return false;
      }
    });
	Log.v(LOG_TAG, "onGenericMotionEventListener set...");
    
    LoaderAPI.pushKeyListener(new OnKeyListener() {
      
      @Override
      public boolean onKey(View v, int keyCode, KeyEvent event) {
        int deviceId = event.getDeviceId();
        onKeyEvent(deviceId, OuyaController.getPlayerNumByDeviceId(deviceId),
          keyCode, event.getAction());
        return false;
      }
    });
	Log.v(LOG_TAG, "onKeyListener set...");
    
    facade = OuyaFacade.getInstance(); 
    facade.init(ctx, developerId);
	Log.v(LOG_TAG, "OuyaFacade initialised...");
  
    try {
      X509EncodedKeySpec keySpec = new X509EncodedKeySpec(applicationKey.getBytes());
      KeyFactory keyFactory = KeyFactory.getInstance("RSA");
      publicKey = keyFactory.generatePublic(keySpec);
    } catch (Exception e) {
      Log.e(LOG_TAG, "Unable to create encryption key", e);
    }
	Log.v(LOG_TAG, "public key calculated...");
  }
  
  public void ouyaTerm()
  {
	Log.v(LOG_TAG, "in ouyaTerm()...");
    View	view = LoaderAPI.getMainView();
    view.setOnGenericMotionListener(null);
    view.setOnKeyListener(null);
    
    facade.shutdown();
    facade = null;
  }
  
  public boolean ouyaFacadeIsInitialised()
  {
	Log.v(LOG_TAG, "in ouyaFacadeIsInitialised()...");
    return facade.isInitialized();
  }
  
  public boolean ouyaFacadeIsRunningOnOUYAHardware()
  {
	Log.v(LOG_TAG, "in ouyaFacadeIsRunningOnOUYAHardware()...");
    return facade.isRunningOnOUYAHardware();
  }
  
  public String ouyaFacadeGetGameData(String key)
  {
	Log.v(LOG_TAG, "in ouyaFacadeGetGameData()...");
	String	result = facade.getGameData(key);
    if(result == null)
    {
      result = new String();
    }
	Log.v(LOG_TAG, "got game data: " + result);
    return result;
  }
  
  public void ouyaFacadePutGameData(String key, String value)
  {
	Log.v(LOG_TAG, "in ouyaFacadePutGameData()...");
    facade.putGameData(key, value);
	Log.v(LOG_TAG, "put " + key + ": " + value);
  }
  
  public void ouyaFacadeSetTestMode()
  {
	Log.v(LOG_TAG, "in ouyaFacadeSetTestMode()...");
    facade.setTestMode();
  }
  
  public void	ouyaFacadeRequestGamerUUID()
  {
	Log.v(LOG_TAG, "in ouyaFacadeRequestGamerUUID()...");
    facade.requestGamerUuid(new OuyaResponseListener<String>() {
      
      @Override
      public void onSuccess(String result) {
		Log.v(LOG_TAG, "in GamerUUID onSuccess()...");
        onGamerUUIDResult(ERROR_NONE, result);
      }
      
      @Override
      public void onFailure(int error, String message, Bundle data) {
        Log.e(LOG_TAG, "GamerUUID request failed:" + message);
        onGamerUUIDResult(error, null);
      }
      
      @Override
      public void onCancel() {
  		Log.v(LOG_TAG, "in GamerUUID onCancel()...");
        onGamerUUIDResult(ERROR_CANCELLED, null);
      }
    });
  }
  
  public void	ouyaFacadeRequestReceipts()
  {
	Log.v(LOG_TAG, "in ouyaFacadeRequestReceipts()...");
    facade.requestReceipts(new OuyaResponseListener<String>() {

      @Override
      public void onCancel() {
  		Log.v(LOG_TAG, "in Receipts onCancel()...");
        onReceiptsResult(ERROR_CANCELLED, null);
      }

      @Override
      public void onFailure(int error, String message, Bundle data) {
        Log.e(LOG_TAG, "Receipts request failed:" + message);
        onReceiptsResult(error, null);
      }

      @Override
      public void onSuccess(String receiptResponse) {
		Log.v(LOG_TAG, "in Receipts onSuccess()...");
        OuyaEncryptionHelper helper = new OuyaEncryptionHelper();
        List<Receipt> receipts;
        try {
          JSONObject response = new JSONObject(receiptResponse);
          if(response.has("key") && response.has("iv")) {
            receipts = helper.decryptReceiptResponse(response, publicKey);
          } else {
            receipts = helper.parseJSONReceiptResponse(receiptResponse);
          }
        } catch (ParseException e) {
          throw new RuntimeException(e);
        } catch (JSONException e) {
          if(e.getMessage().contains("ENCRYPTED")) {
            // This is a hack for some testing code which will be removed
            // before the consumer release
            try {
              receipts = helper.parseJSONReceiptResponse(receiptResponse);
            } catch (IOException ioe) {
              throw new RuntimeException(ioe);
            }
          } else {
            throw new RuntimeException(e);
          }
        } catch (GeneralSecurityException e) {
          throw new RuntimeException(e);
        } catch (IOException e) {
          throw new RuntimeException(e);
        }
        
        Collections.sort(receipts, new Comparator<Receipt>() {
          @Override
          public int compare(Receipt lhs, Receipt rhs) {
            return rhs.getPurchaseDate().compareTo(lhs.getPurchaseDate());
          }
        });
        
       
        onReceiptsResult(ERROR_NONE, (Receipt[]) receipts.toArray());
      }
    });
  }
  
  public void	ouyaFacadeRequestProductList(String[] purchasables)
  {
	Log.v(LOG_TAG, "in ouyaFacadeRequestProductList()...");
    List<Purchasable>	lPurchasables = new ArrayList<Purchasable>();
    for(String i : purchasables)
    {
      lPurchasables.add(new Purchasable(i));
    }
    
    facade.requestProductList(lPurchasables, new OuyaResponseListener<ArrayList<Product>>() {

      @Override
      public void onCancel() {
		Log.v(LOG_TAG, "in ProductList onCancel()...");
        onProductListResult(ERROR_CANCELLED, null);
      }

      @Override
      public void onFailure(int error, String message, Bundle data) {
    	Log.e(LOG_TAG, "ProductList request failed:" + message);
        onProductListResult(error, null);
      }

      @Override
      public void onSuccess(ArrayList<Product> products) {
  		Log.v(LOG_TAG, "in ProductList onSuccess()...");
        onProductListResult(ERROR_NONE, (Product[]) products.toArray());
      }
    });
  }
  
  public void ouyaFacadeRequestPurchase(String productId)
  throws GeneralSecurityException, UnsupportedEncodingException, JSONException
  {
	Log.v(LOG_TAG, "in ouyaFacadeRequestPurchase()...");
    SecureRandom sr = SecureRandom.getInstance("SHA1PRNG");

    // This is an ID that allows you to associate a successful purchase with
    // it's original request. The server does nothing with this string except
    // pass it back to you, so it only needs to be unique within this instance
    // of your app to allow you to pair responses with requests.
    String uniqueId = Long.toHexString(sr.nextLong());

    JSONObject purchaseRequest = new JSONObject();
    purchaseRequest.put("uuid", uniqueId);
    purchaseRequest.put("identifier", productId);
    purchaseRequest.put("testing", "true"); // This value is only needed for testing, not setting it results in a live purchase
    String purchaseRequestJson = purchaseRequest.toString();

    byte[] keyBytes = new byte[16];
    sr.nextBytes(keyBytes);
    SecretKey key = new SecretKeySpec(keyBytes, "AES");

    byte[] ivBytes = new byte[16];
    sr.nextBytes(ivBytes);
    IvParameterSpec iv = new IvParameterSpec(ivBytes);

    Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding", "BC");
    cipher.init(Cipher.ENCRYPT_MODE, key, iv);
    byte[] payload = cipher.doFinal(purchaseRequestJson.getBytes("UTF-8"));

    cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding", "BC");
    cipher.init(Cipher.ENCRYPT_MODE, publicKey);
    byte[] encryptedKey = cipher.doFinal(keyBytes);

    Purchasable purchasable = new Purchasable(
      productId,
      Base64.encodeToString(encryptedKey, Base64.NO_WRAP),
      Base64.encodeToString(ivBytes, Base64.NO_WRAP),
      Base64.encodeToString(payload, Base64.NO_WRAP) );

    facade.requestPurchase(new Purchasable(productId),
      new OuyaResponseListener<String>() {

        @Override
        public void onCancel() {
          Log.v(LOG_TAG, "in Purchase onCancel()...");
          onPurchaseResult(ERROR_CANCELLED, null);
        }

        @Override
        public void onFailure(int error, String message, Bundle arg2) {
          Log.e(LOG_TAG, "ProductList request failed:" + message);
          onPurchaseResult(error, null);
        }

        @Override
        public void onSuccess(String result) {
          Log.v(LOG_TAG, "in Purchase onSuccess()...");
          Product product = null;
          String id = null;
          try {
            OuyaEncryptionHelper helper = new OuyaEncryptionHelper();

            JSONObject response = new JSONObject(result);
            if(response.has("key") && response.has("iv")) {
                id = helper.decryptPurchaseResponse(response, publicKey);
            } else {
                product = new Product(new JSONObject(result));
            }
          } catch (ParseException e) {
            onFailure(OuyaErrorCodes.THROW_DURING_ON_SUCCESS, e.getMessage(), Bundle.EMPTY);
          } catch (JSONException e) {
            if(e.getMessage().contains("ENCRYPTED")) {
              // This is a hack for some testing code which will be removed
              // before the consumer release
              try {
                  product = new Product(new JSONObject(result));
              } catch (JSONException jse) {
                  onFailure(OuyaErrorCodes.THROW_DURING_ON_SUCCESS, e.getMessage(), Bundle.EMPTY);
                  return;
              }
            } else {
              onFailure(OuyaErrorCodes.THROW_DURING_ON_SUCCESS, e.getMessage(), Bundle.EMPTY);
              return;
            }
          } catch (IOException e) {
            onFailure(OuyaErrorCodes.THROW_DURING_ON_SUCCESS, e.getMessage(), Bundle.EMPTY);
            return;
          } catch (GeneralSecurityException e) {
            onFailure(OuyaErrorCodes.THROW_DURING_ON_SUCCESS, e.getMessage(), Bundle.EMPTY);
            return;
          }

          onPurchaseResult(ERROR_NONE, product);
        }
		});
  }

  public boolean	ouyaControllerGetButtonState(int deviceId, int buttonId)
  {
    OuyaController	controller = OuyaController.getControllerByDeviceId(deviceId);
    boolean	buttonState = false;
    if(controller != null)
    {
      buttonState = controller.getButton(buttonId);
    }
    return buttonState;
  }
  
  public float	ouyaControllerGetAxis(int deviceId, int axisId)
  {
    OuyaController	controller = OuyaController.getControllerByDeviceId(deviceId);
    float axisState = .0f;
    if(controller != null)
    {
      axisState = controller.getAxisValue(axisId);
    }
    return axisState;
  }
  
  private native void onGenericMotionEvent(int deviceId, int playerId, float lx, float ly, float rx, float ry, float lTrig, float rTrig);
  private native void onKeyEvent(int deviceId, int playerId, int keyCode, int action);
  
  private native void	onGamerUUIDResult(int error, String uuid);
  private native void	onReceiptsResult(int error, Receipt[] receipts);
  private native void	onProductListResult(int error, Product[] products);
  private native void	onPurchaseResult(int error, Product product);
}
