public class GroveSlide_ex {
	static {
		try {
			System.loadLibrary("javaupm_grove");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main (String args[]) throws InterruptedException {
		upm_grove.GroveSlide slide = new upm_grove.GroveSlide(0);
		
		while (true) {
			float raw_value = slide.raw_value();
			float value = slide.voltage_value();

			System.out.println("raw value: " + raw_value);
			System.out.println("value: " + value);
			
			Thread.sleep(2500);
		}
	}
}
