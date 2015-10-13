public class MPR121Sample {

	static {
		try {
			System.loadLibrary("javaupm_mpr121");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	private static void printButtons(upm_mpr121.MPR121 touch) {
		boolean buttonPresed = false;

		System.out.print("Buttons pressed: ");
		for (int i = 0; i < 12; i++) {
			if ((touch.getM_buttonStates() & (1 << i)) != 0) {
				System.out.print(i + " ");
				buttonPresed = true;
			}
		}

		if (!buttonPresed)
			System.out.print("None ");

		System.out.println();
	}

	public static void main(String[] args) throws InterruptedException {
		// Instantiate an MPR121 on I2C bus 0
		upm_mpr121.MPR121 touch = new upm_mpr121.MPR121(0);

		// init according to AN3944 defaults
		touch.configAN3944();

		while (true) {
			touch.readButtons();
			printButtons(touch);
			Thread.sleep(1000);
		}
	}

}