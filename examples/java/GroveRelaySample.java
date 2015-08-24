public class GroveRelaySample{
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
		upm_grove.GroveRelay relay = new upm_grove.GroveRelay(5);
	
		for( int i = 0 ; i < 3 ; i++ ){
			relay.on();
			if( relay.isOn() )
				System.out.println("Relay is on");
			Thread.sleep(1000);
			
			relay.off();
			if( relay.isOff() )
				System.out.println("Relay is off");
			Thread.sleep(1000);
		}
	}
}