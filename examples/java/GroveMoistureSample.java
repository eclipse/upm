public class GroveMoistureSample{
	static {
		try {
			System.loadLibrary("javaupm_grovemoisture");
		}catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main (String args[]) throws InterruptedException {
		upm_grovemoisture.GroveMoisture gm = new upm_grovemoisture.GroveMoisture(1);
	
		while (true) {
			int moisture_val = gm.value();
			String result;
	
			if (moisture_val >= 0 && moisture_val < 300)
				result = "Dry";                 
			else if ((moisture_val >= 0 && moisture_val < 300))
				result = "Moist";
			else
				result = "Wet";
			
			System.out.println("Moisture Value: " + moisture_val + ", " + result );
		
			Thread.sleep(1000);
		}
	}
}
