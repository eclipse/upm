public class GroveButtonSample{

	static {
		try {
			System.loadLibrary("javaupm_grove");
		}catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		// Create the button object using UART
		upm_grove.GroveButton button = new upm_grove.GroveButton(0);
	
		while (true) {
			System.out.println(button.name() +" value is " + button.value());
			
			Thread.sleep(1000);
		}
	}

}