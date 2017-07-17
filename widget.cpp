#include "widget.h"
#include "ui_widget.h"
#include <scene.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("TeBallicticFlightModelling");
    this->setFixedSize(WIDTH, HEIGHT);
    m_radioButtonsGroup = new QButtonGroup(this);
    m_radioButtonsGroup->addButton(ui->rbtnWithoutAirResistance);
    m_radioButtonsGroup->addButton(ui->rbtnLinearAirResitance);
    m_radioButtonsGroup->addButton(ui->rbtnQuadraticAirResitance);
    ui->rbtnWithoutAirResistance->setChecked(true);
    m_scene = new Scene(this);
    ui->graphicsView->setScene(m_scene);
    //m_scene->emitSignals();
    ui->txtFlightDistanceMax->setReadOnly(true);
    ui->txtFlightHeightMax->setReadOnly(true);
    ui->txtFlightTime->setReadOnly(true);

    //Conectamos los señales con las ranuras

    connect(ui->spinnerVelocity, SIGNAL(valueChanged(double)), m_scene,
            SLOT(changeBallVelocity(double)));
    connect(ui->spinnerJumpAngle, SIGNAL(valueChanged(double)), m_scene,
            SLOT(changeJumpAngle(double)));
    connect(ui->spinnerBallMass, SIGNAL(valueChanged(double)), m_scene,
            SLOT(changeBallMass(double)));
    connect(ui->spinnerAirResistance, SIGNAL(valueChanged(double)), m_scene,
            SLOT(chageAirResistance(double)));
    connect(ui->rbtnWithoutAirResistance, SIGNAL(toggled(bool)), m_scene,
            SLOT(setSimpleModel()));
    connect(ui->rbtnLinearAirResitance, SIGNAL(toggled(bool)), m_scene,
            SLOT(setLinearResistanceModel()));
    connect(ui->rbtnQuadraticAirResitance, SIGNAL(toggled(bool)), m_scene,
            SLOT(setQuadraticResistanceModel()));
    connect(m_scene, SIGNAL(flightDistanceChanged(QString)), ui->txtFlightDistanceMax, SLOT(setText(QString)));
    connect(m_scene, SIGNAL(flightHeightMaxChanged(QString)), ui->txtFlightHeightMax, SLOT(setText(QString)));
    connect(m_scene, SIGNAL(flightTimeChanged(QString)), ui->txtFlightTime, SLOT(setText(QString)));
    connect(ui->btnAnimation, SIGNAL(clicked(bool)), m_scene, SLOT(toggleAnimation()));
    connect(m_scene, &Scene::animationStarted, [&](){
        ui->btnAnimation->setText(QStringLiteral("Stop ball animation"));
        ui->spinnerVelocity->setEnabled(false);
        ui->spinnerJumpAngle->setEnabled(false);
        ui->spinnerBallMass->setEnabled(false);
        ui->spinnerAirResistance->setEnabled(false);
        ui->rbtnLinearAirResitance->setEnabled(false);
        ui->rbtnQuadraticAirResitance->setEnabled(false);
        ui->rbtnWithoutAirResistance->setEnabled(false);
    });
    connect(m_scene, &Scene::animationStopped, [&](){
        ui->btnAnimation->setText(QStringLiteral("Start ball animation"));
        ui->spinnerVelocity->setEnabled(true);
        ui->spinnerJumpAngle->setEnabled(true);
        ui->spinnerBallMass->setEnabled(true);
        ui->spinnerAirResistance->setEnabled(true);
        ui->rbtnLinearAirResitance->setEnabled(true);
        ui->rbtnQuadraticAirResitance->setEnabled(true);
        ui->rbtnWithoutAirResistance->setEnabled(true);
    });
}

Widget::~Widget()
{
    delete ui;
}
