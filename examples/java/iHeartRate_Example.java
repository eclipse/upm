/**
 * Author: Serban Waltter <serban.waltter@rinftech.com>
 */

import upm_new_interfaces.*;

import java.util.ArrayList;

import upm_ehr.*;
import upm_groveehr.*;

/**
 * iHeartRate_Example
 */
public class iHeartRate_Example {

    public static void main(String[] args) {
        ArrayList<iHeartRate> sensors = new ArrayList<iHeartRate>();
        sensors.add(new EHR(2));
        sensors.add(new GroveEHR(3));

        while (true) {
            for (int i = 0; i < sensors.size(); i++) {
                System.out.println("HR from sensor " + i + " is " + sensors.get(i).getHeartRate());
            }
        }
    }
}