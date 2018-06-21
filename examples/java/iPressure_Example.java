/**
 * Author: Serban Waltter <serban.waltter@rinftech.com>
 */

import java.util.ArrayList;

import upm_bmp280.BMP280;
import upm_hp20x.HP20X;
import upm_ms5611.MS5611;
import upm_new_interfaces.*;

/**
 * iPressure_Example
 */
public class iPressure_Example {

    public static void main(String[] args) {
        ArrayList<iPressure> sensors = new ArrayList<iPressure>();
        sensors.add(new BMP280(2));
        sensors.add(new HP20X(3));
        sensors.add(new MS5611(4));

        while (true) {
            for (int i = 0; i < sensors.size(); i++) {
                System.out.println("Pressure from sensor " + i + " is " + sensors.get(i).getPressure());
            }
        }
    }
}