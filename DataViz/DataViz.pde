Table table;
void setup() {
  size(1500, 540);
  colorMode(HSB);
  background(0);
  table = loadTable("test1.csv");
  int rows = table.getRowCount();

  noFill();
  for (int col=0; col<3; col++) {
    stroke(col * 80, 150, 255);
    beginShape();
    for (int rown=0; rown<rows; rown++) {
      TableRow row = table.getRow(rown);
      int v = row.getInt(col);

      vertex(width * rown / (float)rows, v);
    }
    endShape();
    translate(0, 180);
  }
  //save(System.currentTimeMillis() + ".png");
}

void draw() {
}