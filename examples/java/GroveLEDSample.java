public class GroveLEDSample {
	static {
		try {
			System.loadLibrary("javaupm_grove");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main (String args[]) throws InterruptedException {
		upm_grove.GroveLed led = new upm_grove.GroveLed(2);
		
		for (int i = 0; i < 10; ++i) {
			led.on();
			Thread.sleep(1000);
			led.off();
			Thread.sleep(1000);
		}
		led.delete();
	}
}
