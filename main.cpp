
#include <QApplication>
#include <QtWidgets>

#include <stdlib.h>
#include <iostream>
#include <string>

#include "MainWindow.h"
#include "key_filter.h"

#include "header.h"

//#include "MrzsFrameSink.h"  
//#include "variables_global_LinkLayer.h"
//#include "variables_global_APDU.h"
#include "StaticBuffer_for_LPDU_MAX_FRAME_SIZE.h"
#include "LinkFrame.h"
#include "MemoryInspect.h"
//#include "scMrzs.h"

void inspect_LogEntry(std::string &message);

void StartCDOANInit(void);

///extern  char msg[292];//DNP_MAX_DATA_LINK_FRAME_SIZE_WITH_CRC];			
 
uint8_t *getInputMessage(void);
uint16_t *getLenInputMessage(void);
extern void TimeSliceMrzs();

#define UNUSED(x) (void)(x)

key_filter *pkf;

MainWindow *mainWindow;

  StaticBuffer_for_LPDU_MAX_FRAME_SIZE buffer;

//uint16_t DNPCrc1(uint8_t* input1, uint16_t length);

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  key_filter kf;
  app.installEventFilter(pkf=&kf);

qDebug()<<"********SUITE('CDOAN1')********";
  StartCDOANInit();

  WSeq_for_Uint16_t  output = as_wseq_in_StaticBuffer_for_LPDU_MAX_FRAME_SIZEOver1(&buffer);

//RSeq_for_Uint16_t FormatResetLinkStates_in_LinkFrame_static(
//  WSeq_for_Uint16_t* buffer, boolean aIsMaster, uint16_t aDest, uint16_t aSrc);//, Logger* pLogger);
  boolean master = true;
  uint16_t dest = 1;
  uint16_t src = 1024;

  uint8_t master_message[] = {0x05, 0x64, 0x0D, 0xC4, 0x01, 0x00, 0x00, 0x04, 0x2D, 0x37, 0xC0, 0xC2, 0x01, 0x1E, 0x02, 0x00, 0x05, 0x06, 0xFB, 0x9A};
  RSeq_for_Uint16_t rseq;
  rseq.buffer_ = master_message;
  rseq.hHasLength.m_length = sizeof(master_message);
 std::cout<<"rseq.hHasLength.m_length= "<<(uint16_t)rseq.hHasLength.m_length<<'\n';

////T->ResetLinkStates
/*
  RSeq_for_Uint16_t rseq = FormatResetLinkStates_in_LinkFrame_static(
             &output, master, dest, src);//, Logger* pLogger);
*/
  inspect_RSeq(&rseq);

 std::cout<<"rseq.buffer_[0]= "<<(uint16_t)rseq.buffer_[0]<<'\n';
 std::cout<<"rseq.buffer_[1]= "<<(uint16_t)rseq.buffer_[1]<<'\n';
 std::cout<<"rseq.buffer_[2]= "<<(uint16_t)rseq.buffer_[2]<<'\n';
 std::cout<<"rseq.buffer_[3]= "<<(uint16_t)rseq.buffer_[3]<<'\n';
 std::cout<<"rseq.buffer_[4]= "<<(uint16_t)rseq.buffer_[4]<<'\n';

 //копіюєм в inputMessage
uint8_t *inputMessage = getInputMessage();
 uint32_t len = length_in_HasLength_for_Uint16_t(&(rseq.hHasLength));
 *getLenInputMessage() = len;

 if(len > 292) len = 292;
 for(uint16_t i=0; i<len; i++)
 {
  inputMessage[i] = rseq.buffer_[i];
 }

 TimeSliceMrzs();
