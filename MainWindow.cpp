
#include <QApplication>
#include <QtWidgets>
#include "key_filter.h"

#include "header.h"

#include "MainWindow.h"
//#include "loghandler.h"

extern key_filter *pkf;


MainWindow::MainWindow(QWidget *parent): QWidget(parent)
{
  setWindowTitle(QString::fromLocal8Bit("qt_dnp3"));
//  setWindowFlag(QtCore.Qt.WindowStaysOnTopHint)
  setWindowFlags(Qt::WindowStaysOnTopHint);

  QFont tabFont = QFont("Times", 14, QFont::Normal);

  miniDisplayWidget = new MiniDisplayWidget;
  miniButtonWidget  = new MiniButtonWidget;

  QGridLayout *mainLayout = new QGridLayout;

//  mainLayout->addWidget(sdiGroup,          0, 0);
//  mainLayout->addWidget(releGroup,         1, 0);
//  mainLayout->addWidget(miniDisplayWidget, 2, 0);
//  mainLayout->addWidget(dvFKGroup,         2, 1, 3, 1);
  mainLayout->addWidget(miniDisplayWidget, 0, 0);
  mainLayout->addWidget(miniButtonWidget,  1, 0);
//  mainLayout->addWidget(funcButtonWidget,  4, 0);
//  mainLayout->addLayout(buttonLayout);

  mainLayout->setHorizontalSpacing(5);
  mainLayout->setVerticalSpacing(5);
  mainLayout->setSizeConstraint(QLayout::SetFixedSize);

  setLayout(mainLayout);
}

MainWindow::~MainWindow()
{
}//

