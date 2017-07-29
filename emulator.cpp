#include "emulator.h"
#include <QFile>
#include <QTextStream>
#include <QColor>
#include <QDebug>

QTextStream out(stdout);

Emulator::Emulator()
{
    screen = QImage(SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS, QImage::Format_RGB32);
    rotatedScreen = QImage(SCREEN_HEIGHT_PIXELS, SCREEN_WIDTH_PIXELS, QImage::Format_RGB32);
}

void Emulator::VRAMtoScreen()
{
    for (int i = 0; i < SCREEN_HEIGHT_PIXELS; ++i)
    {
        for (int j = 0; j < SCREEN_WIDTH_BYTES; ++j)
        {
            uint8_t currentByte = cpu.memory[VIDEO_RAM_START + i * SCREEN_WIDTH_BYTES + j];

            for (int k = 0; k < 8; ++k)
            {
                QColor color;
                int currentPixel = currentByte >> k & 1;
                if (currentPixel)
                  color = Qt::white;
                else
                  color = Qt::black;

                int xPos = j * 8 + k;
                int yPos = i;
                screen.setPixel(xPos, yPos, color.rgb());
            }
        }
    }

    QTransform rotation;
    rotation.rotate(-90);
    rotatedScreen = screen.transformed(rotation);

    emit screenUpdated(&rotatedScreen);
}

void Emulator::run()
{
    QFile romFile(ROM_FILE_PATH);

    if (!romFile.open(QIODevice::ReadOnly))
        qFatal("Could not open rom file.");
    else
       out << "Opened " + QString(ROM_FILE_PATH) << endl;

    QByteArray fileData = romFile.readAll();
    Q_ASSERT(fileData.size() == ROM_SIZE);

    for (int i = ROM_START; i < ROM_SIZE; ++i)
        cpu.memory[i] = fileData.at(i);

    bool running = true;
    int counter = 0;
    while (running)
    {
        decode(cpu.memory[cpu.registers.PC]);
        counter++;

        if (counter == 100000000)
        {
            counter = 0;
            VRAMtoScreen();
        }

    }

}

