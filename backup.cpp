#include "mbed.h"

#define  turnright              0x80
#define  rightarmup             0x81
#define  rightarmout            0x82
#define  tiltbodyright          0x83
#define  rightarmdown           0x84
#define  rightarmin             0x85
#define  walkforward            0x86
#define  walkbackward           0x87
#define  turnleft               0x88
#define  leftarmup              0x89
#define  leftarmout             0x8A
#define  tiltbodyleft           0x8B
#define  leftarmdown            0x8C
#define  leftarmin              0x8D
#define  stop                   0x8E
#define  rightturnstep          0xA0
#define  righthandthump         0xA1
#define  righthandthrow         0xA2
#define  sleep                  0xA3
#define  righthandpickup        0xA4
#define  leanbackward           0xA5
#define  forwardstep            0xA6
#define  backwardstep           0xA7
#define  leftturnstep           0xA8
#define  lefthandthump          0xA9
#define  lefthandthrow          0xAA
#define  listen                 0xAB
#define  lefthandpickup         0xAC
#define  leanforward            0xAD
#define  reset                  0xAE
#define  Execute                0xB0
#define  Wakeup                 0xB1
#define  Right                  0xB2
#define  Left                   0xB3
#define  Sonic                  0xB4
#define  righthandstrike3       0xC0
#define  righthandsweep         0xC1
#define  burp                   0xC2
#define  righthandstrike2       0xC3
#define  high5                  0xC4
#define  righthandstrike1       0xC5
#define  bulldozer              0xC6
#define  oopsfart               0xC7
#define  lefthandstrike3        0xC8
#define  lefthandsweep          0xC9
#define  whistle                0xCA
#define  lefthandstrike2        0xCB
#define  talkback               0xCC
#define  lefthandstrike1        0xCD
#define  roar                   0xCE
#define  AllDemo                0xD0
#define  PowerOff               0xD1
#define  Demo1                  0xD2
#define  Demo2                  0xD3
#define  Dance                  0xD4
#define RSNoOp                  0xEF

DigitalOut IROut(p20, 1);
AnalogIn IRSensor_left(p17);
AnalogIn IRSensor_right(p18);
AnalogIn IRSensor_front(p19);
BusOut myled(LED1,LED2,LED3,LED4);
Serial blue(p28,p27);
Serial pc(USBTX, USBRX);

float limit = 60;
int bitTime=516;       // Bit time (Theoretically 833 but as low as 516 may work)

//global variables for main and interrupt routine
volatile bool global_button_ready = 0;
volatile int  global_bnum = 0;
volatile int  global_bhit;
//state used to remember previous characters read in a button message
enum statetype {start = 0, got_exclm, got_B, got_num, got_hit};
statetype state = start;

//Interrupt routine to parse message with one new character per serial RX interrupt
void parse_message()
{
    switch (state) {
        case start:
            if (blue.getc()=='!') state = got_exclm;
            else state = start;
            break;
        case got_exclm:
            if (blue.getc() == 'B') state = got_B;
            else state = start;
            break;
        case got_B:
            global_bnum = blue.getc();
            state = got_num;
            break;
        case got_num:
            global_bhit = blue.getc();
            state = got_hit;
            break;
        case got_hit:
            if (blue.getc() == char(~('!' + ' B' + global_bnum + global_bhit)))
            {
                global_button_ready = 1;
            }
            state = start;
            break;
        default:
            blue.getc();
            state = start;
    }
}

void RSSendCommand(int command)
{
    IROut = 0;
    wait_us(8*bitTime);
    for (int i=0;i<8;i++) {
        IROut = 1;
        wait_us(bitTime);
        if ((command & 128) !=0) wait_us(3*bitTime);
        IROut = 0;
        wait_us(bitTime);
        command = command << 1;
    }
    IROut = 1;
    wait(0.20); // Give a 2 sec before next
}

float left_IR()
{
    float a = 1-IRSensor_left;
    return (a*100);
}

float right_IR()
{
    float a = 1-IRSensor_right;
    return (a*100);
}

float front_IR()
{
    float a = 1-IRSensor_front;
    return (a*100);
}

void autonomous_mode()
{
    if (front_IR()<= limit)
    {
        while(front_IR()<=limit)
        {
            RSSendCommand(backwardstep);
            wait(2);
        }
        if (right_IR()>=limit)
        {
            RSSendCommand(rightturnstep);
            RSSendCommand(rightturnstep);
            RSSendCommand(rightturnstep);
            wait(3.5);
            RSSendCommand(stop);
        }
        else if (left_IR()>=limit)
        {
            RSSendCommand(leftturnstep);
            RSSendCommand(leftturnstep);
            RSSendCommand(leftturnstep);
            wait(3.5);
            RSSendCommand(stop);
        }
        else
        {
            while( left_IR()>=limit || right_IR()>=limit)
            {
                RSSendCommand(backwardstep);
            }
            if(right_IR()>=limit)
            {
                RSSendCommand(rightturnstep);
                RSSendCommand(rightturnstep);
                RSSendCommand(rightturnstep);
                wait(3.5);
                RSSendCommand(stop);
            }
            else if(left_IR()>=limit)
            {
                RSSendCommand(leftturnstep);
                RSSendCommand(leftturnstep);
                RSSendCommand(leftturnstep);
                wait(3.5);
                RSSendCommand(stop);
            }
        }
    }
    else {
        RSSendCommand(forwardstep);
    }
}

int main()
{
    //RSSendCommand(burp);
    // wait(2);
    // RSSendCommand(high5);
    // wait(5);
    char bnum=0;
    char bact=0;
    while(1)
    {
        if (blue.getc()=='!')
        {
            if (blue.getc()=='B')
            {
                bnum = blue.getc();
                if (bnum == '1')
                {
                    pc.printf("autonomous_mode\r\n");
                    blue.attach(&parse_message,Serial::RxIrq);
                    while (1)
                    {
                        //check for a new button message ready
                        if(global_button_ready && (global_bnum=='2')) { // button 2 changed
                            global_button_ready = 0; //reset flag after reading button message
                            break;
                        }
                        autonomous_mode();
                    }
                    pc.printf("autonomous_mode gone\r\n");
                }
                else if (bnum == '5')
                {
                    bact = blue.getc();
                    if (bact == '1')
                    {
                        RSSendCommand(forwardstep);
                    }
                }
                else if (bnum == '6')
                {
                    bact = blue.getc();
                    if (bact == '1')
                    {
                        RSSendCommand(backwardstep);
                    }
                }
                else if (bnum == '7')
                {
                    bact = blue.getc();
                    if (bact == '1')
                    {
                        RSSendCommand(leftturnstep);
                    }
                }
                else if (bnum == '8')
                {
                    bact = blue.getc();
                    if (bact == '1')
                    {
                        RSSendCommand(rightturnstep);
                    }
                }
            }
        }
        pc.printf("xx\r\n");
    }
}
