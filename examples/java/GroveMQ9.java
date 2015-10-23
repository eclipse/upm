import upm_gas.Gas;
import upm_gas.MQ2;
import upm_gas.MQ3;
import upm_gas.MQ4;
import upm_gas.MQ5;
import upm_gas.MQ6;
import upm_gas.MQ7;
import upm_gas.MQ8;
import upm_gas.MQ9;
import upm_gas.TP401;
import upm_gas.thresholdContext;

public class GroveMQ9 {

	static {
		try {
			System.loadLibrary("javaupm_gas");
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
		// initialize the sensor on A0
		MQ9 mq9 = new MQ9(0);

		short[] buffer = new short[128];

		thresholdContext context = new thresholdContext();

		context.setAverageReading(0);
		context.setRunningAverage(0);
		context.setAveragedOver(2);

		int len;
		int thres;
		while(true){
			len = mq9.getSampledWindow((long)2, buffer);

			if(len != 0){
				thres = mq9.findThreshold(context, 30, buffer);
				mq9.printGraph(context, (short)5);
				if(thres != 0){
					// do something
					System.out.println("threshold is crossed");
				}
			}
		}
		//! [Interesting]
	}

}