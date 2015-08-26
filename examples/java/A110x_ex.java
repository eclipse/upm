public class A110x_ex {
	static {
		try {
			System.loadLibrary("javaupm_a110x");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}
	
	public static void main(String argv[]) throws InterruptedException {
		upm_a110x.A110X a110x = new upm_a110x.A110X(2);

		while(true) {
			if(a110x.magnetDetected()) {
				System.out.println("magnet detected...");
			}
			else {
				System.out.println("magnet not detected...");
			}
			Thread.sleep(1000);
		}			
	}
}
