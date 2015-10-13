/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//NOT TESTED!!!
public class HM11Sample {

	private static final int BUFSIZ = 1024;

	static {
		try {
			System.loadLibrary("javaupm_hm11");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	private static void printUsage() {
		System.out.println("Usage: java HM11Sample [AT command]");

		System.out.println("If an argument is supplied on the command line, that argument is");
		System.out.println("sent to the module and the response is printed out.");
		System.out.println("If no argument is used, then the address and PIN of the module");
		System.out.println("are queried and the results printed out.");

	}

	private static void sendCommand(upm_hm11.HM11 ble, byte[] cmd) {
		byte[] buffer = new byte[BUFSIZ];
		ble.writeData(cmd);

		// wait up to 1 second
		if (ble.dataAvailable(1000)) {
			ble.readData(buffer);
		} else {
			System.err.println("Timed out waiting for response");
		}

	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a HM11 BLE Module on UART 0
		upm_hm11.HM11 ble = new upm_hm11.HM11(0);

		// make sure port is initialized properly. 9600 baud is the default.
		if (!ble.setupTty()) {
			System.err.println("Failed to setup tty port parameters");
			System.exit(-1);
		}

		printUsage();

		if (args.length > 0) {
			System.out.println("Sending command line argument (" + args[0] + ")...");
			sendCommand(ble, args[0].getBytes());
		} else {
			// query the module address
			String addr = "AT+ADDR?";
			System.out.println("Querying module address (" + addr + ")...");
			sendCommand(ble, addr.getBytes());

			Thread.sleep(1000);

			// query the module address
			String pin = "AT+PASS?";
			System.out.println("Querying module pin (" + pin + ")...");
			sendCommand(ble, pin.getBytes());

			// Other potentially useful commands are:
			//
			// AT+VERS? - query module version
			// AT+ROLE0 - set as slave
			// AT+ROLE1 - set as master
			// AT+CLEAR - clear all previous settings
			// AT+RESET - restart the device
			//
			// A comprehensive list is available from the datasheet at:
			// http://www.seeedstudio.com/wiki/images/c/cd/Bluetooth4_en.pdf

		}
		// ! [Interesting]
	}
}
