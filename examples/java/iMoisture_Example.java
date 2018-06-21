/**
 * Author: Serban Waltter <serban.waltter@rinftech.com>
 */

import upm_new_interfaces.*;

import upm_moisture.*;
import upm_ims.*;

import java.util.ArrayList;

/**
 * iMoisture_Example
 */
public class iMoisture_Example {

    public static void main(String[] args) {
        ArrayList<iMoisture> sensors = new ArrayList<iMoisture>();
        sensors.add(new IMS(0));
        sensors.add(new Moisture(2));

        while (true) {
            for (int i = 0; i < sensors.size(); i++) {
                System.out.println("Moisture from sensor " + i + " is " + sensors.get(i).getMoisture());
            }
        }
    }
}