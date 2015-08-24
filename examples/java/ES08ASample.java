//import upm_servo.ES08A;

public class ES08ASample{
	static {
		try {
			System.loadLibrary("javaupm_servo");
		}catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		upm_servo.ES08A servo = new upm_servo.ES08A(6);
		
		// Sets the shaft to 180, then to 90, then to 0,
	    // then back to 90, and finally back to 180,
	    // pausing for a second in between each angle
	    servo.setAngle (180);
	    System.out.println("Set angle to 180");
		Thread.sleep(1000);

		servo.setAngle (90);
	    System.out.println("Set angle to 90");
		Thread.sleep(1000);
		
		servo.setAngle (0);
	    System.out.println("Set angle to 0");
		Thread.sleep(1000);
		
		servo.setAngle (90);
	    System.out.println("Set angle to 90");
		Thread.sleep(1000);

		servo.setAngle (180);
	    System.out.println("Set angle to 180");
	}
}