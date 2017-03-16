void pong(){
  while (!buttonValue) {
    JYValue = getAxis(0, 0);
    buttonValue = getButton(0, 0);
  }
}
