/**
 * Author: Serban Waltter <serban.waltter@rinftech.com>
 */

import upm_interfaces.*;

import java.util.ArrayList;

import upm_hcsr04.*;
import upm_lidarlitev3.*;
import upm_mb704x.*;

/**
 * iDistance_Example
 */
public class iDistance_Example {

    public static void main(String[] args) {
        ArrayList<iDistance> sensors = new ArrayList<iDistance>();
        sensors.add(new HCSR04(2 ,3));
        sensors.add(new LIDARLITEV3(4, 5));
        sensors.add(new MB704X(6, 7));

        while (true) {
            for (int i = 0; i < sensors.size(); i++) {
                System.out.println("Distance from sensor " + i + " is " + sensors.get(i).getDistance());
            }
        }
    }
}