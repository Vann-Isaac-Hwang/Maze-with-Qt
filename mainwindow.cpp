#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer)
{
    ui->setupUi(this);

    this->setWindowTitle("Maze by Isaac Huang");

    // Set up the timer to paint maze after certain time interval
    connect(timer,&QTimer::timeout,this,[=](){update();repaint();});
    timer->setInterval(RENDER_TIME_INTERVAL);
    timer->start();

    // Allocate memory to the maze
    M=new maze(10,10);

    // Configure checkbox for generating the maze
    ui->genalgoBox->addItem("DFS", NULL);
    ui->genalgoBox->addItem("Prim", NULL);
    ui->genalgoBox->addItem("Kruskal", NULL);

    // Configure checkbox for solving the maze
    ui->solalgoBox->addItem("DFS", NULL);
    ui->solalgoBox->addItem("BFS", NULL);
}

MainWindow::~MainWindow()
{
    // Free memory
    delete ui;
    delete M;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // To tell the compiler that the variable is left unused intentionally

    // Get the height and width of the centralwidget in UI, and automatically resize the maze accordingly
    int Height=ui->centralwidget->geometry().height(), Width=ui->centralwidget->geometry().width();
    // Paint the maze's black background
    paint = new QPainter;
    paint->begin(this);
    paint->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    paint->setPen(QPen(Qt::NoPen));
    int cx=231, cy=35, a;
    a=std::min((Height-40)/row-1,(Width-241)/column-1);
    paint->setBrush(QBrush(Qt::black, Qt::SolidPattern));
    paint->drawRect(cx-1,cy-1,1+(a+1)*column,1+(a+1)*row);

    // Draw cells
    paint->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    for (int i=0;i<row;i++)
    {
        for (int j=0;j<column;j++)
        {
            int bl=M->cell[i][j].l,br=M->cell[i][j].r,bu=M->cell[i][j].u,bd=M->cell[i][j].d; // Shift the cells to show connectivities
            paint->drawRect(cx+(a+1)*j-bl,cy+(a+1)*i-bu,a+bl+br,a+bu+bd);
        }
    }

    // Sign Visited cells
    if (ui->showVisitedBox->isChecked())
    {
        paint->setPen(QPen(Qt::NoPen));
        paint->setBrush(QBrush(qRgb(245,245,230), Qt::SolidPattern));
        // paint->drawRect(cx-5,cy-5,4,4);
        for (int i=0;i<row;i++)
        {
            for (int j=0;j<column;j++)
            {
                if (M->cell[i][j].visited)
                {
                    paint->drawRect(cx+(a+1)*j,cy+(a+1)*i,a,a);
                }
            }
        }
    }

    // Draw Route
    if (ui->showRouteBox->isChecked())
    {
        paint->setPen(QPen(Qt::blue, 3, Qt::SolidLine));
        for (int i=0;i<M->RouteLen-1;i++)
        {
            paint->drawLine(cx+(a+1)*M->Route[i].c+a/2,cy+(a+1)*M->Route[i].r+a/2,cx+(a+1)*M->Route[i+1].c+a/2,cy+(a+1)*M->Route[i+1].r+a/2);
        }
    }

    // Draw starting and ending points
    paint->setPen(QPen(Qt::red, 5, Qt::SolidLine));
    paint->drawPoint(cx+(a+1)*M->st_col+a/2,cy+(a+1)*M->st_row+a/2);
    paint->setPen(QPen(Qt::green, 5, Qt::SolidLine));
    paint->drawPoint(cx+(a+1)*M->des_col+a/2,cy+(a+1)*M->des_row+a/2);

    paint->end();
}

