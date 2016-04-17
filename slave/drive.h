#ifndef drive_h
#define drive_h

class dclass{
  public:
    dclass();
    void SETUP();
    void DRIVE();
    void CALCULATE(int x, int y, int r);
};

extern dclass drive;

#endif
