public class Jhd1313m1Sample{

	static {
		try {
			System.loadLibrary("javaupm_i2clcd");
		}catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		upm_i2clcd.Jhd1313m1 lcd = new upm_i2clcd.Jhd1313m1(1, 0x3E, 0x62);
		
		lcd.setCursor(0,0);
	    lcd.write("Hello World");
	    lcd.setCursor(1,2);
	    lcd.write("Hello World");

		System.out.println("Sleeping for 5 seconds");
		Thread.sleep(5000);
	    lcd.clear();
		
	}
	
}