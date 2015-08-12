public class GroveTemp_ex {
	static {
		try {
			System.loadLibrary("javaupm_grove");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main (String args[]) throws InterruptedException {
		upm_grove.GroveTemp temp = new upm_grove.GroveTemp(3);
		
		for (int i = 0; i < 10; ++i) {
			
			int celsius = temp.value();
			int fahrneheit = celsius * 2 + 32;

			System.out.println("Celsius: " + celsius);
			System.out.println("Fahrneheit: " + fahrneheit);
			
			Thread.sleep(1000);
		}
		temp.delete();
	}
}
