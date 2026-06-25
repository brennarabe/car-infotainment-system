#include <avr/io.h>

// most TWI code comes from provided example
#define TWI_IS_CLOCKHELD() TWI0.MSTATUS & TWI_CLKHOLD_bm
#define TWI_IS_BUSERR() TWI0.MSTATUS & TWI_BUSERR_bm
#define TWI_IS_ARBLOST() TWI0.MSTATUS & TWI_ARBLOST_bm

#define CLIENT_NACK() TWI0.MSTATUS & TWI_RXACK_bm
#define CLIENT_ACK() !(TWI0.MSTATUS & TWI_RXACK_bm)

#define TWI_IS_BUSBUSY() ((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc) == TWI_BUSSTATE_BUSY_gc)

#define TWI_WAIT() while (!((TWI_IS_CLOCKHELD()) || (TWI_IS_BUSERR()) || (TWI_IS_ARBLOST()) || (TWI_IS_BUSBUSY())))


int isTWIBad(void)
{
    //Checks for: NACK, ARBLOST, BUSERR, Bus Busy
    if ((((TWI0.MSTATUS) & (TWI_RXACK_bm | TWI_ARBLOST_bm | TWI_BUSERR_bm)) > 0)
            || ((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc) == TWI_BUSSTATE_BUSY_gc))
    {
        return 1;
    }
    return 0;
}

void initTWI() {
    // Initialize the TWI peripheral
    
    // set SCL as output (NOTE: SDA ALREADY SET AS OUTPUT)
    PORTA.DIRSET = PIN3_bm;
    // enable pull-up resistors
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
    // 50ns hold time (from github example)
    TWI0.CTRLA = TWI_SDAHOLD_50NS_gc; 
    //debug
    TWI0.DBGCTRL = TWI_DBGRUN_bm;
    // clear MSTATUS & set bus state to idle
    TWI0.MSTATUS = TWI_RIF_bm | TWI_WIF_bm | TWI_CLKHOLD_bm | TWI_RXACK_bm |
            TWI_ARBLOST_bm | TWI_BUSERR_bm | TWI_BUSSTATE_IDLE_gc;
    // set baud rate
    TWI0.MBAUD = 10;  
    // host mode
    TWI0.MCTRLA = TWI_ENABLE_bm;
    
}
int startTWI(uint8_t rw) {
    if (TWI_IS_BUSBUSY()) {
        return 5;
    }
    TWI0.MADDR = (0x10 << 1) | rw;
    TWI_WAIT();
    if (isTWIBad()) {
        TWI0.MCTRLB = TWI_MCMD_STOP_gc;
        return 8;
    }
    return 0;
}
int writeToTWI(uint8_t* data, uint8_t len) {
    uint8_t count = 0;
    while (count < len) {
        TWI0.MDATA = data[count];
        TWI_WAIT();
        if (CLIENT_NACK()) {
            return 7;
        }
        count++;
    }
    return 0;
}
int TWI_sendBytes(uint8_t* data, uint8_t len) {
    int tst = startTWI(0);
    if (tst != 0) {
        return tst;
    }
    int success = writeToTWI(data, len);
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;
    
    return success;
}

int writeControlRegs(uint16_t* data) {
    uint8_t to_send[12];
    uint8_t send_idx = 0;
    for (int i = 2; i < 8; i++) {
        to_send[send_idx] = data[i] >> 8;
        send_idx++;
        to_send[send_idx] = data[i] & 0x00FF;
        send_idx++;
    }
    int success = TWI_sendBytes(to_send, 12);
    return success;
}
int readRegs(uint16_t* data) {
    uint8_t bCount = 0;
    uint8_t len = 32;
    uint8_t i = 10;
    uint8_t shift = 1;
    // start TWI
    if (startTWI(1)) {
        return 1;
    }
    // release clock hold
    TWI0.MSTATUS = TWI_CLKHOLD_bm;
    while (bCount < len) {
        TWI_WAIT();
        if (shift){
            data[i] = TWI0.MDATA << 8;
            shift = 0;
        }
        else {
            data[i] |= TWI0.MDATA;
            shift = 1;
            i += 1;
            if (i == 16) {
                i = 0;
            }
        }
        bCount += 1;
        if (bCount != len) {
            TWI0.MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;
        }
    }
    TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
    return 0;
}
