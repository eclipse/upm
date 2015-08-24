public class BISS0001Sample{

	static {
		try {
			System.loadLibrary("javaupm_biss0001");
		}catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		upm_biss0001.BISS0001 motion = new upm_biss0001.BISS0001(7);
	
		while (true) {
			boolean value = motion.value();
			
			if(value)
				System.out.println("Detecting moving object");
			else
				System.out.println("No moving objects detected");
			
			Thread.sleep(1000);
		}
	}

}