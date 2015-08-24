//import upm_guvas12d.GUVAS12D;

public class GUVAS12DSample{
	// analog voltage, usually 3.3 or 5.0
	private static final float GUVAS12D_AREF = 5;
	private static final int SAMPLES_PER_QUERY = 1024;

	
	static {
		try {
			System.loadLibrary("javaupm_guvas12d");
		}catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		upm_guvas12d.GUVAS12D volts = new upm_guvas12d.GUVAS12D(3);
		
		while (true) {
			float value = volts.value(GUVAS12D_AREF, SAMPLES_PER_QUERY);
			
			System.out.println("AREF: " + GUVAS12D_AREF + ", Voltage value (higher means more UV): " + value );
		
			Thread.sleep(1000);
		}
	}

}