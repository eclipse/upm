public class TTP223Sample{

	static {
		try {
			System.loadLibrary("javaupm_ttp223");
		}catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		upm_ttp223.TTP223 touch = new upm_ttp223.TTP223(7);
	
		while (true) {
			if(touch.isPressed())
				System.out.println( touch.name() + " is pressed" );
			else
				System.out.println( touch.name() + " is not pressed" );
			
			Thread.sleep(1000);
		}
	}

}