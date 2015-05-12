#include "sensorsetting.h"
#include "ui_sensorsetting.h"

QT_USE_NAMESPACE

sensorsetting::sensorsetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sensorsetting)
{
    ui->setupUi(this);
    // General specification for the guruwindow # Hide the full screen button and set the name for window and fixed size.
    this->setWindowFlags( Qt::WindowTitleHint |  Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle("Sensor Setup Box");
    this->setFixedSize(this->width(),this->height());
    // checks for the valid integer between the range for baudrate
    intValidator = new QIntValidator(0, 4000000, this);

    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->applyButton, SIGNAL(clicked()),this, SLOT(apply()));
    connect(ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)),this, SLOT(showPortInfo(int)));
    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)),this, SLOT(checkCustomBaudRatePolicy(int)));

    fillPortsParameters();
    fillPortsInfo();
    updateSettings();
}

sensorsetting::~sensorsetting()
{
    delete ui;
}
// initiating the port values for the selection
void sensorsetting::fillPortsParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(QStringLiteral("Custom"));

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(QStringLiteral("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);
}
void sensorsetting::fillPortsInfo()
{
    // clears all the precious stared values
    ui->serialPortInfoListBox->clear();
    static const QString blankString = QObject::tr("N/A");
    QString description;
    QString manufacturer;
    QString serialNumber;
    // loop for all the serial port available
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);
        // getting all the serial port available
        ui->serialPortInfoListBox->addItem(list.first(), list);
    }
}
// update all the value of the serial port values from the user selection.
void sensorsetting::updateSettings()
{
    currentSettings.name = ui->serialPortInfoListBox->currentText();
    // custom baudrate is selected
    if (ui->baudRateBox->currentIndex() == 4)
    {
        currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    }
    else
    {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityBox->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsBox->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();

    currentSettings.localEchoEnabled = ui->localEchoCheckBox->isChecked();
}
// returns all the value of the settings structure.
sensorsetting::Settings sensorsetting::settings() const
{
    return currentSettings;
}
// update the serial port setup with new values.
void sensorsetting::apply()
{
    updateSettings();
    emit control();
    hide();
}
// shows the value of the current com port selected and display in the label
void sensorsetting::showPortInfo(int idx)
{
    if (idx != -1) {
        QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
        ui->descriptionLabel->setText(tr("Description: %1").arg(list.at(1)));
        ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.at(2)));
        ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.at(3)));
        ui->locationLabel->setText(tr("Location: %1").arg(list.at(4)));
        ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.at(5)));
        ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.at(6)));
    }
}
// check the custom baudrate value is accepted or not
void sensorsetting::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
    ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->baudRateBox->clearEditText();
        QLineEdit *edit = ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

