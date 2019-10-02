/**
 * Author: Serban Waltter <serban.waltter@rinftech.com>
 */

import upm_interfaces.*;

import java.util.ArrayList;

import upm_bmp280.*;
import upm_htu21d.*;
import upm_rhusb.*;

/**
 * iHumidity_Example
 */
public class iHumidity_Example {

    public static void main(String[] args) {
        ArrayList<iHumidity> sensors = new ArrayList<iHumidity>();
        sensors.add(new BME280());
        sensors.add(new HTU21D(2));
        sensors.add(new RHUSB("/dev/UART"));

        while (true) {
            for (int i = 0; i < sensors.size(); i++) {
                System.out.println("Humidity from sensor " + i + " is " + sensors.get(i).getHumidity());
            }
        }
    }
}