MiniButtonWidget::MiniButtonWidget(QWidget *parent)
  : QWidget(parent)
{
///*
  connect(pkf, SIGNAL(sig_key_up()),     this, SLOT(upClicked()));//
  connect(pkf, SIGNAL(sig_key_down()),   this, SLOT(downClicked()));//
//  connect(pkf, SIGNAL(sig_key_pgDown()), this, SLOT(pageDownClicked()));//
//  connect(pkf, SIGNAL(sig_key_pgUp()),   this, SLOT(pageUpClicked()));//
  connect(pkf, SIGNAL(sig_key_left()),   this, SLOT(leftClicked()));//
  connect(pkf, SIGNAL(sig_key_right()),  this, SLOT(rightClicked()));//
//  connect(pkf, SIGNAL(sig_key_home()),   this, SLOT(homeClicked()));//
//  connect(pkf, SIGNAL(sig_key_end()),    this, SLOT(endClicked()));//
  connect(pkf, SIGNAL(sig_key_enter()),  this, SLOT(enterClicked()));//
  connect(pkf, SIGNAL(sig_key_esc()),    this, SLOT(escClicked()));//
//  connect(pkf, SIGNAL(sig_key_asterisk()),    this, SLOT(updateClicked()));//
//*/

  QFont labFont = QFont("Times", 14, QFont::Normal);
  QFont boldFont = QFont("Times", 14, QFont::Bold);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  QGroupBox *configGroup = new QGroupBox(QString::fromLocal8Bit(""));
  configGroup->setFont(labFont);

  QGridLayout *miniButtonLayout = new QGridLayout;

  virtualMenuUpButton = createButton(" Message_in_Moc ", SLOT(upClicked()));
  virtualMenuDownButton = createButton("List_for_EventRecord", SLOT(downClicked()));
  virtualMenuUpButton->setFont(boldFont);
  virtualMenuDownButton->setFont(boldFont);

  virtualMenuCloseButton = createButton(" <Close ", SLOT(leftClicked()));
  virtualMenuOpenButton = createButton(" Open> ", SLOT(rightClicked()));
  virtualMenuCloseButton->setFont(boldFont);
  virtualMenuOpenButton->setFont(boldFont);

  virtualMenuPUpButton = createButton(" PageUp ", SLOT(digitClicked()));
  virtualMenuPDownButton = createButton(" PageDown ", SLOT(digitClicked()));
  virtualMenuPUpButton->setFont(boldFont);
  virtualMenuPDownButton->setFont(boldFont);

  virtualMenuHomeButton = createButton(" Home ", SLOT(digitClicked()));
  virtualMenuEndButton = createButton(" End ", SLOT(digitClicked()));
  virtualMenuHomeButton->setFont(boldFont);
  virtualMenuEndButton->setFont(boldFont);

  virtualMenuEnterButton = createButton(" Enter ", SLOT(enterClicked()));
  virtualMenuEscButton = createButton(" Esc ", SLOT(escClicked()));
  virtualMenuEnterButton->setFont(boldFont);
  virtualMenuEscButton->setFont(boldFont);

  virtualMenuResetButton = createButton(" Reset ", SLOT(digitClicked()));
  virtualMenuLogButton = createButton(" Clear ", SLOT(digitClicked()));
  virtualMenuResetButton->setFont(boldFont);
  virtualMenuLogButton->setFont(boldFont);

  virtualMenuUpdateButton = createButton(" Update ", SLOT(digitClicked()));
  virtualMenuFreeButton = createButton(" Free ", SLOT(freeClicked()));
  virtualMenuUpdateButton->setFont(boldFont);
  virtualMenuFreeButton->setFont(boldFont);

  miniButtonLayout->addWidget(virtualMenuUpButton, 1, 0);//, 0, 0);
  miniButtonLayout->addWidget(virtualMenuOpenButton, 1, 1);//, 0, 0);

  miniButtonLayout->addWidget(virtualMenuDownButton, 2, 0);//, 0, 0);
  miniButtonLayout->addWidget(virtualMenuCloseButton, 2, 1);//, 0, 0);

  miniButtonLayout->addWidget(virtualMenuPUpButton, 1, 2);//, 0, 0);
  miniButtonLayout->addWidget(virtualMenuHomeButton, 1, 3);//, 0, 0);

  miniButtonLayout->addWidget(virtualMenuPDownButton, 2, 2);//, 0, 0);
  miniButtonLayout->addWidget(virtualMenuEndButton, 2, 3);//, 0, 0);

  miniButtonLayout->addWidget(virtualMenuEnterButton, 3, 0);//, 0, 0);
  miniButtonLayout->addWidget(virtualMenuEscButton, 3, 1);//, 0, 0);

  miniButtonLayout->addWidget(virtualMenuResetButton, 4, 0);
  miniButtonLayout->addWidget(virtualMenuLogButton, 4, 1);
  miniButtonLayout->addWidget(virtualMenuUpdateButton, 4, 2);
  miniButtonLayout->addWidget(virtualMenuFreeButton, 4, 3);

  miniButtonLayout->setSpacing(100);
  miniButtonLayout->setColumnStretch(1, 1);
//  miniDisplayLayout->setColumnStretch(1, 8);

  miniButtonLayout->setContentsMargins(31, 31, 31, 31);
  miniButtonLayout->setHorizontalSpacing(5);
  miniButtonLayout->setVerticalSpacing(5);
  miniButtonLayout->setSizeConstraint(QLayout::SetFixedSize);

  configGroup->setLayout(miniButtonLayout);
  mainLayout->addWidget(configGroup);
  setLayout(mainLayout);
}//MiniButtonWidget::MiniButtonWidget

QToolButton *MiniButtonWidget::createButton(const QString &text,
    const char *member)
{
  QToolButton *button = new QToolButton;
  button->setText(text);
  button->setSizePolicy(QSizePolicy::Expanding,
                        QSizePolicy::Preferred);

  int side = button->minimumSizeHint().height() + 14;
  button->setMinimumSize(side, side);

  connect(button, SIGNAL(clicked()), this, member);

  return button;
}

void MiniButtonWidget::digitClicked()
{
//  QToolButton *clickedButton = qobject_cast<QToolButton *>(sender());
  qDebug()<<"22223333";

}//digitClicked()

