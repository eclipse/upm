package upm_plugin;

import android.content.Context;
import android.os.Handler;
import android.util.Log;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;

import org.json.JSONArray;
import org.json.JSONException;

import java.util.Map;
import java.util.HashMap;

import upm__PACKAGE_NAME_;

public class _CLASS_NAME_Plugin extends CordovaPlugin {
    private Map<double, _CLASS_NAME_> mObjects = new HashMap<double, _CLASS_NAME_>();

    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        double id = args.getDouble(0);
        if (action.equals("create")) {
            mObjects.put(id, new _CLASS_NAME_());
            return true;
        }

        _CLASS_NAME_ object = mObjects.get(id);
        if (object == null) {
            return false;
        }

_INVOKATIONS_
        return false;
    }
}

