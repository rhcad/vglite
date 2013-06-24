// This example illustrates how C++ interfaces can be used from Java.
//
// 1. `cd' to the output directory (may be './build/java').
// 2. Type the following commands to run this program:
//       java TestJNI
//

public class TestJNI {
  static {
    try {
        try {	// for attaching to the current java process.
            Thread.currentThread().sleep(5000);
        } catch (InterruptedException e) {}

        System.loadLibrary("testjni");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load.\n" + e);
      System.exit(1);
    }
  }

  public static void main(String argv[])
  {
    System.out.println( "Hello TestJNI in Java." );

    TestCanvas.initRand();

    DummyCanvas canvas = new DummyCanvas();
    System.out.println("Created a object of DummyCanvas: " + canvas);

    TestCanvas.testLine(canvas, 1);

    canvas.delete();

    canvas = new DummyCanvas();
    System.out.println("Created a object of DummyCanvas: " + canvas);
    TestCanvas.testRect(canvas, 0);
    canvas.delete();
    canvas = null;

    System.out.println( "Goodbye" );
  }
}