/*
void DNPCrc(char *msg, int16_t start, int16_t length, char *crc);
//void DNPCrc1(uint8_t* input1, uint16_t length);

   uint8_t DNPMessageMrzs[] = {(uint8_t)0x05, (uint8_t)0x64, (uint8_t)0x05, (uint8_t)0xC0, (uint8_t)0x01, (uint8_t)0x00, (uint8_t)0x00, (uint8_t)0x04};
//   char crc[2];
//   DNPCrc(DNPMessageMrzs, 0, 8, crc);//DNP_DATA_LINK_LENGTH_WTHOUT_CRC, crc);						
uint16_t crc1 =  DNPCrc1(DNPMessageMrzs, 8);//uint16_t length);
 std::cout<<"crc= "<<std::hex<<(uint16_t)crc1<<'\n';
// std::cout<<"crc[1]= "<<(uint16_t)crc[1]<<'\n';
*/

  MainWindow mainWindowObj;
  mainWindow = &mainWindowObj;


  mainWindowObj.resize(600, 800);
  mainWindowObj.show();
  return app.exec();
}


key_filter::key_filter(QObject *parent)
  : QObject(parent)
{
}

bool key_filter::eventFilter(QObject *obj, QEvent *event)
{
  UNUSED(obj);

  if(event->type() == QEvent::KeyPress)
  {
    QKeyEvent *ke = static_cast<QKeyEvent*>(event);
    int myKey = ke->key();

    switch(myKey)
    {
    case  Qt::Key_Asterisk:
      emit sig_key_asterisk();
      return true;
    }//switch

    if(ke->modifiers()==Qt::NoModifier)
    switch(myKey)
    {
    case  Qt::Key_1:
      emit sig_key_1();
      return true;
    case  Qt::Key_2:
      emit sig_key_2();
      return true;
    case  Qt::Key_3:
      emit sig_key_3();
      return true;
    case  Qt::Key_4:
      emit sig_key_4();
      return true;

    case  Qt::Key_5:
      emit sig_key_5();
      return true;
    case  Qt::Key_6:
      emit sig_key_6();
      return true;
    case  Qt::Key_7:
      emit sig_key_7();
      return true;
    case  Qt::Key_8:
      emit sig_key_8();
      return true;

    case  Qt::Key_Up:
      emit sig_key_up();
      return true;
    case  Qt::Key_Down:
      emit sig_key_down();
      return true;
    case  Qt::Key_PageDown:
      emit sig_key_pgDown();
      return true;
    case  Qt::Key_PageUp:
      emit sig_key_pgUp();
      return true;
    case  Qt::Key_Left:
      emit sig_key_left();
      return true;
    case  Qt::Key_Right:
      emit sig_key_right();
      return true;
    case  Qt::Key_Home:
      emit sig_key_home();
      return true;
    case  Qt::Key_End:
      emit sig_key_end();
      return true;
    case  Qt::Key_Insert:
      emit sig_key_enter();
      return true;
    case  Qt::Key_Delete:
      emit sig_key_esc();
      return true;
    case  Qt::Key_Escape:
      emit sig_key_esc();
      return true;

    case  Qt::Key_F1:
      emit sig_key_f1();
      return true;
    case  Qt::Key_F2:
      emit sig_key_f2();
      return true;
    case  Qt::Key_F3:
      emit sig_key_f3();
      return true;

    case  Qt::Key_F8:
      emit sig_key_f8();
      return true;
    }//switch

   if(ke->modifiers()!=Qt::ControlModifier) return false;
    switch(myKey)
    {
    case  Qt::Key_1:
      emit sig_ctrlkey(0);
      return true;
    case  Qt::Key_2:
      emit sig_ctrlkey(1);
      return true;
    case  Qt::Key_3:
      emit sig_ctrlkey(2);
      return true;
    case  Qt::Key_4:
      emit sig_ctrlkey(3);
      return true;
    case  Qt::Key_5:
      emit sig_ctrlkey(4);
      return true;
    case  Qt::Key_6:
      emit sig_ctrlkey(5);
      return true;
    case  Qt::Key_7:
      emit sig_ctrlkey(6);
      return true;
    case  Qt::Key_8:
      emit sig_ctrlkey(7);
      return true;
    }//switch

  }
  return false;
}

