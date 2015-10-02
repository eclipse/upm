import upm_htu21d.javaupm_htu21dConstants;

public class HTU21DSample{

	static {
		try {
			System.loadLibrary("javaupm_htu21d");
		}catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		float humidity    = 0;
	    float temperature = 0;
	    float compRH      = 0;
	    
	    upm_htu21d.HTU21D sensor = new upm_htu21d.HTU21D(1);
		sensor.testSensor();	    
	    
		while (true) {
			compRH = sensor.getCompRH();
			humidity = sensor.getHumidity();
			temperature = sensor.getTemperature();
			
			System.out.println( "Humidity: " + humidity + ", Temperature: " + temperature + ", compensated RH: " + compRH);
			
			Thread.sleep(5000);
		}
	}

}