void periodical_operations(void);
void MiniButtonWidget::freeClicked()
{
//  periodical_operations();
//  func20DataDiskretRegistrator();//комтрейд data дикр регистратора
//  func20ConfigDiskretRegistrator();//комтрейд config дикр регистратора
}//freeClicked()

//void loghandler_List_for_EventRecord();
void MiniButtonWidget::downClicked()
{
  qDebug()<<"BIT_KEY_DOWN";
// loghandler_List_for_EventRecord();
//  new_state_keyboard |= (1<<BIT_KEY_DOWN);
//  periodical_operations();//один оборот
}//downClicked()

/*
void inspect_LinkHeader(Memory_LinkHeader* pMemory_LinkHeader, int maxCount);
void inspect_GroupVariationRecord(Memory_GroupVariationRecord* pMemory_GroupVariationRecord, int maxCount);
void inspect_ObjectHeader(Memory_ObjectHeader* pMemory_ObjectHeader, int maxCount);
void inspect_HeaderRecord(Memory_HeaderRecord* pMemory_HeaderRecord, int maxCount);
void inspect_IINField(Memory_IINField* pMemory_IINField, int maxCount);
void inspect_EventLists(Memory_EventLists* pMemory_EventLists, int maxCount);
void inspect_PairSer4cpp_for_IINField_AppControlField(Memory_PairSer4cpp_for_IINField_AppControlField* pMemory_PairSer4cpp_for_IINField_AppControlField, int maxCount);
void inspect_ParsedRequest(Memory_ParsedRequest* pMemory_ParsedRequest, int maxCount);
void inspect_Message(Memory_Message* pMemory_Message, int maxCount);
void inspect_Result_for_APDUHeader_in_APDUHeaderParser(Memory_Result_for_APDUHeader_in_APDUHeaderParser* pMemory_Result_for_APDUHeader_in_APDUHeaderParser, int maxCount);
void inspect_EventRecord(Memory_EventRecord* pMemory_EventRecord, int maxCount);
void inspect_RangeHeader(Memory_RangeHeader* pMemory_RangeHeader, int maxCount);
*/
void MiniButtonWidget::upClicked()
{
  qDebug()<<"BIT_KEY_UP";
//if(pMemory_RangeHeader_1) inspect_RangeHeader(pMemory_RangeHeader_1, 10);
// if(pMemory_Message_1) inspect_Message(pMemory_Message_1, 10);
// if(pMemory_Message_2) inspect_Message(pMemory_Message_2);
// if(pMemory_Result_for_APDUHeader_in_APDUHeaderParser_1)inspect_Result_for_APDUHeader_in_APDUHeaderParser(pMemory_Result_for_APDUHeader_in_APDUHeaderParser_1, 10);
// if(pMemory_ParsedRequest_1)inspect_ParsedRequest(pMemory_ParsedRequest_1, 10);
// if(pMemory_IINField_1)inspect_IINField(pMemory_IINField_1, 10);
// if(pMemory_PairSer4cpp_for_IINField_AppControlField_1)inspect_PairSer4cpp_for_IINField_AppControlField(pMemory_PairSer4cpp_for_IINField_AppControlField_1, 10);
// if(pMemory_ObjectHeader_1)inspect_ObjectHeader(pMemory_ObjectHeader_1);
// if(pMemory_HeaderRecord_1)inspect_HeaderRecord(pMemory_HeaderRecord_1);
// if(pMemory_EventRecord_1)inspect_EventRecord(pMemory_EventRecord_1, 10);
// if(pMemory_EventLists_1)inspect_EventLists(pMemory_EventLists_1, 10);
// if(pMemory_LinkHeader_1)inspect_LinkHeader(pMemory_LinkHeader_1, 10);
// if(pMemory_GroupVariationRecord_1)inspect_GroupVariationRecord(pMemory_GroupVariationRecord_1);

//  inspect_Message(titleGlobal_1, memoryGlobal_1);
//  inspect_Message(titleGlobal_2, memoryGlobal_2);
//  inspect_Message("Moc", &mMessage1_global_in_MockLowerLayer);
// loghandler_EvenLists();
//  new_state_keyboard |= (1<<BIT_KEY_UP);
//  periodical_operations();//один оборот
}//upClicked()