double Emulator::decode(uint8_t op)
{
    switch(op)
    {
      case 0x00:
          return cpu.NOP();
      case 0x01:
          return cpu.LXI_B();
      case 0x02:
          return cpu.STAX_B();
      case 0x03:
          return cpu.INX_B();
      case 0x04:
          return cpu.INR_B();
      case 0x05:
          return cpu.DCR_B();
      case 0x06:
          return cpu.MVI_B();
      case 0x07:
          return cpu.RLC();
      case 0x08:
          return cpu.NOP();
      case 0x09:
          return cpu.DAD_B();
      case 0x0A:
          return cpu.LDAX_B();
      case 0x0B:
          return cpu.DCX_B();
      case 0x0C:
          return cpu.INR_C();
      case 0x0D:
          return cpu.DCR_C();
      case 0x0E:
          return cpu.MVI_C();
      case 0x0F:
          return cpu.RRC();
      case 0x10:
          return cpu.NOP();
      case 0x11:
          return cpu.LXI_D();
      case 0x12:
          return cpu.STAX_D();
      case 0x13:
          return cpu.INX_D();
      case 0x14:
          return cpu.INR_D();
      case 0x15:
          return cpu.DCR_D();
      case 0x16:
          return cpu.MVI_D();
      case 0x17:
          return cpu.RAL();
      case 0x18:
          return cpu.NOP();
      case 0x19:
          return cpu.DAD_D();
      case 0x1A:
          return cpu.LDAX_D();
      case 0x1B:
          return cpu.DCX_D();
      case 0x1C:
          return cpu.INR_E();
      case 0x1D:
          return cpu.DCR_E();
      case 0x1E:
          return cpu.MVI_C();
      case 0x1F:
          return cpu.RAR();
      case 0x20:
          return cpu.NOP();
      case 0x21:
          return cpu.LXI_H();
      case 0x22:
          return cpu.SHLD();
      case 0x23:
          return cpu.INX_H();
      case 0x24:
          return cpu.INR_H();
      case 0x25:
          return cpu.DCR_H();
      case 0x26:
          return cpu.MVI_H();
      case 0x27:
          return cpu.DAA();
      case 0x28:
          return cpu.NOP();
      case 0x29:
          return cpu.DAD_H();
      case 0x2A:
          return cpu.LHLD();
      case 0x2B:
          return cpu.DCX_H();
      case 0x2C:
          return cpu.INR_L();
      case 0x2D:
          return cpu.DCR_L();
      case 0x2E:
          return cpu.MVI_L();
      case 0x2F:
          return cpu.CMA();
      case 0x30:
          return cpu.NOP();
      case 0x31:
          return cpu.LXI_SP();
      case 0x32:
          return cpu.STA();
      case 0x33:
          return cpu.INX_SP();
      case 0x34:
          return cpu.INR_M();
      case 0x35:
          return cpu.DCR_M();
      case 0x36:
          return cpu.MVI_M();
      case 0x37:
          return cpu.STC();
      case 0x38:
          return cpu.NOP();
      case 0x39:
          return cpu.DAD_SP();
      case 0x3A:
          return cpu.LDA();
      case 0x3B:
          return cpu.DCX_SP();
      case 0x3C:
          return cpu.INR_A();
      case 0x3D:
          return cpu.DCR_A();
      case 0x3E:
          return cpu.MVI_A();
      case 0x3F:
          return cpu.CMC();
      case 0x40:
          return cpu.MOV_B_B();
      case 0x41:
          return cpu.MOV_B_C();
      case 0x42:
          return cpu.MOV_B_D();
      case 0x43:
          return cpu.MOV_B_E();
      case 0x44:
          return cpu.MOV_B_H();
      case 0x45:
          return cpu.MOV_B_L();
      case 0x46:
          return cpu.MOV_B_M();
      case 0x47:
          return cpu.MOV_B_A();
      case 0x48:
          return cpu.MOV_C_B();
      case 0x49:
          return cpu.MOV_C_C();
      case 0x4A:
          return cpu.MOV_C_D();
      case 0x4B:
          return cpu.MOV_C_E();
      case 0x4C:
          return cpu.MOV_C_H();
      case 0x4D:
          return cpu.MOV_C_L();
      case 0x4E:
          return cpu.MOV_C_M();
      case 0x4F:
          return cpu.MOV_C_A();
      case 0x50:
          return cpu.MOV_D_B();
      case 0x51:
          return cpu.MOV_D_C();
      case 0x52:
          return cpu.MOV_D_D();
      case 0x53:
          return cpu.MOV_D_E();
      case 0x54:
          return cpu.MOV_D_H();
      case 0x55:
          return cpu.MOV_D_L();
      case 0x56:
          return cpu.MOV_D_M();
      case 0x57:
          return cpu.MOV_D_A();
      case 0x58:
          return cpu.MOV_E_B();
      case 0x59:
          return cpu.MOV_E_C();
      case 0x5A:
          return cpu.MOV_E_D();
      case 0x5B:
          return cpu.MOV_E_E();
      case 0x5C:
          return cpu.MOV_E_H();
      case 0x5D:
          return cpu.MOV_E_L();
      case 0x5E:
          return cpu.MOV_E_M();
      case 0x5F:
          return cpu.MOV_E_A();
      case 0x60:
          return cpu.MOV_H_B();
      case 0x61:
          return cpu.MOV_H_C();
      case 0x62:
          return cpu.MOV_H_D();
      case 0x63:
          return cpu.MOV_H_E();
      case 0x64:
          return cpu.MOV_H_H();
      case 0x65:
          return cpu.MOV_H_L();
      case 0x66:
          return cpu.MOV_H_M();
      case 0x67:
          return cpu.MOV_H_A();
      case 0x68:
          return cpu.MOV_L_B();
      case 0x69:
          return cpu.MOV_L_C();
      case 0x6A:
          return cpu.MOV_L_D();
      case 0x6B:
          return cpu.MOV_L_E();
      case 0x6C:
          return cpu.MOV_L_H();
      case 0x6D:
          return cpu.MOV_L_L();
      case 0x6E:
          return cpu.MOV_L_M();
      case 0x6F:
          return cpu.MOV_L_A();
      case 0x70:
          return cpu.MOV_M_B();
      case 0x71:
          return cpu.MOV_M_C();
      case 0x72:
          return cpu.MOV_M_D();
      case 0x73:
          return cpu.MOV_M_E();
      case 0x74:
          return cpu.MOV_M_H();
      case 0x75:
          return cpu.MOV_M_L();
      case 0x76:
          //cpu.HLT();
          cpu.registers.PC +=1;
          return 7;
      case 0x77:
          return cpu.MOV_M_A();
      case 0x78:
          return cpu.MOV_A_B();
      case 0x79:
          return cpu.MOV_A_C();
      case 0x7A:
          return cpu.MOV_A_D();
      case 0x7B:
          return cpu.MOV_A_E();
      case 0x7C:
          return cpu.MOV_A_H();
      case 0x7D:
          return cpu.MOV_A_L();
      case 0x7E:
          return cpu.MOV_L_M();
      case 0x7F:
          return cpu.MOV_A_A();
      case 0x80:
          return cpu.ADD_B();
      case 0x81:
          return cpu.ADD_C();
      case 0x82:
          return cpu.ADD_D();
      case 0x83:
          return cpu.ADD_E();
      case 0x84:
          return cpu.ADD_H();
      case 0x85:
          return cpu.ADD_L();
      case 0x86:
          return cpu.ADD_M();
      case 0x87:
          return cpu.ADD_A();
      case 0x88:
          return cpu.ADC_B();
      case 0x89:
          return cpu.ADC_C();
      case 0x8A:
          return cpu.ADC_D();
      case 0x8B:
          return cpu.ADC_E();
      case 0x8C:
          return cpu.ADC_H();
      case 0x8D:
          return cpu.ADC_L();
      case 0x8E:
          return cpu.ADC_M();
      case 0x8F:
          return cpu.ADC_A();
      case 0x90:
          return cpu.SUB_B();
      case 0x91:
          return cpu.SUB_C();
      case 0x92:
          return cpu.SUB_D();
      case 0x93:
          return cpu.SUB_E();
      case 0x94:
          return cpu.SUB_H();
      case 0x95:
          return cpu.SUB_L();
      case 0x96:
          return cpu.SUB_M();
      case 0x97:
          return cpu.SUB_A();
      case 0x98:
          return cpu.SBB_B();
      case 0x99:
          return cpu.SBB_C();
      case 0x9A:
          return cpu.SBB_D();
      case 0x9B:
          return cpu.SBB_E();
      case 0x9C:
          return cpu.SBB_H();
      case 0x9D:
          return cpu.SBB_L();
      case 0x9E:
          return cpu.SBB_M();
      case 0x9F:
          return cpu.SBB_A();
      case 0xA0:
          return cpu.ANA_B();
      case 0xA1:
          return cpu.ANA_C();
      case 0xA2:
          return cpu.ANA_D();
      case 0xA3:
          return cpu.ANA_E();
      case 0xA4:
          return cpu.ANA_H();
      case 0xA5:
          return cpu.ANA_L();
      case 0xA6:
          return cpu.ANA_M();
      case 0xA7:
          return cpu.ANA_A();
      case 0xA8:
          return cpu.XRA_B();
      case 0xA9:
          return cpu.XRA_C();
      case 0xAA:
          return cpu.XRA_D();
      case 0xAB:
          return cpu.XRA_E();
      case 0xAC:
          return cpu.XRA_H();
      case 0xAD:
          return cpu.XRA_L();
      case 0xAE:
          return cpu.XRA_M();
      case 0xAF:
          return cpu.XRA_A();
      case 0xB0:
          return cpu.ORA_B();
      case 0xB1:
          return cpu.ORA_C();
      case 0xB2:
          return cpu.ORA_D();
      case 0xB3:
          return cpu.ORA_E();
      case 0xB4:
          return cpu.ORA_H();
      case 0xB5:
          return cpu.ORA_L();
      case 0xB6:
          return cpu.ORA_M();
      case 0xB7:
          return cpu.ORA_A();
      case 0xB8:
          return cpu.CMP_B();
      case 0xB9:
          return cpu.CMP_C();
      case 0xBA:
          return cpu.CMP_D();
      case 0xBB:
          return cpu.CMP_E();
      case 0xBC:
          return cpu.CMP_H();
      case 0xBD:
          return cpu.CMP_L();
      case 0xBE:
          return cpu.CMP_M();
      case 0xBF:
          return cpu.CMP_A();
      case 0xC0:
          return cpu.RNZ();
      case 0xC1:
          return cpu.POP_B();
      case 0xC2:
          return cpu.JNZ();
      case 0xC3:
          return cpu.JMP();
      case 0xC4:
          return cpu.CNZ();
      case 0xC5:
          return cpu.PUSH_B();
      case 0xC6:
          return cpu.ADI();
      case 0xC7:
          return cpu.RST_0();
      case 0xC8:
          return cpu.RZ();
      case 0xC9:
          return cpu.RET();
      case 0xCA:
          return cpu.JZ();
      case 0xCB:
          return cpu.JMP();
      case 0xCC:
          return cpu.CZ();
      case 0xCD:
          return cpu.CALL();
      case 0xCE:
          return cpu.ACI();
      case 0xCF:
          return cpu.RST_1();
      case 0xD0:
          return cpu.RNC();
      case 0xD1:
          return cpu.POP_D();
      case 0xD2:
          return cpu.JNC();
      case 0xD3:
          //instruction = "OUT d8";
          cpu.registers.PC += 2;
          return 10;
      case 0xD4:
          return cpu.CNC();
      case 0xD5:
          return cpu.PUSH_D();
      case 0xD6:
          return cpu.SUI();
      case 0xD7:
          return cpu.RST_2();
      case 0xD8:
          return cpu.RC();
      case 0xD9:
          return cpu.RET();
      case 0xDA:
          return cpu.JC();
      case 0xDB:
          //cpu.IN();
          cpu.registers.PC += 2;
          return 10;
      case 0xDC:
          return cpu.CC();
      case 0xDD:
          return cpu.CALL();
      case 0xDE:
          return cpu.SBI();
      case 0xDF:
          return cpu.RST_3();
      case 0xE0:
          return cpu.RPO();
      case 0xE1:
          return cpu.POP_H();
      case 0xE2:
          return cpu.JPO();
      case 0xE3:
          return cpu.XTHL();
      case 0xE4:
          return cpu.CPO();
      case 0xE5:
          return cpu.PUSH_H();
      case 0xE6:
          return cpu.ANI();
      case 0xE7:
          return cpu.RST_4();
      case 0xE8:
          return cpu.RPE();
      case 0xE9:
          return cpu.PCHL();
      case 0xEA:
          return cpu.JPE();
      case 0xEB:
          return cpu.XCHG();
      case 0xEC:
          return cpu.CPE();
      case 0xED:
          return cpu.CALL();
      case 0xEE:
          return cpu.XRI();
      case 0xEF:
          return cpu.RST_5();
      case 0xF0:
          return cpu.RP();
      case 0xF1:
          return cpu.POP_PSW();
      case 0xF2:
          return cpu.JP();
      case 0xF3:
          return cpu.DI();
      case 0xF4:
          return cpu.CP();
      case 0xF5:
          return cpu.PUSH_PSW();
      case 0xF6:
          return cpu.ORI();
      case 0xF7:
          return cpu.RST_6();
      case 0xF8:
          return cpu.RM();
      case 0xF9:
          return cpu.SPHL();
      case 0xFA:
          return cpu.JM();
      case 0xFB:
          return cpu.EI();
      case 0xFC:
          return cpu.CM();
      case 0xFD:
          return cpu.CALL();
      case 0xFE:
          return cpu.CPI();
      case 0xFF:
          return cpu.RST_7();
      default:
          qFatal("Instruction not implemented: %02x", op);
    }
    return 0;
}
