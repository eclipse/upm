/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_collision.*;

public class Collision_Example {

	public static void main(String[] args) {
		// Initializing the sensor on D2 on the Base Shield
		upm_collision.Collision collision = new upm_collision.Collision(2);
		boolean collisionState = false;

		// ! [Interesting]
		while(true){
			if(collision.isColliding() && !collisionState){
				System.out.println("Collision!!");
				collisionState = true;
			}
			else if (collisionState){
				System.out.println("No Collision!");
				collisionState = false;
			}
		}
		// ! [Interesting]
	}

}
