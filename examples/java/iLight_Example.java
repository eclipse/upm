/**
 * Author: Serban Waltter <serban.waltter@rinftech.com>
 */

import upm_new_interfaces.*;

import java.util.ArrayList;

import upm_apds9002.*;
import upm_bh1750.*;
import upm_max44009.*;

/**
 * iLight_Example
 */
public class iLight_Example {

    public static void main(String[] args) {
        ArrayList<iLight> sensors = new ArrayList<iLight>();
        sensors.add(new APDS9002(0));
        sensors.add(new BH1750());
        sensors.add(new MAX44009(1));

        while (true) {
            for (int i = 0; i < sensors.size(); i++) {
                System.out.println("Luminance from sensor " + i + " is " + sensors.get(i).getLuminance());
            }
        }
    }
}