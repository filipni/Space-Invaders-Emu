#include "emulator.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

QTextStream out(stdout);

Emulator::Emulator()
{

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
    {
        cpu.memory[i] = fileData.at(i);
    }

    bool running = true;
    while (running)
    {
        decode(cpu.memory[cpu.registers.PC]);
        if (cpu.registers.PC > 0x2000)
            running = false;

    }
}

int Emulator::decode(uint8_t op)
{
    double cycles = 0;
    QString instruction("");
    switch(op)
    {
      case 0x00:
          cpu.NOP();
          cpu.registers.PC += 1;
          cycles = 4;
          break;
      case 0x01:
          cpu.LXI_B();
          cpu.registers.PC += 3;
          cycles = 10;
          break;
      case 0x02:
          cpu.STAX_B();
          cpu.registers.PC += 1;
          cycles = 7;
          break;
      case 0x03:
          cpu.INR_B();
          cpu.registers.PC += 1;
          cycles = 5;
          break;
      case 0x04:
          cpu.INR_B();
          cpu.registers.PC += 1;
          cycles = 5;
          break;
      case 0x05:
          cpu.DCR_B();
          cpu.registers.PC += 1;
          cycles = 5;
          break;
      case 0x06:
          cpu.MVI_B();
          cpu.registers.PC +=2;
          cycles = 7;
          break;
      case 0x07:
          cpu.RLC();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x08:
          cpu.NOP();
          cpu.registers.PC += 1;
          cycles = 4;
          break;
      case 0x09:
          cpu.DAD_B();
          cpu.registers.PC +=1;
          cycles = 10;
          break;
      case 0x0A:
          cpu.LDAX_B();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x0B:
          cpu.DCX_B();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x0C:
          cpu.INR_C();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x0D:
          cpu.DCR_C();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x0E:
          cpu.MVI_C();
          cpu.registers.PC +=2;
          cycles = 7;
          break;
      case 0x0F:
          cpu.RRC();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x10:
          cpu.NOP();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x11:
          cpu.LXI_D();
          cpu.registers.PC +=3;
          cycles = 10;
          break;
      case 0x12:
          cpu.STAX_D();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x13:
          cpu.INX_D();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x14:
          cpu.INR_D();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x15:
          cpu.DCR_D();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x16:
          cpu.MVI_D();
          cpu.registers.PC +=2;
          cycles = 7;
          break;
      case 0x17:
          cpu.RAL();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x18:
          cpu.NOP();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x19:
          cpu.DAD_D();
          cpu.registers.PC +=1;
          cycles = 10;
          break;
      case 0x1A:
          cpu.LDAX_D();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x1B:
          cpu.DCX_D();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x1C:
          cpu.INR_E();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x1D:
          cpu.DCR_E();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x1E:
          cpu.MVI_C();
          cpu.registers.PC +=2;
          cycles = 7;
          break;
      case 0x1F:
          cpu.RAR();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x20:
          cpu.NOP();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x21:
          cpu.LXI_H();
          cpu.registers.PC +=3;
          cycles = 10;
          break;
      case 0x22:
          cpu.SHLD();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x23:
          cpu.INX_H();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x24:
          cpu.INR_H();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x25:
          cpu.DCR_H();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x26:
          cpu.MVI_H();
          cpu.registers.PC +=2;
          cycles = 7;
          break;
      case 0x27:
          cpu.DAA();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x28:
          cpu.NOP();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x29:
          cpu.DAD_H();
          cpu.registers.PC +=1;
          cycles = 10;
          break;
      case 0x2A:
          cpu.LHLD();
          cpu.registers.PC +=3;
          cycles = 16;
          break;
      case 0x2B:
          cpu.DCX_H();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x2C:
          cpu.INR_L();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x2D:
          cpu.DCR_L();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x2E:
          cpu.MVI_L();
          cpu.registers.PC +=2;
          cycles = 7;
          break;
      case 0x2F:
          cpu.CMA();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x30:
          cpu.NOP();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x31:
          cpu.LXI_SP();
          cpu.registers.PC +=3;
          cycles = 10;
          break;
      case 0x32:
          cpu.STA();
          cpu.registers.PC +=1;
          cycles = 13;
          break;
      case 0x33:
          cpu.INX_SP();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x34:
          cpu.INR_M();
          cpu.registers.PC +=1;
          cycles = 10;
          break;
      case 0x35:
          cpu.DCR_M();
          cpu.registers.PC +=1;
          cycles = 10;
          break;
      case 0x36:
          cpu.MVI_M();
          cpu.registers.PC +=2;
          cycles = 10;
          break;
      case 0x37:
          cpu.STC();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x38:
          cpu.NOP();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x39:
          cpu.DAD_SP();
          cpu.registers.PC +=1;
          cycles = 10;
          break;
      case 0x3A:
          cpu.LDA();
          cpu.registers.PC +=3;
          cycles = 13;
          break;
      case 0x3B:
          cpu.DCX_SP();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x3C:
          cpu.INR_A();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x3D:
          cpu.DCR_A();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x3E:
          cpu.MVI_A();
          cpu.registers.PC +=2;
          cycles = 7;
          break;
      case 0x3F:
          cpu.CMC();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x40:
          cpu.MOV_B_B();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x41:
          cpu.MOV_B_C();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x42:
          cpu.MOV_B_D();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x43:
          cpu.MOV_B_E();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x44:
          cpu.MOV_B_H();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x45:
          cpu.MOV_B_L();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x46:
          cpu.MOV_B_M();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x47:
          cpu.MOV_B_A();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x48:
          cpu.MOV_C_B();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x49:
          cpu.MOV_C_C();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x4A:
          cpu.MOV_C_D();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x4B:
          cpu.MOV_C_E();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x4C:
          cpu.MOV_C_H();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x4D:
          cpu.MOV_C_L();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x4E:
          cpu.MOV_C_M();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x4F:
          cpu.MOV_C_A();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x50:
          cpu.MOV_D_B();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x51:
          cpu.MOV_D_C();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x52:
          cpu.MOV_D_D();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x53:
          cpu.MOV_D_E();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x54:
          cpu.MOV_D_H();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x55:
          cpu.MOV_D_L();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x56:
          cpu.MOV_D_M();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x57:
          cpu.MOV_D_A();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x58:
          cpu.MOV_E_B();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x59:
          cpu.MOV_E_C();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x5A:
          cpu.MOV_E_D();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x5B:
          cpu.MOV_E_E();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x5C:
          cpu.MOV_E_H();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x5D:
          cpu.MOV_E_L();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x5E:
          cpu.MOV_E_M();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x5F:
          cpu.MOV_E_A();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x60:
          cpu.MOV_H_B();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x61:
          cpu.MOV_H_C();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x62:
          cpu.MOV_H_D();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x63:
          cpu.MOV_H_E();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x64:
          cpu.MOV_H_H();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x65:
          cpu.MOV_H_L();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x66:
          cpu.MOV_H_M();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x67:
          cpu.MOV_H_A();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x68:
          cpu.MOV_L_B();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x69:
          cpu.MOV_L_C();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x6A:
          cpu.MOV_L_D();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x6B:
          cpu.MOV_L_E();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x6C:
          cpu.MOV_L_H();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x6D:
          cpu.MOV_L_L();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x6E:
          cpu.MOV_L_M();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x6F:
          cpu.MOV_L_A();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x70:
          cpu.MOV_M_B();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x71:
          cpu.MOV_M_C();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x72:
          cpu.MOV_M_D();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x73:
          cpu.MOV_M_E();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x74:
          cpu.MOV_M_H();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x75:
          cpu.MOV_M_L();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x76:
          //cpu.HLT();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x77:
          cpu.MOV_M_A();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x78:
          cpu.MOV_A_B();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x79:
          cpu.MOV_A_C();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x7A:
          cpu.MOV_A_D();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x7B:
          cpu.MOV_A_E();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x7C:
          cpu.MOV_A_H();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x7D:
          cpu.MOV_A_L();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x7E:
          cpu.MOV_L_M();
          cpu.registers.PC +=1;
          cycles = 7;
          break;
      case 0x7F:
          cpu.MOV_A_A();
          cpu.registers.PC +=1;
          cycles = 5;
          break;
      case 0x80:
          cpu.ADD_B();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x81:
          cpu.ADD_C();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x82:
          cpu.ADD_D();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x83:
          cpu.ADD_E();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x84:
          cpu.ADD_H();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x85:
          cpu.ADD_L();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x86:
          cpu.ADD_M();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x87:
          cpu.ADD_A();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x88:
          cpu.ADC_B();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x89:
          cpu.ADC_C();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x8A:
          cpu.ADC_D();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x8B:
          cpu.ADC_E();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x8C:
          cpu.ADC_H();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x8D:
          cpu.ADC_L();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x8E:
          cpu.ADC_M();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x8F:
          cpu.ADC_A();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x90:
          cpu.SUB_B();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x91:
          cpu.SUB_C();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x92:
          cpu.SUB_D();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x93:
          cpu.SUB_E();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x94:
          cpu.SUB_H();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x95:
          cpu.SUB_L();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x96:
          cpu.SUB_M();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x97:
          cpu.SUB_A();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x98:
          cpu.SBB_B();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x99:
          cpu.SBB_C();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x9A:
          cpu.SBB_D();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x9B:
          cpu.SBB_E();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x9C:
          cpu.SBB_H();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x9D:
          cpu.SBB_L();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x9E:
          cpu.SBB_M();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0x9F:
          cpu.SBB_A();
          cycles = 4;
          cpu.registers.PC +=1;
          break;
      case 0xA0:
          cpu.ANA_B();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xA1:
          cpu.ANA_C();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xA2:
          cpu.ANA_D();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xA3:
          cpu.ANA_E();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
          break;
      case 0xA4:
          cpu.ANA_H();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xA5:
          cpu.ANA_L();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xA6:
          cpu.ANA_M();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xA7:
          cpu.ANA_A();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xA8:
          cpu.XRA_B();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xA9:
          cpu.XRA_C();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xAA:
          cpu.XRA_D();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xAB:
          cpu.XRA_E();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xAC:
          cpu.XRA_H();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xAD:
          cpu.XRA_L();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xAE:
          cpu.XRA_M();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xAF:
          cpu.XRA_A();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xB0:
          cpu.ORA_B();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xB1:
          cpu.ORA_C();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xB2:
          cpu.ORA_D();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xB3:
          cpu.ORA_E();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xB4:
          cpu.ORA_H();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xB5:
          cpu.ORA_L();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xB6:
          cpu.ORA_M();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xB7:
          cpu.ORA_A();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xB8:
          cpu.CMP_B();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xB9:
          cpu.CMP_C();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xBA:
          cpu.CMP_D();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xBB:
          cpu.CMP_E();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xBC:
          cpu.CMP_H();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xBD:
          cpu.CMP_L();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xBE:
          cpu.CMP_M();
          cpu.registers.PC +=1;
          cycles = 4;
          break;
      case 0xBF:
          cpu.CMP_A();
          cpu.registers.PC +=1;
          break;
      case 0xC0:
          cpu.RNZ();
          cycles = 11 / 5;
          break;
      case 0xC1:
          cpu.POP_B();
          cpu.registers.PC +=1;
          break;
      case 0xC2:
          cpu.JNZ();
          cycles = 10;
          break;
      case 0xC3:
          cpu.JMP();
          cycles = 10;
          break;
      case 0xC4:
          cpu.CNZ();
          cycles = 17 / 11;
          break;
      case 0xC5:
          cpu.PUSH_B();
          cpu.registers.PC +=1;
          cycles = 11;
          break;
      case 0xC6:
          cpu.ADI();
          cpu.registers.PC +=2;
          break;
      case 0xC7:
          cpu.RST_0();
          cycles = 11;
          break;
      case 0xC8:
          cpu.RZ();
          cycles = 11 / 5;
          break;
      case 0xC9:
          cpu.RET();
          cycles = 10;
          break;
      case 0xCA:
          cpu.JZ();
          cycles = 10;
          break;
      case 0xCB:
          cpu.JMP();
          cycles = 10;
          break;
      case 0xCC:
          cpu.CZ();
          cycles = 17 / 11;
          break;
      case 0xCD:
          cpu.CALL();
          cycles = 17;
          break;
      case 0xCE:
          cpu.ACI();
          cpu.registers.PC +=2;
          break;
      case 0xCF:
          cpu.RST_1();
          cycles = 11;
          break;
      case 0xD0:
          cpu.RNC();
          cycles = 11 / 5;
          break;
      case 0xD1:
          cpu.POP_D();
          cpu.registers.PC +=1;
          cycles = 10;
          break;
      case 0xD2:
          cpu.JNC();
          cycles = 10;
          break;
      case 0xD3:
          instruction = "OUT d8";
          cpu.registers.PC +=2;
          break;
      case 0xD4:
          cpu.CNC();
          cycles = 17 / 11;
          break;
      case 0xD5:
          cpu.PUSH_D();
          cpu.registers.PC +=1;
          cycles = 11;
          break;
      case 0xD6:
          cpu.SUI();
          cpu.registers.PC +=2;
          break;
      case 0xD7:
          cpu.RST_2();
          cycles = 11;
          break;
      case 0xD8:
          cpu.RC();
          cycles = 11 / 5;
          break;
      case 0xD9:
          cpu.RET();
          cycles = 10;
          break;
      case 0xDA:
          cpu.JC();
          cycles = 10;
          break;
      case 0xDB:
          //cpu.IN();
          cpu.registers.PC +=2;
          break;
      case 0xDC:
          cpu.CC();
          cycles = 17 / 11;
          break;
      case 0xDD:
          cpu.CALL();
          cycles = 17;
          break;
      case 0xDE:
          cpu.SBI();
          cpu.registers.PC +=2;
          break;
      case 0xDF:
          cpu.RST_3();
          cycles = 11;
          break;
      case 0xE0:
          cpu.RPO();
          cycles = 11 / 5;
          break;
      case 0xE1:
          cpu.POP_H();
          cpu.registers.PC +=1;
          cycles = 10;
          break;
      case 0xE2:
          cpu.JPO();
          cycles = 10;
          break;
      case 0xE3:
          cpu.XTHL();
          cpu.registers.PC +=1;
          break;
      case 0xE4:
          cpu.CPO();
          cycles = 17 / 11;
          break;
      case 0xE5:
          cpu.PUSH_H();
          cpu.registers.PC +=1;
          break;
      case 0xE6:
          cpu.ANI();
          cpu.registers.PC +=2;
          break;
      case 0xE7:
          cpu.RST_4();
          cycles = 11;
          break;
      case 0xE8:
          cpu.RPE();
          cycles = 11 / 5;
          break;
      case 0xE9:
          cpu.PCHL();
          cpu.registers.PC +=1;
          break;
      case 0xEA:
          cpu.JPE();
          cycles = 10;
          break;
      case 0xEB:
          cpu.XCHG();
          cpu.registers.PC +=1;
          break;
      case 0xEC:
          cpu.CPE();
          cycles = 17 / 11;
          break;
      case 0xED:
          cpu.CALL();
          cycles = 17;
          break;
      case 0xEE:
          cpu.XRI();
          cpu.registers.PC +=2;
          break;
      case 0xEF:
          cpu.RST_5();
          cycles = 11;
          break;
      case 0xF0:
          cpu.RP();
          cycles = 11 / 5;
          break;
      case 0xF1:
          cpu.POP_PSW();
          cpu.registers.PC +=1;
          cycles = 10;
          break;
      case 0xF2:
          cpu.JP();
          cycles = 10;
          break;
      case 0xF3:
          cpu.DI();
          cpu.registers.PC +=1;
          break;
      case 0xF4:
          cpu.CP();
          cycles = 17 / 11;
          break;
      case 0xF5:
          cpu.PUSH_PSW();
          cpu.registers.PC +=1;
          cycles = 10;
          break;
      case 0xF6:
          cpu.ORI();
          cpu.registers.PC +=2;
          break;
      case 0xF7:
          cpu.RST_6();
          cycles = 11;
          break;
      case 0xF8:
          cpu.RM();
          cycles = 11 / 5;
          break;
      case 0xF9:
          cpu.SPHL();
          cpu.registers.PC +=1;
          break;
      case 0xFA:
          cpu.JM();
          cycles = 10;
          break;
      case 0xFB:
          cpu.EI();
          cpu.registers.PC +=1;
          break;
      case 0xFC:
          cpu.CM();
          cycles = 17 / 11;
          break;
      case 0xFD:
          cpu.CALL();
          cycles = 17;
          break;
      case 0xFE:
          cpu.CPI();
          cpu.registers.PC +=2;
          break;
      case 0xFF:
          cpu.RST_7();
          cycles = 11;
          break;
      default:
          QString errorString = "Instruction not implemented: " + QString().sprintf("%02x", op);
          qFatal(errorString.toStdString().c_str());
      }
      return cycles;
}
