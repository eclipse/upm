import upm_flex.Flex;

public class FlexSensorExample {

	static {
		try {
			System.loadLibrary("javaupm_flex");
			System.loadLibrary("mraajava");
		} catch (UnsatisfiedLinkError e) {
			System.err.println(
					"Native code library failed to load. See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.\n" +
							e);
			System.exit(1);
		}
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//! [Interesting]
		// The was tested with a Spectra Symbol flex sensor.
		// We attached a 22K resistor to a breadboard,
		// with 1 end attached to GND and the other connected to
		// both the flex sensor and A0.
		// The flex sensor was connected on 1 pin to the 22K resistor and A0
		// and on the other pin to 5V.

		// Instantiate a Flex sensor on analog pin A0
		Flex flex = new Flex(0);
		while(true){
			System.out.println("Flex Sensor value is: "+flex.value());
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		//! [Interesting]
	}

}