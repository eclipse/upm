public class GroveSpeakerSample {
	static {
		try {
			System.loadLibrary("javaupm_grovespeaker");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main (String args[]) throws InterruptedException {
		upm_grovespeaker.GroveSpeaker speaker = new upm_grovespeaker.GroveSpeaker(3);
		
		speaker.playAll();
		speaker.playSound('c', true, "med");
		
		temp.delete();
	}
}
