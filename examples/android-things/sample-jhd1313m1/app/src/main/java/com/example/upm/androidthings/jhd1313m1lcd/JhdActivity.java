package com.example.upm.androidthings.jhd1313m1lcd;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import java.io.IOException;

/**
  * Example of using Grove Jhd1313m1 LCD.
  *
  * This activity initializes the LCD and displays RGB color combination with text.
  *
  */

public class JhdActivity extends Activity {
    private static final String TAG = "JhdActivity";
    static {
        try {
            System.loadLibrary("javaupm_jhd1313m1");
        } catch (UnsatisfiedLinkError e) {
            System.err.println(
                    "Native library failed to load.\n" + e);
            System.exit(1);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(TAG, "Starting JhdActivity");
        // Instantiate an jhd1313m1 instance on I2C bus 0
        try {
            upm_jhd1313m1.Jhd1313m1 lcd =
                new upm_jhd1313m1.Jhd1313m1(0, 0x3E, 0x62);

            lcd.clear();
            int ndx = 0;
            short[][] rgb = new short[][]{
                {0xd1, 0x00, 0x00},
                {0xff, 0x66, 0x22},
                {0xff, 0xda, 0x21},
                {0x33, 0xdd, 0x00},
                {0x11, 0x33, 0xcc},
                {0x22, 0x00, 0x66},
                {0x33, 0x00, 0x44}
            };
            
	    while (true) {
                // Alternate rows on the LCD
                lcd.setCursor(ndx % 2, 0);
                // Change the color
                short r = rgb[ndx % 7][0];
                short g = rgb[ndx % 7][1];
                short b = rgb[ndx % 7][2];
                lcd.setColor(r, g, b);
                lcd.write("Hello World " + ndx);
                // Echo via printf
                System.out.println("Hello World" + ndx++);
                System.out.format("rgb: 0x%02x%02x%02x\n", r, g, b);
                Thread.sleep(1000);
            }
        } catch (Exception e) {
            Log.e(TAG, "Error in UPM APIs", e);
        }
    }
}
