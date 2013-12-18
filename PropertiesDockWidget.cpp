#include "PropertiesDockWidget.h"

PropertiesDockWidget::PropertiesDockWidget(std::shared_ptr<XBDM::DevConsole> console, QWidget *parent) :
    QDockWidget(parent)
{
    QWidget *dockWidgetContents = new QWidget();

    // this is all for the fancy thing at the top, with the image, debug name, and console type
    QWidget *fancyDataWidget = new QWidget(dockWidgetContents);
    fancyDataWidget->setGeometry(QRect(70, 170, 181, 81));

    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout(fancyDataWidget);
    horizontalLayout_2->setSpacing(6);

    QLabel *imgConsole = new QLabel(fancyDataWidget);
    imgConsole->setMinimumSize(QSize(64, 64));
    imgConsole->setMaximumSize(QSize(64, 64));

    horizontalLayout_2->addWidget(imgConsole);

    QVBoxLayout *verticalLayout_3 = new QVBoxLayout();
    verticalLayout_3->setSpacing(6);

    QSpacerItem *verticalSpacer_3 = new QSpacerItem(20, 40);
    verticalLayout_3->addItem(verticalSpacer_3);

    QLabel *lblName = new QLabel(fancyDataWidget);
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    lblName->setFont(font);
    verticalLayout_3->addWidget(lblName);

    QLabel *lblConsoleType = new QLabel(fancyDataWidget);
    QFont font1;
#ifdef __APPLE__
    font1.setPointSize(11);
#else
    font1.setPointSize(8);
#endif
    font1.setBold(false);
    font1.setWeight(50);
    font1.setKerning(true);
    font1.setStyleStrategy(QFont::PreferDefault);
    lblConsoleType->setFont(font1);
    verticalLayout_3->addWidget(lblConsoleType);

    QSpacerItem *verticalSpacer_2 = new QSpacerItem(20, 40);
    verticalLayout_3->addItem(verticalSpacer_2);

    horizontalLayout_2->addLayout(verticalLayout_3);
    horizontalLayout_2->setStretch(1, 9);


    // this all for the properties list
    QVBoxLayout *verticalLayout_4 = new QVBoxLayout(dockWidgetContents);
    verticalLayout_4->setSpacing(6);
    verticalLayout_4->setContentsMargins(11, 11, 11, 11);
    verticalLayout_4->addWidget(fancyDataWidget);

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(6);

    QVBoxLayout *verticalLayout_2 = new QVBoxLayout();
    verticalLayout_2->setSpacing(6);

    QLabel *label_2 = new QLabel(dockWidgetContents);
    verticalLayout_2->addWidget(label_2);

    QLabel *label_3 = new QLabel(dockWidgetContents);
    verticalLayout_2->addWidget(label_3);

    QLabel *label_5 = new QLabel(dockWidgetContents);
    verticalLayout_2->addWidget(label_5);

    QLabel *label_6 = new QLabel(dockWidgetContents);
    verticalLayout_2->addWidget(label_6);

    QLabel *label_7 = new QLabel(dockWidgetContents);
    verticalLayout_2->addWidget(label_7);

    QLabel *label_8 = new QLabel(dockWidgetContents);
    verticalLayout_2->addWidget(label_8);

    QLabel *label_9 = new QLabel(dockWidgetContents);
    verticalLayout_2->addWidget(label_9);

    QLabel *label_10 = new QLabel(dockWidgetContents);
    verticalLayout_2->addWidget(label_10);

    QLabel *label_11 = new QLabel(dockWidgetContents);
    verticalLayout_2->addWidget(label_11);

    horizontalLayout->addLayout(verticalLayout_2);

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(10);

    QLabel *lblHddEnabled = new QLabel(dockWidgetContents);
    lblHddEnabled->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    verticalLayout->addWidget(lblHddEnabled);

    QLabel *lblDebugMemorySize = new QLabel(dockWidgetContents);
    lblDebugMemorySize->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    verticalLayout->addWidget(lblDebugMemorySize);

    QLabel *lblFeatures = new QLabel(dockWidgetContents);
    lblFeatures->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    verticalLayout->addWidget(lblFeatures);

    QLabel *lblPlatform = new QLabel(dockWidgetContents);
    lblPlatform->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    verticalLayout->addWidget(lblPlatform);

    QLabel *lblMotherboard = new QLabel(dockWidgetContents);
    lblMotherboard->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    verticalLayout->addWidget(lblMotherboard);

    QLabel *lblBaseKrnlVersion = new QLabel(dockWidgetContents);
    lblBaseKrnlVersion->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    verticalLayout->addWidget(lblBaseKrnlVersion);

    QLabel *lblKrnlVersion = new QLabel(dockWidgetContents);
    lblKrnlVersion->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    verticalLayout->addWidget(lblKrnlVersion);

    QLabel *lblRecoveryVersion = new QLabel(dockWidgetContents);
    lblRecoveryVersion->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    verticalLayout->addWidget(lblRecoveryVersion);

    QLabel *lblActiveTitle = new QLabel(dockWidgetContents);
    lblActiveTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    verticalLayout->addWidget(lblActiveTitle);
    horizontalLayout->addLayout(verticalLayout);
    verticalLayout_4->addLayout(horizontalLayout);

    verticalLayout_4->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    verticalLayout_4->setStretch(0, 0);
    verticalLayout_4->setStretch(1, 0);
    verticalLayout_4->setStretch(2, 9);

    label_2->setText("HDD Enabled: ");
    label_3->setText("Debug Memory Size: ");
    label_5->setText("Features: ");
    label_6->setText("Platform: ");
    label_7->setText("Motherboard: ");
    label_8->setText("Base Kernel Version: ");
    label_9->setText("Kernel Version: ");
    label_10->setText("Recovery Version: ");
    label_11->setText("Active Title: ");

    setWindowTitle("Properties");

    // load all of the settings into the GUI
    bool ok;
    lblHddEnabled->setText(console->IsHddEnabled(ok) ? "Yes" : "No");
    lblFeatures->setText(qs(console->GetFeatures(ok)));
    lblPlatform->setText(qs(console->GetPlatform(ok)));
    lblMotherboard->setText(qs(console->GetMotherboard(ok)));
    lblBaseKrnlVersion->setText(qs(console->GetBaseKernelVersion(ok)));
    lblKrnlVersion->setText(qs(console->GetKernelVersion(ok)));
    lblRecoveryVersion->setText(qs(console->GetRecoveryVersion(ok)));
    lblDebugMemorySize->setText(qhex(console->GetDebugMemorySize(ok)));

    // i only want the active title label to show the name of the executable
    // a tooltip can show the full path
    QString activeTitlePath = qs(console->GetActiveTitle(ok));
    lblActiveTitle->setText(activeTitlePath.mid(activeTitlePath.lastIndexOf("\\") + 1));
    lblActiveTitle->setToolTip(activeTitlePath);

    QString consoleType = qs(console->GetType(ok));
    consoleType = consoleType.mid(0, 1).toUpper() + consoleType.mid(1).toLower();
    lblConsoleType->setText("<span style=\" color:#737373;\">" + consoleType + "</span>");

#ifdef __APPLE__
    lblName->setText("<span style=\" font-size:15pt; font-weight:600;\">" + qs(console->GetDebugName(ok)) + "</span>");
#else
    lblName->setText("<span style=\" font-size:11pt; font-weight:600;\">" + qs(console->GetDebugName(ok)) + "</span>");
#endif

    if (consoleType == "Devkit" && qs(console->GetFeatures(ok)).contains("1GB_RAM"))
        imgConsole->setPixmap(QPixmap(":/images/images/xna.png"));
    else if (consoleType == "Devkit")
        imgConsole->setPixmap(QPixmap(":/images/images/devkit.png"));
    else if (consoleType == "Testkit")
        imgConsole->setPixmap(QPixmap(":/images/images/testkit.png"));
    else
        imgConsole->setPixmap(QPixmap(":/images/images/retail.png"));

    setWidget(dockWidgetContents);
}
