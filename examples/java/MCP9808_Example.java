/*
 * Author: Stefan Andritoiu <stefan.andritoiu@gmail.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
import java.io.*;
import upm_mcp9808.MCP9808;

public class MCP9808_Example {

	public static void main(String[] args) throws InterruptedException, IOException {
		// ! [Interesting]

		int command;
		upm_mcp9808.MCP9808 temp = new upm_mcp9808.MCP9808(6);
		BufferedReader read = new BufferedReader(new InputStreamReader(System.in));

		do {
			System.out.println("1 - read temp");
			System.out.println("2 - sleep mode");
			System.out.println("3 - wake up");
			System.out.println("4 - set mode to " + (temp.isCelsius() == true ? "Fahrenheit" : "Celsius"));
			System.out.println("5 - show status bits");
			System.out.println("6 - Set Tcrit");
			System.out.println("7 - Set Tupper");
			System.out.println("8 - Set Tlower");
			System.out.println("9 - Display monitor temps");
			System.out.println("10 - Enable alert default");
			System.out.println("11 - Enable alert interrupt");
			System.out.println("12 - Clear interrupt");
			System.out.println("13 - Clear alert mode");
			System.out.println("14 - Get Hysteresis");
			System.out.println("15 - Set Hysteresis");
			System.out.println("16 - Get Resolution");
			System.out.println("17 - Set Resolution");
			System.out.println("18 - Get Manufacturer ID");
			System.out.println("19 - Get Device ID");
			System.out.println("-1 - exit");
			System.out.print("Enter a command:");

			try {
				command = Integer.parseInt(read.readLine());
			} catch (Exception e) {
				System.err.println("Not valid command");
				command = -1;
			}

			float t;
			switch (command) {
			case 1:
				System.out.println("Temp: " + temp.getTemp() + " degrees " + (temp.isCelsius() ? "Celsius" : "Fahrenheit"));
				break;
			case 2:
				System.out.println("shutdown sensor (sleep mode)");
				temp.shutDown();
				break;
			case 3:
				System.out.println("wake up sensor");
				temp.shutDown(false);
				break;
			case 4:
				System.out.println("set mode to " + (temp.isCelsius() ? "Fahrenheit" : "Celsius"));
				temp.setMode(!temp.isCelsius());
				break;
			case 5:
				System.out.println("Tcrit = " + temp.isTcrit());
				System.out.println("Tupper = " + temp.isTupper());
				System.out.println("Tlower = " + temp.isTlower());
				break;
			case 6:
				System.out.print("Enter a value: ");
				try {
					t = Float.valueOf(read.readLine()).floatValue();
				} catch (Exception e) {
					System.err.println("\nNot a valid value\n");
					break;
				}
				temp.setMonitorReg(MCP9808.MCP9808_REG.CRIT_TEMP, t);
				break;
			case 7:
				System.out.print("Enter a value: ");
				try {
					t = Float.valueOf(read.readLine()).floatValue();
				} catch (Exception e) {
					System.err.println("\nNot a valid value\n");
					break;
				}
				temp.setMonitorReg(MCP9808.MCP9808_REG.UPPER_TEMP, t);
				break;
			case 8:
				System.out.print("Enter a value: ");
				try {
					t = Float.valueOf(read.readLine()).floatValue();
				} catch (Exception e) {
					System.err.println("\nNot a valid value\n");
					break;
				}
				temp.setMonitorReg(MCP9808.MCP9808_REG.LOWER_TEMP, t);
				break;
			case 9:
				System.out.println("Tcrit = " + temp.getMonitorReg(MCP9808.MCP9808_REG.CRIT_TEMP));
				System.out.println("Tupper = " + temp.getMonitorReg(MCP9808.MCP9808_REG.UPPER_TEMP));
				System.out.println("Tlower = " + temp.getMonitorReg(MCP9808.MCP9808_REG.LOWER_TEMP));
				break;
			case 10:
				System.out.println("Set alert mode default");
				temp.setAlertMode(MCP9808.MCP9808_CONFIG.ALERTCTRL.swigValue());
				break;
			case 11:
				System.out.println("Set alert mode interrupt");
				temp.setAlertMode(
						MCP9808.MCP9808_CONFIG.ALERTMODE.swigValue() | MCP9808.MCP9808_CONFIG.ALERTCTRL.swigValue());
				break;
			case 12:
				temp.clearInterrupt();
				break;
			case 13:
				System.out.println("Clear alerts");
				temp.clearAlertMode();
				break;
			case 14:
				System.out.println("Hysteresis: " + temp.getHysteresis());
				break;
			case 15:
				int u;
				System.out.print("Enter 1 to 4: ");
				try {
					u = Integer.parseInt(read.readLine());
				} catch (Exception e) {
					System.err.println("\nNot a valid command\n");
					break;
				}
				switch (u) {
				case 1:
					temp.setHysteresis(MCP9808.MCP9808_CONFIG.HYST_0);
					break;
				case 2:
					temp.setHysteresis(MCP9808.MCP9808_CONFIG.HYST_1_5);
					break;
				case 3:
					temp.setHysteresis(MCP9808.MCP9808_CONFIG.HYST_3_0);
					break;
				case 4:
					temp.setHysteresis(MCP9808.MCP9808_CONFIG.HYST_6_0);
					break;
				default:
					System.out.println("\nNot a valid option\n");
					break;
				}
				break;
			case 16:
				System.out.println("Resolution: " + temp.getResolution());
				break;
			case 17:
				int v;
				System.out.print("Enter 1 to 4: ");
				try {
					v = Integer.parseInt(read.readLine());
				} catch (Exception e) {
					System.err.println("\nNot a valid command\n");
					break;
				}
				switch (v) {
				case 1:
					temp.setResolution(MCP9808.MCP9808_RESOLUTION.RES_LOW);
					break;
				case 2:
					temp.setResolution(MCP9808.MCP9808_RESOLUTION.RES_MEDIUM);
					break;
				case 3:
					temp.setResolution(MCP9808.MCP9808_RESOLUTION.RES_HIGH);
					break;
				case 4:
					temp.setResolution(MCP9808.MCP9808_RESOLUTION.RES_PRECISION);
					break;
				default:
					System.out.println("\nNot a valid option\n");
					break;
				}
				break;
			case 18:
				System.out.println("Manufacturer ID: " + temp.getManufacturer());
				break;
			case 19:
				System.out.println("Get device ID: " + temp.getDevicedId());
				break;
			case -1:
				break;
			default:
				System.out.println("That option is not available. Try again");
				break;
			}
		} while (command != -1);

		// ! [Interesting]
	}
}

