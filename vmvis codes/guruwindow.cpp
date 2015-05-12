#include "guruwindow.h"
#include "ui_guruwindow.h"

guruwindow::guruwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::guruwindow)
{
    ui->setupUi(this);

    // General specification for the guruwindow # Hide the full screen button and set the name for window and fixed size.
    this->setWindowFlags( Qt::WindowTitleHint |  Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle("Server Setup");
    this->setFixedSize(this->width(),this->height());

    // ### network section TCP/IP server
    // when disconnect server is clicked the disconnectserver is called from gurutcp.cpp
    connect(ui->serverdisconnect,SIGNAL(clicked()),&start,SLOT(disconnectserver()));
    // when the server is started and there is a new connection, then the datas are sent.
    connect(&start,SIGNAL(senddata()),this,SLOT(acceptdata()));
    // disable for the initial process
    ui->serverdisconnect->setEnabled(false);
    ui->portinput->setEnabled(false);
    ui->ipinput->setEnabled(false);
    ui->verifypass->setEnabled(false);

    // ### camera section
    // Set all to false to start safe
    ui->stopcam->setEnabled(false);
    ui->startcam->setEnabled(false);
    ui->cameratype->setEditable(false);

    // ### sensor section
    // test sensor sending the value from slider to the 3D design.
    connect(&testxyz,SIGNAL(sendx(int)),ui->sensordisp,SLOT(setXRotation(int)));
    connect(&testxyz,SIGNAL(sendy(int)),ui->sensordisp,SLOT(setYRotation(int)));
    connect(&testxyz,SIGNAL(sendz(int)),ui->sensordisp,SLOT(setZRotation(int)));
    // sending data from real sensor to 3D design.
    connect(this,SIGNAL(outputx(int)),ui->sensordisp,SLOT(setXRotation(int)));
    connect(this,SIGNAL(outputy(int)),ui->sensordisp,SLOT(setYRotation(int)));
    connect(this,SIGNAL(outputz(int)),ui->sensordisp,SLOT(setZRotation(int)));
    // disable all the control
    ui->sensorsetup->setEnabled(false);
    ui->sensorconnect->setEnabled(false);
    ui->sensordisconnect->setEnabled(false);

    // ### terminal section
    ui->console->setEnabled(false);
    ui->serialconnect->setEnabled(false);
    ui->serialdisconnect->setEnabled(false);
    serial = new QSerialPort(this);
    settings = new sensorsetting;
    // connecting the serial data of the sensor
    connect(ui->serialsetup, SIGNAL(clicked()), settings, SLOT(show()));
    connect(ui->clear, SIGNAL(clicked()), ui->console, SLOT(clear()));
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(settings,SIGNAL(control()),this,SLOT(opencontrol()));

    // ### Action
    connect(ui->actionPrint,SIGNAL(triggered()),ui->console,SLOT(print()));
}

guruwindow::~guruwindow()
{
    delete ui;
    delete camera;
}

//#################### NETWORK SECTION - TCP/IP #############################

// <1.1> Toggle between Radio buttons to enable or disable the port and IP input
void guruwindow::on_allportandip_toggled(bool checked)
{
    if(checked==TRUE)
    {
    ui->portinput->setEnabled(false);
    ui->ipinput->setEnabled(false);
    }
}

void guruwindow::on_singleport_toggled(bool checked)
{
    if(checked==TRUE)
    {
    ui->portinput->setEnabled(true);
    ui->ipinput->setEnabled(false);
    }
}

void guruwindow::on_singleip_toggled(bool checked)
{
    if(checked==TRUE)
    {
    ui->portinput->setEnabled(true);
    ui->ipinput->setEnabled(true);
    }
}

// </1.1>
// <1.2> Checks the value inputed for Port and IP is valid or not

void guruwindow::on_portinput_editingFinished()
{
    QString portvalue = ui->portinput->text();
    bool check;
    portvalue.toUShort(&check);
    if(!check)
    {
        QMessageBox::information(this, "Port Value","Enter correct Port Vaule");
        ui->port->setStyleSheet("QLabel { color : red; }");
    }
    else
        ui->port->setStyleSheet("QLabel { color : black; }");
}

void guruwindow::on_ipinput_editingFinished()
{
    bool ipcheck = start.ipaddress.setAddress(ui->ipinput->text());
    if(!ipcheck)
    {
        QMessageBox::information(this, "IP Value","Enter correct IP Vaule");
        ui->ip->setStyleSheet("QLabel { color : red; }");
    }
    else
        ui->ip->setStyleSheet("QLabel { color : black; }");
}

// </1.2>
// <1.3> Check the password is entered or not and also check for the password match.

void guruwindow::on_wordpass_editingFinished()
{
    if(ui->wordpass->text()=='\0')
    {
        QMessageBox::information(this, "Password Missing","Enter Password");
        ui->password->setStyleSheet("QLabel { color : red; }");
    }
    else
    {
    this->password = ui->wordpass->text();
    ui->verifypass->setEnabled(true);
    ui->password->setStyleSheet("QLabel { color : black; }");
    }
}

void guruwindow::on_verifypass_editingFinished()
{
    start.verifypass = ui->verifypass->text();
    if(start.verifypass != password)
    {
        QMessageBox::information(this, "Password Mismatch","Enter correct Password");
        ui->verpasslabel->setStyleSheet("QLabel { color : red; }");
    }
    else
    ui->verpasslabel->setStyleSheet("QLabel { color : black; }");
}

// </1.3>
// <1.4> Action for the pushButtons Connect or disconnect to server with the value specified.

void guruwindow::on_serverconnect_clicked()
{
    int select1;
    if(ui->allportandip->isChecked())
    {
        start.setpost = 0;
        start.ipaddress = QHostAddress::Any;
        if((ui->wordpass->text()!='\0')&(start.verifypass == password))
        {
        ui->password->setStyleSheet("QLabel { color : black; }");
        ui->verpasslabel->setStyleSheet("QLabel { color : black; }");
        QMessageBox selection;
        selection.setText("All Port and IP will be allowed");
        selection.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        selection.setDefaultButton(QMessageBox::Ok);
        select1 = selection.exec();
        }
        else
        {
            if(ui->wordpass->text()=='\0')
            {
                QMessageBox::information(this, "Password Missing","Enter Password");
                ui->password->setStyleSheet("QLabel { color : red; }");
            }
            if(start.verifypass != password)
            {
                QMessageBox::information(this, "Password Mismatch","Enter correct Password");
                ui->verpasslabel->setStyleSheet("QLabel { color : red; }");
            }
        }

    }
    else if(ui->singleport->isChecked())
    {
        start.ipaddress = QHostAddress::Any;
        QString input_host = ui->portinput->text();
        bool check;
        start.setpost = input_host.toUShort(&check);
        if(check)
        {
         ui->port->setStyleSheet("QLabel { color : black; }");

         if((ui->wordpass->text()!='\0')&(start.verifypass == password))
         {
         ui->password->setStyleSheet("QLabel { color : black; }");
         ui->verpasslabel->setStyleSheet("QLabel { color : black; }");
         QMessageBox selection;
         selection.setText(ui->portinput->text() + " Port is set");
         selection.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
         selection.setDefaultButton(QMessageBox::Ok);
         select1 = selection.exec();
         }
         else
         {
             if(ui->wordpass->text()=='\0')
             {
                 QMessageBox::information(this, "Password Missing","Enter Password");
                 ui->password->setStyleSheet("QLabel { color : red; }");
             }
             if(start.verifypass != password)
             {
                 QMessageBox::information(this, "Password Mismatch","Enter correct Password");
                 ui->verpasslabel->setStyleSheet("QLabel { color : red; }");
             }
         }
        }
        else
        {
            QMessageBox::information(this, "Port Value","Enter correct Port Vaule");
            ui->port->setStyleSheet("QLabel { color : red; }");
        }
    }
    else if(ui->singleip->isChecked())
    {
        bool ipcheck = start.ipaddress.setAddress(ui->ipinput->text());
        QString input_host = ui->portinput->text();
        bool check;
        start.setpost = input_host.toUShort(&check);
        if(check & ipcheck)
        {
         ui->port->setStyleSheet("QLabel { color : black; }");
         ui->ip->setStyleSheet("QLabel { color : black; }");
         if((ui->wordpass->text()!='\0')&(start.verifypass == password))
         {
         ui->password->setStyleSheet("QLabel { color : black; }");
         ui->verpasslabel->setStyleSheet("QLabel { color : black; }");
         QMessageBox selection;
         selection.setText(ui->portinput->text() + " Port is set.\n" + ui->ipinput->text() + " IP is Set.");
         selection.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
         selection.setDefaultButton(QMessageBox::Ok);
         select1 = selection.exec();
         }
         else
         {
             if(ui->wordpass->text()=='\0')
             {
                 QMessageBox::information(this, "Password Missing","Enter Password");
                 ui->password->setStyleSheet("QLabel { color : red; }");
             }
             if(start.verifypass != password)
             {
                 QMessageBox::information(this, "Password Mismatch","Enter correct Password");
                 ui->verpasslabel->setStyleSheet("QLabel { color : red; }");
             }
         }
        }
        else
        {
            if(!check)
            {
            QMessageBox::information(this, "Port Value","Enter correct Port Vaule");
            ui->port->setStyleSheet("QLabel { color : red; }");
            }
            if(!ipcheck)
            {
            QMessageBox::information(this, "IP Value","Enter correct IP Vaule");
            ui->ip->setStyleSheet("QLabel { color : red; }");
            }

        }

    }
    // Get the request for user to accept the configuration to start the server.
    switch(select1)
    {
        case QMessageBox::Ok:
        start.start_connection();
        ui->serverconnect->setEnabled(false);
        ui->serverdisconnect->setEnabled(true);
        break;
        case QMessageBox::Cancel:
        ui->serverconnect->setEnabled(true);
        ui->serverdisconnect->setEnabled(false);
        break;
    }
}


void guruwindow::on_serverdisconnect_clicked()
{
    ui->serverconnect->setEnabled(true);
    ui->serverdisconnect->setEnabled(false);
}

void guruwindow::acceptdata()
{
    // sending the data through TCP/IP only when the server is started safe.
    connect(&testxyz,SIGNAL(sendx(int)),&start,SLOT(setXchange(int)));
    connect(&testxyz,SIGNAL(sendy(int)),&start,SLOT(setYchange(int)));
    connect(&testxyz,SIGNAL(sendz(int)),&start,SLOT(setZchange(int)));
    // send data from the real sensor
    connect(this,SIGNAL(outputx(int)),&start,SLOT(setXchange(int)));
    connect(this,SIGNAL(outputy(int)),&start,SLOT(setYchange(int)));
    connect(this,SIGNAL(outputz(int)),&start,SLOT(setZchange(int)));

}

// </1.4>
//#################### CAMERA SECTION #############################
// push button to detect all the cameras available
void guruwindow::on_refresh_clicked()
{
    ui->cameratype->setEnabled(true);
    ui->cameratype->clear();
    // used to store the camera device number
    QByteArray cameraDevice;
    //  foreach keyword that make it very easy to iterate over all the items stored in a container.
    foreach(const QByteArray &deviceName, QCamera::availableDevices()) {
        QString description = camera->deviceDescription(deviceName);
        // get the first device to be activated
        if (cameraDevice.isEmpty()) {
            cameraDevice = deviceName;
        }
        // adding the item to combobox
        ui->cameratype->addItem(description);
    }
    // making the setting for the first found device
    setCamera(cameraDevice);
    ui->startcam->setEnabled(true);
    ui->refresh->setEnabled(false);
}
//camera initial setting
void guruwindow::setCamera(const QByteArray &cameraDevice)
{
    // create the object for Qcamera
    if (cameraDevice.isEmpty())
        camera = new QCamera;
    else
        camera = new QCamera(cameraDevice);
    // if error occur give a warning
    connect(camera, SIGNAL(error(QCamera::Error)), this, SLOT(displayCameraError()));
    // Display the image for selected device
    camera->setViewfinder(ui->viewfinder);
}

// when there is change in the combo box get the index and reset the device selected.
void guruwindow::on_cameratype_currentIndexChanged(int index)
{
    int count = 0;
    // getting the index from the user and setting the camera.
    //int index = ui->cameratype->currentIndex();
    QByteArray cameraDevice;
    //  foreach keyword that make it very easy to iterate over all the items stored in a container.
    foreach(const QByteArray &deviceName, QCamera::availableDevices()) {
        // QString description = camera->deviceDescription(deviceName);
        // get the first device to be activated
        if (index == count) {
            cameraDevice = deviceName;
        }
         // ui->cameratype->addItem(description);
        count = count + 1;
    }
    setCamera(cameraDevice);
}

// if error is found display as warning
void guruwindow::displayCameraError()
{
    QMessageBox::warning(this, tr("Camera error"), camera->errorString());
}
// click to start the camera and disable all other control
void guruwindow::on_startcam_clicked()
{
    camera->start();
    ui->refresh->setEnabled(false);
    ui->cameratype->setEnabled(false);
    ui->startcam->setEnabled(false);
    ui->stopcam->setEnabled(true);
}
// stop the camera
void guruwindow::on_stopcam_clicked()
{
    camera->stop();
    ui->refresh->setEnabled(true);
    ui->startcam->setEnabled(true);
    ui->stopcam->setEnabled(false);
}

//#################### SENSOR SECTION #############################
// selecting the sensor type.
void guruwindow::on_selectsensor_currentIndexChanged(int index)
{
    // for ti sensor display the buttons
    if(index == 0)
    {
        ui->sensorsetup->setEnabled(false);
        ui->sensorconnect->setEnabled(false);
        ui->sensordisconnect->setEnabled(false);
    }
    // for other sensor get the data for console
    if(index == 1)
    {
        connect(ui->console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
        ui->sensorsetup->setEnabled(false);
        ui->sensorconnect->setEnabled(false);
        ui->sensordisconnect->setEnabled(false);
    }
}
// displaying the test dialog box when clicked.
void guruwindow::on_testglxyz_clicked()
{
    testxyz.show();
}
// initiating the sensor connection by passing the value 255,7,3.
void guruwindow::on_sensorsetup_clicked()
{
    ui->sensorsetup->setEnabled(false);
    ui->sensorconnect->setEnabled(true);
    ui->sensordisconnect->setEnabled(false);
    QByteArray value;
    value.resize(3);
    value[0]=255;
    value[1]=7;
    value[2]=3;
    serial->write(value);
    ui->console->putData(value);
}
// giving the value 255,8,7,0,0,0,0 to get the feed back of the sensor
void guruwindow::on_sensorconnect_clicked()
{
    ui->sensordisconnect->setEnabled(true);
    QByteArray value;
    value.resize(7);
    value[0]=255;
    value[1]=8;
    value[2]=7;
    value[3]=0;
    value[4]=0;
    value[5]=0;
    value[6]=0;
    serial->write(value);
    ui->console->putData(value);
}
// disconnecting the sensor by disconnecting 255,9,3
void guruwindow::on_sensordisconnect_clicked()
{
    ui->sensorsetup->setEnabled(true);
    ui->sensorconnect->setEnabled(false);
    ui->sensordisconnect->setEnabled(false);
    QByteArray value;
    value.resize(3);
    value[0]=255;
    value[1]=9;
    value[2]=3;
    serial->write(value);
    ui->console->putData(value);
}
// writing data to serial port
void guruwindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
// Reading data and displaying in the console
void guruwindow::readData()
{
    QByteArray data = serial->readAll();
    // finding 7 byte data
    if(data.size() == 7)
    {
        int unsigned valuex = 0;
        int unsigned valuey = 0;
        int unsigned valuez = 0;
        for(int i=0;i<=data.size();i++)
        {
            valuex = 0;
            valuey = 0;
            valuez = 0;
            // filtering the data
            quint8 outx = 0;
            quint8 outy = 0;
            quint8 outz = 0;

            outx |= data[4];
            outy |= data[5];
            outz |= data[6];
            // converting the byte data to angle.
            valuex = (outx * 360) / 255;
            valuey = (outy * 360) / 255;
            valuez = (outz * 360) / 255;
        }
        emit outputx(valuex);
        emit outputy(valuey);
        emit outputz(valuez);
    }
    ui->console->putData(data);
}

//#################### Terminal SECTION #############################
// connecting the serial with the data from the settings
void guruwindow::on_serialconnect_clicked()
{
    sensorsetting::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
            ui->sensorsetup->setEnabled(true);
            ui->actionPrint->setEnabled(true);
            ui->console->setEnabled(true);
            ui->console->setLocalEchoEnabled(p.localEchoEnabled);
            ui->serialconnect->setEnabled(false);
            ui->serialdisconnect->setEnabled(true);
            ui->serialsetup->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}
// disconnect serial port
void guruwindow::on_serialdisconnect_clicked()
{
    serial->close();
    ui->console->setEnabled(false);
    ui->serialconnect->setEnabled(true);
    ui->actionPrint->setEnabled(false);
    ui->serialdisconnect->setEnabled(false);
    ui->serialsetup->setEnabled(true);
    ui->actionCom_Port->setEnabled(true);
    ui->sensorsetup->setEnabled(false);
    ui->sensorconnect->setEnabled(false);
    ui->sensordisconnect->setEnabled(false);
    ui->statusBar->showMessage(tr("Disconnected"));
}
// handles the serial error
void guruwindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        on_sensordisconnect_clicked();
    }
}
// enables only when the button is clicked
void guruwindow::opencontrol()
{
    ui->serialsetup->setEnabled(false);
    ui->serialconnect->setEnabled(true);
    ui->sensorsetup->setEnabled(false);
}

//#################### ACTION SECTION #############################
// action to close
void guruwindow::on_actionClose_triggered()
{
    this->close();
}
// action to open the testxyz
void guruwindow::on_actionTest_Setup_triggered()
{
    testxyz.show();
}
// message about the watch
void guruwindow::on_actionAbout_Ti_Chronos_triggered()
{
    QMessageBox::information(this, "About Chronos","The Application uses TI Chronos smart Watch.\n Please refer: http://processors.wiki.ti.com/index.php/EZ430-Chronos");
}
// action for the port
void guruwindow::on_actionCom_Port_triggered()
{
    settings->show();
    ui->actionCom_Port->setEnabled(false);
}
// message about the software
void guruwindow::on_actionAbout_V_M_V_I_S_triggered()
{
    QMessageBox::information(this, "About VMVIS","The Application is a simple QT GUI design for Communication like TCP/IP,\n Camera and Serial port connections.");
}
// message about help
void guruwindow::on_actionHelp_triggered()
{
    QMessageBox::information(this, "Need help","Please Visit www.vishnumuthu.com for support and \n feel free to contact me.\n v.vishnumuthu@gmail.com");
}