void MainWindow::on_sizesetButton_clicked()
{
    // Get target size from input
    int tar_row=ui->rowEdit->text().toInt();
    int tar_column=ui->columnEdit->text().toInt();
    if (tar_row<2||tar_column<2) // Too small
    {
        QString sizeErr="Size >= 2X2";
        ui->promptlabel->setText(sizeErr);
        return;
    }
    else if (tar_row>250||tar_column>250) // Too big
    {
        QString sizeErr="Size <= 250X250";
        ui->promptlabel->setText(sizeErr);
        return;
    }
    // Set to target size
    row=tar_row;
    column=tar_column;
    std::cout<<"Size of maze set to: "<<row<<"X"<<column<<std::endl;
    QString qstr="Size of maze set to: "+QString::number(row)+"X"+QString::number(column);
    ui->promptlabel->setText(qstr);
    // Recreate maze
    delete M;
    M=new maze(row,column);
}

void MainWindow::on_GenerateButton_clicked()
{
    delete M;
    M=new maze(row,column);
    if (ui->genalgoBox->currentText()=="DFS")
        M->mazeGenDFS();
    else if (ui->genalgoBox->currentText()=="Prim")
        M->mazeGenPrim();
    else if (ui->genalgoBox->currentText()=="Kruskal")
        M->mazeGenKruskal();
}

void MainWindow::on_SolveButton_clicked()
{
    if (ui->solalgoBox->currentText()=="DFS")
        M->mazeSolveDFS();
    else if (ui->solalgoBox->currentText()=="BFS")
        M->mazeSolveBFS();
}


void MainWindow::on_saveButton_clicked()
{
    // Call the function in class maze to store the maze as xxx.iskmaz
    M->mazeSave(ui->filenameEdit->text().toStdString());
}


void MainWindow::on_loadButton_clicked()
{
    // Load maze from file
    QString Qfilename=QFileDialog::getOpenFileName(this,tr("open a file."),"D:/",tr("All files(*.*)"));
    std::string filename=Qfilename.toStdString();
    std::cout<<filename;
    std::ifstream mazeFile;
    mazeFile.open(filename, std::ios::in);
    delete M;
    if (mazeFile.is_open())
    {
        std::string line;
        int i=0;
        while (std::getline(mazeFile, line)) // Read each line of file
        {
            if (i==0) // First row, get maze's size
            {
                sscanf(line.c_str(), "%d,%d", &row,&column);
                M=new maze(row,column);
                i=1;
            }
            else // Following rows, initialize maze's cells
            {
                int row,col,l,r,u,d;
                sscanf(line.c_str(), "%d,%d,%d,%d,%d,%d", &row,&col,&l,&r,&u,&d);
                M->cell[row][col].l=l, M->cell[row][col].r=r, M->cell[row][col].u=u, M->cell[row][col].d=d;
            }
        }
    }
    else
    {
        std::cout<<"Failed to load Maze."<<std::endl;
    }
    mazeFile.close();
}

void MainWindow::on_destisetButton_clicked()
{
    // Get target destination from input
    int r=ui->desrowEdit->text().toInt(), c=ui->descolumnEdit->text().toInt();

    if (r>=row||c>=column||r<0||c<0) // Out of range
    {
        ui->promptlabel->setText("Destination out of range!");
        return;
    }
    else if (r==M->st_row&&c==M->st_col) // Destination coincide with starting point
    {
        ui->promptlabel->setText("Destination coincide with starting point!");
        return;
    }
    // Set destination
    M->des_row=r, M->des_col=c;
    QString qstr="Destination set to: "+QString::number(M->des_row)+","+QString::number(M->des_col);
    ui->promptlabel->setText(qstr);
}


void MainWindow::on_depsetButton_clicked()
{
    // Get target point of departure from input
    int r=ui->deprowEdit->text().toInt(), c=ui->depcolumnEdit->text().toInt();

    if (r>=row||c>=column||r<0||c<0) // Out of range
    {
        ui->promptlabel->setText("Point of departure out of range!");
        return;
    }
    else if (r==M->des_row&&c==M->des_col) // Destination coincide with starting point
    {
        ui->promptlabel->setText("Starting point coincide with destination!");
        return;
    }
    // Set destination
    M->st_row=r, M->st_col=c;
    QString qstr="Starting point set to: "+QString::number(M->des_row)+","+QString::number(M->des_col);
    ui->promptlabel->setText(qstr);
}

