// DummyCanvas.java

public class DummyCanvas extends GiCanvas {

  public  DummyCanvas() {
    System.err.println(this + ".DummyCanvas()");
  }

  @Override
  protected void finalize() {
    System.err.println(this + ".finalize()");
    super.finalize();
  }

  @Override
  public  synchronized void delete() {
    System.err.println(this + ".delete()");
    super.delete();
  }

  @Override
  protected void swigDirectorDisconnect() {
    System.err.println(this + ".swigDirectorDisconnect()");
    super.swigDirectorDisconnect();
  }

  @Override
  public void swigReleaseOwnership() {
    System.err.println(this + ".swigReleaseOwnership()");
    super.swigReleaseOwnership();
  }

  @Override
  public void swigTakeOwnership() {
    System.err.println(this + ".swigTakeOwnership()");
    super.swigTakeOwnership();
  }

  @Override
  public void setPen(int argb, float width, int style, float phase) {
    System.err.println(this + ".setPen()");
  }

  @Override
  public void setBrush(int argb, int style) {
    System.err.println(this + ".setBrush()");
  }

  @Override
  public void clearRect(float x, float y, float w, float h) {
    System.err.println(this + ".clearRect()");
  }

  @Override
  public void drawRect(float x, float y, float w, float h, boolean stroke, boolean fill) {
    System.err.println(this + ".drawRect()");
  }

  @Override
  public void drawLine(float x1, float y1, float x2, float y2) {
    System.err.println(this + ".drawLine()");
    //super.drawLine(x1, y1, x2, y2);
  }

  @Override
  public void drawEllipse(float x, float y, float w, float h, boolean stroke, boolean fill) {
    System.err.println(this + ".drawEllipse()");
  }

  @Override
  public void beginPath() {
    System.err.println(this + ".beginPath()");
  }

  @Override
  public void moveTo(float x, float y) {
    System.err.println(this + ".moveTo()");
  }

  @Override
  public void lineTo(float x, float y) {
    System.err.println(this + ".lineTo()");
  }

  @Override
  public void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
    System.err.println(this + ".bezierTo()");
  }

  @Override
  public void quadTo(float cpx, float cpy, float x, float y) {
    System.err.println(this + ".quadTo()");
  }

  @Override
  public void closePath() {
    System.err.println(this + ".closePath()");
  }

  @Override
  public void drawPath(boolean stroke, boolean fill) {
    System.err.println(this + ".drawPath()");
  }

  @Override
  public void saveClip() {
    System.err.println(this + ".saveClip()");
  }

  @Override
  public void restoreClip() {
    System.err.println(this + ".restoreClip()");
  }

  @Override
  public boolean clipRect(float x, float y, float w, float h) {
    System.err.println(this + ".clipRect()");
    return false;
  }

  @Override
  public boolean clipPath() {
    System.err.println(this + ".clipPath()");
    return false;
  }

  @Override
  public void drawHandle(float x, float y, int type) {
    System.err.println(this + ".drawHandle()");
  }

  @Override
  public void drawBitmap(String name, float xc, float yc, float w, float h, float angle) {
    System.err.println(this + ".drawBitmap()");
  }

  @Override
  public float drawTextAt(String text, float x, float y, float h, int align) {
    System.err.println(this + ".drawTextAt()");
    return h;
  }
}
