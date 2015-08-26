public class Adxl345 {
    static {
        try {
            System.loadLibrary("javaupm_adxl345");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("error in loading native library");
            System.exit(-1);
        }
    }

    public static void main(String argv[]) throws InterruptedException {
        upm_adxl345.Adxl345 obj = new upm_adxl345.Adxl345(0);
        int[] raw = new int[3];
        float[] accel = new float[3];

        while (true) {
            obj.update();
            raw = obj.getRawValues();
            accel = obj.getAcceleration();

            System.out.println("raw data: " + raw[0] + " " + raw[1] + " " +
                    raw[2]);
            System.out.println("accel data: " + accel[0] + " " + accel[1] + " "
                    + accel[2]);

            Thread.sleep(1000);
        }
    }
}