void MiniButtonWidget::enterClicked()
{
  qDebug()<<"BIT_KEY_ENTER";
//  new_state_keyboard |= (1<<BIT_KEY_ENTER);
//  periodical_operations();//один оборот
//  periodical_operations();//один оборот
}//enterClicked()

void MiniButtonWidget::escClicked()
{
  qDebug()<<"BIT_KEY_ESC";
//  new_state_keyboard |= (1<<BIT_KEY_ESC);
//  periodical_operations();//один оборот
//  periodical_operations();//один оборот
}//escClicked()
void MiniButtonWidget::leftClicked()
{
  qDebug()<<"BIT_KEY_LEFT";
//  new_state_keyboard |= (1<<BIT_KEY_LEFT);
//  periodical_operations();//один оборот
}//leftClicked()
void MiniButtonWidget::rightClicked()
{
  qDebug()<<"BIT_KEY_RIGHT";
//  new_state_keyboard |= (1<<BIT_KEY_RIGHT);
//  periodical_operations();//один оборот
}//rihtClicked()

//-----------------------------------------------------
MiniDisplayWidget::MiniDisplayWidget(QWidget *parent)
  : QWidget(parent)
{
  QFont labFont = QFont("Times", 14, QFont::Normal);
  QFont boldFont = QFont("Times", 14, QFont::Bold);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  QGroupBox *configGroup = new QGroupBox(QString::fromLocal8Bit(""));
  configGroup->setFont(labFont);

  virtualMenu1Label = new QLabel(QString::fromLocal8Bit("111111"));
  virtualMenu2Label = new QLabel(QString::fromLocal8Bit("22222"));
  virtualMenu3Label = new QLabel(QString::fromLocal8Bit("33333"));
  virtualMenu4Label = new QLabel(QString::fromLocal8Bit("44444"));

  miniDisplayLayout = new QGridLayout;

//  miniDisplayLayout->addWidget(virtualMenu1Button,    1, 0+1);
//  miniDisplayLayout->addWidget(virtualMarker1Label,   1, 1+1);
  miniDisplayLayout->addWidget(virtualMenu1Label,     1-1, 0);//2+1);
//  miniDisplayLayout->addWidget(virtualMenu1ExtLabel,  1, 3+1);

//  miniDisplayLayout->addWidget(virtualMenu2Button,    2, 0+1);
//  miniDisplayLayout->addWidget(virtualMarker2Label,   2, 1+1);
  miniDisplayLayout->addWidget(virtualMenu2Label,     2-1, 0);//2+1);
//  miniDisplayLayout->addWidget(virtualMenu2ExtLabel,  2, 3+1);

//  miniDisplayLayout->addWidget(virtualMenu3Button,    3, 0+1);
//  miniDisplayLayout->addWidget(virtualMarker3Label,   3, 1+1);
  miniDisplayLayout->addWidget(virtualMenu3Label,     3-1, 0);//2+1);
//  miniDisplayLayout->addWidget(virtualMenu3ExtLabel,  3, 3+1);

//  miniDisplayLayout->addWidget(virtualMenu4Button,    4, 0+1);
//  miniDisplayLayout->addWidget(virtualMarker4Label,   4, 1+1);
  miniDisplayLayout->addWidget(virtualMenu4Label,     4-1, 0);//2+1);
//  miniDisplayLayout->addWidget(virtualMenu4ExtLabel,  4, 3+1);

  miniDisplayLayout->setSpacing(100);
  miniDisplayLayout->setColumnStretch(1, 1);

  miniDisplayLayout->setContentsMargins(18, 18, 18, 18);
  miniDisplayLayout->setHorizontalSpacing(5);
  miniDisplayLayout->setVerticalSpacing(5);

  configGroup->setLayout(miniDisplayLayout);
  mainLayout->addWidget(configGroup);
  setLayout(mainLayout);
}//MiniDisplayWidget::MiniDisplayWidget(QWidget *parent)



