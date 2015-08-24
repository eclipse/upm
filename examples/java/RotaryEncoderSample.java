public class RotaryEncoderSample{

	static {
		try {
			System.loadLibrary("javaupm_rotaryencoder");
		}catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		// Create the button object using UART
		upm_rotaryencoder.RotaryEncoder rotaryencoder = new upm_rotaryencoder.RotaryEncoder(2, 3);
	
		while (true) {
			System.out.println("Position: " + rotaryencoder.position());
			
			Thread.sleep(1000);
		}
	}

}